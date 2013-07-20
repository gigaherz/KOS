#include "Kernel.h"

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

#define RTC_ADDR_MASK   0x80
#define RTC_SECOND      0x00
#define RTC_ALRM_SECOND 0x01
#define RTC_MINUTE      0x02
#define RTC_ALRM_MINUTE 0x03
#define RTC_HOUR        0x04
#define RTC_ALRM_HOUR   0x05
#define RTC_DO_WEEK     0x06
#define RTC_DO_MONTH    0x07
#define RTC_MONTH       0x08
#define RTC_YEAR        0x09

#define RTC_STATUS_A  0x0A
#define RTC_SRA_UIP   0x80
#define RTC_SRA_BASE  0x70
#define RTC_SRA_RATE  0x0F

#define RTC_STATUS_B  0x0B
#define RTC_SRB_UPDT  0x80  // Update - 0=enable, 1=disable
#define RTC_SRB_PI    0x40
#define RTC_SRB_AI    0x20
#define RTC_SRB_UI    0x10
#define RTC_SRB_SQU   0x08
#define RTC_SRB_DM    0x04   // Data Mode - 1=binary, 0=BCD
#define RTC_SRB_24HR  0x02
#define RTC_SRB_DLS   0x01

#define RTC_STATUS_C  0x0C
#define RTC_SRC_IRQ   0x80
#define RTC_SRC_PS    0x40
#define RTC_SRC_AS    0x20
#define RTC_SRC_US    0x10
#define RTC_SRC_RSVD  0x0F

#define DECODE_BCD(x) (((x)>>4)*10+((x)&15))
#define CREATE_BCD(x) ((((x)/10)<<4)+((x)%10))

DateTime dtCache;

UInt32 timerID = 0;
UInt32 tdAccum = 0;
UInt32 tdCounter = 0;
UInt32 correctedInterval = 1000;

void KCMOSWrite(Byte offset, Byte value)
{
	// Remember, only the low-order 5 bits in the address register are changed.
	Byte cAddr = KPortInputB( CMOS_ADDR );
	KPortOutputB( CMOS_ADDR, (cAddr & RTC_ADDR_MASK) | offset );
	KPortOutputB( CMOS_DATA, value );
	// NOTE : If bytes 16-45 are ever updated, the checksum should be recalculated and stored.
}

Byte KCMOSRead(Byte offset)
{
	// Remember, only the low-order 5 bits in the address register are changed.
	Byte cAddr = KPortInputB( CMOS_ADDR );
	KPortOutputB( CMOS_ADDR, (cAddr & RTC_ADDR_MASK) | offset );    
	Byte cResult = KPortInputB( CMOS_DATA );
	return (cResult);
}

int KDateTimeCompare(DateTime& dt1, DateTime& dt2)
{
	if (dt1.year > dt2.year)
		return 1;
	if (dt1.year < dt2.year)
		return -1;

	if (dt1.month > dt2.month)
		return 1;
	if (dt1.month < dt2.month)
		return -1;

	if (dt1.day > dt2.day)
		return 1;
	if (dt1.day < dt2.day)
		return -1;

	if (dt1.dayOfWeek > dt2.dayOfWeek)
		return 1;
	if (dt1.dayOfWeek < dt2.dayOfWeek)
		return -1;

	if (dt1.hour > dt2.hour)
		return 1;
	if (dt1.hour < dt2.hour)
		return -1;

	if (dt1.minute > dt2.minute)
		return 1;
	if (dt1.minute < dt2.minute)
		return -1;

	if (dt1.second > dt2.second)
		return 1;
	if (dt1.second < dt2.second)
		return -1;

	return 0;
}

DateTime KRTCRead()
{
	DateTime dt1;
	DateTime dt2;

	dt1.second = 61; // initialize to an invalid value 
	do
	{
		// exchange lpst1 and lpst2
		dt2 = dt1;

		// Wait until not updating.
		while (KCMOSRead(RTC_STATUS_A) & RTC_SRA_UIP)
		{}

		// Read all values.
		dt1.year = KCMOSRead(RTC_YEAR);
		dt1.month = KCMOSRead(RTC_MONTH); 
		dt1.day = KCMOSRead(RTC_DO_MONTH);
		dt1.hour = KCMOSRead(RTC_HOUR); 
		dt1.minute = KCMOSRead(RTC_MINUTE); 
		dt1.second = KCMOSRead(RTC_SECOND); 
		dt1.dayOfWeek = KCMOSRead(RTC_DO_WEEK);
	}
    while (KDateTimeCompare(dt1, dt2) != 0);

	//lpst->wMilliseconds = 0; // Not sure how you would get this

    if (!(KCMOSRead(RTC_STATUS_B) & RTC_SRB_DM)) {
        // Values returned in BCD.
		dt1.year = DECODE_BCD(KCMOSRead(RTC_YEAR));
		dt1.month = DECODE_BCD(KCMOSRead(RTC_MONTH)); 
		dt1.day = DECODE_BCD(KCMOSRead(RTC_DO_MONTH));
		dt1.hour = DECODE_BCD(KCMOSRead(RTC_HOUR)); 
		dt1.minute = DECODE_BCD(KCMOSRead(RTC_MINUTE)); 
		dt1.second = DECODE_BCD(KCMOSRead(RTC_SECOND)); 
		dt1.dayOfWeek = DECODE_BCD(KCMOSRead(RTC_DO_WEEK));
    }

	// OK - PC RTC returns 1998 as 98.
	dt1.year += (dt1.year > 70)? 1900 : 2000;
	return dt1;
}

