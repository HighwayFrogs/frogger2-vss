/*
	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: ptexture.h
	Purpose		: Procedurally generated textures
	Programmer	: Jim Hubbard
	Date		: 2/12/99 10:30

----------------------------------------------------------------------------------------------- */
#ifndef __PTEXTURE_H
#define __PTEXTURE_H



typedef struct TAGPROCTEXTURE
{
	struct TAGPROCTEXTURE *next;

	unsigned char *buf1, *buf2;
	short *palette;

	TEXTURE *tex;

	void  (*Update) ();

} PROCTEXTURE;


extern PROCTEXTURE *prcTexList;
extern POLYGON *rpList;


extern void CreateAndAddProceduralTexture( TEXENTRY *tex, char *name );
extern void CreateAndAddRandomPoly( TEXTURE *tex, VECTOR *pos, VECTOR *normal, float w, float h );
extern void ProcessProcTextures( );
extern void FreeProcTextures( );
extern void FreeRandomPolyList( );

extern void ProcessPTFire( PROCTEXTURE *pt );


#endif