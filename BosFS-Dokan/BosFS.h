
#pragma once
#pragma pack(push,1)
#pragma warning(push,disable:C4200)

//helper types
typedef signed __int8  s8;
typedef signed __int16 s16;
typedef signed __int32 s32;
typedef signed __int64 s64;

typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

typedef struct uuid_s {
    u32 Data1;
    u16 Data2;
    u16 Data3;
    u8  Data4[8];
} uuid;

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

typedef union TimeStamp_u	//time is considered from GMT
{
	struct TimeStamp_s //exactly 8 bytes
	{
		//from big to small
		u16 structmode:1;		//set to 1 if the timestamp uses this struct, else ticks should be used.
		u16 year:15;
		u8  month;
		u8  day;

		u8  hour;
		u8  minute;
		u8  second;
		u8  fraction; // in 1/256th of a second
	} datetime;
	u64 ticks; // number of 1/256ths of a second since moment 0
} TimeStamp;

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

struct DirEntry
{
	s8 name[240];
	uuid nodeId;
};

struct Directory
{
	u32 numEntries;
	DirEntry entries[];
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

struct NodeInfo //4096 bytes/node!
{
	uuid nodeId;
	uuid owner;
	TimeStamp creation;
	TimeStamp lastModified;
	u32  nameCount;
	u32  streamCount;
	StreamEntry streams[168];
	u32  reserved;
	u32  nextNode;
};

/*
The FS Header is located at the beginning of the first sector, and repeated 4 times over the partition.
The actual position of the first sector depends on the container.
*/

/*
[22:48] <+gigaherz> the main header will be at the first sector "0"
[22:49] <+gigaherz> and it will have 4 backups at "powers of two", so they get spread on the disk
[22:49] <+gigaherz> (less chances that ALL of them get corrupted at the same time :P)
[22:53] <+gigaherz> like if it's a 80gb partition with 4k blocks, then there will be 20971520 blocks
[22:53] <+gigaherz> and that means there are 24 powers of two in the partition
[22:53] <+gigaherz> so, the headers would be every 6 powers of two:
[22:54] <+gigaherz> one at 0, one at 2^6, one at 2^12, one at 2^18 (yes I know 2^0 is 1, not 0, but meh :P)

Node Indexing:
There is a table with all the nodes. This table is unsorted and can be fragmented.
There is an index that tells the block where each node is. The index is sorted and can be fragmented.
There is a secondary index that tells the first node in each block in the index. This index is sorted and can be fragmented.

*/

struct Header
{
	s8 signature[6];	//="BosFS" (null-terminated C-style ASCII string)
	u8 version;			//=1
	u8 revision;		//=1

	uuid fsId;

	u32 blockSize;
	u32 blockCount;

	u32 blockAllocTable;	//points to the BLOCK where the Block Allocation Table starts

	u32 nodeLookup;			//points to the block where the index block lookup table starts
	u32 nodeLookupLevel;	//tells how many bits to use for the lookup pointers

	u32 nodeIndex1;			//points to the block where the first-level index starts
	u32 nodeTable;			//points to the BLOCK where the node index starts

	u32 fragmentTable;

	DirEntry rootNode;		// the name of the entry defines the "label" of the file system

	u32 firstUserBlock;		//points to the first block usable for user data (space before this block is reserved for FS structure expansion)

	u32 hash;				//a (fast) 32bit crc32 hash
};

#pragma warning(pop)

#define BOSFS_VERSION 1
#define BOSFS_REVISION 1

// Dependant Includes
#include "Device.h"

//Utility Functions
int bitsInNumber(int number);
char* uuid_to_string(const uuid& src);

__int64 __inline ceilDiv(__int64 a,__int64 b)
{
	return (a+b-1)/b;
}

// Filesystem Functions
bool bfsCreateFileSystem(device_handle device, device_pointer dev_size, __int32 block_size);

#pragma pack(pop)



