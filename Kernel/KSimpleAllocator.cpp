#include "Kernel.h"

void MapMultiplePages(UInt32 virtual_address, UInt32 physical_address, UInt32 page_count, bool writable, bool userMode, bool global);

///////////////////////////////////////////////////////////////////////////////
//
//  Virtual Memory Allocator
//

struct MemoryArea {
	UInt32    base;
	UInt32    size;
	MemoryArea *prev;
	MemoryArea *next;
};

MemoryArea *Areas;
UInt32 NextUnused;

MemoryArea *FirstUnused;	// Unsorted
MemoryArea *FirstFree;		// Sorted smallest first
MemoryArea *FirstUsed;		// Sorted by start address

const UInt32 totalPages = 0x80000000 / 4096;
const UInt32 pages = (totalPages * sizeof(MemoryArea) / 4096);

MemoryArea *GetUnused()
{
	MemoryArea *area;

	if (FirstUnused != NULL)
	{
		area = FirstUnused;
		FirstUnused = area->next;
	}
	else
	{
		KAssert(NextUnused < totalPages);

		area = &(Areas[NextUnused++]);
	}

	KMemZero(area, sizeof(MemoryArea));
	return area;
}

void PutUnused(MemoryArea *area)
{
	KMemZero(area, sizeof(MemoryArea));
	area->next = FirstUnused;
	FirstUnused = area;
}

MemoryArea* FindFreeOverlapping(UInt32 start, UInt32 count)
{
	MemoryArea * area = FirstFree;

	while (area != NULL)
	{
		if (area->base <= start + count || area->base + area->size >= start)
		{
			return area;
		}

		area = area->next;
	}

	return NULL;
}

MemoryArea* FindFreeBySize(UInt32 count)
{
	MemoryArea * area = FirstFree;

	while (area != NULL)
	{
		if (area->size >= count)
		{
			return area;
		}

		area = area->next;
	}

	return NULL;
}

void RemoveFree(MemoryArea *area)
{
	if (area->prev)
	{
		area->prev->next = area->next;
	}
	else
	{
		FirstFree = area->next;
	}

	if (area->next)
	{
		area->next->prev = area->prev;
	}

	area->prev = NULL;
	area->next = NULL;
}

void AddFree(MemoryArea *area)
{
	MemoryArea *nf = FirstFree;
	MemoryArea *pf = NULL;

	while (nf != NULL)
	{
		if (nf->size > area->size)
		{
			break;
		}

		pf = nf;
		nf = nf->next;
	}

	area->prev = pf;
	area->next = nf;

	if (pf)
	{
		pf->next = area;
	}
	else
	{
		FirstFree = area;
	}

	if (nf)
	{
		nf->prev = area;
	}
}

MemoryArea* FindUsedByAddress(UInt32 start)
{
	MemoryArea * area = FirstUsed;

	while (area != NULL)
	{
		if (area->base == start)
		{
			return area;
		}

		if (area->base > start)
		{
			return NULL;
		}

		area = area->next;
	}

	return NULL;
}

void RemoveUsed(MemoryArea *area)
{
	if (area->prev)
	{
		area->prev->next = area->next;
	}
	else
	{
		FirstUsed = area->next;
	}

	if (area->next)
	{
		area->next->prev = area->prev;
	}

	area->prev = NULL;
	area->next = NULL;
}

void AddUsed(MemoryArea *area)
{
	MemoryArea *nf = FirstUsed;
	MemoryArea *pf = NULL;

	while (nf != NULL)
	{
		if (nf->base > area->base)
		{
			break;
		}

		pf = nf;
		nf = nf->next;
	}

	area->prev = pf;
	area->next = nf;

	if (pf)
	{
		pf->next = area;
	}
	else
	{
		FirstUsed = area;
	}

	if (nf)
	{
		nf->prev = area;
	}
}

// removes count pages from the beginning of the area
MemoryArea *Take(MemoryArea *area, UInt32 count)
{
	MemoryArea *newArea = GetUnused();

	newArea->base = area->base;
	newArea->size = count;

	area->base += count;
	area->size -= count;

	return newArea;
}

void KVirtualMarkUsed(UInt32 start, UInt32 count)
{
	MemoryArea *area = FindFreeOverlapping(start, count);

	if (area == NULL)
		return;
		
	KAssert(area->base <= start && area->base + area->size <= start + count);

	RemoveFree(area);

	if (area->base < start)
	{
		MemoryArea *newArea = Take(area, start - area->base);

		AddFree(newArea);
	}

	MemoryArea *usedArea = area;

	if (area->size > count)
	{
		usedArea = Take(area, count);

		AddFree(area);
	}

	AddUsed(usedArea);
}

void KSimpleAllocatorInit(UInt32 baseAddress, UInt32 *pageCount)
{
	// Assume there will be <pages> pages available AFTER calling this function, and that there's ONE page at the moment

	*pageCount = pages;

	Areas = (MemoryArea*) baseAddress;

	FirstFree = Areas;

	FirstFree->next = 0;
	FirstFree->base = 0x80000000;
	FirstFree->size = totalPages;

	FirstUsed = NULL;
	FirstUnused = NULL;
	NextUnused = 1;

	KVirtualMarkUsed(baseAddress, pages);
}

//UIntPtr KVirtualReserve(UInt32 num_pages, UInt32 ownerID, UInt32* allocated_pages);

UIntPtr KVirtualAcquire(UInt32 count, UInt32 ownerID)
{
	MemoryArea *area = FindFreeBySize(count);

	if (area == NULL)
	{
		return 0;
	}

	RemoveFree(area);

	MemoryArea *usedArea = area;

	if (area->size > count)
	{
		usedArea = Take(area, count);

		AddFree(area);
	}

	AddUsed(usedArea);

	UInt32 actualCount;

	UInt32 physicalBase = KPhysicalAllocatorAcquire(count, ownerID, &actualCount);

	MapMultiplePages(usedArea->base, physicalBase, count, true, false, true);
	
	return area->base;
}

void KVirtualRelease(UIntPtr alloc_base, UInt32 ownerID);
