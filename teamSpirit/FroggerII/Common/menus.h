/*

	This file is part of Frogger2, (c) 1997 Interactive Studios Ltd.


	File		: menus.h
	Programmer	: James (Random) Healey
	Date		: 27/05/99

----------------------------------------------------------------------------------------------- */

#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED


typedef struct LEVELNAMESTRUCT
{
	char name[64];
} LNAMESTRUCT;

extern SPRITE *sp;
extern SPRITEOVERLAY *atari,*konami,*flogo[10], *islLogo[3];
extern SPRITEOVERLAY *sprOver;

void RunTitleScreen();
void RunLevelSelect();
void RunCharSelect();
void RunPauseMenu();
void FreeMenuItems();

#endif
