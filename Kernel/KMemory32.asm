
	SEGMENT .data
	
GDTP DD 0
	 DD 0

	SEGMENT .text
	
	GLOBAL ?FlushGDT@@YAXPAXH@Z
?FlushGDT@@YAXPAXH@Z:
	
	;; Need to set up a temporary GDT
	MOV ebx, 0x80000000
	
	MOV DWORD [ebx+0], 0x00180000
	MOV DWORD [ebx+4], 0x00000008
	MOV DWORD [ebx+8], 0x00000000
	MOV DWORD [ebx+12],0x00000000
	MOV DWORD [ebx+16],0x0000FFFF
	MOV DWORD [ebx+20],0x80CF9A00
	MOV DWORD [ebx+24],0x0000FFFF
	MOV DWORD [ebx+28],0x80CF9200

	;;MOV ebx, GDTP
	;;MOV eax, [esp+8]
	;;MOV [ebx+2], ax
	;;MOV eax, [esp+4]
	;;MOV [ebx+4], eax

	lgdt [ebx+2]

	jmp 8:flush2

flush2:
	mov ax, 10h
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	ret

	GLOBAL ?ReadCR0@@YI?CIXZ
?ReadCR0@@YI?CIXZ:
	mov eax, cr0
	ret

	GLOBAL ?WriteCR0@@YIXI@Z
?WriteCR0@@YIXI@Z:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr0,  eax
	pop ebp
	ret

	GLOBAL ?ReadCR3@@YIIXZ
?ReadCR3@@YIIXZ:
	mov eax, cr3
	ret

	GLOBAL ?WriteCR3@@YIXI@Z
?WriteCR3@@YIXI@Z:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr3, eax
	pop ebp
	ret
	
	GLOBAL ?InvalidateTlbEntry@@YIXI@Z
?InvalidateTlbEntry@@YIXI@Z:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	invlpg [eax]
	pop ebp
	ret