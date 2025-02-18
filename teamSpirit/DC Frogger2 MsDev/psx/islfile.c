#include "include.h"

extern texurestring[256];
extern texurestring2[256];

char	*genericStak = NULL;
int		genericStakSize = 0;

char* fileLoad(char *filename,int *bytesRead)
{
    PKMDWORD    filePtr;
    GDFS        gdfs = NULL;
    long        FileBlocks;
    int			i,flag = TRUE;
    char		buffer[256],*fptr;
	Sint32		status;
	int			retry = 0;
	Sint32		gflag;
	Uint32		length;
	long		crc = 0;
	char		*crcPtr;

//	syCacheICI();
	
	gdFsChangeDir("\\");

	// change to the appropriate directory
	i = 0;
	fptr = filename;
	while(*fptr != 0)
	{
		switch(*fptr)
		{
			case '\\':
				buffer[i] = 0;
				gdFsChangeDir(buffer);
				fptr++;	
				i=0;			
				break;
				
			default:
				buffer[i++] = *fptr++;				
				break;		
		}
	}
	buffer[i] = 0;

//	if((strcmp(buffer,"GENERIC.STK") == 0)&&(genericStak))
//	{
//		filePtr = Align32Malloc(genericStakSize);
//		memcpy(genericStak,filePtr)
//	}
	
    // Open input file.
    while((!gdfs) && (retry < 50))
    {
	    gdfs = gdFsOpen(buffer, NULL);
	    if(gdfs == NULL)
	    	retry++;
	}
	if(gdfs == NULL)
    	return NULL;

    // Get file size (in blocks/sectors).

	// *ASL* 21/07/2000 - Re-cast second parameters..
    if(bytesRead)
		gdFsGetFileSize(gdfs, (Sint32 *)bytesRead);
    gdFsGetFileSctSize(gdfs, (Sint32 *)&FileBlocks);

    // Allocate memory to nearest block size (2048 bytes).    
	if((strcmp(buffer,"Sqrtable.bin") == 0)||(strcmp(buffer,"acostab.bin") == 0))
	    filePtr = syMalloc(FileBlocks * 2048);
	else	
   		filePtr = Align32Malloc(FileBlocks * 2048);
	if(filePtr == NULL)
		utilPrintf("Error\n");

    // Read file to memory region (Destination address must be 32-byte aligned).
    gdFsReqRd32(gdfs, FileBlocks, filePtr);

    // Wait for file access to finish.
    while(gdFsGetStat(gdfs) != GDD_STAT_COMPLETE)
    {
	    status = gdFsGetStat(gdfs);
    	if(status == GDD_STAT_ERR)
    	{
    		utilPrintf("Error\n");
  	 		Align32Free(filePtr);
		    gdFsClose(gdfs);
  	 		return NULL;
    	}
    }

    // Close file.
    gdFsClose(gdfs);

	// *ASL* 21/07/2000 - Re-cast to char pointer stream from dc dword pointer (PKMDWORD)
    return (char *)filePtr;
}
