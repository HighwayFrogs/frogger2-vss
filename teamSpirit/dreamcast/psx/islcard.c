/************************************************************************************
	PSX/DC LIBRARY	(c) 2000 BLitz Games Ltd.

	islcard.h:		memory card functions
************************************************************************************/

#include <kamui2.h>
#include <shinobi.h>
//#include <nindows.h>
#include "backup.h"
//#include "error.h"
//#include "ok.h"
#include <sg_bup.h>
#include <sg_pad.h>

#include "islcard.h"

#define GAMENAME16		"Frogger2"
#define GAMENAME32		"Frogger2"

#include "lcdicons.h"
#include "saveicon.h"


static Uint32					gState;
static BUS_BACKUPFILEHEADER		saveHeader;
static char						loadSaveBuffer[8192];

/**************************************************************************
	FUNCTION:	cardInitialise()
	PURPOSE:	Initialise memory card routines
	PARAMETERS:	
	RETURNS:	
**************************************************************************/

void cardInitialise()
{
	BupInit();
}

/**************************************************************************
	FUNCTION:	cardDestroy()
	PURPOSE:	Clear up memory card routines
	PARAMETERS:	
	RETURNS:	
**************************************************************************/

void cardDestroy()
{
	BupExit();
}

static int translateErrorCode()
{
	Sint32	err;

	err = buGetLastError(DRIVE);
	switch(err)
	{
	case BUD_ERR_OK:				// No error
		return McErrNone;
	case BUD_ERR_NO_DISK:			// Memory card is not found
		return McErrCardNotExist;
	case BUD_ERR_ILLEGAL_DISK:		// Last block failed
	case BUD_ERR_UNKNOWN_DISK:		// Memory card is unknown
	case BUD_ERR_UNFORMAT:			// Disk is not formatted
		return McErrCardInvalid;
	case BUD_ERR_DISK_FULL:			// Memory card is full
		return McErrBlockFull;
	case BUD_ERR_FILE_NOT_FOUND:	// File not found
	case BUD_ERR_CANNOT_OPEN:		// File cannot be opened
	case BUD_ERR_CANNOT_CREATE:		// Cannot create execute file
	case BUD_ERR_ACCESS_DENIED:		// File access denied
		return McErrFileNotExist;
	case BUD_ERR_FILE_EXIST:		// File already exists
		return McErrAlreadyExist;
	case BUD_ERR_WRITE_ERROR:		// Write error
	case BUD_ERR_VERIFY:			// Verify error
	case BUD_ERR_FILE_BROKEN:		// File is corrupted
	case BUD_ERR_BUPFILE_CRC:		// Backup file CRC error
	case BUD_ERR_BUPFILE_ILLEGAL:	// Backup file illegal
		return CARDREAD_CORRUPT;
	}
}

