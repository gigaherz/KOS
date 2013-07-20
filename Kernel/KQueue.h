#pragma once

struct QueueItem // Abstract: anything could be used just as long as the first sizeof(QueueItem*) bytes at the pointer are the next pointer
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