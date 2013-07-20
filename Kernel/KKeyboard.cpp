// Pract5.cpp: define el punto de entrada de la aplicación de consola.
//
#include "Kernel.h"

const char KK_Table_LCase[]= 
{
	0, 27, //1
	     '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', '~', '\b', // 14
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0, '+', '\r',  0, //29
	       'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '\\', 0,  0,  0, '‡', //43
	        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0,  0,  0, ' ', //57
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //70
	'7', '8', '9', '-', //74
	'4', '5', '6', '+', //78
	'1', '2', '3', '0', //82
	'.', 0, 0, 0,  //86
	0, 0 //88

};

const char KK_Table_UCase[]=
{
	0, 27, //1
	'!', '"', '#', '$', '%', '&', '/', '(', ')', '=', '?',  '\\', '\b', // 14
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 0, '*', '\r',  0, //29
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '@', 0,  0,  0, '€', //43
	        'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', 0,  0,  0, ' ', //57
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //70
	'7', '8', '9', '-', //74
	'4', '5', '6', '+', //78
	'1', '2', '3', '0', //82
	'.', 0, 0, 0,  //86
	0, 0 //88

};

bool keyStates[128] = {0};

bool altPressed = false;
bool shiftPressed = false;
bool ctrlPressed = false;

bool CapsLock = false;
bool ScrollLock = false;
bool NumLock = false;

static void KK_DisableInterrupt()
{
	int a = KInterruptDisable();
	KPortOutputB(0x21,KPortInputB(0x21)|2); // Mask Keyboard interrupt
	if(a) KInterruptEnable();
}

static void KK_EnableInterrupt()
{
	int a = KInterruptDisable();
	KPortOutputB(0x21,KPortInputB(0x21)&(~2)); // Unmask Keyboard interrupt
	if(a) KInterruptEnable();
}


static char KK_GetBufferState()
{
	return KPortInputB(0x64);
}

static char KK_GetBufferCode()
{
	return KPortInputB(0x60);
}

static char KK_WaitBufferCode()
{
	short state=0;
	do
	{
		state=(KK_GetBufferState()&1);
	}
	while(state==0);

	return KK_GetBufferCode();
}

static char KK_GetChar()
{
	char eventCode = KK_WaitBufferCode();
	char makeBreak = (eventCode & 0x80);
	char scanCode = (eventCode & 0x7f);

	if(eventCode == 0)
		return 0;

	if(makeBreak==0) //si es apretar
	{
		keyStates[scanCode] = false;

		int ascii;

		if(shiftPressed)
			ascii = KK_Table_UCase[scanCode];
		else
			ascii = KK_Table_LCase[scanCode];

		if(ascii>0)
		{
			return ascii;
		}
		else if((scanCode==42)||(scanCode==54)) // shift keys
		{
			shiftPressed = true;
		}
		else if((scanCode==56))
		{
			altPressed = true;
		}
		else if((scanCode==29))
		{
			ctrlPressed = true;
		}
	}
	else
	{
		keyStates[scanCode] = false;

		if((scanCode==42)||(scanCode==54)) // shift keys
		{
			shiftPressed = false;
		}
		else if((scanCode==56))
		{
			altPressed = false;
		}
		else if((scanCode==29))
		{
			ctrlPressed = false;
		}
	}

	return 0;
}

static ResultCode __stdcall KK_InterruptHandler(Regs *regs)
{
	char t[2] = {
		KK_GetChar(),
		0
	};

	// TODO: Add chars to queue.

	//if(t[0] > 0)
	//{
	//	if(t[0]<32)
	//	{
	//		KCString text=L"";
	//		switch(t[0])
	//		{
	//		case 9: text=L"\\t"; break;
	//		case 13: text=L"\\n"; break;
	//		}
	//		KDisplayPrint(L"Key Pressed: '");
	//		KDisplayPrint(text);
	//		KDisplayPrint(L"'.\r\n");
	//	}
	//	else KDisplayPrintF(L"Key Pressed: '%c'.\r\n",t[0]);
	//}

	return 0;
}

void KKeyboardInit()
{
	KInterruptRegisterIRQHandler(1, KK_InterruptHandler);
//	KK_DisableInterrupt();
	KK_EnableInterrupt();
}

int KKeyboardReadChar()
{
	char code;

	do 
	{
		__asm hlt;
		code = KK_GetChar();
	} while(code == 0);

	return code;
}
