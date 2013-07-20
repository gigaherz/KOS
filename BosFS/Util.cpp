#include "stdafx.h"
#include "BosFS.h"
#include <math.h>
#include <assert.h>

//Utility Functions
int bitsInNumber(int number)
{
	int bits=0;
	if(number >= 0x10000) { bits += 16; number >>= 16; }
	if(number >= 0x100) { bits += 8; number >>= 8; }
	if(number >= 0x10) { bits += 4; number >>= 4; }
	if(number >= 0x4) { bits += 2; number >>= 2; }
	if(number >= 0x1) { bits += 1; number >>= 1; }
	return bits;
}

const char* uuidToStringA(const uuid& src)
{
	// WARNING: NOT concurrent NOR thread-safe
	static char uuid_text[100];
	sprintf(uuid_text,"%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
		src.Data1,src.Data2,src.Data3,
		src.Data4[0],src.Data4[1],src.Data4[2],src.Data4[3],
		src.Data4[4],src.Data4[5],src.Data4[6],src.Data4[7]);
	return uuid_text;
}

const wchar_t* uuidToStringW(const uuid& src)
{
	// WARNING: NOT concurrent NOR thread-safe
	static wchar_t uuid_text[100];
	_swprintf(uuid_text,L"%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
		src.Data1,src.Data2,src.Data3,
		src.Data4[0],src.Data4[1],src.Data4[2],src.Data4[3],
		src.Data4[4],src.Data4[5],src.Data4[6],src.Data4[7]);
	return uuid_text;
}

#ifdef WIN32
#include <windows.h>

/* UUID Generation, MkUUID */
#include <objbase.h>

void MkUUID(uuid* dest)
{
	CoCreateGuid((GUID*)dest);
}

void MkTimeStamp(TimeStamp* dest)
{
	SYSTEMTIME t;

	GetSystemTime(&t);

	dest->ticks = 0; //cleanup first
	dest->datetime.structmode = 1;
	dest->datetime.year     = (u16)t.wYear;
	dest->datetime.month    = (u8)t.wMonth;
	dest->datetime.day      = (u8)t.wDay;
	dest->datetime.hour     = (u8)t.wHour;
	dest->datetime.minute   = (u8)t.wMinute;
	dest->datetime.second   = (u8)t.wSecond;
	dest->datetime.fraction = (u8)((t.wMilliseconds * 256) / 1000);
}
#endif