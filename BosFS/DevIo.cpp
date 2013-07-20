#include "stdafx.h"
#include "BosFS.h"
#include <math.h>
#include <assert.h>

#include "Types.h"
#include "DevIo.h"

#ifdef WIN32
#include <windows.h>

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

s32 DevGetSectorSize(device_handle device)
{
	return 512; // TODO: Use a real value!
}

#endif
