#include "Kernel.h"
#include "KVirtualMemory_x86.h"

volatile PageDirectory32* page_directory;

Bool pagingEnabled = false;

void ClearPageEntry(volatile PageEntry32* entry)
{
    entry->whole = 0;
}

void SetPageEntry(volatile PageEntry32* entry, UInt32 physical_addr, bool writable, bool userMode, bool global)
{
    entry->whole = 0;
    entry->fk.present = 1;
    entry->fk.writable = writable;
    entry->fk.supervisor = !userMode;
    entry->fk.global = global;
    entry->fk.base = physical_addr>>12;
}

void SetDirectoryEntry(volatile PageDirectoryEntry32* entry, UInt32 physical_addr, bool writable, bool userMode)
{
    *(UInt32*)entry = 0;
    entry->present = 1;
    entry->writable = writable;
    entry->supervisor = !userMode;
    entry->base = physical_addr>>12;
}

volatile PageEntry32* FindPageEntry(UInt32 address, volatile PageTable32** page_table)
{
    UInt32 pageNumber = (address & 0x003FF000)>>12;
    UInt32 directory  = (address & 0xFFC00000)>>22;

    volatile PageDirectoryEntry32* dir = &(page_directory->table[directory]);

    if (!dir->present)
        return 0;

    UInt32 tableAddress = dir->base << 12;

    volatile PageTable32* table = (PageTable32*)tableAddress;

    if (page_table)
        *page_table = table;

    return &table->entry[pageNumber];
}

// Checks the existence of an entry,
// and creates the appropriate page table if necessary
volatile PageEntry32* CreatePagePath(UInt32 address)
{
    UInt32 pageNumber = (address & 0x003FF000)>>12;
    UInt32 directory  = (address & 0xFFC00000)>>22;

    volatile PageDirectoryEntry32* dir = &(page_directory->table[directory]);

    UInt32 tableAddress;
    if (!dir->present)
    {
        UInt32 size;
        tableAddress = KPhysicalAllocatorAcquire(1,1,&size);

        // keep entries > 2gb set to kernel mode.
        SetDirectoryEntry(dir, tableAddress, true, (directory<512));
    }
    else tableAddress = dir->base << 12;

    PageTable32* table = (PageTable32*)tableAddress;

    return &table->entry[pageNumber];
}

void MapPage(UInt32 virtual_address, UInt32 physical_address, bool writable, bool userMode, bool global)
{
    virtual_address &= 0xFFFFF000;
    physical_address &= 0xFFFFF000;
    volatile PageEntry32* entry = CreatePagePath(virtual_address);

    SetPageEntry(entry,physical_address,writable,userMode,global);

	if (pagingEnabled)
		InvalidateTlbEntry(virtual_address);
}

void UnmapPage(UInt32 virtual_address)
{
    volatile PageEntry32* entry = FindPageEntry(virtual_address,0);
    if (!entry)
        return;

    ClearPageEntry(entry);

	if (pagingEnabled)
		InvalidateTlbEntry(virtual_address);
}

void MapMultiplePages(UInt32 virtual_address, UInt32 physical_address, UInt32 page_count, bool writable, bool userMode, bool global)
{
    for (UInt32 i=0;i<page_count;i++)
    {
        MapPage(virtual_address,physical_address,writable,userMode, global);
        virtual_address+=4096;
        physical_address+=4096;
    }
}

void MapMultiplePages2(UInt32 virtual_address, UInt32* physical_addresses, UInt32 page_count, bool writable, bool userMode, bool global)
{
	for (UInt32 i = 0; i < page_count; i++)
	{
		MapPage(virtual_address, physical_addresses[i], writable, userMode, global);
		virtual_address += 4096;
	}
}

void KVirtualBeginInit()
{
    page_directory = (PageDirectory32 *)0x80280000;

    // Clear page directory
    KSerialPrintF(L"Clearing Page directory p1...\r\n");
    KMemSet((UInt8*)page_directory, 0, 4096);

    // Map non-allocable stuff from the bottom of the ram into the upper 2gb of the kernel address space
    KSerialPrint(L"Maping 2gb-based ranges...\r\n");
    // map the first 512KB of memory
    MapMultiplePages(0x80000000, 0x00000000, 128, false, false, true);

    // map the second 512KB of memory
    MapMultiplePages(0x80080000, 0x00080000, 128, true, false, true);

    // map the second 1MB of memory
    MapMultiplePages(0x80100000, 0x00100000, 128, false, false, true);
    MapMultiplePages(0x80180000, 0x00180000, 128, true, false, true);

    KSerialPrint(L"Done mapping.\r\n");

}

void KVirtualFinishInit()
{
	KSerialPrint(L"Preparing the memory allocator...\r\n");

	UInt32 neededPages = 0;

	// see KMain: 
	MapMultiplePages(0x80400000, 0x00400000, 1, true, false, true);

	KSimpleAllocatorInit(0x80200000, &neededPages);

	MapMultiplePages(0x80401000, 0x00401000, neededPages - 1, true, false, true);

	KSerialPrint(L"Memory allocator ready.\r\n");
}

void KVirtualEnable()
{
    WriteCR3((UInt32)page_directory);

	UInt32 oldCR0 = ReadCR0();

    WriteCR0(oldCR0 | CR0_PAGING);	
}
