#include "stdafx.h"
#include "BosFS.h"

#ifdef WIN32

void MkUUID(uuid* dest)
{
	CoCreateGuid((GUID*)dest);
}

void MkTimeStamp(TimeStamp* dest)
{
	SYSTEMTIME t;
	GetSystemTime(&t);
	dest->ticks=0; //cleanup first
	dest->datetime.structmode=1;
	dest->datetime.year=t.wYear;
	dest->datetime.month=t.wMonth;
	dest->datetime.day=t.wDay;
	dest->datetime.hour=t.wHour;
	dest->datetime.minute=t.wMinute;
	dest->datetime.second=t.wSecond;
	dest->datetime.fraction=t.wMilliseconds*256/1000;
}

device_handle DevOpen(const char* devstr, int mode)
{
	return CreateFile(devstr,FILE_READ_DATA | (mode==DEV_READWRITE)?FILE_WRITE_DATA:0,
		FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_FLAG_RANDOM_ACCESS,NULL);
}

void DevClose(device_handle device)
{
	CloseHandle(device);
}

device_pointer DevRead(device_handle device,void* dest, device_pointer offset, device_pointer size)
{
	DWORD bytesRead;
	LONG offsetlow=offset;
	LONG offsethigh=offset>>32;
	SetFilePointer(device,offsetlow,&offsethigh,FILE_BEGIN);
	BOOL result=ReadFile(device,dest,size,&bytesRead,NULL);
	if(result==0)
		return DEV_READWRITE_ERROR;
	return bytesRead;
}

device_pointer DevWrite(device_handle device,void* dest, device_pointer offset, device_pointer size)
{
	DWORD bytesWritten;
	LONG offsetlow=offset;
	LONG offsethigh=offset>>32;
	SetFilePointer(device,offsetlow,&offsethigh,FILE_BEGIN);
	BOOL result=WriteFile(device,dest,size,&bytesWritten,NULL);
	if(result==0)
		return DEV_READWRITE_ERROR;
	return bytesWritten;
}

#endif
