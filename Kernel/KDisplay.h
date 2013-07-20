#pragma once

//////////////////////////////////////////////////////////////////////////
// Display
void KDisplayInit();
void KDisplayClear();
void KDisplayWrite(KCString string, int nchars);
void KDisplayPrint(KCString string);
void __cdecl KDisplayPrintF(KCString fmt, ...);
void KDisplaySetAttributes(char attributes);

#define CHAR_BACK(x) ((x)<<4)
#define CHAR_FORE(x) (x)
#define CHAR_BLINK   0x80
#define CHAR_HIGHLIGHT 0x08

// TODO: add the rest of the colors
#define COLOR_BLACK 0
#define COLOR_GRAY  7
#define COLOR_DARKGRAY 8
#define COLOR_WHITE 15