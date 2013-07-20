#pragma once

//////////////////////////////////////////////////////////////////////////
// Interrupt System

// This defines what the stack looks like after an ISR was running
struct Regs
{
	UInt32 GS, FS, ES, DS;
	UInt32 EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX;
	UInt32 Interrupt_Number, Error_Code;
	UInt32 EIP, CS, EFlags, UserESP, SS;
};

// 
typedef ResultCode (__stdcall * KI_Handler)(Regs *regs);

void KInterruptInit(int mapHigh);

KI_Handler KInterruptRegisterHandler(UInt32 interrupt_number, KI_Handler handler);
KI_Handler KInterruptRegisterIRQHandler(UInt32 interrupt_number, KI_Handler handler);
