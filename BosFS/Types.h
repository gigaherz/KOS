#pragma once

#ifdef WIN32
//helper types
typedef signed __int8  s8;
typedef signed __int16 s16;
typedef signed __int32 s32;
typedef signed __int64 s64;

typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;
#endif

#pragma pack(push,1)

typedef struct uuid_s {
    u32 Data1;
    u16 Data2;
    u16 Data3;
    u8  Data4[8];
} uuid;

#pragma pack(pop)