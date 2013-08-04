#include "Kernel.h"

#define CR0_PG  31

#define CR3_PSE 4
#define CR3_PAE 5

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 32bit Page Table (PAE off)
//
struct PageEntry32_4kb
{
	UInt32 present:1;
	UInt32 writable:1;
	UInt32 supervisor:1;
	UInt32 writeThrough:1;
	UInt32 uncached:1;
	UInt32 accessed:1;
	UInt32 dirty:1;
	UInt32 attribute:1;
	UInt32 global:1; // ignored
	UInt32 system:3;
	UInt32 base:20;
};
KCTAssert(sizeof(PageEntry32_4kb)==4);

struct PageEntry32_4mb
{
	UInt32 present:1;
	UInt32 writable:1;
	UInt32 supervisor:1;
	UInt32 writeThrough:1;
	UInt32 uncached:1;
	UInt32 pageSize:1;
	UInt32 dirty:1;
	UInt32 attribute:1;
	UInt32 global:1; // ignored
	UInt32 system:3;
	UInt32 accessed:1;
	UInt32 reserved:9;
	UInt32 base:10;
};
KCTAssert(sizeof(PageEntry32_4mb)==4);

struct PageEntry32_4mb_pse
{
	UInt32 present:1;
	UInt32 writable:1;
	UInt32 supervisor:1;
	UInt32 writeThrough:1;
	UInt32 uncached:1;
	UInt32 pageSize:1;
	UInt32 dirty:1;
	UInt32 attribute:1;
	UInt32 global:1; // ignored
	UInt32 system:3;
	UInt32 accessed:1;
	UInt32 base_high:4;
	UInt32 reserved:5;
	UInt32 base_low:10;
};
KCTAssert(sizeof(PageEntry32_4mb_pse)==4);

struct PageEntry32_notPresent
{
	UInt32 present:1;
	UInt32 system:31;
};
KCTAssert(sizeof(PageEntry32_notPresent)==4);

union PageEntry32
{
	PageEntry32_4kb fk;
	PageEntry32_4mb fm;
	PageEntry32_4mb_pse fm_pse;
	PageEntry32_notPresent np;
	UInt32 whole;
};
KCTAssert(sizeof(PageEntry32)==4);

struct PageTable32
{
	PageEntry32 entry[1024];
};
KCTAssert(sizeof(PageTable32)==4096);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 32bit Page Directory (PAE off)
//
struct PageDirectoryEntry32
{
	UInt32 present:1;
	UInt32 writable:1;
	UInt32 supervisor:1;
	UInt32 writeThrough:1;
	UInt32 uncached:1;
	UInt32 accessed:1;
	UInt32 available:1;
	UInt32 pagesize:1;
	UInt32 ignored:1; // ignored
	UInt32 system:3;
	UInt32 base:20;
};
KCTAssert(sizeof(PageDirectoryEntry32)==4);

struct PageDirectory32
{
	PageDirectoryEntry32 table[1024];
};
KCTAssert(sizeof(PageDirectory32)==4096);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 64bit Page Table (PAE on)
//
struct PageEntry64_4kb
{
	UInt32 present:1;
	UInt32 writable:1;
	UInt32 supervisor:1;
	UInt32 writeThrough:1;
	UInt32 uncached:1;
	UInt32 accessed:1;
	UInt32 dirty:1;
	UInt32 attribute:1;
	UInt32 global:1; // ignored
	UInt32 system:3;
	UInt32 base_low:20;
	UInt32 base_high;
};
KCTAssert(sizeof(PageEntry64_4kb)==8);

struct PageEntry64_2mb
{
	UInt32 present:1;
	UInt32 writable:1;
	UInt32 supervisor:1;
	UInt32 writeThrough:1;
	UInt32 uncached:1;
	UInt32 pageSize:1;
	UInt32 dirty:1;
	UInt32 attribute:1;
	UInt32 global:1;
	UInt32 system:3;
	UInt32 accessed:1;
	UInt32 reserved:8;
	UInt32 base_low:11;
	UInt32 base_high;
};
KCTAssert(sizeof(PageEntry64_2mb)==8);

struct PageEntry64_notPresent
{
	UInt32 present:1;
	UInt32 system:31;
	UInt32 system_high;
};
KCTAssert(sizeof(PageEntry64_notPresent)==8);

union PageEntry64
{
	PageEntry64_4kb fk;
	PageEntry64_2mb fm;
	PageEntry64_notPresent np;
	UInt64 whole;
};
KCTAssert(sizeof(PageEntry64)==8);

