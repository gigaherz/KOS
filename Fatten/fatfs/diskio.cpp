/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/
#include "diskio.h"
#include "../FAT.h"
#include <stdio.h>

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and image file handles.  */

HANDLE driveHandle[1] = {INVALID_HANDLE_VALUE};
const int driveHandleCount = sizeof(driveHandle) / sizeof(FILE*);

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	if(drv == 0) // only one drive (image file) supported atm.
	{
		if(driveHandle[0]!=INVALID_HANDLE_VALUE)
			return 0;

		driveHandle[0]=CreateFile(imageFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL /* | FILE_FLAG_RANDOM_ACCESS */, NULL);

		if(driveHandle[0]!=INVALID_HANDLE_VALUE)
			return 0;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	if(drv < driveHandleCount)
	{
		if(driveHandle[drv] != INVALID_HANDLE_VALUE)
			return 0;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	DWORD result;

	if(drv < driveHandleCount)
	{
		if(driveHandle[drv] != INVALID_HANDLE_VALUE)
		{
			if(SetFilePointer(driveHandle[drv], sector * 512, NULL, SEEK_SET) == INVALID_SET_FILE_POINTER)
				return RES_ERROR;

			if(!ReadFile(driveHandle[drv], buff, 512 * count, &result, NULL))
				return RES_ERROR;

			if(result == (512 * count))
				return RES_OK;

			return RES_ERROR;
		}
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	DWORD result;

	if(drv < driveHandleCount)
	{
		if(driveHandle[drv] != INVALID_HANDLE_VALUE)
		{
			if(SetFilePointer(driveHandle[drv], sector * 512, NULL, SEEK_SET) == INVALID_SET_FILE_POINTER)
				return RES_ERROR;

			if(!WriteFile(driveHandle[drv], buff, 512 * count, &result, NULL))
				return RES_ERROR;

			if(result == (512 * count))
				return RES_OK;

			return RES_ERROR;
		}
	}

	return RES_PARERR;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	if(drv < driveHandleCount)
	{
		if(driveHandle[drv] != INVALID_HANDLE_VALUE)
		{
			switch(ctrl)
			{
			case CTRL_SYNC:
				return RES_OK;
			case GET_SECTOR_SIZE:
				*(DWORD*)buff = 512;
				return RES_OK;
			case GET_BLOCK_SIZE:
				*(DWORD*)buff = 512;
				return RES_OK;
			case GET_SECTOR_COUNT:
				{
					*(DWORD*)buff = GetFileSize(driveHandle[drv], NULL) / 512;
				}
				return RES_OK;
			case SET_SECTOR_COUNT:
				{
					SetFilePointer(driveHandle[drv], (*(DWORD*)buff)*512, NULL, SEEK_SET);
					SetEndOfFile(driveHandle[drv]);
				}
			}
		}
	}

	return RES_PARERR;
}

