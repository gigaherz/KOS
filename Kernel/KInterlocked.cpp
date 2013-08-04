#define NO_OFFSETOF
#include "Kernel.h"

#define _INC_VADEFS
#define _VA_LIST_DEFINED
#include <intrin.h>

#pragma intrinsic (_InterlockedIncrement)
#pragma intrinsic (_InterlockedDecrement)

#pragma intrinsic (_InterlockedExchange)
#pragma intrinsic (_InterlockedExchangeAdd)

#pragma intrinsic (_InterlockedCompareExchange)

#pragma intrinsic (_InterlockedOr)
#pragma intrinsic (_InterlockedOr8)
#pragma intrinsic (_InterlockedOr16)

#pragma intrinsic (_InterlockedXor)
#pragma intrinsic (_InterlockedXor8)
#pragma intrinsic (_InterlockedXor16)

#pragma intrinsic (_InterlockedAnd)
#pragma intrinsic (_InterlockedAnd8)
#pragma intrinsic (_InterlockedAnd16)

Int32 KInterlockedAnd(Int32 volatile * var, Int32 add)
{
    return _InterlockedAnd((volatile long*)var, add);
}

Int8 KInterlockedAnd8(Int8 volatile* var, Int8 add)
{
    return _InterlockedAnd8((volatile char*)var, add);
}

Int16 KInterlockedAnd16(Int16 volatile* var, Int16 add)
{
    return _InterlockedAnd16(var, add);
}

Int32 KInterlockedOr(Int32 volatile* var, Int32 add)
{
    return _InterlockedOr((volatile long*)var, add);
}

Int8 KInterlockedOr8(Int8 volatile* var, Int8 add)
{
    return _InterlockedOr8((volatile char*)var, add);
}

Int16 KInterlockedOr16(Int16 volatile* var, Int16 add)
{
    return _InterlockedOr16(var, add);
}

Int32 KInterlockedXor(Int32 volatile* var, Int32 add)
{
    return _InterlockedXor((volatile long*)var, add);
}

Int8 KInterlockedXor8(Int8 volatile* var, Int8 add)
{
    return _InterlockedXor8((volatile char*)var, add);
}

Int16 KInterlockedXor16(Int16 volatile* var, Int16 add)
{
    return _InterlockedXor16(var, add);
}

Int32 KInterlockedIncrement(Int32 volatile* var)
{
    return _InterlockedIncrement((volatile long*)var);
}

Int32 KInterlockedDecrement(Int32 volatile* var)
{
    return _InterlockedDecrement((volatile long*)var);
}

Int32 KInterlockedExchange(Int32 volatile* var, Int32 _new)
{
    return _InterlockedExchange((volatile long*)var, _new);
}

Int32 KInterlockedExchangeAdd(Int32 volatile* var, Int32 add)
{
    return _InterlockedExchangeAdd((volatile long*)var, add);
}

Int32 KInterlockedCompareExchange(Int32 volatile* var, Int32 _cmp, Int32 _new)
{
    return _InterlockedCompareExchange((volatile long*)var, _cmp, _new);
}
