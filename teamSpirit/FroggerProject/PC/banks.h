/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: banks.h
	Programmer	: David Swift
	Date		: 21/3/00

----------------------------------------------------------------------------------------------- */

#ifndef BANKS_H_INCLUDED
#define BANKS_H_INCLUDED

#include <isltex.h>

#ifdef __cplusplus
extern "C"
{
#endif

//from psx textures.h
#define MAX_TEXTURE_BANKS		5

extern TextureBankType *textureBanks [ MAX_TEXTURE_BANKS ];
extern char saveName[32];
void LoadTextureBank(int textureBank);
void FreeTextureBank(TextureBankType *textureBank);
void LoadObjectBank(int objectBank);
void LoadGame(void);
void SaveGame(void);

void CreateLevelObjects(unsigned long worldID,unsigned long levelID);

#define FreeWaterObjectList()

/*
#ifdef _______A
#define _d(x)(*x--)
#define _(x)(x++)
#define __(x)(*x)
#define ___ = 
#define ____ for(
#define _____ c
#define ______ i
#define _______ )
#define ________ d
#endif
*/

#ifdef __cplusplus
}
#endif
#endif