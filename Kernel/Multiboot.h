/* multiboot.h - the header for Multiboot */
/* Copyright (C) 1999, 2001  Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

#pragma pack(push,4)
struct MultibootHeader
{
    UInt32 	magic;			// required
    UInt32 	flags;			// required
    UInt32 	checksum;		// required
    UInt32 	headerAddr;		// if flags[16] is set
    UInt32 	loadAddr;		// if flags[16] is set
    UInt32 	loadEndAddr;	// if flags[16] is set
    UInt32 	bssEndAddr;		// if flags[16] is set
    UInt32 	entryAddr;		// if flags[16] is set
    UInt32 	modeType;		// if flags[2] is set
    UInt32 	width;			// if flags[2] is set
    UInt32 	height;			// if flags[2] is set
    UInt32 	depth;			// if flags[2] is set
};

/* The Multiboot information. */
struct MultibootInfo
{
    UInt32 flags;

    UInt32 memLower;
    UInt32 memUpper;

    UInt32 bootDevice;

    UInt32 cmdLine;

    UInt32 modsCount;
    UInt32 modsAddr;

    UInt32 syms[3];

    UInt32 mmapLength;
    UInt32 mmapAddr;

    UInt32 drivesLength;
    UInt32 drivesAddr;

    UInt32 configTable;

    UInt32 bootloaderName;

    UInt32 apmTable;

    UInt32 vbeControlInfo;
    UInt32 vbeModeInfo;
    UInt32 vbeMode;
    UInt32 vbeInterfaceSeg;
    UInt32 vbeInterfaceOff;
    UInt32 vbeInterfaceLen;

};

/* The module structure. */
struct MultibootModule
{
    UInt32 modStart;
    UInt32 modEnd;
    UInt32 string;
    UInt32 reserved;
} ;

/* The memory map. Be careful that the offset 0 is base_addr_low
but no size. */
struct MultibootMemoryMap
{
    UInt32 size;
    UInt32 baseAddrLow;
    UInt32 baseAddrHigh;
    UInt32 lengthLow;
    UInt32 lengthHigh;
    UInt32 type;
};

#pragma pack(pop)