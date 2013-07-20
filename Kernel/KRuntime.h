#pragma once

//////////////////////////////////////////////////////////////////////////
// Tools (Kernel Runtime)

typedef const char* va_list;

#define _ADDRESSOF(n)   ( (int)&(n) )
#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define va_start(ap,v)  ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap)      ( ap = (va_list)0 )

#define is_digit(c) ((c) >= '0' && (c) <= '9')
#define is_lower(c) ((c) >= 'a' && (c) <= 'z')
#define is_upper(c) ((c) >= 'A' && (c) <= 'Z')

#define is_alpha(c) (is_lower(c)||is_upper(c))
#define is_alnum(c) (is_alpha(c)||is_digit(c))

#define offsetof(x,y) ((UIntPtr)&(((x*)0)->y))

typedef void (__cdecl * cbpf_callback)(void *userdata, KCString data, int len);

void KMemCpy(void *dest, void *src, int copyLength);
void KMemSet(void *dest, int byteFill, int fillLength);
void KMemZero(void *dest, int fillLength);

int  KStrLen(KCString string);
void KStrCpy(KString dest, KCString src);
int  KStrCmp(KCString src1, KCString src2);

int  KStrLenA(const char* string);
void KStrCpyA(char* dest, const char* src);
int  KStrCmpA(const char* src1, const char* src2);

int  KStrToI(KCString src);
void KItoStr(KString dest, int len, int num);
void KUtoStr(KString dest, int len, int num);
void KHtoStr(KString dest, int len, int num);

int __cdecl KSimplePrintF(cbpf_callback cb, void *userdata, KCString fmt, va_list args);

//////////////////////////////////////////////////////////////////////////
// Misc
int  KInterruptDisable();
void KInterruptEnable();

void KPortOutputB(UInt32 port, UInt8 _byte);
void KPortOutputW(UInt32 port, UInt16 _word);
void KPortOutputD(UInt32 port, UInt32 _dword);
UInt8   KPortInputB(UInt32 port);
UInt16  KPortInputW(UInt32 port);
UInt32  KPortInputD(UInt32 port);
