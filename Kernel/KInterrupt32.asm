
	SEGMENT .data

IDTP DW 0
	 DD 0

	SEGMENT .text

	GLOBAL ?KInterruptDisable@@YIHXZ
?KInterruptDisable@@YIHXZ:
	PUSHF
	POP EAX
	SHR EAX,9
	AND EAX,1
	CLI
	RET

	GLOBAL ?KInterruptEnable@@YIXXZ
?KInterruptEnable@@YIXXZ:
	STI
	RET
		
	GLOBAL ?LoadIDT@@YAXPAXH@Z
?LoadIDT@@YAXPAXH@Z:
	mov ebx, IDTP
	mov eax, [esp+8]
	mov [ebx+0], ax
	mov eax, [esp+4]
	sub eax, 0x80000000
	mov [ebx+2], eax

	lidt [IDTP]
	
	ret
	
	EXTERN ?ISR_Handle_Interrupt@@YAXPAURegs@@@Z

ISR_Handler_Common:
	;; This is our common ISR stub. It saves the processor state, sets
	;; up for kernel mode segments, calls the handler,
	;; and finally restores the stack frame.
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10   ;; Load the Kernel Data Segment descriptor!
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp   ;; Push us the stack
	push eax
	mov eax, ?ISR_Handle_Interrupt@@YAXPAURegs@@@Z
	call eax       ;; A special call, preserves the 'eip' register
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8     ;; Cleans up the pushed error code and pushed ISR number
	iret           ;; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

%macro ISR_Make_Handler_ZC 1
	GLOBAL _ISR_Handler_%1
_ISR_Handler_%1:
	cli     ;; disable interrupts while processing
	push 0  ;; push error code 0 for interrupts without error code
	push %1 ;; push exception code
	jmp ISR_Handler_Common
%endmacro

%macro ISR_Make_Handler_EC 1
	GLOBAL _ISR_Handler_%1
_ISR_Handler_%1:
	cli
	push %1
	jmp ISR_Handler_Common
%endmacro

;; Exceptions
ISR_Make_Handler_ZC 0
ISR_Make_Handler_ZC 1
ISR_Make_Handler_ZC 2
ISR_Make_Handler_ZC 3
ISR_Make_Handler_ZC 4
ISR_Make_Handler_ZC 5
ISR_Make_Handler_ZC 6
ISR_Make_Handler_ZC 7
ISR_Make_Handler_EC 8
ISR_Make_Handler_ZC 9
ISR_Make_Handler_EC 10
ISR_Make_Handler_EC 11
ISR_Make_Handler_EC 12
ISR_Make_Handler_EC 13
ISR_Make_Handler_EC 14
ISR_Make_Handler_ZC 15
ISR_Make_Handler_ZC 16
ISR_Make_Handler_ZC 17
ISR_Make_Handler_ZC 18
ISR_Make_Handler_ZC 19
ISR_Make_Handler_ZC 20
ISR_Make_Handler_ZC 21
ISR_Make_Handler_ZC 22
ISR_Make_Handler_ZC 23
ISR_Make_Handler_ZC 24
ISR_Make_Handler_ZC 25
ISR_Make_Handler_ZC 26
ISR_Make_Handler_ZC 27
ISR_Make_Handler_ZC 28
ISR_Make_Handler_ZC 29
ISR_Make_Handler_ZC 30
ISR_Make_Handler_ZC 31

;; Interrupts
ISR_Make_Handler_ZC 32
ISR_Make_Handler_ZC 33
ISR_Make_Handler_ZC 34
ISR_Make_Handler_ZC 35
ISR_Make_Handler_ZC 36
ISR_Make_Handler_ZC 37
ISR_Make_Handler_ZC 38
ISR_Make_Handler_ZC 39
ISR_Make_Handler_ZC 40
ISR_Make_Handler_ZC 41
ISR_Make_Handler_ZC 42
ISR_Make_Handler_ZC 43
ISR_Make_Handler_ZC 44
ISR_Make_Handler_ZC 45
ISR_Make_Handler_ZC 46
ISR_Make_Handler_ZC 47
