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

/*

Object Manager:

	ObjectAllocate
	ObjectOpenExisting
	ObjectCallMethod
	ObjectRelease

	ObjectWait
	ObjectSignal

	ObjectGetOwner
	ObjectSetOwner

	ObjectGetAccessList
	ObjectSetAccessList

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