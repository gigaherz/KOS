#pragma once

#include "../Kernel/KDriver.h"

typedef void* KDriverApi;
enum KDriverApiCodes
{
	KDA_KAssert_Internal,
	KDA_KDebugPrint,

	KDA_KDisplayClear,
	KDA_KDisplayWrite,
	KDA_KDisplayPrint,
	KDA_KDisplaySetAttributes,

	KDA_KInterruptRegisterHandler,
	KDA_KInterruptRegisterIRQHandler,

	KDA_KKeyboardReadChar,
	KDA_KKeyTranslate,

	KDA_KPhysicalAllocatorAcquire,
	KDA_KPhysicalAllocatorRelease,

	KDA_KVirtualAllocatorReserve,
	KDA_KVirtualAllocatorRelease,

	KDA_KVirtualAllocatorMap,

	KDA_KVirtualAllocatorAcquire,

	KDA_KMemCpy,
	KDA_KMemSet,
	KDA_KMemZero,

	KDA_KStrLen,
	KDA_KStrCpy,
	KDA_KStrCmp,

	KDA_KStrLenA,
	KDA_KStrCpyA,
	KDA_KStrCmpA,

	KDA_KStrToI,
	KDA_KItoStr,
	KDA_KUtoStr,
	KDA_KHtoStr,

	KDA_KInterruptDisable,
	KDA_KInterruptEnable,

	KDA_KPortOutputB,
	KDA_KPortOutputW,
	KDA_KPortInputB,
	KDA_KPortInputW,
	KDA_KPortInputD,
	KDA_KSerialWrite,
	KDA_KSerialPrint,
	KDA_KTimerGetTicks,

	KDAEnd
};

#define KAPI __fastcall

#define KDRIVERAPI_DECL(ret,dec,name,params) \
	typedef ret (dec * P##name##_T)params; \
	extern P##name##_T P##name

KDRIVERAPI_DECL(void,KAPI,KAssert_Internal,(bool condition, KCString string));
KDRIVERAPI_DECL(void,KAPI,KDebugPrint,(KCString string));

KDRIVERAPI_DECL(void,KAPI,KDisplayClear,());
KDRIVERAPI_DECL(void,KAPI,KDisplayWrite,(KCString string, int nchars));
KDRIVERAPI_DECL(void,KAPI,KDisplayPrint,(KCString string));
KDRIVERAPI_DECL(void,KAPI,KDisplaySetAttributes,(char attributes));


KDRIVERAPI_DECL(KI_Handler,KAPI,KInterruptRegisterHandler,(UInt32 interrupt_number, KI_Handler handler));
KDRIVERAPI_DECL(KI_Handler,KAPI,KInterruptRegisterIRQHandler,(UInt32 interrupt_number, KI_Handler handler));

KDRIVERAPI_DECL(int,KAPI,KKeyboardReadChar,());
KDRIVERAPI_DECL(char,KAPI,KKeyTranslate,(int keycode));

KDRIVERAPI_DECL(UIntPtr,KAPI,KPhysicalAllocatorAcquire,(UInt32 num_pages, UInt32 ownerID, UInt32* allocated_pages));
KDRIVERAPI_DECL(void,KAPI,KPhysicalAllocatorRelease,(UIntPtr alloc_base, UInt32 ownerID));

KDRIVERAPI_DECL(UIntPtr,KAPI,KVirtualAllocatorReserve,(UInt32 num_pages, UInt32 ownerID, UInt32* allocated_pages));
KDRIVERAPI_DECL(void,KAPI,KVirtualAllocatorRelease,(UIntPtr alloc_base, UInt32 ownerID));

KDRIVERAPI_DECL(UIntPtr,KAPI,KVirtualAllocatorMap,(UIntPtr virtual_base, UIntPtr physical_base, UInt32 page_count));

KDRIVERAPI_DECL(UIntPtr,KAPI,KVirtualAllocatorAcquire,(UInt32 num_pages, UInt32 ownerID));

KDRIVERAPI_DECL(void,KAPI,KMemCpy,(void *dest, void *src, int copyLength));
KDRIVERAPI_DECL(void,KAPI,KMemSet,(void *dest, int byteFill, int fillLength));
KDRIVERAPI_DECL(void,KAPI,KMemZero,(void *dest, int fillLength));

KDRIVERAPI_DECL(int,KAPI,KStrLen,(KCString string));
KDRIVERAPI_DECL(void,KAPI,KStrCpy,(KString dest, KCString src));
KDRIVERAPI_DECL(int,KAPI,KStrCmp,(KCString src1, KCString src2));

KDRIVERAPI_DECL(int,KAPI,KStrLenA,(const char* string));
KDRIVERAPI_DECL(void,KAPI,KStrCpyA,(char* dest, const char* src));
KDRIVERAPI_DECL(int,KAPI,KStrCmpA,(const char* dest, const char* src));

KDRIVERAPI_DECL(int,KAPI,KStrToI,(KCString src));
KDRIVERAPI_DECL(void,KAPI,KItoStr,(KString dest, int len, int num));
KDRIVERAPI_DECL(void,KAPI,KUtoStr,(KString dest, int len, int num));
KDRIVERAPI_DECL(void,KAPI,KHtoStr,(KString dest, int len, int num));

KDRIVERAPI_DECL(int,KAPI,KInterruptDisable,());
KDRIVERAPI_DECL(void,KAPI,KInterruptEnable,());

KDRIVERAPI_DECL(void,KAPI,KPortOutputB,(UInt32 port, UInt8 _byte));
KDRIVERAPI_DECL(void,KAPI,KPortOutputW,(UInt32 port, UInt16 _word));
KDRIVERAPI_DECL(void,KAPI,KPortOutputD,(UInt32 port, UInt32 _dword));
KDRIVERAPI_DECL(UInt8,KAPI,KPortInputB,(UInt32 port));
KDRIVERAPI_DECL(UInt16,KAPI,KPortInputW,(UInt32 port));
KDRIVERAPI_DECL(UInt32,KAPI,KPortInputD,(UInt32 port));

KDRIVERAPI_DECL(void,KAPI,KSerialWrite,(KCString data, int nchars));
KDRIVERAPI_DECL(void,KAPI,KSerialPrint,(KCString data));

KDRIVERAPI_DECL(UInt32,KAPI,KTimerGetTicks,());

#define DRIVER_EXPORT ResultCode __declspec(dllexport) __stdcall
