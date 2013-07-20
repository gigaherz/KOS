// KMultibootFix.cpp : Defines the entry point for the console application.
//
#include <stdio.h>

typedef unsigned __int32 UInt32;

#pragma pack(4)
struct MultibootHeader {
	UInt32 	magic;			// required
 	UInt32 	flags;			// required
 	UInt32 	checksum;		// required
 	UInt32 	header_addr;	// if flags[16] is set
 	UInt32 	load_addr;		// if flags[16] is set
 	UInt32 	load_end_addr;	// if flags[16] is set
 	UInt32 	bss_end_addr;	// if flags[16] is set
 	UInt32 	entry_addr;		// if flags[16] is set
 	UInt32 	mode_type;		// if flags[2] is set
 	UInt32 	width;			// if flags[2] is set
 	UInt32 	height;			// if flags[2] is set
 	UInt32 	depth;			// if flags[2] is set 
};

MultibootHeader mbhdr;

int main(int argc, char* argv[])
{
	int count=0;
	int addr=0;

	if(argc!=2)
	{
		printf("ERROR: Wrong number of arguments.\n");
		return 1;
	}

	FILE *fexe = fopen(argv[1],"r+b");

	if(!fexe)
	{
		printf("ERROR: Unable to open file.\n");
		return 1;
	}

	// step 1: make sure there's only 1 magic number for multiboot
	fseek(fexe,0,SEEK_SET);
	while(!feof(fexe))
	{
		int magic=0;
		int flags=0;
		int checksum=0;

		if(!fread(&magic,1,4,fexe)) break;
		if(!fread(&flags,1,4,fexe)) break;
		if(!fread(&checksum,1,4,fexe)) break;

		if(magic==0x1BADB002) {
			addr = ftell(fexe)-12;

			fseek(fexe,addr,SEEK_SET);
			if(!fread(&mbhdr,sizeof(MultibootHeader),1,fexe))
			{
				printf("ERROR: Unable to read multiboot header from file.\n");
				return 1;
			}

			printf("Multiboot header at: 0x%08x\n",addr);
			printf(" - Header Address:   0x%08x\n", mbhdr.header_addr);
			printf(" - Load Address:     0x%08x\n", mbhdr.load_addr);
			printf(" - Load End Address: 0x%08x\n", mbhdr.load_end_addr);
			printf(" - BSS Address:      0x%08x\n", mbhdr.bss_end_addr);
			printf(" - Entry Point:      0x%08x\n\n", mbhdr.entry_addr);

			if(
				(mbhdr.load_end_addr < mbhdr.load_addr) ||
				(mbhdr.bss_end_addr < mbhdr.load_end_addr) ||
				(mbhdr.entry_addr < mbhdr.load_addr) ||
				(mbhdr.entry_addr >= mbhdr.load_end_addr))
			{
				printf("ERROR: Malformed Multiboot header! Ignoring.");
				continue;
			}

			count++;

			fseek(fexe,addr+4,SEEK_SET);
		}
		else fseek(fexe,-8,SEEK_CUR);
	}

	if(count<1)
	{
		printf("ERROR: Cannot find the multiboot magic number!\n");
		return 1;
	}

	if(count>1)
	{
		printf("WARNING: More than one multiboot magic number found. The last one will be used.\n");
	}

	printf("Multiboot Header at 0x%08x with header offset 0x%08x\n",addr,mbhdr.header_addr);
	if((addr+sizeof(mbhdr))>8192)
	{
		printf("Multiboot Header needs to be relocated. Using 0x00800 as the target address.\n");

		int T = mbhdr.header_addr-addr;

		mbhdr.header_addr = T + 0x0800;
		addr = 0x800;
		printf("New header addr: 0x%08x.\n",mbhdr.header_addr);
	}

	fseek(fexe,addr,SEEK_SET);
	if(!fwrite(&mbhdr,sizeof(MultibootHeader),1,fexe))
	{
		printf("ERROR: Unable to write fixed multiboot header to file.\n");
		return 1;
	}

	fflush(fexe);
	fclose(fexe);

	printf("Finished.\n");
	return 0;
}
