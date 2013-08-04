#include "Kernel.h"
#include "Multiboot.h"

// This is a simple allocator which does NOT yet use the x86 MMU

struct AllocEntry
{
UInt32 BlockSize :
    8; // bytes = 4kb << size
UInt32 Flags     :
    8;
UInt32 OwnerID   :
    16;
    UIntPtr AllocBase;
} *AllocationHeader;

UInt32 LastPageNumber; // Exclusive
UInt32 AllocationSize;

#define MEM_FLAG_USED 0x80
#define MEM_FLAG_FIXED 0x40

void KAllocatorMergeBuddies(bool showDebug=true) // Does NOT defragment memory.
{
    // Slow version of the code. Lazy compacting method checks ALL possible compactions bottom-up
    for (UInt32 b = 1; (1u<<b) < LastPageNumber; b++)
    {
        UInt32 width = (1<<b);
        UInt32 halfWidth = width>>1;
        for (UInt32 i = 0; i < LastPageNumber; i+=width)
        {
            if ((AllocationHeader[i].BlockSize == (b-1)) &&
                    (AllocationHeader[i+halfWidth].BlockSize == (b-1)) &&
                    (AllocationHeader[i].Flags == 0) &&
                    (AllocationHeader[i+halfWidth].Flags == 0))
            {
                //if(showDebug) KDebugPrintF(L"Merging 0x%08x into a block of level %d...\r\n", i<<12,b);
                // merge
                for (UInt32 j = i; j < (i+width); j++)
                {
                    AllocationHeader[i].BlockSize = b;
                }
            }
        }
    }

#if 0
    // Unfinished code. doesn't handle compacting when the previous block was B, the current buddies were (B-1), and we just compacted them into B again, creating a new match
    for (int i=0; i< LastPageNumber; )
    {
        int b = AllocationHeader[i].BlockSize;
        int width = 1<<b;

        int idx = i>>b;

        if ((AllocationHeader[i].Flags == 0) &&
                (AllocationHeader[i+width].Flags == 0) &&
                ((idx&1)==0))
        {
            int doubleWidth = width<<1;
            for (int j=0;j<doubleWidth;j++)
            {
                AllocationHeader[i].BlockSize = b+1;
            }
        }
        else i+=width; // even if we compacted, we could still
    }
#endif
}

void KPhysicalAllocatorInit(UInt32 low_mem, UInt32 high_mem, void *map_ptr, UInt32 map_size)
{
    UInt32 FirstFreePage = 0x00400000;

    KDebugPrintF(L"Low Mem is %d KB, High Mem is %d KB\r\n", low_mem, high_mem);

    UInt32  EndOfMem = ((high_mem + 1024) << 10);

    KDebugPrintF(L"End of usable ram: 0x%08x\r\n", EndOfMem);

    LastPageNumber = EndOfMem / 4096;
    AllocationSize = sizeof(AllocEntry) * LastPageNumber; // each entry refers to a 4kb page
    AllocationSize = (AllocationSize+ 4095) & (~4095);    // align to a page boundary

    AllocationHeader = (AllocEntry*)(FirstFreePage+0x80000000);

    KMemZero(AllocationHeader,AllocationSize);

    // mark the first 4mb as used
    for (int i=0; i < (0x0400); i++)
    {
        AllocationHeader[i].BlockSize = 10; // it's a 4mb block (22 bits - 12 bits of 4kb page size)
        AllocationHeader[i].Flags = MEM_FLAG_USED | MEM_FLAG_FIXED;
        AllocationHeader[i].OwnerID = 0; // system
    }

    // mark the first 4mb as used
    int AllocationPages = AllocationSize >> 12;
    for (int i=0x0400; i < (0x0400 + AllocationPages); i++)
    {
        AllocationHeader[i].BlockSize = 0;
        AllocationHeader[i].Flags = MEM_FLAG_USED | MEM_FLAG_FIXED;
        AllocationHeader[i].OwnerID = 1; // Memory Allocator
    }

    KAllocatorMergeBuddies(false);

#if 0
    for (MemoryMap* MapEntry = (MemoryMap*)map_ptr; (UInt8*)MapEntry <= ((UInt8*)map_ptr+map_size);
            (MemoryMap*)((UIntPtr)MapEntry + MapEntry->size + sizeof(MapEntry->size)))
    {
        KDebugPrint(L" - Memory Block at ");
        KDebugPrintF(L"0x%08x:%08x, size 0x%08x:%08x, type 0x%08x.",
                     MapEntry->base_addr_high, MapEntry->base_addr_low,
                     MapEntry->length_high, MapEntry->length_low,
                     MapEntry->type);
        KDebugPrint(L".\r\n");
    }
#endif
}

UIntPtr KPhysicalAllocatorAcquire(UInt32 num_pages, UInt32 ownerID, UInt32* allocated_pages)
{
    UInt32 minWidth = LastPageNumber;
    UInt32 minWidthIdx = 0xFFFFFFF;

    for (UInt32 i=0; i< LastPageNumber; )
    {
        UInt32 width = 1<<AllocationHeader[i].BlockSize;

        if (AllocationHeader[i].Flags == 0)
        {
            if ((width < minWidth) && (width >= num_pages))
            {
                minWidth = width;
                minWidthIdx = i;
            }
        }

        i+=width;
    }

    // Could use BSR/BSF isntructions for tis, but bleh
    UInt32 reqWidth = 0;
    while ((1u<<reqWidth)<num_pages)
        reqWidth++;

    if (minWidthIdx < 0xFFFFFFFF)
    {
        UInt32 abase = minWidthIdx<<12;
        for (UInt32 i = 0; i < reqWidth; i++)
        {
            AllocationHeader[i+minWidthIdx].BlockSize = reqWidth;
            AllocationHeader[i+minWidthIdx].Flags = MEM_FLAG_USED;
            AllocationHeader[i+minWidthIdx].OwnerID = ownerID;
            AllocationHeader[i+minWidthIdx].AllocBase = abase;
        }
        for (UInt32 i = reqWidth; i < minWidth; i++) // mark the rest of the block as free
        {
            AllocationHeader[i+minWidthIdx].BlockSize = 0;
            AllocationHeader[i+minWidthIdx].Flags = 0;
            AllocationHeader[i+minWidthIdx].OwnerID = 0;
            AllocationHeader[i+minWidthIdx].AllocBase = 0;
        }

        KAllocatorMergeBuddies();

        if (allocated_pages)
            *(allocated_pages) = 1<<reqWidth;

        return ((UIntPtr)minWidthIdx) << 12;
    }

    if (allocated_pages)
        *(allocated_pages) = 0;

    // Not enough free space... maybe I should use 0xFFFFFFFF as a invalid value instead?
    return 0;
}

void KPhysicalAllocatorRelease(UIntPtr alloc_base, UInt32 ownerID)
{
    int first_page = alloc_base >> 12;

    KAssert(AllocationHeader[first_page].AllocBase == alloc_base);

    for (int i=first_page; ; i++)
    {
        if (((AllocationHeader[i].Flags&0xC0) == 0x80) &&
                (AllocationHeader[i].OwnerID == ownerID) &&
                (AllocationHeader[i].AllocBase == alloc_base))
        {
            AllocationHeader[i].BlockSize = 0;
            AllocationHeader[i].Flags = 0;
            AllocationHeader[i].OwnerID = 0;
            AllocationHeader[i].AllocBase = 0;
        }
        else break;
    }

    KAllocatorMergeBuddies();
}