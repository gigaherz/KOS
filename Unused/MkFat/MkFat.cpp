/*
Based on code from the Microsoft Invisible Computing Project
released under the Microsoft Shared Source License 1.0 as seen:
http://research.microsoft.com/en-us/um/redmond/projects/invisible/EULA.htm

Original code Copyright (c) Microsoft Corporation. All rights reserved. 
*/
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <malloc.h>
#include <windows.h>

#include "MkFat.h"

uint    t_align = MY_SECTOR_SIZE;

FatLabel TheLabel;

unsigned char   *buf = NULL;

int file_zero(
			  int     outfile,
			  uint    bytes)
{
	unsigned int zero_size;
	int ret;

	if (buf == NULL)
		buf = (unsigned char *) malloc(BLOCK_SIZE);
	memset(buf,0,BLOCK_SIZE);

	do {
		zero_size = bytes < BLOCK_SIZE ? bytes : BLOCK_SIZE;

		ret = _write(outfile, buf, zero_size);
		if (ret != zero_size)
			break;

		bytes -= zero_size;

	} while (bytes > 0);
	return bytes;
}

int file_copy(
			  int     infile,
			  int     outfile,
			  uint    bytes)
{
	unsigned int copy_size;
	int wsize, ret;
	copy_size = bytes < BLOCK_SIZE ? bytes : BLOCK_SIZE;

	if (buf == NULL)
		buf = (unsigned char *) malloc( BLOCK_SIZE);

	do {
		wsize = _read(infile, buf, copy_size);
		ret = wsize;
		if (ret <= 0)
			break;

		ret = _write(outfile, buf, wsize);
		if (ret != wsize)
			break;

		bytes -= wsize;
		if (bytes < copy_size)
			copy_size = bytes;
	} while (bytes > 0);
	return bytes;
}

static void quit(int code, const char* fmt, ...)
{
	va_list lst;

	va_start(fmt,lst);
	vfprintf(stderr,fmt,lst);
	va_end(lst);

	exit(code);
}
/*
*/
int SizeOfTheseFiles( int argc, char *argv[])
{
	int s = 0;
	int f;

	while (argc-- > 0) {

		if (strcmp(*argv,"-ro") == 0) {
			if (argc-- == 0) /* trailing '-ro' with no file name */
				break;
			argv++;
		}
		f = _open(*argv,O_RDONLY|O_BINARY, 0);
		if (f < 0)
			quit(3, "Could not open %s for reading\n", *argv);
		s += (_filelength(f) + t_align - 1) & ~(t_align - 1);
		_close(f);
		argv++;
	}
	return s;
}

/*
*/
char *GenerateFreeName()
{
	static char TheName[128];
	int f, n = 0;

	do 
	{
		TheName[0] = 0;
		sprintf(TheName, "%d.vfd", n++);
		/* good nuf */
		f = _open(TheName, O_RDONLY, 0);
		if (f < 0)
			return TheName;

		_close(f);
	}
	while (n < 9999);

	return NULL;
}

/* Turn a string into a FAT 8.3 name
*/
static void ConvertFileName(char *ncp, char *cp)
{
	int len = 0;
	char c, *p, *sep;

	/* First strip CP of path leaders
	*/
	p = cp;
	sep = NULL;
	do 
	{
		c = *p++;
		if (c == '/' || c == '\\')
			sep = p;
	} while (c != 0);
	if (sep)
		cp = sep;

	/* Now do our thing
	*/
	while ((c = *cp) != 0) 
	{
		if ((c == '.') && (len < 9)) 
		{
			while (len < 8) 
			{
				*ncp++ = ' ';
				len++;
			}
			cp++;
			continue;
		}

		if (++len > 11)
			return;

		/* Upcase it.  Shouldn't, really.
		*/
		if (c >= 'a' && c <= 'z')
			c -= 'a' - 'A';

		*ncp++ = c;
		cp++;
	}

	/* Pad it
	*/
	while (len++ < 11)
		*ncp++ = ' ';

}

