#include "Kernel.h"

/* This is a simple string array. It contains the message that
*  corresponds to each and every exception. We get the correct
*  message by accessing like: Interrupt_Unhandled[interrupt_number] */
KCString Interrupt_Unhandled[48] =
{
    // 0 to 3
    L"Division By Zero",
    L"Debug",
    L"Non Maskable Interrupt",
    L"Breakpoint",

    // 4 to 7
    L"Into Detected Overflow",
    L"Out of Bounds",
    L"Invalid Opcode",
    L"No Coprocessor",

    // 8 to 11
    L"Double Fault",
    L"Coprocessor Segment Overrun",
    L"Bad TSS",
    L"Segment Not Present",

    // 12 to 15
    L"Stack Fault",
    L"General Protection Fault",
    L"Page Fault Exception",
    L"Unknown Interrupt",

    // 16 to 19
    L"Coprocessor Fault",
    L"Alignment Check Failed",
    L"Machine Check Failed",
    L"Reserved",

    // 20 to 23
    L"Reserved", L"Reserved", L"Reserved", L"Reserved",

    // 24 to 27
    L"Reserved", L"Reserved", L"Reserved", L"Reserved",

    // 28 to 31
    L"Reserved", L"Reserved", L"Reserved", L"Reserved",

    // 32 to 47 (PIC1 + PIC2)
    L"Interrupt 0 (Timer) Unhandled",
    L"Interrupt 1 (Keyboard) Unhandled",
    L"Interrupt 2 (PIC2) Unhandled",
    L"Interrupt 3 (COM2/COM4) Unhandled",
    L"Interrupt 4 (COM1/COM3) Unhandled",
    L"Interrupt 5 (LPT2 / Sound) Unhandled",
    L"Interrupt 6 (Floppy) Unhandled",
    L"Interrupt 7 (LPT1 / SB) Unhandled",
    L"Interrupt 8 (RTC Alarm) Unhandled",
    L"Interrupt 9 Unhandled",
    L"Interrupt 10 Unhandled",
    L"Interrupt 11 Unhandled",
    L"Interrupt 12 (PS/2 Mouse) Unhandled",
    L"Interrupt 13 (COP/FPU/IPC) Unhandled",
    L"Interrupt 14 (ATA/primery) Unhandled",
    L"Interrupt 15 (ATA/secondary) Unhandled",
};

KI_Handler Registered_Handlers[48];

void __cdecl ISR_Handle_Interrupt(Regs *regs)
{
    if (regs == 0)
    {
        KDebugPrint(L"ISR_Handle_Interrupt called with regs=0!!!\r\n");
        return;
    }

    unsigned int num = regs->Interrupt_Number;

    /* Is this a fault whose number is from 0 to 31? */
    if (num < 48)
    {
        if (Registered_Handlers[num]) // if the interrupt code has a registered handler, call it.
            Registered_Handlers[num](regs);
        else if (num < 32)
        {
            /* Display the description for the Exception that occurred. */
            KSerialPrintF(L"WARNING: Exception %d not handled.\r\n", num);
        }
        else
        {
            KSerialPrintF(L"WARNING: Irq %d not handled.\r\n", num-32);
        }

        if (num >= 32)
        {
            /* If the IDT entry that was invoked was greater than 40
            *  (meaning IRQ8 - 15), then we need to send an EOI to
            *  the slave controller */
            if (num >= 40)
            {
                KPortOutputB(0xA0, 0x20);
            }

            /* In either case, we need to send an EOI to the master
            *  interrupt controller too */
            KPortOutputB(0x20, 0x20);
        }
    }
}

#pragma pack(1)

/* Defines an IDT entry */
struct IDT_Entry
{
    UInt16 data[4]; // hack!
};

struct IDT_Entry IDT[256];

/* This exists in 'start.asm', and is used to load our IDT */
extern void idt_load();

#define FLAGS(presence,ring) (((presence&1)<<7)|((ring&3)<<5)|(0xE))

#define PRESENT   0x80

#define RING_0    0x00
#define RING_1    0x20
#define RING_2    0x40
#define RING_3    0x60

#define GATE_INTERRUPT 0x0E
#define GATE_TRAP      0x0F
#define GATE_TASK      0x05

static void __inline BuildIDTEntry(IDT_Entry *dest, UInt32 base, UInt32 sel, UInt32 flags)
{
    dest->data[0] = base;
    dest->data[1] = sel;
    dest->data[2] = flags<<8;
    dest->data[3] = base>>16;
}

void __cdecl LoadIDT(void* base, int limit);

#define ISR_Decl_Handler(num) extern "C" void __cdecl ISR_Handler_##num()

