#include "stdafx.h"
#include <math.h>
#include <assert.h>

#include "Storage.h"

s64 alignedOffset(s64 offset, s32 alignment)
{
	// alignment must be a power of two

	s32 a = alignment - 1;
	return (offset + a) & ~a;
}

//Actual Functions
bool bfsCreateFileSystem(device_handle device, device_pointer dev_size, s32 block_shift)
{
	Header hdr;

	ZeroMemory(&hdr,sizeof(hdr));

	printf("Setting up header...\n");

	printf(" * Initializing BosFS v%d.%d\n",BOSFS_VERSION,BOSFS_REVISION);
	strcpy((char*)hdr.signature,"BosFS");
	hdr.version = BOSFS_VERSION;
	hdr.revision= BOSFS_REVISION;

	MkUUID(&hdr.fsId);
	printf(" * FS uuid: %s\n",uuidToStringA(hdr.fsId));

	s32 logicalSectorSize = DevGetSectorSize(device); // assume the size is a power of 2

	s32 block_size = logicalSectorSize << block_shift;

	s32 blocks64K = ceilDiv(65536, block_size);

	hdr.blockCount = dev_size / block_size; // truncate towards 0, may leave unused sectors at the end if the number of sectors was odd
	hdr.blockWidth = block_shift;
	hdr.blockAllocTable = blocks64K; //place the BAT at 64K into the volume

	printf(" * Blocks: %d of size %d\n", hdr.blockCount, block_size);

	s64 batSize   = hdr.blockCount;		//1 byte/block
	s64 batBlocks = ceilDiv(batSize, block_size);

	printf(" * Block Allocation Table at block %d and size %d(%d blocks)\n",hdr.blockAllocTable,batSize,batBlocks);

	hdr.streamLookupLevel = 0;
	hdr.streamLookup = alignedOffset(hdr.blockAllocTable + batBlocks, blocks64K); // align upwards to the next 64k boundary
	hdr.streamIndex1 = hdr.streamLookup + blocks64K;	// leave 64K of space for expansion
	hdr.streamTable  = hdr.streamIndex1 + blocks64K*2;	// leave 128K of space for expansion
	
	printf(" * Node Block Lookup at block %d\n",hdr.streamLookup);
	printf(" * Node Index 1 at block %d\n",hdr.streamIndex1);
	printf(" * Node Table at block %d\n",hdr.streamTable);

	hdr.firstUserBlock = hdr.streamTable + batBlocks; //leave some blocks of space for the node table to grow a bit before getting fragmented...
	
	printf(" * End of reserved area at block %d\n",hdr.firstUserBlock);

	MkUUID(&hdr.rootNode.nodeId);

	strcpy((char*)hdr.rootNode.name,"BosFS filesystem image");

	printf(" * Root Node uuid: %s\n",uuidToStringA(hdr.rootNode.nodeId));
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