/* Initialize the FAT fs label
*/
static void SetLabel(int n_files, int fatsize, int secsize, int nsecs)
{
	memset(&TheLabel, 0, sizeof TheLabel);

	TheLabel.secsiz = secsize;
	TheLabel.clsiz = 1;
	TheLabel.nrsvsect = 1;
	TheLabel.nfat = 1;
	TheLabel.dirents = n_files;
	if (nsecs > 4087)
		TheLabel.bigsect = nsecs;
	else
		TheLabel.psect = nsecs;

	/* TheLabel.descr = ?; */
	TheLabel.fatlen = fatsize / DOS_ALIGN;
	TheLabel.bsigrec = 0x29;

	*(short*)(TheLabel.label.magic) = BIOS_PARTITION_TABLE_MAGIC;

	if (sizeof TheLabel != DOS_ALIGN)
		quit(-1, "INTERNAL ERROR: LABEL SIZE x%x != 0x200\n",
		(void*)sizeof TheLabel);
}

/* The FAT table
*/
unsigned short *TheFAT;

static void FatSetEntry( int FBits, uint Entry, unsigned short Value)
{
	if (FBits == 16) {
		TheFAT[Entry] = Value;
		return;
	}

	Value &= 0xfff;
	FBits = (Entry * 3) / 4;/* 12/16 == 3/4 */

	switch (Entry & 0x3) {
	case 0:
		TheFAT[FBits] = (unsigned short)((TheFAT[FBits] & 0xf000) | Value);
		break;
	case 1:
		TheFAT[FBits] = (unsigned short)((TheFAT[FBits] & 0x0fff) | (Value << 12));
		FBits++;
		TheFAT[FBits] = (unsigned short)((TheFAT[FBits] & 0xff00) | (Value >>  4));
		break;
	case 2:
		TheFAT[FBits] = (unsigned short)((TheFAT[FBits] & 0x00ff) | (Value <<  8));
		FBits++;
		TheFAT[FBits] = (unsigned short)((TheFAT[FBits] & 0xfff0) | (Value >>  8));
		break;
	case 3:
		TheFAT[FBits] = (unsigned short)((TheFAT[FBits] & 0x000f) | (Value <<  4));
		break;
	}
}

/* Allocate FAT entries, sequentially
*/
unsigned short FatSequential( unsigned short firstcl, int fsize)
{
	int b, clsiz;

	clsiz = TheLabel.psect;
	if (clsiz == 0)
		clsiz = TheLabel.bigsect;
	b = (clsiz > 4087) ? 16 : 12;
	clsiz = (TheLabel.secsiz * TheLabel.clsiz);

	if (TheFAT == NULL) {
		TheFAT = (unsigned short*)malloc( TheLabel.fatlen * DOS_ALIGN );
		memset(TheFAT, 0, TheLabel.fatlen * DOS_ALIGN );
		FatSetEntry( b, 0, 0xffff); /* circa */
		FatSetEntry( b, 1, 0xffff); /* circa */
	}

	while (fsize > clsiz) {
		FatSetEntry( b, (uint) firstcl, (unsigned short)(firstcl + 1));
		firstcl++;
		fsize -= clsiz;
	}

	/* Terminate list
	*/
	FatSetEntry( b, firstcl++, 0xffff);
	return firstcl;
}


