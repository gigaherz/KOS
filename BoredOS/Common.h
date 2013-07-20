#pragma once

#include "OsTypes.h"

#define CPU_X86 1
#define CPU_X64 2

#define CPU_TYPE CPU_X86

#if CPU_TYPE==CPU_X86
#define POINTER_INT	u32			//defines the integer number formar needed to hold a pointer
#define NATIVE_INT	u32
#elif
#define POINTER_INT	u64			//x64 pointers are 64bit
#define NATIVE_INT	u32			//x64 supports 64bit native types, but the common one is still 32bit
#endif

typedef union {
	u64 qword;
	u32 dwords[2];
	u16  words[4];
	u8   bytes[8];
} mmxreg;

typedef union {
	f64 doubles[2];
	f32 singles[4];
	u64 qwords[2];
	u32 dwords[4];
	u16  words[8];
	u8  bytes[16];
} xmmreg;

struct Core_Regs {
	//segment registers
	//special registers
	//...
	u32 EDI;
	u32 ESI;
	u32 EBP;
	u32 ESP;
	u32 EBX;
	u32 EDX;
	u32 ECX;
	u32 EAX;
};

struct Fpu_State {
	u16 FCW;
	u16 FSW;
	u16 FTW;
	u16 FOP;
	u32 IP;
	u16 CS;
	u8  Reserved1[2];
	u16 DP;
	u16 DS;
	u8  Reserved2[2];
	u32 MXCSR;
	u8  Reserved3[4];
	union {
		struct {
			f80 data;
			u8  reserved[6];
		} stx;
		struct {
			mmxreg data;
			u8  padding[2];
			u8  reserved[6];
		} mmx;
	} ST[8];
	xmmreg xmm[8];
	u8 Reserved4[224];
};

