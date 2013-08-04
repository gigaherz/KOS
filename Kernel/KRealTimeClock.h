#pragma once

struct DateTime
{
    UInt16 year;
    UInt8  month;
    UInt8  day;
    UInt8  hour;
    UInt8  minute;
    UInt8  second;

    UInt32 dayOfWeek;
};

//////////////////////////////////////////////////////////////////////////
// Real-Time Clock
void KRTCInit();
DateTime KRTCGetDateTime();