/* This does the real work.  Appends the given list of files
* to make up the FAT filesystem (cluster space). While going,
* creates the root directory, and marks clusters as in use
* in the FAT table.
*/
static char *StickThemIn( int argc, char *argv[], int fout)
{
	int f, ro;
	uint fl;
	FatDirectoryRecord *root;
	char *rt;
	unsigned short next_cluster = 2;

	fl = ((argc * sizeof(*root)) + DOS_ALIGN - 1) & ~(DOS_ALIGN - 1);
	root = (FatDirectoryRecord*)malloc( fl );
	memset(root, 0, fl);
	rt = (char *)root;

	while (argc-- > 0) {

		/* Open next file
		*/
		if (strcmp(*argv,"-ro") == 0) {
			if (argc-- == 0) /* trailing '-ro' with no file name */
				break;
			argv++;
			ro = 1;
		}
		else
			ro = 0;

		f = _open(*argv,O_RDONLY|O_BINARY, 0);
		if (f < 0)
			quit(3, "Could not open %s for reading\n", *argv);

		/* Set metadata
		*/
		ConvertFileName(root->name, *argv);
		root->attr = FAT_ATTR_ARCHIVE;
		if (ro)
			root->attr |= FAT_ATTR_ARCHIVE;

		fl = _filelength(f);
		root->start = next_cluster;
		root->size = fl;

		printf("File %s at x%x.x%x\n", *argv, next_cluster, fl);

		next_cluster = FatSequential(next_cluster, fl);

		/* Append it
		*/
		if (file_copy( f, fout, fl) != 0)
			quit(5, "Error appending file %s\n", *argv);

		/* Pad if necessary
		*/
		fl = (fl & (t_align - 1));
		if (fl) {
			fl = t_align - fl;

			printf("Pad %s to x%x\n", *argv, fl);
#if 0
			(void) _lseek(fout, fl, SEEK_CUR);
#else
			(void) file_zero(fout, fl);
#endif
		}

		/* Move on to next file
		*/
		_close(f);
		argv++;
		root++;
	}
	return rt;
}

/* How much overhead
*/
static uint FatFsGoop(uint data_size, uint n_files, uint *pfats)
{
	uint bsiz, fsiz, rsiz;

	/* one for boot */
	bsiz = DOS_ALIGN;

	/* one for FAT (conservative!) */
	fsiz = ( ((data_size / t_align) * 2) + DOS_ALIGN - 1 ) & ~(DOS_ALIGN - 1);

	/* one for root */
	rsiz = ((n_files * 32) + t_align - 1 ) & ~(t_align - 1);

	printf("Boot+FAT+Root+Data = %d + %d + %d + %d == %d\n",
		bsiz, fsiz, rsiz, data_size,
		bsiz + fsiz + rsiz + data_size);

	*pfats = fsiz;
	return bsiz + fsiz + rsiz;
}

/* Main control and only entry point
*/
char *MKFatImage(char   *fname,
				 int     argc,
				 char    *argv[])
{
	int f;
	uint datsiz, metasiz, fats;
	char *rt;

	/* How much data do we have
	*/
	datsiz = SizeOfTheseFiles(argc, argv);

	printf("SizeOfFiles is x%x\n", datsiz);

	/* Compute metadata sizes
	*/
	metasiz = FatFsGoop(datsiz, argc, &fats);

	/* Initialize label
	*/
	SetLabel( argc, fats, t_align, (metasiz + datsiz) / t_align);

	f = _open(fname, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
	if (f < 0)
		quit(6, "Could not open destination file: %s\n", fname);

	/* Leave room for metadata
	*/
	(void) _lseek(f, metasiz, SEEK_SET);

	/* Copy all files, create root directory
	*/
	rt = StickThemIn(argc,argv,f);

	/* Write out metadata
	*/
	(void) _lseek(f, 0, SEEK_SET);
	_write(f, &TheLabel, DOS_ALIGN);
	_write(f, TheFAT, fats);
	_write(f, rt, metasiz - (DOS_ALIGN + fats));

	/* Done
	*/
	_close(f);

	return fname;
}

int main(int argc, char* argv[])
{
	char* fname;

	argc--;
	argv++;

	if(argc == 0)
		quit(1,"ERROR: Nothing to do.");

	if( (strcmp(argv[0],"-f")==0)||
		(strcmp(argv[0],"/f")==0))
	{
		if(argc<2)
			quit(2,"ERROR: Missing parameter for %s\n",argv[0]);

		fname = argv[1];
		argc-=2;
		argv+=2;

		printf("Using output file: %s\n", fname);
	}
	else
		fname = GenerateFreeName();

	// Recheck arg count
	if(argc == 0)
		quit(1,"ERROR: Nothing to do.");
	
	MKFatImage(fname, argc,argv);
	return 0;
}

