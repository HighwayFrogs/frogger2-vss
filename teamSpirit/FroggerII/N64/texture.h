/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: texture.h
	Programmer	: Andy Eder
	Date		: 19/04/99 12:22:29

----------------------------------------------------------------------------------------------- */

#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED


#define MAX_TEXTURE_BANKS		5

typedef struct
{
	char	*data;
	char	*freePtr;
	int		numTextures;

}TEXTURE_BANK;

extern TEXTURE_BANK	textureBanks[MAX_TEXTURE_BANKS];
extern unsigned long numTextureBanks;


extern void LoadTextureBank(int num);
extern void InitTextureBanks();
extern void FindTexture(TEXTURE **texPtr, int texID, BOOL report, char *txtName);


#endif