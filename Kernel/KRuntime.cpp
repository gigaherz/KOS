#include "Kernel.h"

void KMemCpy1(void *dest, void *src, int copyLength)
{
    for (char *dptr=(char*)dest,*sptr=(char*)src;copyLength>0;copyLength--)
    {
        *(dptr++)=*(sptr++);
    }
}

void KMemCpy4(void *dest, void *src, int copyLength)
{
    for (int *dptr=(int*)dest,*sptr=(int*)src;copyLength>0;copyLength-=4)
    {
        *(dptr++)=*(sptr++);
    }
}

void KMemCpy(void *dest, void *src, int copyLength)
{
    if ((copyLength|(UIntPtr)dest|(UIntPtr)src)&3) // unaligned or not whole word
        return KMemCpy1(dest,src,copyLength);

    KMemCpy4(dest,src,copyLength);
}

void KMemSet(void *dest, int byteFill, int fillLength)
{
    byteFill &= 0x000000FF;
    UInt32 intFill = byteFill * 0x01010101;

    char* bptr = (char*)dest;

    while (((((UInt32)bptr)&3) != 0)&&(fillLength-->0))
        *(bptr++) = byteFill;

    int* iptr = (int*)bptr;

    while (fillLength>4)
    {
        *(iptr++) = byteFill;
        fillLength-=4;
    }

    bptr = (char*)iptr;
    while (fillLength-->0)
        *(bptr++) = byteFill;

}

void KMemZero(void *dest, int fillLength)
{
    char* bptr = (char*)dest;

    while (((((UInt32)bptr)&3) != 0)&&(fillLength-->0))
        *(bptr++) = 0;

    int* iptr = (int*)bptr;

    while (fillLength>4)
    {
        *(iptr++) = 0;
        fillLength-=4;
    }

    bptr = (char*)iptr;
    while (fillLength-->0)
        *(bptr++) = 0;

}

int KStrLenA(const char* string)
{
    int len = 0;
    while (*(string++))
        len++;
    return len;
}

void KStrCpyA(char* dest, const char* src)
{
    while (*(src))
        *(dest++)=*(src++);
    *dest=0;
}

int KStrCmpA(const char* src1, const char* src2)
{
    while (*(src1) && *(src2))
    {
        int t = *(src1++) - *(src2++);
        if (t<0)
            return -1;
        if (t>0)
            return 1;
    }

    int t = *(src1) - *(src2);

    if (t<0)
        return -1;
    if (t>0)
        return 1;

    return 0;
}

int KStrLen(KCString string)
{
    int len = 0;
    while (*(string++))
        len++;
    return len;
}

void KStrCpy(KString dest, KCString src)
{
    while (*(src))
        *(dest++)=*(src++);
    *dest=0;
}

int KStrCmp(KCString src1, KCString src2)
{
    while (*(src1) && *(src2))
    {
        int t = *(src1++) - *(src2++);
        if (t<0)
            return -1;
        if (t>0)
            return 1;
    }

    int t = *(src1) - *(src2);

    if (t<0)
        return -1;
    if (t>0)
        return 1;

    return 0;
}

int KStrToI(KCString str)
{
    int temp=0;
    bool negate = false;

    if (*str == '-')
    {
        negate=true;
        str++;
    }

    while (is_digit(*str))
    {
        temp=temp*10 + (*str-'0');
        str++;
    }

    return temp;
}

void KItoStr(KString dest, int len, int num)
{
    // pre-process: normalize negatives
    if (num < 0)
    {
        *(dest++) = '-';
        num = -num;
        len--;
    }

    // part 1: convert
    int temp = num;
    int digits = 0;
    Char* ptr = dest;

    while ((temp > 0) && (digits < len))
    {
        int tnum = temp % 10;
        temp /= 10;
        *(ptr++)='0'+tnum;
        digits++;
    }

    if (digits == 0)
        *(ptr++) = '0';

    *ptr = 0;

    // part 2: reverse

    int offset = 0;

    digits--;
    while (offset<digits)
    {
        Char t = dest[offset];
        dest[offset] = dest[digits];
        dest[digits] = t;
        offset++;
        digits--;
    }
}

