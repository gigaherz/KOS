#include "Kernel.h"

struct ClassInformation
{
    UInt32 id;
	UIntPtr objectSize;

	// TODO: Add metadata
};

PoolHeader *classPool;

void InitializeObjectManager()
{
	classPool = KPoolAllocate('OBJC', 256, 64 * sizeof(Char));
}

// TODO: Requires a working name system
ResultCode ClassGetById(UInt32 id, ClassInformation*& ret)
{
    return OK;
}

ResultCode ClassRegister(UInt32 id, UIntPtr objectSize, UIntPtr reserved_metadata)
{
    return OK;
}

ResultCode ObjectAllocate(UInt32 ownerID, KCString name, UInt32 classID, Handle& object);
ResultCode ObjectRelease(Handle object);

// TODO: Requires a working name system
ResultCode ObjectOpenExisting(KCString name, Handle& object);

ResultCode ObjectCallMethod(Handle object, UInt32 methodIndex, UIntPtr inputData, UIntPtr inputSize, UIntPtr outputData, UIntPtr outputSize);

ResultCode ObjectWait(Handle object);
ResultCode ObjectSignal(Handle object);
