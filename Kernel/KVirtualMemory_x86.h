#pragma once

#define CR0_PG  31
#define CR0_PAGING (1<<CR0_PG)

#define CR3_PSE 4
#define CR3_PAE 5

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Control functions
//

extern volatile UInt32 ReadCR0();
extern volatile UInt32 ReadCR3();
extern void WriteCR0(UInt32 value);
extern void WriteCR3(UInt32 value);
extern void InvalidateTlbEntry(UInt32 address);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 32bit Page Table (PAE off)
//
struct PageEntry32_4kb
{
UInt32 present:
    1;
UInt32 writable:
    1;
UInt32 supervisor:
    1;
UInt32 writeThrough:
    1;
UInt32 uncached:
    1;
UInt32 accessed:
    1;
UInt32 dirty:
    1;
UInt32 attribute:
    1;
UInt32 global:
    1; // ignored
UInt32 system:
    3;
UInt32 base:
    20;
};
KCTAssert(sizeof(PageEntry32_4kb)==4);

struct PageEntry32_4mb
{
UInt32 present:
    1;
UInt32 writable:
    1;
UInt32 supervisor:
    1;
UInt32 writeThrough:
    1;
UInt32 uncached:
    1;
UInt32 pageSize:
    1;
UInt32 dirty:
    1;
UInt32 attribute:
    1;
UInt32 global:
    1; // ignored
UInt32 system:
    3;
UInt32 accessed:
    1;
UInt32 reserved:
    9;
UInt32 base:
    10;
};
KCTAssert(sizeof(PageEntry32_4mb)==4);

struct PageEntry32_4mb_pse
{
UInt32 present:
    1;
UInt32 writable:
    1;
UInt32 supervisor:
    1;
UInt32 writeThrough:
    1;
UInt32 uncached:
    1;
UInt32 pageSize:
    1;
UInt32 dirty:
    1;
UInt32 attribute:
    1;
UInt32 global:
    1; // ignored
UInt32 system:
    3;
UInt32 accessed:
    1;
UInt32 base_high:
    4;
UInt32 reserved:
    5;
UInt32 base_low:
    10;
};
KCTAssert(sizeof(PageEntry32_4mb_pse)==4);

struct PageEntry32_notPresent
{
UInt32 present:
    1;
UInt32 system:
    31;
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
UInt32 present:
    1;
UInt32 writable:
    1;
UInt32 supervisor:
    1;
UInt32 writeThrough:
    1;
UInt32 uncached:
    1;
UInt32 accessed:
    1;
UInt32 available:
    1;
UInt32 pagesize:
    1;
UInt32 ignored:
    1; // ignored
UInt32 system:
    3;
UInt32 base:
    20;
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
UInt32 present:
    1;
UInt32 writable:
    1;
UInt32 supervisor:
    1;
UInt32 writeThrough:
    1;
UInt32 uncached:
    1;
UInt32 accessed:
    1;
UInt32 dirty:
    1;
UInt32 attribute:
    1;
UInt32 global:
    1; // ignored
UInt32 system:
    3;
UInt32 base_low:
    20;
    UInt32 base_high;
};
KCTAssert(sizeof(PageEntry64_4kb)==8);

struct PageEntry64_2mb
{
UInt32 present:
    1;
UInt32 writable:
    1;
UInt32 supervisor:
    1;
UInt32 writeThrough:
    1;
UInt32 uncached:
    1;
UInt32 pageSize:
    1;
UInt32 dirty:
    1;
UInt32 attribute:
    1;
UInt32 global:
    1;
UInt32 system:
    3;
UInt32 accessed:
    1;
UInt32 reserved:
    8;
UInt32 base_low:
    11;
    UInt32 base_high;
};
KCTAssert(sizeof(PageEntry64_2mb)==8);

struct PageEntry64_notPresent
{
UInt32 present:
    1;
UInt32 system:
    31;
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
UInt32 present:
    1;
UInt32 writable:
    1;
UInt32 supervisor:
    1;
UInt32 writeThrough:
    1;
UInt32 uncached:
    1;
UInt32 accessed:
    1;
UInt32 available:
    1;
UInt32 zero:
    1;
UInt32 ignored:
    1;
UInt32 system:
    3;
UInt32 base_low:
    20;
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
UInt32 present:
    1;
UInt32 reserved0:
    2;
UInt32 writeThrough:
    1;
UInt32 uncached:
    1;
UInt32 reserved1:
    4;
UInt32 system:
    3;
UInt32 base_low:
    20;
    UInt32 base_high;
};
KCTAssert(sizeof(PageDirectoryPointerEntry64)==8);

struct PageDirectoryPointers64
{
    PageDirectoryPointerEntry64* directory[4];
};
