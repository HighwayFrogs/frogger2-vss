/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: banks.h
	Programmer	: David Swift
	Date		: 21/3/00

----------------------------------------------------------------------------------------------- */

#include <windows.h>
#include "banks.h"
#include "mdx.h"
#include "layout.h"
#include "Main.h"
#include <islutil.h>

TextureBankType *textureBanks [ MAX_TEXTURE_BANKS ];

void LoadObjectBank(int objectBank)					
{
	char fileName[MAX_PATH] = "";

	switch ( objectBank )
	{
		case INGAMEGENERIC_OBJ_BANK:
			strcpy(fileName, "gamegeneric"); break;

// Garden

		case GARDENMASTER_OBJ_BANK:
			strcpy(fileName, "gardenmaster");	break;

		case GARDENLEV1_OBJ_BANK:
			strcpy(fileName, "garden1"); break;

		case GARDENLEV2_OBJ_BANK:
			strcpy(fileName, "garden1"); break;

		case GARDENMULTI_OBJ_BANK:
			strcpy(fileName, "gardenmulti"); break;

// Ancient

		case ANCIENTMASTER_OBJ_BANK:
			strcpy(fileName, "ancientsmaster"); break;

		case ANCIENTLEV1_OBJ_BANK:				
			strcpy(fileName, "ancients1"); break;

		case ANCIENTLEV2_OBJ_BANK:				
			strcpy(fileName, "ancients2"); break;

		case ANCIENTLEV3_OBJ_BANK:				
			strcpy(fileName, "ancients3"); break;

		case ANCIENTBOSS_OBJ_BANK:				
			strcpy(fileName, "ancientsboss"); break;

		case ANCIENTMULTI_OBJ_BANK:
			strcpy(fileName, "ancientsmulti"); break;

// Space

		case SPACEMASTER_OBJ_BANK:					
			strcpy(fileName, "spacemaster"); break;

		case SPACELEV1_OBJ_BANK:				
			strcpy( fileName, "space1" ); break;

		case SPACELEV2_OBJ_BANK:
			strcpy(fileName, "space2"); break;

		case SPACELEV3_OBJ_BANK:
			strcpy(fileName, "space3"); break;

		case SPACEBOSS_OBJ_BANK:
			strcpy(fileName, "spaceboss"); break;

// City

		case CITYMASTER_OBJ_BANK:
			strcpy(fileName, "citymaster"); break;

		case CITYBOSS_OBJ_BANK:
			strcat (fileName,"cityboss"); break;

		case CITYLEV1_OBJ_BANK:
			strcpy(fileName, "city1"); break;

		case CITYLEV2_OBJ_BANK:
			strcpy(fileName, "city2"); break;

		case CITYLEV3_OBJ_BANK:
			strcpy(fileName, "city3"); break;

		case CITYMULTI_OBJ_BANK:
			strcpy(fileName, "citymulti"); break;

// Subterranean

		case SUBTERRANEANMASTER_OBJ_BANK:
			strcpy(fileName, "submaster"); break;

		case SUBTERRANEANBOSS_OBJ_BANK:
			strcat (fileName, "subboss"); break;

		case SUBTERRANEANBOSSB_OBJ_BANK:
			strcat (fileName, "subtest"); break;

		case SUBTERRANEANLEV1_OBJ_BANK:
			strcpy(fileName, "sub1"); break;

		case SUBTERRANEANLEV2_OBJ_BANK:
			strcpy(fileName, "sub2"); break;

		case SUBTERRANEANLEV3_OBJ_BANK:
			strcpy(fileName, "sub2"); break;

// Laboratory

		case LABORATORYMASTER_OBJ_BANK:
			strcpy(fileName, "labmaster"); break;

		case LABORATORYLEV1_OBJ_BANK:
			strcpy(fileName, "lab1"); break;

		case LABORATORYLEV2_OBJ_BANK:
			strcpy(fileName, "lab2"); break;

		case LABORATORYLEV3_OBJ_BANK:
			strcpy(fileName, "lab3"); break;

		case LABORATORYBOSSA_OBJ_BANK:
			strcat(fileName, "labbossa"); break;
	
		case LABORATORYBOSSB_OBJ_BANK:
			strcat (fileName, "labbossb"); break;

		case LABORATORYBOSSC_OBJ_BANK:
			strcat (fileName, "labbossc"); break;

		case LABORATORYMULTI_OBJ_BANK:
			strcpy(fileName, "labmulti"); break;

// Halloween

		case HALLOWEENMASTER_OBJ_BANK:
			strcpy(fileName, "halloweenmaster"); break;

		case HALLOWEENLEV1_OBJ_BANK:
			strcpy(fileName, "halloween1"); break;

		case HALLOWEENLEV2_OBJ_BANK:
			strcpy(fileName, "halloween2"); break;

		case HALLOWEENLEV3_OBJ_BANK:
			strcpy(fileName, "halloween3"); break;

		case HALLOWEENBOSS_OBJ_BANK:
			strcat(fileName, "halloweenboss"); break;

		case HALLOWEENBOSSB_OBJ_BANK:
			strcat(fileName, "halloweenbossb");	break;

// Super-retro
		case RETROLEV1_OBJ_BANK:
			strcat(fileName,"sretro1");
			break;
		case RETROLEV2_OBJ_BANK:
			strcat(fileName,"sretro2");
			break;
		case RETROLEV3_OBJ_BANK:
			strcat(fileName,"sretro3");
			break;
		case RETROLEV4_OBJ_BANK:
			strcat(fileName,"sretro4");
			break;
		case RETROLEV5_OBJ_BANK:
			strcat(fileName,"sretro5");
			break;
		case RETROLEV6_OBJ_BANK:
			strcat(fileName,"sretro6");
			break;
		case RETROLEV7_OBJ_BANK:
			strcat(fileName,"sretro7");
			break;
		case RETROLEV8_OBJ_BANK:
			strcat(fileName,"sretro8");
			break;
		case RETROLEV9_OBJ_BANK:
			strcat(fileName,"sretro9");
			break;
		case RETROLEV10_OBJ_BANK:
			strcat(fileName,"sretro10");
			break;
		case RETROMASTER_OBJ_BANK:
			strcat(fileName,"sretroboss");
			break;
			
// Front-end

		case FRONTEND_OBJ_BANK:
			strcpy(fileName, "title"); break;

		case FRONTEND1_OBJ_BANK:
			strcpy(fileName, "start"); break;

		case FRONTEND2_OBJ_BANK:
			strcpy(fileName, "levela"); break;

		case FRONTEND3_OBJ_BANK:
			strcpy(fileName, "levelb"); break;

		case FRONTEND4_OBJ_BANK:
			strcpy(fileName, "multisel"); break;

		case FRONTEND5_OBJ_BANK:
			strcpy(fileName, "language"); break;

// Multiplayer

		case MULTIPLAYER_OBJ_BANK:
			strcpy(fileName, "language"); break;

		default:
			utilPrintf("Didn't load object bank '%d'\n", objectBank);
			return;
	}

	//newobj(fileName);

	LoadObjBank(fileName,baseDirectory);
}

