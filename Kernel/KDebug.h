#pragma once

//////////////////////////////////////////////////////////////////////////
// Debugging
#define SFY(x) L#x
#define STRFY(x) SFY(x)

#define WFY(x) L##x
#define WIDEIFY(x) WFY(x)

void KAssert_Internal(bool condition, KCString string);

#define MDummy(x) dummy_##x
#define KDummy(x) MDummy(x)
#define KCTAssert(expr) \
	struct KDummy(__COUNTER__) { unsigned int bf : expr; }

#define KAssert(condition) \
	KAssert_Internal( (condition) , \
	L"Assertion failed in '" WIDEIFY(__FILE__) L"(" STRFY(__LINE__) L"):" \
	L" Condition '" STRFY(condition) L"' false in: " WIDEIFY(__FUNCSIG__) L"\r\n")

void KDebugPrint(KCString string);
void __cdecl KDebugPrintF(KCString fmt, ...);

void KDebugDoNothing();
