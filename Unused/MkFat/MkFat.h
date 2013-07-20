/*
Based on code from the Microsoft Invisible Computing Project
released under the Microsoft Shared Source License 1.0 as seen:
http://research.microsoft.com/en-us/um/redmond/projects/invisible/EULA.htm

Original code Copyright (c) Microsoft Corporation. All rights reserved. 
*/
#pragma once

#define DOS_ALIGN 512

typedef unsigned int uint;

#pragma pack(1)

/*
* Information in a directory entry
*/
struct FatDirectoryRecord {
	char name[8];          /* file name */
	char ext[3];           /* file extension */
	char attr;             /* attribute byte */
#define FAT_ATTR_RONLY      0x01
#define FAT_ATTR_HIDDEN     0x02
#define FAT_ATTR_SYS        0x04
#define FAT_ATTR_LABEL      0x08    /* optional entry, only in rootdir */
#define FAT_ATTR_DIR        0x10
#define FAT_ATTR_ARCHIVE    0x20    /* e.g. dirty bit */
#define FAT_ATTR_xxx        0xc0

	char reserved[10];     /* mbz */
	char time[2];          /* time stamp */
	char date[2];          /* date stamp */
	short start;           /* starting cluster number */
	int  size;             /* size of the file */
};

/*
* Information contained in sector zero of a FAT filesystem
*/
struct BiosPartitionEntry {
	char   bootid;         /* bootable or not */
#define BIOS_BOOTABLE   128

	char   beghead;        /* beginning head, sector, cylinder */
	char   begsect;        /* begcyl is a 10-bit number. High 2 bits */
	char   begcyl;         /*     are in begsect. */

	char   systid;         /* filesystem type */
#define SYS_FAT 6
#define SYS_EXT 5

	char   endhead;        /* ending head, sector, cylinder */
	char   endsect;        /* endcyl is a 10-bit number.  High 2 bits */
	char   endcyl;         /*     are in endsect. */

	int    offset;
	int    n_sectors;
};

struct BiosPartitionTable{
	BiosPartitionEntry partitions[4];
	/* alignment !! */
	char  magic[2];
#define BIOS_PARTITION_TABLE_MAGIC    ((short)0xaa55)
};

struct FatLabel {
	char   jump[3];        /* Jump to boot code */
	char   banner[8];      /* OEM name & version */
	short  secsiz;         /* Bytes per sector */
	char   clsiz;          /* Cluster size in sectors */
	short  nrsvsect;       /* Number of reserved (boot) sectors */
	char   nfat;           /* Number of FAT tables hopefully 2 */
	short  dirents;        /* Number of root directory entries */
	short  psect;          /* Total sectors on disk */
	char   descr;          /* Media descriptor=first byte of FAT */
	short  fatlen;         /* Sectors in FAT */
	short  nsect;          /* Sectors/track */
	short  nheads;         /* Heads */
	int    nhs;            /* number of hidden sectors */
	int    bigsect;        /* big total sectors */
	char   driveno;        /* drive number */
	char   reserved;
	char   bsigrec;        /* ext boot signature record (x29) */
	char   volid[4];       /* 32bit volume id */
	char   volume_label[11];   /* */
	char   res1[8];        /* actually, "FATnn   " nn=12/16 */
	char   bootcode[384]; /* boot code */
	BiosPartitionTable label;       /* right at the end */
};

#define BLOCK_SIZE (64*1024)

/* Better be a power of 2 or else
*/
#ifndef MY_SECTOR_SIZE
#define MY_SECTOR_SIZE  512/*128*/
#endif

