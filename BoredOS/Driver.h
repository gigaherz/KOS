#ifndef DRIVER_H_INCLUDED
#define DRIVER_H_INCLUDED

#include "Common.h"
#include "Process.h"

extern "C" {
typedef int (__stdcall ServiceFunction)(ProcessInfo *callerProcess);
typedef ServiceFunction *ServiceFunctionPointer;
}

#pragma pack(push,1)
struct VersionInfo		// 8 bytes
{
	__int8  High;
	__int8  Low;
	__int16 Revision;
	__int32 Build;
};

struct TimeStamp	// 8 bytes
{
	__int8  Day;
	__int8  Month;
	__int16 Year;
	__int8  Hour;
	__int8  Minute;
	__int8  Second;
	__int8  Sec100; //1/100th of a second units
};

struct DriverDescriptor
{
	wchar_t Name[60];		//120 bytes
	Version Version;		//8 bytes

	wchar_t Author[60];		//64 bytes
	TimeStamp BuildDate;	//8 bytes

	struct ServiceFunction	//128 bytes
	{
		wchar_t Name[60];	//120 bytes
		ServiceFunctionPointer Pointer; //8(4) bytes
#if POINTER_IS_32_BIT
		int padding;
#endif
	} ServiceFunctions[510];
};

#pragma pack(pop)

class Driver
{
private:

public:
	Driver();
	ServiceFunctionPointer GetServiceFunctionPointer(_TCHAR *serviceName);
};

#endif//DRIVER_H_INCLUDED