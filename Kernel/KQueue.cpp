#include "Kernel.h"

void KQueueInit(QueueInfo* l, int structOffset)
{
	l->lock = 0;
	l->structOffset = structOffset;
	l->head = NULL;
	l->tail = NULL;
}

void KQueueEnqueue(QueueInfo* l, void* data)
{
	QueueItem * item = (QueueItem*)(((Byte*)data)+l->structOffset);

	KSpinLock(&(l->lock));

	if(l->tail)
	{
		l->tail->next = item;
	}
	else // if no tail, then list is empty so head needs to be assigned too
	{
		l->head = item;
	}
	item->next = NULL;
	l->tail = item;

	KSpinUnlock(&(l->lock));
}

void* KQueueDequeue(QueueInfo* l)
{
	KSpinLock(&(l->lock));

	QueueItem* ret = NULL;
	if(l->head)
	{
		ret = l->head;
		l->head = l->head->next;

		if(l->head==NULL)
			l->tail=NULL;
	}

	KSpinUnlock(&(l->lock));

	return (((Byte*)ret)-l->structOffset);
}
