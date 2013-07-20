#include "Kernel.h"

#define NUM_THREADS 128

struct ThreadInfo
{
	Regs regs;
};

ThreadInfo threads[NUM_THREADS];

UInt32 threadRunning;	// the running thread
UInt32 threadReadyHead;	// the queue of threads ready to run
UInt32 threadWaitHead;	// the queue of threads waiting for something to complete
