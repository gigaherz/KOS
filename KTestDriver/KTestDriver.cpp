#include "../KDriverRuntime/KDriverApi.h"

DRIVER_EXPORT Initialize()
{
	// Put initialization stuff here
	KDebugPrint(L"Test Driver was loaded!");
	return 0;
}

// Called when unloading the module
DRIVER_EXPORT Cleanup()
{
	// Put cleanup stuff here
	return 0;
}

// Called when a new device has been detected.
// If the driver can (and wants to) handle the device, it should assign a new device header to retValue, and return 0.
// If the driver can't (or doesn't want to) handle the device, it should return an adequate error code.
DRIVER_EXPORT ConnectDevice(KDeviceInfo *info, KDeviceHeader** retValue)
{
	// Put device identification and other stuff here
	return 0;
}