void KUtoStr(KString dest, int len, UInt32 num)
{
    // part 1: convert
    int temp = num;
    int digits = 0;
    Char *ptr = dest;

    while ((temp > 0) && (digits < len))
    {
        int tnum = temp % 10;
        temp /= 10;
        *(ptr++)='0'+tnum;
        digits++;
    }

    if (digits == 0)
        *(ptr++) = '0';

    *ptr = 0;

    // part 2: reverse

    int offset = 0;

    digits--;
    while (offset<digits)
    {
        Char t = dest[offset];
        dest[offset] = dest[digits];
        dest[digits] = t;
        offset++;
        digits--;
    }
}

void KHtoStr(KString dest, int len, UInt32 num)
{
    // part 1: convert
    UInt32 temp = num;
    int digits = 0;
    Char *ptr = dest;

    while ((temp > 0) && (digits < len))
    {
        int tnum = temp & 15;
        temp >>= 4;

        if (tnum<10)
            *(ptr++)=L'0'+tnum;
        else *(ptr++)=L'A'+tnum-10;

        digits++;
    }

    if (digits == 0)
        *(ptr++) = '0';

    *ptr = 0;

    // part 2: reverse

    int offset = 0;

    digits--;
    while (offset<digits)
    {
        Char t = dest[offset];
        dest[offset] = dest[digits];
        dest[digits] = t;
        offset++;
        digits--;
    }
}

int __cdecl KSimplePrintF(cbpf_callback cb, void *userdata, KCString fmt, va_list args)
{
    Char  buf[64];
    Char* pbuf = buf;
    int   plen = sizeof(buf)/sizeof(Char);
    int   written = 0;

    Char num_buf[15]; // shouldn't go > 12, but whatever

    va_list list = args;

    while (*fmt)
    {
        Char ch = *(fmt++);

        if (ch == '%' && (*fmt!='%') && (*fmt!=0))
        {
            KCString efmt=fmt;
            while (*efmt && !is_lower(*efmt))
            {
                efmt++;
            }

            if (!*efmt)
            {
                continue;
            }

			if (*efmt == 's')
			{
				if (pbuf > buf)
				{
					cb(userdata, buf, pbuf - buf);
					pbuf = buf;
					plen = sizeof(buf);
				}

				KCString str = va_arg(list, KCString);
				int len = KStrLen(str);

				cb(userdata, str, len);

				fmt = efmt + 1;
				continue;
			}

            switch (*efmt)
            {
            case 'd':
                KItoStr(num_buf,14,va_arg(list,Int32));
                break;
            case 'u':
                KUtoStr(num_buf,14,va_arg(list,UInt32));
                break;
            case 'x':
                KHtoStr(num_buf,14,va_arg(list,UInt32));
                break;
            case 'c':
                num_buf[0] = va_arg(list,Char);
                num_buf[1] = 0;
                break;
            default:
                continue;
            }

            KString pnum=num_buf;

            Char pad_char = ' ';

            if (*fmt == '0')
            {
                pad_char='0';

                if (num_buf[0] == '-')
                {
                    written++;
                    *(pbuf++) = '-';
                    if (--plen==0)
                    {
                        cb(userdata,buf,pbuf-buf);
                        pbuf = buf;
                        plen = sizeof(buf);
                    }

                    pnum++;
                }

                fmt++;
            }

            if (is_digit(*fmt))
            {
                int pad_amount = KStrToI(fmt);
                int num_amount = KStrLen(num_buf);

                while (num_amount < pad_amount)
                {
                    num_amount++;
                    written++;
                    *(pbuf++) = pad_char;
                    if (--plen==0)
                    {
                        cb(userdata,buf,pbuf-buf);
                        pbuf = buf;
                        plen = sizeof(buf);
                    }
                }
            }

            for (; *pnum; pnum++)
            {
                written++;
                *(pbuf++) = *pnum;
                if (--plen==0)
                {
                    cb(userdata,buf,pbuf-buf);
                    pbuf = buf;
                    plen = sizeof(buf);
                }
            }


            fmt = efmt+1;
        }
        else
        {
            if ((ch=='%')&&(*fmt=='%')) fmt++; // skip second %
            written++;
            *(pbuf++) = ch;
            if (--plen==0)
            {
                cb(userdata,buf,pbuf-buf);
                pbuf = buf;
                plen = sizeof(buf);
            }
        }
    }

    if (pbuf != buf)
    {
        cb(userdata,buf,pbuf-buf);
    }

    return written;
}
