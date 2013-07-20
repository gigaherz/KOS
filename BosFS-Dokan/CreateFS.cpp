// BosFS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BosFS.h"
#include "Device.h"

bool bfsCreateFileSystem(device_handle device, device_pointer dev_size, __int32 block_size)
{
	Header hdr;

	ZeroMemory(&hdr,sizeof(hdr));

	printf("Setting up header...\n");

	printf(" * Initializing BosFS v%d.%d\n",BOSFS_VERSION,BOSFS_REVISION);
	strcpy((char*)hdr.signature,"BosFS");
	hdr.version = BOSFS_VERSION;
	hdr.revision= BOSFS_REVISION;

	MkUUID(&hdr.fsId);
	printf(" * FS uuid: %s\n",uuid_to_string(hdr.fsId));

	hdr.blockCount=ceilDiv(dev_size,block_size);
	hdr.blockSize=block_size;
	hdr.blockAllocTable=16; //place the BAT at sector 16
	printf(" * Blocks: %d of size %d\n",hdr.blockCount,hdr.blockSize);

	int batSize=hdr.blockCount;				//1 byte/block
	int batBlocks=ceilDiv(batSize,hdr.blockSize);
	printf(" * Block Allocation Table at block %d and size %d(%d blocks)\n",hdr.blockAllocTable,batSize,batBlocks);

	hdr.nodeLookupLevel=0;
	hdr.nodeLookup=(hdr.blockAllocTable + batBlocks+16) & ~15;	//"leave" 16 blocks for index expansion 
	hdr.nodeIndex1=hdr.nodeLookup+16;
	hdr.nodeTable=hdr.nodeIndex1+32;
	printf(" * Node Block Lookup at block %d\n",hdr.nodeLookup);
	printf(" * Node Index 1 at block %d\n",hdr.nodeIndex1);
	printf(" * Node Table at block %d\n",hdr.nodeTable);

	hdr.firstUserBlock = hdr.nodeTable + batBlocks; //leave some blocks of space for the node table to grow a bit before getting fragmented...
	printf(" * End of reserved area at block %d\n",hdr.firstUserBlock);

	MkUUID(&hdr.rootNode.nodeId);
	strcpy((char*)hdr.rootNode.name,"BosFS filesystem image");
	printf(" * Root Node uuid: %s\n",uuid_to_string(hdr.rootNode.nodeId));
	printf(" * Root Node Name: %s\n",hdr.rootNode.name);

	printf("Writing header...\n");
	DevWrite(device,&hdr,0,sizeof(hdr));

	printf("Generating BAT table...\n");
	BlockInfo* bat=new BlockInfo[batSize];

	printf(" * Initializing...\n");
	ZeroMemory(bat,batSize);
	for(int i=0;i<hdr.firstUserBlock;i++)
	{
		bat[i].flags.system=1;

	}

	printf(" * Marking Header block...\n");
	bat[0].flags.used=1;

	printf(" * Marking BAT blocks...\n");
	for(int i=hdr.blockAllocTable;i<batBlocks;i++)
	{
		bat[i].flags.used=1;
	}

	printf(" * Marking Node Index 2 blocks...\n");
	bat[hdr.nodeLookup].flags.used=1;

	printf(" * Marking Node Index 1 blocks...\n");
	bat[hdr.nodeIndex1].flags.used=1;

	printf(" * Marking Node Table blocks...\n");
	bat[hdr.nodeTable].flags.used=1;

	printf("Writing BAT...\n");
	DevWrite(device,bat,hdr.blockAllocTable*hdr.blockSize,batSize);

	delete(bat);

	printf("Creating root Node...\n");
	NodeInfo root;
	ZeroMemory(&root,sizeof(root));

	MkTimeStamp(&root.creation);
	root.lastModified=root.creation;
	root.nodeId=hdr.rootNode.nodeId;
	root.nameCount=1;
	root.streamCount=0;

	printf("Writing root Node...\n");
	DevWrite(device,&root,hdr.nodeTable*hdr.blockSize,sizeof(root));

	printf("Creating node indices...\n");
	u8* lookupBlock=new u8[hdr.blockSize];
	u8* index1Block=new u8[hdr.blockSize];
	u8* index2Block=new u8[hdr.blockSize];
	ZeroMemory(index1Block,hdr.blockSize);
	ZeroMemory(index2Block,hdr.blockSize);

	NodeIndex *idx1=(NodeIndex*)index1Block;
	NodeIndex *idx2=(NodeIndex*)index2Block;

	idx1->table[0].nodeId=root.nodeId;
	idx1->table[0].block=hdr.nodeTable;		//there is only 1 node, so point to it

	idx2->table[0].nodeId=root.nodeId;
	idx2->table[0].block=hdr.nodeIndex1; 	//there is only 1 block in the index to point to it

	printf("Writing node indices...\n");
	DevWrite(device,idx1,hdr.nodeIndex1*hdr.blockSize,hdr.blockSize);
	DevWrite(device,idx2,hdr.nodeLookup*hdr.blockSize,hdr.blockSize);

	delete(index1Block);
	delete(index2Block);

	printf("Creating directory stream on root node...\n");
	printf("TODO: Create the directory stream... XD\n");

	return true;
}
