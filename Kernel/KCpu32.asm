
	SEGMENT .text

	GLOBAL ?KCpuId@@YIHIAAUBaseRegs@@@Z
?KCpuId@@YIHIAAUBaseRegs@@@Z:

	;; bit 21 in the EFLAGS
	;; If a software procedure can set and clear this flag, the processor executing the
	;; procedure supports the CPUID instruction.
	
	PUSH ebx
	PUSH edi

	MOV edi, edx
	MOV eax, ecx
	XOR ecx, ecx		;; ecx should be zero for CPUID(4)
	CPUID
	MOV [edi+0], eax
	MOV [edi+1], ecx
	MOV [edi+2], edx
	MOV [edi+3], ebx
	
	POP edi
	POP ebx
	XOR eax,eax
	RET
