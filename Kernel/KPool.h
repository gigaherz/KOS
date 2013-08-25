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

UInt32 IndexOf(PoolHeader *hdr, void *element);
void* ElementAt(PoolHeader *hdr, UInt32 index);
PoolHeader *KPoolAllocate(UInt32 poolID, UInt32 size, UInt32 elementSize);
void *KPoolAcquireElement(PoolHeader *hdr);
void KPoolReleaseElement(PoolHeader *hdr, void* element);

#pragma warning(pop)
