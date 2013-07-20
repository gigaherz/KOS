#pragma once

//////////////////////////////////////////////////////////////////////////
// Serial port
void KSerialInit();
void KSerialWrite(KCString data, int nchars);
void KSerialPrint(KCString data);

void __cdecl KSerialPrintF(KCString fmt, ...);