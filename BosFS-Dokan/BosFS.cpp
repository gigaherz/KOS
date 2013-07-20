// BosFS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BosFS.h"
#include <math.h>
#include <assert.h>

int _tmain(int argc, _TCHAR* argv[])
{
	//check sizes
#define assertsizeof(x) printf("Size of %s is %d\n",#x,sizeof(x));

	assertsizeof(uuid);
	assertsizeof(BlockInfo_u);
	assertsizeof(StreamFragment);
	assertsizeof(DirEntry);
	assertsizeof(NodeIndexEntry);
	assertsizeof(StreamEntry);
	assertsizeof(NodeInfo);
	assertsizeof(Header);

	assert(sizeof(uuid)==16);
	assert(sizeof(BlockInfo)==1);
	assert(sizeof(StreamFragment)==12);
	assert(sizeof(DirEntry)==256);
	assert(sizeof(NodeIndexEntry)==20);
	assert(sizeof(StreamEntry)==24);
	assert(sizeof(NodeInfo)==4096);
	assert(sizeof(Header)==320);


	//simulate
	device_handle dev=DevOpen("I:\\fstest.bfs",DEV_READWRITE);
	if(dev==DEV_OPEN_ERROR)
	{
		printf("Error opening the file.\n");
		return 1;
	}

	bfsCreateFileSystem(dev,2u<<30,4096); //create a 2gb filesystem

	DevClose(dev);

	return 0;
}

