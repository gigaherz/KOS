#pragma once

#ifdef WIN32
#include <windows.h>

/* Device Access -- a real device implementations might require some "block work"
					if the dirver saves data that doesn't fit in whole sectors */

// The following defines and types will be used by the code.
typedef HANDLE device_handle;	//used by the code to identify the device
typedef __int64 device_pointer;	//type required for the access operations

#define DEV_READONLY 0
#define DEV_READWRITE 1

#define DEV_OPEN_ERROR INVALID_HANDLE_VALUE
#define DEV_READWRITE_ERROR -1
#define DEV_READWRITE_EOF    0

device_handle DevOpen(const char* devstr, int mode);
void DevClose(device_handle device);
device_pointer DevRead(device_handle device, void* dest, device_pointer offset, device_pointer size);
device_pointer DevWrite(device_handle device, void* dest, device_pointer offset, device_pointer size);
s32 DevGetSectorSize(device_handle device);
#endif
