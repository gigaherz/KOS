#pragma once

#pragma warning(push)

#pragma warning(disable: 4200)

struct ElementEntry
{
	void *pointer;
	ElementEntry *next;
};

struct PoolHeader
{
	UInt32 poolID;
	UInt32 poolSize;
    UInt32 elementSize;
	ElementEntry *firstFree;
	ElementEntry elements[0];
};

PoolHeader *KPoolAllocate(UInt32 poolID, UInt32 size, UInt32 elementSize);
void *KPoolAcquireElement(PoolHeader *hdr);
void KPoolReleaseElement(PoolHeader *hdr, void* element);

UInt32 KPoolIndexOf(PoolHeader *hdr, void *element);
void* KPoolElementAt(PoolHeader *hdr, UInt32 index);

#pragma warning(pop)
