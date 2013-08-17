#pragma once

//////////////////////////////////////////////////////////////////////////
// Memory
void KMemoryInit(UInt32 low_mem, UInt32 high_mem, void *map_ptr, UInt32 map_size);

//////////////////////////////////////////////////////////////////////////
// Physical memory
void KPhysicalAllocatorInit(UInt32 low_mem, UInt32 high_mem, void *map_ptr, UInt32 map_size);

UIntPtr KPhysicalAllocatorAcquire(UInt32 num_pages, UInt32 ownerID, UInt32* allocated_pages);
void KPhysicalAllocatorRelease(UIntPtr alloc_base, UInt32 ownerID);

//////////////////////////////////////////////////////////////////////////
// Virtual Memory
void KVirtualAllocatorInit();

UIntPtr KVirtualAllocatorReserve(UInt32 num_pages, UInt32 ownerID, UInt32* allocated_pages);
void KVirtualAllocatorRelease(UIntPtr alloc_base, UInt32 ownerID);

UIntPtr KVirtualAllocatorMap(UIntPtr virtual_base, UIntPtr physical_base, UInt32 page_count);

UIntPtr KVirtualAllocatorAcquire(UInt32 num_pages, UInt32 ownerID);

void KVirtualEnable();
void KVirtualAllocatorInitFinish();

