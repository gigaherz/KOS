#include "Kernel.h"

const int MAX_WAITERS = 32;

struct SemaphoreQueueItem
{
    QueueItem item;

    Int32 threadId;

};

struct SemaphoreInfo
{
    Int32 lock;

    SemaphoreQueueItem items[MAX_WAITERS]; // To be replaced with something of variable size once Heaps are implemented
    QueueInfo freeQueue;

    QueueInfo queue;

    UInt32 counter;
};


void KSemaphoreInit(SemaphoreInfo* sema, UInt32 initialCount /* , UInt32 maxWaiters */)
{
    KQueueInit(&sema->queue,offsetof(SemaphoreQueueItem,item));
    KQueueInit(&sema->freeQueue,offsetof(SemaphoreQueueItem,item));

    for (int i=0;i<MAX_WAITERS;i++)
    {
        KQueueEnqueue(&sema->freeQueue,&sema->items[i]);
    }

    sema->counter = initialCount;
}

void KSemaphoreWait(SemaphoreInfo* sema)
{
    KSpinLock(&sema->lock);

    if (sema->counter > 0)
    {
        sema->counter--;
    }
    else
    {
        SemaphoreQueueItem* item = (SemaphoreQueueItem*)KQueueDequeue(&(sema->freeQueue));
        Int32 threadId = KThreadGetCurrentThreadId();
        item->threadId = threadId;
        KQueueEnqueue(&sema->queue, item);
        KSpinUnlock(&sema->lock);
        KThreadSuspend(threadId);
        KSpinLock(&sema->lock);
    }
    KSpinUnlock(&sema->lock);
}