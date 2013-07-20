#include "KDriverApi.h"

PKAssert_Internal_T PKAssert_Internal;
PKDebugPrint_T PKDebugPrint;

PKDisplayClear_T PKDisplayClear;
PKDisplayWrite_T PKDisplayWrite;
PKDisplayPrint_T PKDisplayPrint;
PKDisplaySetAttributes_T PKDisplaySetAttributes;

PKInterruptRegisterHandler_T PKInterruptRegisterHandler;
PKInterruptRegisterIRQHandler_T PKInterruptRegisterIRQHandler;

PKKeyboardReadChar_T PKKeyboardReadChar;
PKKeyTranslate_T PKKeyTranslate;

PKPhysicalAllocatorAcquire_T PKPhysicalAllocatorAcquire;
PKPhysicalAllocatorRelease_T PKPhysicalAllocatorRelease;

PKVirtualAllocatorReserve_T PKVirtualAllocatorReserve;
PKVirtualAllocatorRelease_T PKVirtualAllocatorRelease;

PKVirtualAllocatorMap_T PKVirtualAllocatorMap;

PKVirtualAllocatorAcquire_T PKVirtualAllocatorAcquire;

PKMemCpy_T PKMemCpy;
PKMemSet_T PKMemSet;
PKMemZero_T PKMemZero;

PKStrLen_T PKStrLen;
PKStrCpy_T PKStrCpy;
PKStrCmp_T PKStrCmp;

PKStrLenA_T PKStrLenA;
PKStrCpyA_T PKStrCpyA;
PKStrCmpA_T PKStrCmpA;

PKStrToI_T PKStrToI;
PKItoStr_T PKItoStr;
PKUtoStr_T PKUtoStr;
PKHtoStr_T PKHtoStr;

PKInterruptDisable_T PKInterruptDisable;
PKInterruptEnable_T PKInterruptEnable;

PKPortOutputB_T PKPortOutputB;
PKPortOutputW_T PKPortOutputW;
PKPortOutputD_T PKPortOutputD;
PKPortInputB_T PKPortInputB;
PKPortInputW_T PKPortInputW;
PKPortInputD_T PKPortInputD;
PKSerialWrite_T PKSerialWrite;
PKSerialPrint_T PKSerialPrint;
PKTimerGetTicks_T PKTimerGetTicks;


#define LOADENTRY(name) P##name	= (P##name##_T)ApiTable[KDA_##name];

extern "C"
void DriverEntry(KDriverApi* ApiTable)
{
	LOADENTRY(KAssert_Internal);
	LOADENTRY(KDebugPrint);

	LOADENTRY(KDisplayClear);
	LOADENTRY(KDisplayWrite);
	LOADENTRY(KDisplayPrint);
	LOADENTRY(KDisplaySetAttributes);

	LOADENTRY(KInterruptRegisterHandler);
	LOADENTRY(KInterruptRegisterIRQHandler);

	LOADENTRY(KKeyboardReadChar);
	LOADENTRY(KKeyTranslate);

	LOADENTRY(KPhysicalAllocatorAcquire);
	LOADENTRY(KPhysicalAllocatorRelease);

	LOADENTRY(KVirtualAllocatorReserve);
	LOADENTRY(KVirtualAllocatorRelease);

	LOADENTRY(KVirtualAllocatorMap);

	LOADENTRY(KVirtualAllocatorAcquire);

	LOADENTRY(KMemCpy);
	LOADENTRY(KMemSet);
	LOADENTRY(KMemZero);

	LOADENTRY(KStrLen);
	LOADENTRY(KStrCpy);
	LOADENTRY(KStrCmp);

	LOADENTRY(KStrLenA);
	LOADENTRY(KStrCpyA);
	LOADENTRY(KStrCmpA);

	LOADENTRY(KStrToI);
	LOADENTRY(KItoStr);
	LOADENTRY(KUtoStr);
	LOADENTRY(KHtoStr);

	LOADENTRY(KInterruptDisable);
	LOADENTRY(KInterruptEnable);

	LOADENTRY(KPortOutputB);
	LOADENTRY(KPortOutputW);
	LOADENTRY(KPortInputB);
	LOADENTRY(KPortInputW);
	LOADENTRY(KPortInputD);
	LOADENTRY(KSerialWrite);
	LOADENTRY(KSerialPrint);
	LOADENTRY(KTimerGetTicks);

}

void KAssert_Internal(bool condition, KCString string)
{
	return PKAssert_Internal(condition, string);
}

void KDebugPrint(KCString string)
{
	return PKDebugPrint(string);
}

