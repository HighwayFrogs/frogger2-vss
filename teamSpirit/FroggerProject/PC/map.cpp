/*

	Note to readers - copied directly from PSX version for now
	This could be almost entirely shared code!

*/




#include <islmem.h>
#include <islfile.h>
#include <stdio.h>
#include <islutil.h>

#include "Define.h"

#include "LayOut.h"
#include "MemLoad.h"
#include "NewPsx.h"

#include "map.h"


int numBabies = 0;
int tileCount = 0;
int scenicCount = 0;

void *collisionFile = NULL;
void *entityFile = NULL;
void *scenicFile = NULL;

GAMETILE	*gTStart[4];
GAMETILE	*bTStart[5];

GAMETILE	*firstTile;
SCENIC		*firstScenic;


void LoadScenics ( int collBank )
{
	int c;
	int fileLength;
	unsigned char* p;

	int *ptrAddr;

	char fileName[64];

	SCENIC *cur;

	switch ( collBank )
	{
		case GARDENLEV1_COL:
				sprintf ( fileName, "SCENICS\\GARDEN1.SCE" );
			break;

		case GARDENLEV2_COL:
				sprintf ( fileName, "SCENICS\\GARDEN2.SCE" );
			break;

		case ANCIENTLEV1_COL:
			sprintf ( fileName, "SCENICS\\ANCIENTS1.SCE" );
			break;

		case ANCIENTLEV2_COL:
			sprintf ( fileName, "SCENICS\\ANCIENTS2.SCE" );
			break;

		case ANCIENTLEV3_COL:
			sprintf ( fileName, "SCENICS\\ANCIENTS3.SCE" );
			break;

		case ANCIENTMASTER_COL:
			sprintf ( fileName, "SCENICS\\ANCIENTSBOSS.SCE" );
			break;
		
		case SPACELEV1_COL:
			sprintf ( fileName, "SCENICS\\SPACE1.SCE" );
			break;

		case SPACELEV2_COL:
			sprintf ( fileName, "SCENICS\\SPACE2.SCE" );
			break;

		case CITYLEV3_COL:
			sprintf ( fileName, "SCENICS\\CITY3.SCE" );
			break;

		case SUBTERRANEANLEV1_COL:
			sprintf ( fileName, "SCENICS\\SUB1.SCE" );
			break;

		case SUBTERRANEANLEV2_COL:
			sprintf ( fileName, "SCENICS\\SUB2.SCE" );
			break;

		case LABORATORYLEV1_COL:
			sprintf ( fileName, "SCENICS\\LAB1.SCE" );
			break;

		case LABORATORYLEV2_COL:
			sprintf ( fileName, "SCENICS\\LAB2.SCE" );
			break;

		case LABORATORYLEV3_COL:
			sprintf ( fileName, "SCENICS\\LAB3.SCE" );
			break;


		case HALLOWEENLEV1_COL:
			sprintf ( fileName, "SCENICS\\HALLO1.SCE" );
			break;

		case HALLOWEENLEV2_COL:
			sprintf ( fileName, "SCENICS\\HALLO2.SCE" );
			break;

		case HALLOWEENLEV3_COL:
			sprintf ( fileName, "SCENICS\\HALLO3.SCE" );
			break;


		case FRONTEND1_COL:
			sprintf ( fileName, "SCENICS\\START.SCE" );
			break;

	}
	// ENDSWITCH

	//bb

	if(scenicFile)
		FREE(scenicFile);

	scenicFile = fileLoad(fileName, &fileLength);
	p = (unsigned char*)scenicFile;

	// Get a pointer to the addr and get scenic count from it...
	ptrAddr			= ( int* )p;
	scenicCount = ( int ) *ptrAddr;

	// Move on 4 bytes so that we can get the number of babies.
	p += 4;

	c = 0;

	firstScenic = ( SCENIC* ) scenicFile;
	cur = firstScenic;

	utilPrintf("%d\n\n\n\n", sizeof(SCENIC));
	for ( c = 0; cur; c++, cur = cur->next )
	{
		if ( ( int ) cur->next != -1 )
			cur->next = ( SCENIC* ) ( ( int ) cur->next + ( int ) firstScenic );
		else
				// If not a valid pointer then set the pointer to NULL
			cur->next = 0;
		// ENDELSEIF
//		utilPrintf("%s, %d, %d, %d, %d\n", cur->name, cur->pos.vx*SCALE, cur->pos.vy, cur->pos.vz, scenicCount);

		cur->pos.vx *= SCALE;
		cur->pos.vy *= SCALE;
		cur->pos.vz *= SCALE;

		utilPrintf("%s, %d, %d, %d, %d\n", cur->name, cur->pos.vx*SCALE, cur->pos.vy, cur->pos.vz, scenicCount);
	}
	// ENDFOR

}