struct PageTable64
{
	PageEntry64 entry[512];
};
KCTAssert(sizeof(PageTable64)==4096);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 64bit Page Directory entries (PAE on)
//
struct PageDirectoryEntry64
{
	UInt32 present:1;
	UInt32 writable:1;
	UInt32 supervisor:1;
	UInt32 writeThrough:1;
	UInt32 uncached:1;
	UInt32 accessed:1;
	UInt32 available:1;
	UInt32 zero:1;
	UInt32 ignored:1;
	UInt32 system:3;
	UInt32 base_low:20;
	UInt32 base_high;
};
KCTAssert(sizeof(PageDirectoryEntry64)==8);

struct PageDirectory64
{
	PageDirectoryEntry64 table[512];
};
KCTAssert(sizeof(PageDirectory64)==4096);

struct PageDirectoryPointerEntry64
{
	UInt32 present:1;
	UInt32 reserved0:2;
	UInt32 writeThrough:1;
	UInt32 uncached:1;
	UInt32 reserved1:4;
	UInt32 system:3;
	UInt32 base_low:20;
	UInt32 base_high;
};
KCTAssert(sizeof(PageDirectoryPointerEntry64)==8);

struct PageDirectoryPointers64
{
	PageDirectoryPointerEntry64* directory[4];
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Control functions
//

extern volatile UInt32 ReadCR0();
extern volatile UInt32 ReadCR3();
extern void WriteCR0(UInt32 value);
extern void WriteCR3(UInt32 value);

volatile PageDirectory32* page_directory;
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

	if(!dir->present)
		return 0;

	UInt32 tableAddress = dir->base << 12;

	volatile PageTable32* table = (PageTable32*)tableAddress;

	if(page_table)
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
	if(!dir->present)
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
}

void UnmapPage(UInt32 virtual_address)
{
	volatile PageEntry32* entry = FindPageEntry(virtual_address,0);
	if(!entry)
		return;

	ClearPageEntry(entry);
}

void MapMultiplePages(UInt32 virtual_address, UInt32 physical_address, UInt32 page_count, bool writable, bool userMode, bool global)
{
	for(UInt32 i=0;i<page_count;i++)
	{
		MapPage(virtual_address,physical_address,writable,userMode, global);
		virtual_address+=4096;
		physical_address+=4096;
	}
}

void KVirtualAllocatorInit()
{
	page_directory = (PageDirectory32 *)0x80280000;

	// Clear page directory
	KSerialPrintF(L"Clearing Page directory p1...\r\n");
	KMemSet((UInt8*)page_directory, 0, 0);
	KSerialPrintF(L"Clearing Page directory p2...\r\n");
	KMemSet((UInt8*)page_directory, 0, 4096);

	//// Map non-allocable stuff from the bottom of the ram into the upper 2gb of the kernel address space
	//KSerialPrint(L"Maping 0-based ranges...");
	//// map the first 512KB of memory
	//KSerialPrint(L"0");
	//MapMultiplePages(0x00000000, 0x00000000, 128, false, false, false);

	//// map the second 512KB of memory
	//KSerialPrint(L"\r\n1");
	//MapMultiplePages(0x00080000, 0x00080000, 128, true, false, false);

	//// map the second 1MB of memory
	//KSerialPrint(L"\r\n2");
	//MapMultiplePages(0x00100000, 0x00100000, 128, false, false, false);
	//KSerialPrint(L"\r\n3");
	//MapMultiplePages(0x00180000, 0x00180000, 128, true, false, false);
	//KSerialPrint(L"\r\n");
	
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

void KVirtualAllocatorInitFinish()
{

}

void KVirtualEnable()
{
	// write_cr3, read_cr3, write_cr0, and read_cr0 all come from the assembly functions
	WriteCR3((UInt32)page_directory); // put that page directory address into CR3
	volatile UInt32 oldCR0 = ReadCR0();
	KDebugPrintF(L"Old CR0: 0x%08x\r\n",oldCR0);

	WriteCR0(oldCR0 | 0x80000000); // set the paging bit in CR0 to 1

	// go celebrate or something 'cause PAGING IS ENABLED!!!!!!!!!!
}

UIntPtr KVirtualAllocatorReserve(UInt32 num_pages, UInt32 ownerID, UInt32* allocated_pages);
void KVirtualAllocatorRelease(UIntPtr alloc_base, UInt32 ownerID);

UIntPtr KVirtualAllocatorMap(UIntPtr virtual_base, UIntPtr physical_base, UInt32 page_count);

UIntPtr KVirtualAllocatorAcquire(UInt32 num_pages, UInt32 ownerID);
