#pragma once

// Represents an item in a queue.
// The actual items need to have enough space somewhere to hold the pointer to the next item.
// The pointer offset is specified to the initialization function, and must be constant.
struct QueueItem
{
    QueueItem* next;
};

struct QueueInfo
{
    Int32 lock;
    UIntPtr structOffset;
    QueueItem* head;
    QueueItem* tail;
};

void KQueueInit(QueueInfo* l, int structOffset);
void KQueueEnqueue(QueueInfo* l, void* data);
void* KQueueDequeue(QueueInfo* l);