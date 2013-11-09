#pragma once

// Process, Module, Thread, Buffer/Section/whatever, Semaphore/Mutex, Timer, Stream/File/Pipe/Socket

/****
 *
 *
 * Process (Create, Destroy)
 * Module (Load, GetExportCount, GetExport, Release)
 * Thread (Create, SetAddress, Destroy)
 * Section (Create, GetAddres, GetLength, Release)
 * Semaphore (Create, Acquire, Release, Destroy)
 * Timer
 * Stream
 * Pipe
 * AccessList
 * Socket
 * File
 * Directory
 */

///////////////////////////////////////////////////////////////////////////////
// Object Manager
//

ResultCode ClassRegister(KCString name, UIntPtr objectSize, UIntPtr reserved_metadata, UInt32& classID);
ResultCode ClassGetByName(KCString name);

ResultCode ObjectAllocate(UInt32 ownerID, KCString name, UInt32 classID, Handle& object);
ResultCode ObjectRelease(Handle object);

ResultCode ObjectOpenExisting(KCString name, Handle& object);

ResultCode ObjectCallMethod(Handle object, UInt32 methodIndex, UIntPtr inputData, UIntPtr inputSize, UIntPtr outputData, UIntPtr outputSize);

ResultCode ObjectWait(Handle object);
ResultCode ObjectSignal(Handle object);

/*

Specialized Object Methods:

	ProcessCreate
	ProcessDestroy

	ModuleLoad
	ModuleGetExportCount
	ModuleGetExport

	ThreadCreate
	ThreadSetAddress

	SectionCreate
	SectionGetAddress
	SectionGetLength

	SemaphoreCreate
	SemaphoreAcquireWait
	SemaphoreReleaseSignal

	TimerCreate
	TimerSetInterval
	TimerSetCallback

	StreamRead
	StreamWrite
	StreamGetPosition
	StreamSetPosition
	StreamGetLength
	StreamSetLength
	StreamGetAvailable

	PipeCreate
	PipeGetFirstEndPoint
	PipeGetSecondEndPoint


*/