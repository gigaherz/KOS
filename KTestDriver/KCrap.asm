	
	SEGMENT .text

	GLOBAL @__security_check_cookie@4

@__security_check_cookie@4:
	XOR eax,eax
	RET 4
