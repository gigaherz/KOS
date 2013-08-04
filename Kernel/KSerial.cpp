#include "Kernel.h"

#define COM_ADDR 0x03f8
#define COM_IRQ 4

// DLAB=0 regs
#define COM_RDR (COM_ADDR+0)
#define COM_WRR (COM_ADDR+0)
#define COM_IER (COM_ADDR+1)
#define COM_IIR (COM_ADDR+2)

// DLAB=1 regs
#define COM_DLL (COM_ADDR+0)
#define COM_DLH (COM_ADDR+1)
#define COM_FCR (COM_ADDR+2)

// Other regs
#define COM_LCR (COM_ADDR+3)
#define COM_MCR (COM_ADDR+4)
#define COM_LSR (COM_ADDR+5)
#define COM_MSR (COM_ADDR+6)
#define COM_SCR (COM_ADDR+7)


void KSerialInit()
{
    KPortOutputB(COM_IER, 0);   /* Turn off interrupts - Port1 */

    /*         PORT 1 - Communication Settings         */

    KPortOutputB(COM_LCR, 0x80);  /* SET DLAB ON */
    KPortOutputB(COM_DLL, 0x01);  /* Set Baud rate - Divisor Latch Low Byte */
    /* Default 0x03 =  38,400 BPS */
    /*         0x01 = 115,200 BPS */
    /*         0x02 =  57,600 BPS */
    /*         0x06 =  19,200 BPS */
    /*         0x0C =   9,600 BPS */
    /*         0x18 =   4,800 BPS */
    /*         0x30 =   2,400 BPS */
    KPortOutputB(COM_DLH, 0x00);  /* Set Baud rate - Divisor Latch High Byte */
    KPortOutputB(COM_LCR, 0x03);  /* 8 Bits, No Parity, 1 Stop Bit -- DLAB off*/
    KPortOutputB(COM_FCR, 0xC7);  /* FIFO Control Register */
    KPortOutputB(COM_MCR, 0x0B);  /* Turn on DTR, RTS, and OUT2 */
}

void KSerialWrite(Char data)
{
    // wait for last char to be sent
    int c;

    do
    {
        c = KPortInputB(COM_LSR);
    }
    while ((c&(1<<5))==0);			// check transmitter register empty

    KPortOutputB(COM_WRR, (UInt8)data); /* Send Char to Serial Port */
}

void KSerialWrite(KCString data, int len)
{
    while (len--)
    {
        KSerialWrite(*(data++));
    }
}

void KSerialPrint(KCString data)
{
    while (*data)
    {
        KSerialWrite(*(data++));
    }
}

static void __cdecl KDisplayWriteCallback(void*userdata, KCString string, int nchars)
{
    KSerialWrite(string,nchars);
}

void __cdecl KSerialPrintF(KCString fmt, ...)
{
    va_list lst;

    va_start(lst,fmt);
    KSimplePrintF(KDisplayWriteCallback, 0, fmt, lst);
    va_end(lst);
}
