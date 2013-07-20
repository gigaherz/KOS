#pragma once

Int32 KInterlockedAnd(Int32 volatile * var, Int32 add);
Int8  KInterlockedAnd8(Int8 volatile* var, Int8 add);
Int16 KInterlockedAnd16(Int16 volatile* var, Int16 add);
Int32 KInterlockedOr(Int32 volatile* var, Int32 add);
Int8  KInterlockedOr8(Int8 volatile* var, Int8 add);
Int16 KInterlockedOr16(Int16 volatile* var, Int16 add);
Int32 KInterlockedXor(Int32 volatile* var, Int32 add);
Int8  KInterlockedXor8(Int8 volatile* var, Int8 add);
Int16 KInterlockedXor16(Int16 volatile* var, Int16 add);
Int32 KInterlockedIncrement(Int32 volatile* var);
Int32 KInterlockedDecrement(Int32 volatile* var);
Int32 KInterlockedExchange(Int32 volatile* var, Int32 _new);
Int32 KInterlockedExchangeAdd(Int32 volatile* var, Int32 add);
Int32 KInterlockedCompareExchange(Int32 volatile* var, Int32 _cmp, Int32 _new);
