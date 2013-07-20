#pragma once

#include "Storage.h"

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
