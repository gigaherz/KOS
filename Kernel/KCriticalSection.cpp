#include "Kernel.h"

// win32-ish critical section

struct CriticalSection {

    Int32 AccessLock;		// Lock for accessing the Critical Section data
    Int32 UseLock;			// Status of the critical section
    Int32 RecursionCount;

    Handle OwningThread;

    //Handle LockSemaphore;

};

void KCriticalSectionPrepare(CriticalSection* cs)
{
	cs->AccessLock = 0;
	cs->UseLock = 0;
	cs->RecursionCount = 0;
	cs->OwningThread = 0;
}

void KCriticalSectionEnter(CriticalSection* cs)
{
	KSpinLock(&cs->AccessLock);
	if(cs->OwningThread == KThreadGetCurrentThreadId())
	{
		cs->RecursionCount++;
	}
	else
	{
		// Wait on Semaphore
	}
	KSpinUnlock(&cs->AccessLock);
}

void KCriticalSectionLeave(CriticalSection* cs)
{
	KSpinLock(&cs->AccessLock);
	if(cs->RecursionCount>0)
	{
		cs->RecursionCount--;
		KSpinUnlock(&cs->AccessLock);
		return;
	}
	else
	{
		cs->OwningThread=0;
		cs->UseLock=0;
		// Release Semaphore
	}
	KSpinUnlock(&cs->AccessLock);
}
