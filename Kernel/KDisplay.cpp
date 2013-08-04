#include "Kernel.h"

int KD_X=0;
int KD_Y=0;
Char *KD_Base=0;
Char *KD_Ptr=0;
Char KD_CharAttr=0;

bool hasDisplay = false;

UInt8*  const ScrMode = ((UInt8*)(0x80000449));

UInt8*  const ScrWidth = ((UInt8*)(0x8000044A));
UInt8*  const ScrHeight = ((UInt8*)(0x80000484));

UInt8*  const ScrPage = ((UInt8*)(0x80000462));
UInt16* const ScrCursor = ((UInt16*)(0x80000450));

UInt16* const ScrCrtc = ((UInt16*)(0x80000463));


static int XYOffset(int X, int Y)
{
    return (Y*80)+X;
}

static Char* Base()
{
    UInt8 VMode = *ScrMode;

    switch (VMode)
    {
    case 7:			// Hercules 80x25
        return (Char*)(0x800B0000);

    case 0:			// CGA 40x25
    case 1:
        return (Char*)(0x00000000);

    case 2:			// CGA 80x25
    case 3:
        return (Char*)(0x800B8000);

    default:
        return (Char*)(0x00000000);
    }
}

static void update_cursor_position()
{
    int page=*ScrPage; // from 0 to 7
    int w=*ScrWidth;
    int h=*ScrHeight+1;

    int cursor = ((((w*h)|0x00ff)+1)*page) + KD_Y*80 + KD_X;

    ScrCursor[page] = cursor;

    UInt16 crtc_addr = *ScrCrtc;

    // CRTC regs 0x0e and 0x0f
    KPortOutputB(crtc_addr,0x0e);
    KPortOutputB(crtc_addr+1,(cursor&0xff00)>>8);
    KPortOutputB(crtc_addr,0x0f);
    KPortOutputB(crtc_addr+1,cursor&0x00ff);
}

static void advance_page()
{
    //int page = *ScrPage;
    //page = (page+1)%8;
    //*ScrPage = page;
    KDisplayClear();
}

static void gotoxy(int X, int Y)
{
    KD_X = X%(*ScrWidth);
    KD_Y = Y%(*ScrHeight+1);

    KD_Ptr = KD_Base + XYOffset(KD_X,KD_Y);
    update_cursor_position();
}

static void putchar(Char c)
{
    *(KD_Ptr++) = c;
    KD_X++;
    if (KD_X>=(*ScrWidth))
    {
        KD_X%=(*ScrWidth);
        KD_Y++;
        if (KD_Y>=(*ScrHeight))
        {
            KD_Y=0;
            advance_page();
        }

        KD_Ptr = KD_Base + XYOffset(KD_X,KD_Y);
        update_cursor_position();
    }
}

void KDisplayInit()
{
    KD_Base = Base();
    hasDisplay = KD_Base > 0;
    if (!hasDisplay)
        return;
    gotoxy(0,0);
    KD_CharAttr = (CHAR_BACK(COLOR_BLACK) | CHAR_FORE(COLOR_GRAY))<<8;
}

static void __fastcall KDisplayPutC(Char c)
{
    if (!hasDisplay)
        return;
    if (c<32)
    {
        switch (c)
        {
        case 10:
            gotoxy(KD_X,KD_Y+1);
            break;
        case 13:
            gotoxy(0,KD_Y);
            break;
        }
    }
    else
    {
        putchar(KD_CharAttr | c);
    }
}

void KDisplayPrint(KCString str)
{
    if (!hasDisplay)
        return;

    while (*str)
    {
        KDisplayPutC(*(str++));
    }
}

void KDisplayWrite(KCString str, int count)
{
    if (!hasDisplay)
        return;

    while (count-->0)
    {
        KDisplayPutC(*(str++));
    }
}

void KDisplaySetAttributes(Char attributes)
{
    KD_CharAttr = attributes << 8;
}

void KDisplayClear()
{
    if (!hasDisplay)
        return;

    for (int i=0;i<(80*25);i++)
    {
        KD_Base[i]=KD_CharAttr;
    }
}

static void __cdecl KDisplayWriteCallback(void*userdata, KCString string, int nchars)
{
    KDisplayWrite(string,nchars);
}

void __cdecl KDisplayPrintF(KCString fmt, ...)
{
    if (!hasDisplay)
        return;

    va_list lst;

    va_start(lst,fmt);
    KSimplePrintF(KDisplayWriteCallback, 0, fmt, lst);
    va_end(lst);
}

