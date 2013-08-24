#include "Kernel.h"

#pragma pack(1)
struct GDT_Entry
{
    UInt16 Limit_Low;
    UInt16 Base_Low;
    UInt8  Base_Middle;
    UInt8  Access;
    UInt8  Granularity;
    UInt8  Base_High;
};

/* Our GDT, with some entries */
struct GDT_Entry GDT[3];

#define TYPE_EX 0x08
#define TYPE_DC 0x04
#define TYPE_RW 0x02
#define TYPE_AC 0x01

#define RING_0  0x00
#define RING_1  0x20
#define RING_2  0x40
#define RING_3  0x60

#define PRESENT 0x80

#define DTYPE_SEGMENT 0x10

#define GRANULARITY_1BYTE 0x00
#define GRANULARITY_4KBYTE 0x80

#define OPERAND_SIZE_16 0x00
#define OPERAND_SIZE_32 0x40

#define ACCESS_TSS 0x89
#define GRANULARITY_TSS 0x40

void BuildGDTEntry(GDT_Entry *dest, UInt32 base, UInt32 limit, UInt32 access, UInt32 granularity)
{
    dest->Limit_Low   = limit;
    dest->Base_Low    = base;
    dest->Base_Middle = base>>16;
    dest->Access      = access;
    dest->Granularity = granularity&0xF0 | ((limit>>16)&0xF);
    dest->Base_High   = base>>24;
}

void __cdecl FlushGDT(void* base, int limit);

// Isntall new GDT
void InstallGDT()
{
    /* Our NULL descriptor */
    BuildGDTEntry(GDT+0, 0,0x00000000,0x00,0x00);

    /* Code Segment descriptor */
    BuildGDTEntry(GDT+1,
                  0x00000000,
                  0xFFFFFFFF,
                  PRESENT|DTYPE_SEGMENT|RING_0|TYPE_RW|TYPE_EX,
                  GRANULARITY_4KBYTE|OPERAND_SIZE_32
                 );

    /* Data Segment descriptor */
    BuildGDTEntry(GDT+2,
                  0x00000000,
                  0xFFFFFFFF,
                  PRESENT|DTYPE_SEGMENT|RING_0|TYPE_RW,
                  GRANULARITY_4KBYTE|OPERAND_SIZE_32
                 );

    /* Task State Segment descriptor - TODO */
    //BuildGDTEntry(GDT+3, (UInt32)&TSS,sizeof(TSS),ACCESS_TSS,GRANULARITY_TSS);

    // TODO: Add ring3 segment descriptors

    FlushGDT(&GDT, sizeof(GDT)-1);
}

void KMemoryInit(UInt32 low_mem, UInt32 high_mem, void *map_ptr, UInt32 map_size)
{
    KSerialPrint(L"Initializing Physical allocator...\r\n");
    KPhysicalAllocatorInit(low_mem, high_mem, map_ptr, map_size);
    KSerialPrint(L"Initializing Virtual memory...\r\n");
    KVirtualBeginInit();
    KSerialPrint(L"Enabling Virtual memory...\r\n");
    KVirtualEnable();
    KSerialPrint(L"Installing new GDT...\r\n");
    InstallGDT();
    KVirtualFinishInit();
    KSerialPrint(L"GDT installed.\r\n");
}
