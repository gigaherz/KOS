#include "Kernel.h"

Int32 KThreadGetCurrentThreadId()
{
    // TODO: Implement kernel threads.
    // Until then, always return the root thread id.
    return 0;
}

ResultCode KThreadSuspend(Int32 threadId)
{
    // TODO: Return OK if successful, or ThreadAlreadySuspended if the thread was already in a suspended state
    return NotImplemented;
}

ResultCode KThreadResume(Int32 threadId)
{
    // TODO: Return OK if successful, or ThreadNotSuspended if the thread was not in a suspended state
    return NotImplemented;
}

void KThreadYield()
{
	// TODO: Yield to another thread, or return immediately if no other threads are ready
}

// TODO:

Int32 KThreadCreate(/* process context, heap size, stack size */)
{

	return 0;
}

void KThreadSetAddress(/* changes the memory address of the next instruction to run */)
{
}

void KThreadSetCpuContext(/* cpu context */)
{
}

void KT_SaveCurrentState()
{
}

void KT_RestoreState()
{
}