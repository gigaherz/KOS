#include "Kernel.h"

volatile int nothing;

void KDebugDoNothing()
{
	nothing = 0;
}

void KAssert_Internal(bool condition, KCString string)
{
	if(!condition)
	{
		KDebugPrint(string);
		__asm int 3
	}
}

void KDebugWrite(KCString string, int nchars)
{
	KSerialWrite(string,nchars);
	KDisplayWrite(string,nchars);
}

void KDebugPrint(KCString string)
{
	KSerialPrint(string);
	KDisplayPrint(string);
}

void __cdecl KDebugWriteCallback(void*userdata, KCString string, int nchars)
{
	KDebugWrite(string,nchars);
}

void KDebugPrintF(KCString fmt, ...)
{
	va_list lst;

	va_start(lst,fmt);
	KSimplePrintF(KDebugWriteCallback, 0, fmt, lst);
	va_end(lst);
}