void LoadLevelEntities ( int worldID, int levelID )
{
	int fileLength;
	char fileName[256];

//	sprintf ( fileName, "\\maps\\entity-%d-%d.dat", worldID, levelID );
	sprintf ( fileName, "MAPS\\ENTITY-%d-%d.dat", worldID, levelID );

	//bb
	if(entityFile)
		FREE(entityFile);

	entityFile = ( void * ) fileLoad ( fileName, &fileLength );

	if ( !entityFile )
		return;
	// ENDIF

	if ( !MemLoadEntities ( entityFile ) )
	{
		utilPrintf ( "MemLoadEntities failed\n" );
	}
	// ENDIF

	//bb
	if(entityFile)
		FREE(entityFile);
}

void LoadMapBank ( int collBank )
{
//	DBPRINTF(("Loading Map Banks, Collision And Scenics.\n"));

	LoadCollision ( collBank );
	LoadScenics		( collBank );
}


void LoadCollision ( int collBank )
{

	int c, start, fileLength;

	int *ptrAddr;
	unsigned char *p;

	int babStart[5];	// Temp holders for the baby game tiles....

	char fileName[64];

	GAMETILE *cur;

//	fileName = MALLOC0( sizeof(32) );
	switch ( collBank )
	{
		case GARDENLEV1_COL:
			sprintf ( fileName, "COLLISION\\GARDEN1.COL" );
			break;

		case GARDENLEV2_COL:
			sprintf ( fileName, "COLLISION\\GARDEN2.COL" );
			break;

		case ANCIENTLEV1_COL:
			sprintf ( fileName, "COLLISION\\ANCIENTS1.COL" );
			break;

		case ANCIENTLEV2_COL:
			sprintf ( fileName, "COLLISION\\ANCIENTS2.COL" );
			break;

		case ANCIENTLEV3_COL:
			sprintf ( fileName, "COLLISION\\ANCIENTS3.COL" );
			break;

		case ANCIENTMASTER_COL:
			sprintf ( fileName, "COLLISION\\ANCIENTSBOSS.COL" );
			break;

		case SPACELEV1_COL:
			sprintf ( fileName, "COLLISION\\SPACE1.COL" );
			break;

		case SPACELEV2_COL:
			sprintf ( fileName, "COLLISION\\SPACE2.COL" );
			break;

		case CITYLEV3_COL:
			sprintf ( fileName, "COLLISION\\CITY3.COL" );
			break;

		case SUBTERRANEANLEV1_COL:
			sprintf ( fileName, "COLLISION\\SUB1.COL" );
			break;

		case SUBTERRANEANLEV2_COL:
			sprintf ( fileName, "COLLISION\\SUB2.COL" );
			break;

		case LABORATORYLEV1_COL:
			sprintf ( fileName, "COLLISION\\LAB1.COL" );
			break;

		case LABORATORYLEV2_COL:
			sprintf ( fileName, "COLLISION\\LAB2.COL" );
			break;

		case LABORATORYLEV3_COL:
			sprintf ( fileName, "COLLISION\\LAB3.COL" );
			break;

		case HALLOWEENLEV1_COL:
			sprintf ( fileName, "COLLISION\\HALLO1.COL" );
			break;

		case HALLOWEENLEV2_COL:
			sprintf ( fileName, "COLLISION\\HALLO2.COL" );
			break;

		case HALLOWEENLEV3_COL:
			sprintf ( fileName, "COLLISION\\HALLO3.COL" );
			break;


		case FRONTEND1_COL:
			sprintf ( fileName, "COLLISION\\START.COL" );
			break;




	}
	// ENDSWITCH

//	strcat ( fileName, ".col" );

//bb
	if(collisionFile)
		FREE(collisionFile);

	collisionFile = ( void* ) fileLoad ( fileName, &fileLength );

	p = (unsigned char*) collisionFile;

	// Get a pointer to the addr and get tile count from it...
	ptrAddr		= ( int* ) firstTile;
	tileCount = ( int ) *ptrAddr;

	// Move on 4 bytes so that we can get the start tile.
	p += 4;

	// Get a pointer to the addr and get a temp pointer to the Frogger startTile.....
	ptrAddr = ( int* ) firstTile;
	start		= ( int ) *ptrAddr;

	// Move on 4 bytes so that we can get the number of babies.
	p += 4;

	// Get a pointer to the addr and get the number of babies.....
	ptrAddr		= ( int* ) firstTile;
	numBabies	= ( int ) *ptrAddr;

	// Move on 4 bytes so that we can get the first baby game tile....
	p += 4;

	for ( c = 0; c < numBabies; c++ )
	{
		// Get a pointer to the addr and get the game tile.....
		babStart [ c ]	= *(int*)p;

		// Move on 4 bytes so that we can get the next baby game tile....
		p += 4;

	}
	// ENDFOR - for ( c = 0; c < numBabies; c++ )
	
	firstTile = (GAMETILE*)p;
	// These need to be done last, so that we get the correct firstTile offset in the file.....

	//  Set the offsets for the memory pointers in memory for the baby game tiles....
	for ( c = 0; c < numBabies; c++ )
	{
		bTStart [c] = ( GAMETILE* ) ( ( int ) babStart [ c ] + ( int ) firstTile );
	}

	//  And lets do the same for the frogger start tile....

	gTStart [0] = ( GAMETILE* ) ( ( int ) start + ( int ) firstTile );


	c = 0;
	cur = firstTile;

	for ( c = 0; c < tileCount; c++, cur = cur->next )
	{
		// If the memory pointer is not -1 then set them up so that they offset into the correct place in the tile list...

		if ( ( int ) cur->tilePtrs[0] != -1 )
			cur->tilePtrs[0] = ( GAMETILE* ) ( ( int ) cur->tilePtrs[0] + ( int ) firstTile );
		else
				// If not a valid pointer then set the pointer to NULL
			cur->tilePtrs[0] = 0;
		// ENDELSEIF

		if ( ( int ) cur->tilePtrs[1] != -1 )
			cur->tilePtrs[1] = ( GAMETILE* ) ( ( int ) cur->tilePtrs[1] + ( int ) firstTile );
		else
				// If not a valid pointer then set the pointer to NULL
			cur->tilePtrs[1] = 0;
		// ENDELSEIF

		if ( ( int ) cur->tilePtrs[2] != -1 )
			cur->tilePtrs[2] = ( GAMETILE* ) ( ( int ) cur->tilePtrs[2] + ( int ) firstTile );
		else
				// If not a valid pointer then set the pointer to NULL
			cur->tilePtrs[2] = 0;
		// ENDELSEIF

		if ( ( int ) cur->tilePtrs[3] != -1 )
			cur->tilePtrs[3] = ( GAMETILE* ) ( ( int ) cur->tilePtrs[3] + ( int ) firstTile );
		else
				// If not a valid pointer then set the pointer to NULL
			cur->tilePtrs[3] = 0;
		// ENDELSEIF


		if ( (int)cur->next != -1 )
			cur->next = ( GAMETILE* ) ( ( int ) cur->next + ( int ) firstTile );
		else
				// If not a valid pointer then set the pointer to NULL
			cur->next = 0;
		// ENDELSEIF


		cur->centre.vx *= SCALE;
		cur->centre.vy *= SCALE;
		cur->centre.vz *= SCALE;

// 		for ( i = 0; i < 4; i++ )
// 		{
// 			cur->dirVector[i].vz *= -1;
// 		}
// 		// ENDFOR
// 
// 		cur->normal.vz *= -1;

	}
	// ENDFOR


}

void FreeEntityBank ( void )
{
	utilPrintf("Freeing Entity Bank : MAP\n");

	FREE ( ( void* ) entityFile );
}


void FreeMap ( void )
{
	utilPrintf("Freeing Map Bank : MAP\n");

	FREE ( ( void* ) collisionFile );
}

void FreeScenic ( void )
{
	utilPrintf("Freeing Scenic Bank : MAP\n");

	FREE ( ( void* ) scenicFile );
}
