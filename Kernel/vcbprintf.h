#pragma once

typedef const char* va_list;

#define _ADDRESSOF(n)   ( (int)&(n) )
#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define va_start(ap,v)  ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap)      ( ap = (va_list)0 )

typedef void (__cdecl * cbpf_callback)(void *userdata, const char* data, int len);

int __cdecl vcbprintf(cbpf_callback cb, void *userdata, const char* format, va_list args );
