#include <stdio.h>
#include <memory.h>
#include "../ServiceHandler/ServiceApi.h"
#include "../ServiceHandler/APIServer_h.h"

int SystemLogPrint(const char* text)
{
	int len=strlen(text);
	byte* msg = (byte*)malloc(len);

	if(msg==NULL) return -1;

	memcpy(msg,text,len);

	int ret= CallFunction(1,msg,len,&len);

	free(msg);

	return ret;
}