ISR_Decl_Handler( 0);
ISR_Decl_Handler( 1);
ISR_Decl_Handler( 2);
ISR_Decl_Handler( 3);
ISR_Decl_Handler( 4);
ISR_Decl_Handler( 5);
ISR_Decl_Handler( 6);
ISR_Decl_Handler( 7);
ISR_Decl_Handler( 8);
ISR_Decl_Handler( 9);
ISR_Decl_Handler(10);
ISR_Decl_Handler(11);
ISR_Decl_Handler(12);
ISR_Decl_Handler(13);
ISR_Decl_Handler(14);
ISR_Decl_Handler(15);
ISR_Decl_Handler(16);
ISR_Decl_Handler(17);
ISR_Decl_Handler(18);
ISR_Decl_Handler(19);
ISR_Decl_Handler(20);
ISR_Decl_Handler(21);
ISR_Decl_Handler(22);
ISR_Decl_Handler(23);
ISR_Decl_Handler(24);
ISR_Decl_Handler(25);
ISR_Decl_Handler(26);
ISR_Decl_Handler(27);
ISR_Decl_Handler(28);
ISR_Decl_Handler(29);
ISR_Decl_Handler(30);
ISR_Decl_Handler(31);

// Interrupts from PIC1
ISR_Decl_Handler(32);
ISR_Decl_Handler(33);
ISR_Decl_Handler(34);
ISR_Decl_Handler(35);
ISR_Decl_Handler(36);
ISR_Decl_Handler(37);
ISR_Decl_Handler(38);
ISR_Decl_Handler(39);

// Interrupts from PIC2
ISR_Decl_Handler(40);
ISR_Decl_Handler(41);
ISR_Decl_Handler(42);
ISR_Decl_Handler(43);
ISR_Decl_Handler(44);
ISR_Decl_Handler(45);
ISR_Decl_Handler(46);
ISR_Decl_Handler(47);

#define ISR_Get_Handler(num) ISR_Handler_##num

typedef void (__cdecl * PISR_Handler)();

const PISR_Handler ISR_Handler[] =
{
    ISR_Get_Handler( 0),	ISR_Get_Handler( 1),	ISR_Get_Handler( 2),	ISR_Get_Handler( 3),
    ISR_Get_Handler( 4),	ISR_Get_Handler( 5),	ISR_Get_Handler( 6),	ISR_Get_Handler( 7),
    ISR_Get_Handler( 8),	ISR_Get_Handler( 9),	ISR_Get_Handler(10),	ISR_Get_Handler(11),
    ISR_Get_Handler(12),	ISR_Get_Handler(13),	ISR_Get_Handler(14),	ISR_Get_Handler(15),
    ISR_Get_Handler(16),	ISR_Get_Handler(17),	ISR_Get_Handler(18),	ISR_Get_Handler(19),
    ISR_Get_Handler(20),	ISR_Get_Handler(21),	ISR_Get_Handler(22),	ISR_Get_Handler(23),
    ISR_Get_Handler(24),	ISR_Get_Handler(25),	ISR_Get_Handler(26),	ISR_Get_Handler(27),
    ISR_Get_Handler(28),	ISR_Get_Handler(29),	ISR_Get_Handler(30),	ISR_Get_Handler(31),

    // Interrupts from PIC1
    ISR_Get_Handler(32),	ISR_Get_Handler(33),	ISR_Get_Handler(34),	ISR_Get_Handler(35),
    ISR_Get_Handler(36),	ISR_Get_Handler(37),	ISR_Get_Handler(38),	ISR_Get_Handler(39),

    // Interrupts from PIC2
    ISR_Get_Handler(40),	ISR_Get_Handler(41),	ISR_Get_Handler(42),	ISR_Get_Handler(43),
    ISR_Get_Handler(44),	ISR_Get_Handler(45),	ISR_Get_Handler(46),	ISR_Get_Handler(47),
};

static void InstallIDT(int mapHigh)
{
    int offset = 0x80000000;
    if (mapHigh)
        offset=0;
    for (int i=0; i<48; i++)
    {
        BuildIDTEntry(IDT+i, (UInt32)(ISR_Handler[i])-offset,0x08,PRESENT|RING_0|GATE_INTERRUPT);
    }
    for (int i=48; i<256; i++)
    {
        BuildIDTEntry(IDT+i, 0, 0, 0);
    }

    LoadIDT(IDT+0x80000000-offset, sizeof(IDT)-1);
}

static void KInterruptRemapPICIrqs()
{
    KPortOutputB(0x20, 0x11); // disable?
    KPortOutputB(0xA0, 0x11);
    KPortOutputB(0x21, 0x20); // 32
    KPortOutputB(0xA1, 0x28); // 40
    KPortOutputB(0x21, 0x04); // ??
    KPortOutputB(0xA1, 0x02);
    KPortOutputB(0x21, 0x01);
    KPortOutputB(0xA1, 0x01);
    KPortOutputB(0x21, 0x0);
    KPortOutputB(0xA1, 0x0);
}

KI_Handler KInterruptRegisterHandler(UInt32 vector_number, KI_Handler handler)
{
    int we = KInterruptDisable();
    if (vector_number < 48)
    {
        KI_Handler old = Registered_Handlers[vector_number];
        Registered_Handlers[vector_number] = handler;
        return old;
    }

    if (we) KInterruptEnable();
    return 0;
}

KI_Handler KInterruptRegisterIRQHandler(UInt32 interrupt_number, KI_Handler handler)
{
    return KInterruptRegisterHandler(interrupt_number+32, handler);
}

void KInterruptInit(int mapHigh)
{

    KMemZero(Registered_Handlers,sizeof(Registered_Handlers));

    InstallIDT(mapHigh);

    KInterruptRemapPICIrqs();

    //KInterruptEnable();
}