Bool Bare_SetRealTime(DateTime & dt)
{
	Byte Year = dt.year % 100;

#ifdef HARDWARE_TIME_SET_PROBLEM
    if (dt.second == 59)
    {
		dt.second = 58;
    }
#endif

	// Read the update in progress bit, wait for it to be clear.
	// This bit will be set once per second for about 2 us. (Undoc. PC, page 897)
	while (KCMOSRead(RTC_STATUS_A) & RTC_SRA_UIP)
	{}
    
	// Disable updates while the values are changed
    Byte cStatusRegB = KCMOSRead(RTC_STATUS_B);

    KCMOSWrite(RTC_STATUS_B, cStatusRegB | RTC_SRB_UPDT);

	if ( !(cStatusRegB & RTC_SRB_DM) )
	{
        // BCD Mode
        KCMOSWrite(RTC_YEAR,     CREATE_BCD(Year)); 
        KCMOSWrite(RTC_MONTH,    CREATE_BCD(dt.month)); 
        KCMOSWrite(RTC_DO_MONTH, CREATE_BCD(dt.day)); 
        KCMOSWrite(RTC_HOUR,     CREATE_BCD(dt.hour)); 
        KCMOSWrite(RTC_MINUTE,   CREATE_BCD(dt.minute)); 
        KCMOSWrite(RTC_SECOND,   CREATE_BCD(dt.second)); 
        KCMOSWrite(RTC_DO_WEEK,  CREATE_BCD(dt.dayOfWeek)); 
	}
	else
    {
        // Binary mode
        KCMOSWrite(RTC_YEAR,     Year); 
        KCMOSWrite(RTC_MONTH,    dt.month); 
        KCMOSWrite(RTC_DO_MONTH, dt.day); 
        KCMOSWrite(RTC_HOUR,     dt.hour); 
        KCMOSWrite(RTC_MINUTE,   dt.minute); 
		KCMOSWrite(RTC_SECOND,   dt.second); 
        KCMOSWrite(RTC_DO_WEEK,  dt.dayOfWeek); 
	}

	// Reenable updates
	KCMOSWrite(RTC_STATUS_B, cStatusRegB & ~RTC_SRB_UPDT);

	return True;
}

int KDaysInMonth(DateTime dt)
{
	static const int dym[12] =
	{ // J  F  M  A  M  J
		31, 0,31,30,31,30,
      // J  A  S  O  N  D
		31,31,30,31,30,31
	};

	int days = dym[dt.month-1];

	if(days)
		return days;

	// else Feb

	// is leap year?
	bool mod4 = (dt.year & 3) == 0;

	if(!mod4)
		return 28;

	if((dt.year % 100) == 0)
	{
		if((dt.year % 400) == 0)
			return 29;

		return 28;
	}

	return 29;
}

void KAddSeconds(DateTime& dt, int seconds)
{
	
	if(++dt.second < 60)
		return;

	dt.second = 0;

	if(++dt.minute < 60)
		return;

	dt.minute = 0;

	if(++dt.hour < 24)
		return;

	dt.hour = 0;

	if(++dt.day <= KDaysInMonth(dt))
		return;

	dt.day = 1;

	if(++dt.month <= 12)
		return;

	dt.month = 0;

	++dt.year;

}

Bool rtcQuickUpdate(UInt32 tmrID, UInt32 tickDiff)
{
	tdAccum += tickDiff;
	tdCounter+=tickDiff;
	while(tdCounter >= correctedInterval)
	{
		tdCounter-=correctedInterval;

		KAddSeconds(dtCache,1);
	}

	if(tdAccum >= 10000)
	{
		tdAccum -= 10000;
		DateTime dtCurrent = KRTCRead();
		int compare = KDateTimeCompare(dtCache,dtCurrent);
		if(compare!=0)
		{
			correctedInterval += compare;
			KDisplayPrintF(L"C! %d\r\n\n",correctedInterval);
			KTimerSetInterval(timerID, correctedInterval);
			dtCache = dtCurrent;
		}
	}

	return True;
}

DateTime KRTCGetDateTime()
{
	return dtCache;
}

void KRTCInit()
{
	tdAccum   = 0;
	tdCounter = 0;
	DateTime dt2 = KRTCRead();
	do
	{
		dtCache = KRTCRead();
	}
	while(KDateTimeCompare(dt2,dtCache)==0);

	KDebugPrintF(L"Inicializing RTC ... %d\r\n", KTimerAlloc(1000, rtcQuickUpdate, &timerID));
}