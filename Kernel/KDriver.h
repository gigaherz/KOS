#pragma once

#include "Kernel.h"

#define KFUNCTION_PTR(name) ResultCode (__stdcall * name)

struct KDeviceInfo
{
    KDeviceInfo* parent;

    UInt32 deviceClass;
    UInt32 makerID;
    UInt32 modelID;
};

struct KDeviceHeader;

struct KDriverHeader
{
    // Called when loading the driver module
    KFUNCTION_PTR(Initialize)();

    // Called when unloading the module
    KFUNCTION_PTR(Cleanup)();

    // Called when a new device has been detected.
    // If the driver can (and wants to) handle the device, it should assign a new device header to retValue, and return 0.
    // If the driver can't (or doesn't want to) handle the device, it should return an adequate error code.
    KFUNCTION_PTR(ConnectDevice)(KDeviceInfo *info, KDeviceHeader** retValue);

};