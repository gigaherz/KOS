#pragma once

#include "Types.h"

#include <math.h>

#pragma pack(push,1)
typedef union TimeStamp_u	//time is considered from GMT
{
	struct TimeStamp_s //exactly 8 bytes
	{
		//from big to small
		u16 structmode:1;		// set to 1 if the timestamp uses this struct, else ticks should be used.
		u16 year:15;			// if this is still used by the year 16384, fix it.
		u8  month;
		u8  day;

		u8  hour;
		u8  minute;
		u8  second;
		u8  fraction; // in 1/256th of a second
	} datetime;
	u64 ticks; // number of 1/256ths of a second since moment 0. most significant bit must be 0
} TimeStamp;
#pragma pack(pop)

//Utility Functions
s32 bitsInNumber(s32 number);

s64 __inline ceilDiv(s64 a, s64 b)
{
	return (s64)ceil((double)a/(double)b);
}

const char* uuidToStringA(const uuid& src);
const wchar_t* uuidToStringW(const uuid& src);

#ifdef WIN32

void MkUUID(uuid* dest);
void MkTimeStamp(TimeStamp* dest);

#endif