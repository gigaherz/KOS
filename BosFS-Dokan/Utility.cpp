// BosFS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BosFS.h"
#include "Device.h"

//Utility Functions
int bitsInNumber(int number)
{
	int bits=0;
	while(number>0)
	{
		bits++;
		number>>=1;
	}
	return bits;
}


char uuid_text[100];
char* uuid_to_string(const uuid& src)
{
	sprintf(uuid_text,"%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
		src.Data1,src.Data2,src.Data3,
		src.Data4[0],src.Data4[1],src.Data4[2],src.Data4[3],
		src.Data4[4],src.Data4[5],src.Data4[6],src.Data4[7]);
	return uuid_text;
}
