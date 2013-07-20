#pragma once

#include "OSTypes.h"

struct FileInformation
{
	String FileName;
	UInt64 Length;
	UInt32 Flags;
};

#define IN
#define REF *
#define OUT *

// Directory Listing
ResultCode DirectoryOpen ( 
	String IN name, 
	Handle OUT handle );

ResultCode DirectoryClose (
	Handle IN dir );

ResultCode DirectoryRewind (
	Handle IN dir );

ResultCode DirectoryGetCurrent (
	Handle IN dir, 
	Handle OUT entry );

ResultCode DirectoryGoNext (
	Handle IN dir );

ResultCode DirectoryCreateEntry ( 
	Handle IN dir, 
	String IN entryName,
	Handle IN file,
	Handle OUT entry );

ResultCode DirectoryEntryGetName ( 
	Handle IN entry, 
	String OUT name );

ResultCode DirectoryEntrySetName ( 
	Handle IN entry, 
	String IN name );

ResultCode DirectoryEntryGetAccessList ( 
	Handle IN entry, 
	AccessList OUT list );

ResultCode DirectoryEntrySetAccessList ( 
	Handle IN entry, 
	AccessList REF list );

ResultCode DirectoryEntryDelete (
	Handle IN entry );

// File Access
ResultCode FileOpen (
	String IN path,
	Handle OUT file );

ResultCode FileClose (
	Handle IN file );

ResultCode FileGetInformation ( 
	Handle IN file, 
	FileInformation REF info );

ResultCode FileSetInformation (
	Handle IN file, 
	FileInformation REF info );

ResultCode FileGetStream ( 
	Handle IN file, 
	String IN streamName, 
	Handle OUT streamHandle );

ResultCode FileCreateStream ( 
	Handle IN file, 
	String IN streamName, 
	Handle OUT streamHandle );

ResultCode FileDeleteStream ( 
	Handle IN file, 
	String IN streamName );

// Stream Access
ResultCode StreamRead ( 
	Handle IN stream, 
	UInt32 IN byteSize, 
	void* REF buffer );

ResultCode StreamWrite ( 
	Handle IN stream, 
	UInt32 IN byteSize, 
	void* REF buffer );

ResultCode StreamSeek ( 
	Handle IN stream, 
	UInt64 IN position, 
	ReferencePosition IN from );

// Stream Pipes
ResultCode StreamPipeCreate (
	Handle OUT pipe );

ResultCode StreamPipeDispose (
	Handle IN pipe );

ResultCode StreamPipeGetReadStream ( 
	Handle IN pipe, 
	Handle OUT stream );
ResultCode StreamPipeGetWriteStream ( 
	Handle IN pipe, 
	Handle OUT stream );

// Message Pipe Access
ResultCode MessagePipeCreate (
	Handle OUT pipe );

ResultCode MessagePipeDispose (
	Handle IN pipe );

ResultCode MessagePipeSend ( 
	Handle IN pipe, 
	Message REF stream );

ResultCode MessagePipeReceive ( 
	Handle IN pipe,
	Message OUT stream );

ResultCode MessagePipeReceivePending (
	Handle IN pipe,
	Message OUT stream );

#if FALSE

// Processes, Modules and Threads
ResultCode ProcessCreate
ResultCode ProcessAttachModule
ResultCode ProcessCreateThread
ResultCode ThreadCreate
ResultCode ThreadSetAddress
ResultCode ThreadSuspend
ResultCode ThreadContinue
ResultCode ModuleLoad
ResultCode ModuleUnload
ResultCode ModuleGetExport

// Memory and Address Mapping
ResultCode MemoryAlloc
ResultCode MemoryRelease
ResultCode MemoryMapViewOfMemory
ResultCode MemoryMapViewOfFile
ResultCode MemoryUnmap

// Structured Exception Handling
ResultCode ExceptionPushHandler
ResultCode ExceptionPopHandler
ResultCode ExceptionSetSeverityFilter
ResultCode ExceptionAddResultTypeFilter
ResultCode ExceptionRaise
ResultCode ExceptionRaiseAfterDelay

// Synchronization
ResultCode MutexAlloc
ResultCode MutexDispose
ResultCode MutexAcquire
ResultCode MutexRelease
ResultCode SemaphoreAlloc
ResultCode SemaphoreDispose
ResultCode SemaphoreSetCounter
ResultCode SemaphoreCountDown
ResultCode SemaphoreCountUp
ResultCode WaitableQueueAlloc
ResultCode WaitableQueueDispose
ResultCode WaitableQueueWait
ResultCode WaitableQueueNotify
ResultCode WaitableQueueNotifyAll
// More to come…

#endif