void KDisplayClear()
{
	return PKDisplayClear();
}

void KDisplayWrite(KCString string, int nchars)
{
	return PKDisplayWrite(string, nchars);
}

void KDisplayPrint(KCString string)
{
	return PKDisplayPrint(string);
}

void KDisplaySetAttributes(char attributes)
{
	return PKDisplaySetAttributes(attributes);
}

KI_Handler KInterruptRegisterHandler(UInt32 interrupt_number, KI_Handler handler)
{
	return PKInterruptRegisterHandler(interrupt_number, handler);
}

KI_Handler KInterruptRegisterIRQHandler(UInt32 interrupt_number, KI_Handler handler)
{
	return PKInterruptRegisterIRQHandler(interrupt_number, handler);
}

int KKeyboardReadChar()
{
	return PKKeyboardReadChar();
}

char KKeyTranslate(int keycode)
{
	return PKKeyTranslate(keycode);
}

UIntPtr KPhysicalAllocatorAcquire(UInt32 num_pages, UInt32 ownerID, UInt32* allocated_pages)
{
	return PKPhysicalAllocatorAcquire(num_pages, ownerID, allocated_pages);
}

void KPhysicalAllocatorRelease(UIntPtr alloc_base, UInt32 ownerID)
{
	return PKPhysicalAllocatorRelease(alloc_base, ownerID);
}

UIntPtr KVirtualAllocatorReserve(UInt32 num_pages, UInt32 ownerID, UInt32* allocated_pages)
{
	return PKVirtualAllocatorReserve(num_pages, ownerID, allocated_pages);
}

void KVirtualAllocatorRelease(UIntPtr alloc_base, UInt32 ownerID)
{
	return PKVirtualAllocatorRelease(alloc_base, ownerID);
}

UIntPtr KVirtualAllocatorMap(UIntPtr virtual_base, UIntPtr physical_base, UInt32 page_count)
{
	return PKVirtualAllocatorMap(virtual_base, physical_base, page_count);
}

UIntPtr KVirtualAllocatorAcquire(UInt32 num_pages, UInt32 ownerID)
{
	return PKVirtualAllocatorAcquire(num_pages, ownerID);
}

void KMemCpy(void *dest, void *src, int copyLength)
{
	return PKMemCpy(dest, src, copyLength);
}

void KMemSet(void *dest, int byteFill, int fillLength)
{
	return PKMemSet(dest, byteFill, fillLength);
}

void KMemZero(void *dest, int fillLength)
{
	return PKMemZero(dest, fillLength);
}

int KStrLen(KCString string)
{
	return PKStrLen(string);
}

void KStrCpy(KString dest, KCString src)
{
	return PKStrCpy(dest, src);
}

int KStrCmp(KCString src1, KCString src2)
{
	return PKStrCmp(src1, src2);
}

int KStrLenA(const char* string)
{
	return PKStrLenA(string);
}

void KStrCpyA(char* dest, const char* src)
{
	return PKStrCpyA(dest, src);
}

int KStrCmpA(const char* src1, const char* src2)
{
	return PKStrCmpA(src1, src2);
}

int KStrToI(KCString src)
{
	return PKStrToI(src);
}

void KItoStr(KString dest, int len, int num)
{
	return PKItoStr(dest, len, num);
}

void KUtoStr(KString dest, int len, int num)
{
	return PKUtoStr(dest, len, num);
}

void KHtoStr(KString dest, int len, int num)
{
	return PKHtoStr(dest, len, num);
}

int KInterruptDisable()
{
	return PKInterruptDisable();
}

void KInterruptEnable()
{
	return PKInterruptEnable();
}

void KPortOutputB(UInt32 port, UInt8 _byte)
{
	return PKPortOutputB(port, _byte);
}

void KPortOutputW(UInt32 port, UInt16 _word)
{
	return PKPortOutputW(port, _word);
}

void KPortOutputD(UInt32 port, UInt32 _dword)
{
	return PKPortOutputD(port, _dword);
}

UInt8 KPortInputB(UInt32 port)
{
	return PKPortInputB(port);
}

UInt16 KPortInputW(UInt32 port)
{
	return PKPortInputW(port);
}

UInt32 KPortInputD(UInt32 port)
{
	return PKPortInputD(port);
}

void KSerialWrite(KCString data, int nchars)
{
	return PKSerialWrite(data, nchars);
}

void KSerialPrint(KCString data)
{
	return PKSerialPrint(data);
}

UInt32 KTimerGetTicks()
{
	return PKTimerGetTicks();
}
