#pragma once

//////////////////////////////////////////////////////////////////////////
// Types

// API Base types
typedef signed __int8  Int8, SInt8, SByte;
typedef signed __int16 Int16,SInt16,Short;
typedef signed __int32 Int32,SInt32,Int;
typedef signed __int64 Int64,SInt64,Long;

typedef unsigned __int8  UInt8, UByte, Byte;
typedef unsigned __int16 UInt16,UShort;
typedef unsigned __int32 UInt32,UInt;
typedef unsigned __int64 UInt64,ULong;

typedef bool Bool;
#define True true
#define False false

#ifdef _M_IX86
typedef UInt32 UIntPtr;
#elif defined(_M_X64)
typedef UInt64 UIntPtr;
#endif

typedef UInt32 ResultCode;

typedef UIntPtr Handle;

typedef wchar_t Char, *KString;

typedef const Char* KCString;

#define NULL 0
