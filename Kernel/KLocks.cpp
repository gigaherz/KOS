#include "Kernel.h"

void KSpinLock(Int32 volatile * lock)
{
    // try to acquire lock
    while (KInterlockedExchange(lock,1)!=0)
    {
        // if it's acquired by another thread,
        // wait until it becomes free before trying again
        while (*lock!=0)
        {
            //KThreadYield();
        }
    }
}

void KSpinUnlock(Int32 volatile * lock)
{
    *lock=0;
}

void KSeqLockedWriteEnter(Int32 volatile *lock, Int32 volatile *counter)
{
    KSpinLock(lock);
    KInterlockedIncrement(counter);
}

void KSeqLockedWriteExit(Int32 volatile *lock, Int32 volatile *counter)
{
    KInterlockedIncrement(counter);
    KSpinUnlock(lock);
}

void KSeqLockedReadBegin(Int32 volatile *lock, Int32 volatile *counter, Int32 volatile *pre_count)
{
    *pre_count = *counter;
}

// returns True if the read is consistent
Bool KSeqLockedReadEnd(Int32 volatile *lock, Int32 volatile *counter, Int32 volatile *pre_count)
{
    Int32 pre = *pre_count;
    Int32 post = *counter;
    return !((pre!=post) || (pre&1) || (post&1));
}