static int translateErrorCodeRead()
{
	Sint32	err;

	err = buGetLastError(DRIVE);
	switch(err)
	{
	case BUD_ERR_OK:				// No error
		return CARDREAD_OK;
	case BUD_ERR_NO_DISK:			// Memory card is not found
		return CARDREAD_NOCARD;
	case BUD_ERR_ILLEGAL_DISK:		// Last block failed
	case BUD_ERR_UNKNOWN_DISK:		// Memory card is unknown
		return CARDREAD_BADCARD;
	case BUD_ERR_UNFORMAT:			// Disk is not formatted
		return CARDREAD_NOTFORMATTED;
	case BUD_ERR_DISK_FULL:			// Memory card is full
		return CARDREAD_NOTFOUNDANDFULL;
	case BUD_ERR_FILE_NOT_FOUND:	// File not found
		return CARDREAD_NOTFOUND;
	case BUD_ERR_CANNOT_OPEN:		// File cannot be opened
	case BUD_ERR_CANNOT_CREATE:		// Cannot create execute file
	case BUD_ERR_ACCESS_DENIED:		// File access denied
		return CARDREAD_CORRUPT;
	case BUD_ERR_FILE_EXIST:		// File already exists
		return CARDREAD_CORRUPT;
	case BUD_ERR_WRITE_ERROR:		// Write error
	case BUD_ERR_VERIFY:			// Verify error
	case BUD_ERR_FILE_BROKEN:		// File is corrupted
	case BUD_ERR_BUPFILE_CRC:		// Backup file CRC error
	case BUD_ERR_BUPFILE_ILLEGAL:	// Backup file illegal
		return CARDREAD_CORRUPT;
	}
}
/*
#define	McErrNone			(0)
#define	McErrCardNotExist	(1)
#define	McErrCardInvalid	(2)
#define	McErrNewCard		(3)
#define	McErrNotFormat		(4)
#define	McErrFileNotExist	(5)
#define	McErrAlreadyExist	(6)
#define	McErrBlockFull		(7)

#define CARDREAD_OK					0
#define CARDREAD_NOCARD				1
#define CARDREAD_BADCARD			2
#define CARDREAD_NOTFORMATTED		4
#define CARDREAD_NOTFOUND			5
#define CARDREAD_CORRUPT			6
#define CARDREAD_NOTFOUNDANDFULL	7

#define CARDWRITE_OK				0
#define CARDWRITE_NOCARD			1
#define CARDWRITE_BADCARD			2
#define CARDWRITE_NOTFORMATTED		4
#define CARDWRITE_FULL				7

#define BUD_ERR_OK              // No error
#define BUD_ERR_BUSY            // Busy                       
#define BUD_ERR_INVALID_PARAM   // Invalid function parameter  
#define BUD_ERR_ILLEGAL_DISK    // Illegal disk                
#define BUD_ERR_UNKNOWN_DISK    // Not supported disk          
#define BUD_ERR_NO_DISK         // Disk is not connected       
#define BUD_ERR_UNFORMAT        // Disk is not formatted       
#define BUD_ERR_DISK_FULL       // Disk full                   
#define BUD_ERR_FILE_NOT_FOUND  // File not found              
#define BUD_ERR_FILE_EXIST      // File already exists         
#define BUD_ERR_CANNOT_OPEN     // Cannot open file            
#define BUD_ERR_CANNOT_CREATE   // Cannot create executable    
#define BUD_ERR_EXECFILE_EXIST  // Executable file exists      
#define BUD_ERR_CANNOT_DELETE   // Cannot delete file          
#define BUD_ERR_ACCESS_DENIED   // Access is refused           
#define BUD_ERR_VERIFY          // Verify error                
#define BUD_ERR_WRITE_ERROR     // Write error                 
#define BUD_ERR_FILE_BROKEN     // File is broken              
#define BUD_ERR_BUPFILE_CRC     // CRC Error                   
#define BUD_ERR_BUPFILE_ILLEGAL // File is not backup file     
#define BUD_ERR_GENERIC         // Unknown error               
*/

/**************************************************************************
	FUNCTION:	cardRead()
	PURPOSE:	Read game save from memory card
	PARAMETERS:	save name, pointer to data, size of data
	RETURNS:	0 if loaded OK, +ve Sony errors, -1 not found, -2 corrupt, -3 not found and full card
**************************************************************************/

int cardRead(char *memCardName, void *gSaveData, int gameSaveDataSize)
{
	const BACKUPINFO		*binfo;
	int						rtn;
	BUS_BACKUPFILEHEADER	hdr;
	int						timeOut = 0;
	
	
	cardDisplay(LCD_loading);
	while(!pdVmsLcdIsReady(PDD_PORT_A1))
	{
		if(timeOut++ > 1000)
			return 	CARDREAD_NOCARD;
	}

	gState = S_NOT_READY;

	while(1)
	{
		binfo = BupGetInfo(DRIVE);
		switch (gState)
		{
		case S_NOT_READY:
			if (binfo->Ready)
				gState = S_READY;
			break;
		case S_READY:
			BupLoad(DRIVE, memCardName, loadSaveBuffer);
			gState = S_LOAD;
			if (!binfo->Ready)
				gState = S_NOT_READY;
			break;
		case S_LOAD:
			if (buStat(DRIVE) == BUD_STAT_READY)
				gState = S_COMPLETE;
			break;
		case S_COMPLETE:
			buAnalyzeBackupFileImage(&hdr, loadSaveBuffer);
			if(gSaveData)
				memcpy(gSaveData, hdr.save_data, gameSaveDataSize);
			rtn = translateErrorCodeRead();
//			rtn = translateErrorCode();
			if (rtn==McErrNone)
				cardDisplay(LCD_ok);
			else
				cardDisplay(LCD_error);
			return rtn;
		}
#ifdef _NINJA_
		njWaitVSync();
#else
		kmWaitVBlank();
#endif
	}
}


