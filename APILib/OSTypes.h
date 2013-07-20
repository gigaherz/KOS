#pragma once

// Base types
typedef signed __int8  Int8, SByte;
typedef signed __int16 Int16,Short;
typedef signed __int32 Int32,Int;
typedef signed __int64 Int64,Long;

typedef unsigned __int8  UInt8, UByte, Byte;
typedef unsigned __int16 UInt16,UShort;
typedef unsigned __int32 UInt32,UInt;
typedef unsigned __int64 UInt64,ULong;

#ifdef _M_IX86
typedef UInt32 UIntPtr;
#elif defined(_M_X64)
typedef UInt64 UIntPtr;
#endif

typedef UInt ResultCode;

typedef UIntPtr Handle;

typedef wchar_t Char; // All the functions and strings use 16bit unicode characters

enum ReferencePosition:Int {
	ReferencePosition_Begin,
	ReferencePosition_Current,
	ReferencePosition_End
};

struct String {
	UInt32 Length;
	const Char* Data;
};

struct AccessList;

struct Message
{
	UInt32 Length;
	Byte*  Data;
};