void LoadTextureBank(int textureBank)					
{
	char fileName[256];

	switch ( textureBank )
	{
		case GARDEN_TEX_BANK:
			strcpy( fileName, "Garden" );
			break;

		case ANCIENT_TEX_BANK:
			strcpy( fileName, "ancient" );
			break;

		case SPACE_TEX_BANK:				
			strcpy( fileName, "space" );
			break;

		case CITY_TEX_BANK:				
			strcpy(fileName, "city");
			break;

		case SUBTERRANEAN_TEX_BANK:
			strcpy(fileName, "subter");
			break;

		case LABORATORY_TEX_BANK:
			strcpy(fileName, "lab");
			break;

		case HALLOWEEN_TEX_BANK:
			strcpy(fileName, "Halloween");
			break;
		
		case FRONT_TEX_BANK:
			strcpy(fileName, "front");
			break;
		
		case TITLES_TEX_BANK:
		case TITLESGENERIC_TEX_BANK:
			strcpy( fileName, "titles" );
			break;

		case FRONTEND_TEX_BANK:				
			strcpy( fileName, "titles" );
			break;

		case INGAMEGENERIC_TEX_BANK:
			strcpy( fileName, "System" );
			break;

		default:
			return;
	}

	LoadTexBank(fileName,baseDirectory);	
	return;
}

void FreeTextureBank(TextureBankType *textureBank)		
{
	utilPrintf("!!! FreeTextureBank() : TEXTURE BANKS ARE NOT FREED CORRECTLY !!!\n");
}

void FreeAllTextureBanks(void)							
{
	utilPrintf("!!! FreeAllTextureBanks() : TEXTURE BANKS ARE NOT FREED CORRECTLY !!!\n");
}
/*
void newobj(char* fn)
{
	int i;char*c,*d;
	____ _____ ___ fn, ______ ___ 0; __(_____);	_(_____), _(______));
	____ ________ ___ _____	+7,	_(______); ______; ______--,
	_d(________) ___ _d(_____) _______;	____ _____ ___ fn, ________ 
	___ "newobj\\"; __(________); __(_(_____)) ___ __(_(________)) _______;
}
*/
