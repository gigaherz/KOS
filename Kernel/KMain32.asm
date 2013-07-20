	
	SEGMENT .text
	
	EXTERN ?KMain@@YIXPAUMultibootInfo@@@Z

	GLOBAL _NtProcessStartup
	
;;multiboot_header:
;;	DD 0x1BADB002 ;; magic
;;	DD 0x00010007 ;; flags
;;	DD 0x00000000-0x1BADB002-0x00010007 ;; checksum of magic+flags, so that sum+magic+flags=0
;;	DD multiboot_header					;; offset of this struct
;;	DD 0x00100000,						;; memory address where the code should start
;;	DD 0x00106000,						;; memory address where the code should end
;;	DD 0x00200000,						;; memory address for the end of the BSS segment
;;	DD _NtProcessStartup,				;; entry point
;;	DD 1,						;; display mode 1=text
;;	DD 80,						;; display width
;;	DD 25,						;; display height
;;	DD 0						;; display bit depth, 0 for text mode / don't care


_NtProcessStartup:
	;; Setup stack and make parameter visible to the KMain function
	XOR ECX,ECX
	CMP EAX, 0x2BADB002
	JNE NEAR ok_continue
	MOV ECX,EBX
ok_continue:
	
	;; Need to set up a temporary GDT
	MOV ebx, 0x00000000
	
	MOV DWORD [ebx+0], 0x00180000
	MOV DWORD [ebx+4], 0x00000008
	MOV DWORD [ebx+8], 0x00000000
	MOV DWORD [ebx+12],0x00000000
	MOV DWORD [ebx+16],0x0000FFFF
	MOV DWORD [ebx+20],0x80CF9A00
	MOV DWORD [ebx+24],0x0000FFFF
	MOV DWORD [ebx+28],0x80CF9200
	lgdt [ebx+2]

	jmp 8:flush2

flush2:
	mov ax, 10h
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	MOV ESP, 0x80400000
	
	CALL ?KMain@@YIXPAUMultibootInfo@@@Z
re_halt:
	HLT
	jmp re_halt
