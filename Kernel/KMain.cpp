#include "Kernel.h"

#include "Multiboot.h"

extern "C" void __declspec(noreturn) __cdecl KernelStartup();

void KMain(MultibootInfo* mbinfo);

// YES, I need to turn MultibootFix into MultibootCreate, reading the PE headers for section info.
// I'm just extremely lazy.
const static MultibootHeader mbheader =
{
    0x1BADB002,
    0x00010007,
    0x00000000-0x1BADB002-0x00010007,
    ((UInt32)&mbheader)-0x80000000, // fix for the base trick
    0x00100000,
    0x00109000, // to be grown whenever the kernel size goes over it. extremely annoying.
    0x00300000, // 1mb of stack will be available from 0x300000 to 0x400000, for kernel use
    (UInt32)(KernelStartup)-0x80000000, // fix for the base trick
    1,
    80,
    25,
    0
};

void KMain(MultibootInfo* mbinfo)
{
    //KStrCpy(0, L"ERROR: Reading from NULL!!!");

    KSerialInit();
    KSerialPrint(L"Serial port initialized.\r\n"); // can't use debug prints until the serial port AND display are initialized, as DebugPrint uses both.

    if (!mbinfo)
    {
        KSerialPrint(L"This kernel requires a Multiboot compliant bootloader.\r\n");
        KSerialPrint(L"System halted.\r\n");
        return;
    }

    KSerialPrint(L"Pre-Initializing Interrupt Manager (low)... ");
    KInterruptInit(0);
    KInterruptEnable();
    KInterruptDisable();
    KSerialPrint(L"OK.\r\n");

    KSerialPrint(L"Initializing Memory Manager...\r\n");

    UInt32 LM=0;
    UInt32 HM=0;

    if (mbinfo->flags & 1)
    {
        LM = mbinfo->mem_lower;
        HM = mbinfo->mem_upper;
    }

    MemoryMap *map = NULL;
    if (mbinfo->flags & (1<<6))
    {
        map = (MemoryMap *)mbinfo->mmap_addr;
    }

    KMemoryInit(LM, HM, map, mbinfo->mmap_length);
    KSerialPrint(L"OK.\r\n");

    KSerialPrint(L"Re-Initializing Interrupt Manager (high)... ");
    KInterruptInit(1);

    KSerialPrint(L"Initializing display... ");
    KDisplayInit(/*(mbinfo->flags&(1<<11))?mbinfo->vbe_mode:0*/);
    KSerialPrint(L"OK.\r\n");

    KDisplayClear();
    KDisplayPrint(L"Display Initialized.\r\n");

    KKeyboardInit();
    KDebugPrint(L"Keyboard Initialized.\r\n");

    KTimerInit();

    KDebugPrint(L"Timer Initialized.\r\n");
    KDebugPrint(L"Enabling interrupts...");

    KInterruptEnable();
    //__asm int 3

    KDebugPrint(L"OK.\r\n");

    KRTCInit();

    KDisplayPrint(L"Entering idleness loop...\r\n");
    KDisplayPrint(L"(you can press some keys to test the interrupt system...)\r\n");

    UInt32 oldTicks = KTimerGetTicks();
    UInt32 nSeconds = 0;
    while (true)
    {
        if (KKeyboardHasInput())
        {
            KDisplayPrint(L"\r                                ");
            KDisplayPrintF(L"\rKey Pressed: %c\r\n", KKeyboardReadChar());
        }
        else
        {
            KCpuIdleWait();

            UInt32 newTicks = KTimerGetTicks();
            if ((newTicks-oldTicks) >= 1000)
            {
                nSeconds++;

                DateTime dt = KRTCGetDateTime();

                KDisplayPrint(L"\r                                ");
                KDisplayPrintF(L"\r%02d:%02d:%02d",nSeconds, dt.hour, dt.minute, dt.second);
                oldTicks = newTicks;
            }
        }
    }
}
