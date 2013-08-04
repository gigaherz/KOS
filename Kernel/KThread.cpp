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