/**************************************************************************
	FUNCTION:	cardWrite()
	PURPOSE:	Write game save to memory card
	PARAMETERS:	save name, pointer to data, size of data
	RETURNS:	0 if saved OK, +ve Sony errors, -1 no room
**************************************************************************/

int cardWrite(char *memCardName, void *gSaveData, int gSaveDataSize)
{
	const BACKUPINFO		*binfo;
	Sint32					nblock;
	void					*buf;
	BUS_BACKUPFILEHEADER	hdr;
	int						rtn;

	cardDisplay(LCD_saving);
	while(!pdVmsLcdIsReady(PDD_PORT_A1));

	memset(&hdr, 0, sizeof(hdr));
	strcpy(hdr.vms_comment, GAMENAME16);
	strcpy(hdr.btr_comment, GAMENAME32);
	strcpy(hdr.game_name, GAMENAME16);
	hdr.icon_palette = iconPalette;
	hdr.icon_data = iconData;
	hdr.icon_num = 1;
	hdr.icon_speed = 1;
	hdr.visual_data = NULL;
	hdr.visual_type = BUD_VISUALTYPE_NONE;
	hdr.save_data = gSaveData;
	hdr.save_size = gSaveDataSize;
	nblock = buCalcBackupFileSize(hdr.icon_num,	hdr.visual_type, hdr.save_size);
	buf = syMalloc(nblock * 512);
	nblock = buMakeBackupFileImage(buf, &hdr);

	gState = S_NOT_READY;

	while(1)
	{
		binfo = BupGetInfo(DRIVE);
		switch (gState)
		{
		case S_NOT_READY:
			if (binfo->Ready)
				gState = S_READY;
			break;
		case S_READY:
			BupSave(DRIVE, memCardName, buf, nblock);
			gState = S_SAVE;
			if (!binfo->Ready)
				gState = S_NOT_READY;
			break;
		case S_SAVE:
			if (buStat(DRIVE) == BUD_STAT_READY)
				gState = S_COMPLETE;
			break;
		case S_COMPLETE:
			rtn = translateErrorCode();
			if (rtn==McErrNone)
				cardDisplay(LCD_ok);
			else
				cardDisplay(LCD_error);
			return rtn;
		}
#ifdef _NINJA_
		njWaitVSync();
#else
		kmWaitVBlank();
#endif
	}
}


/**************************************************************************
	FUNCTION:	cardDisplay()
	PURPOSE:	Display bitmap on card LCD if available
	PARAMETERS:	pointer to bitmap data
	RETURNS:	
**************************************************************************/

void cardDisplay(Uint8 *bitmap)
{
	int		loop;
	pdGetPeripheral(PDD_PORT_A1);
	pdLcdGetDirection(PDD_PORT_A1);
	if (pdVmsLcdIsReady(PDD_PORT_A1))
		while(pdVmsLcdWrite(PDD_PORT_A1, bitmap, PDD_LCD_FLAG_HVFLIP) == PDD_LCDERR_BUSY);

/*	for(loop=0; loop<60; loop++)
#ifdef _NINJA_
		njWaitVSync();
#else
		kmWaitVBlank();
#endif
	pdVmsLcdWrite(PDD_PORT_A1, LCD_saving, PDD_LCD_FLAG_HVFLIP);
	for(loop=0; loop<60; loop++)
#ifdef _NINJA_
		njWaitVSync();
#else
		kmWaitVBlank();
#endif*/
}

