#pragma once

#include "Types.h"
#include "Util.h"
#include "DevIo.h"

#pragma pack(push,1)

// disable non-standard extension warning
#pragma warning(push)
#pragma warning(disable:4200)

typedef union BlockInfo_u
{
	u8 number;
	struct blockInfoFalgs
	{
		u8 used:1;			//this block contains data
		u8 restricted:1;	//this block contains data that should not be read by the user
		u8 reserved:4;		//this bits are reserved for future use and should always be set to zero
		u8 system:1;		//this block contains filesystem data
		u8 bad:1;			//this block has one or more bad sectors and should not be used
	} flags;
} BlockInfo;

struct BlockAlloc
{
	BlockInfo table[];
};

struct StreamFragment
{
	u32 firstBlock;	// first block in the fragment -- if the first block is 0, it's a sparse fragment (no data in disk, all zeros when read)
	u32 count;		// number of blocks in the fragment -- if count=0, this fragment is "free"
	u32 nextFragment; // index of the next fragment in the stream, 0xFFFFFFFF if no more fragments
};

struct FragmentTable
{
	u32 nextChunk; //points to the block where the table continues, set to 0 in the last block.
	StreamFragment table[]; //the number of entries depends on the block size (as many as the available space in the block)
};

struct NodeIndexEntry
{
	uuid nodeId;
	u32 block;
};

struct NodeIndex
{
	u32 localLevel;
	NodeIndexEntry table[]; //the number of entries depends on the block size (as many as the available space in the block)
};

struct NodeHashIndex
{
	u32 indexOffset[];
};

struct StreamEntry
{
	u8  streamName[16]; //'DATA'/'DIRECTORY'/'SECURITY'/...
	u32 dataLength;		//size of the stream data
	u32 firstFragment;	//
};

/*
The FS Header is located at the beginning of the first block,
and repeated 4 times over the partition, for a total of 5 copies.

The location of the first block will be at the beginning of the partition,
which corresponds with the logical block index 0:
	offset[0] = 0
The backups of the header will be located at:
	offset[i] = (1<<(N*i))-1, with i=1..4
where
	N = floor(K/4)
	K = floor(log2(partition size / block size))

Example:
	Partition size: 80 000 000 000 bytes
	Block size: 4096 bytes
	Block count: 19531250 blocks
	K = floor(24,219280948873623478703194294894) = 24
	N = floor(24/4) = 6

	offset[0] = 0
	offset[1] = (1<<(6*1))-1 = (1<< 6)-1
	offset[2] = (1<<(6*2))-1 = (1<<12)-1
	offset[3] = (1<<(6*3))-1 = (1<<18)-1
	offset[4] = (1<<(6*4))-1 = (1<<24)-1
	
Node Indexing:
	There is a table with all the nodes. This table is unsorted and can be fragmented.
	There is a primary index that contains the location of each node within the table. The index is sorted and can be fragmented.
	There is a secondary index that contains the number of the first node in each block of the primary index. This index is sorted and can be fragmented.

*/

struct Header
{
	s8 signature[6];	//="BosFS" (null-terminated C-style ASCII string)
	u8 version;			//=1
	u8 revision;		//=1

	uuid fsId;

	u64 blockWidth : 4;		// number of bits to shift the logical sector size: if lss=512, and blockWidth=3 then blockBytes=4096 and if blockWidth=15 (max possible value) then blockBytes=16777216 (16MBytes)
	u64 blockCount : 60;	// number of blocks 

	u32 blockAllocTable;	//points to the BLOCK where the Block Allocation Table starts

	u32 streamLookup;		//points to the block where the index block lookup table starts
	u32 streamLookupLevel;	//tells how many bits to use for the lookup pointers

	u32 streamIndex1;		//points to the block where the first-level index starts
	u32 streamTable;		//points to the BLOCK where the node index starts

	u32 fragmentTable;

	DirEntry rootNode;		// the name of the entry defines the "label" of the file system

	u32 firstUserBlock;		//points to the first block usable for user data (space before this block is reserved for FS structure expansion)

	u32 hash;				//a (fast) 32bit crc32 hash
};
#pragma warning(pop)

#define BOSFS_VERSION 1
#define BOSFS_REVISION 1

#pragma pack(pop)

bool bfsCreateFileSystem(device_handle device, device_pointer dev_size, s32 block_size);