/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.
	
	  
	File		: develop.c
	Programmer	: Andrew Eder
	Date		: 22/2/99
	
----------------------------------------------------------------------------------------------- */

#define F3DEX_GBI_2

#include <ultra64.h>

#include "incs.h"


#define MAXMENUOPTIONS	4

unsigned long developmentMode = 0;

/*	--------------------------------------------------------------------------------
	Function		: RunDevelopmentRoutines
	Purpose			: for testing various development routines
	Parameters		: 
	Returns			: void
	Info			: 
*/
void RunDevelopmentMenu()
{
	static unsigned long currentSelection = 0;
	static u16 button, lastbutton;
	static TEXTOVERLAY *dev,*objView,*sndView,*watView,*prcView;
	int i = 0, xPos, j;

	if(frameCount == 1)
	{
		FreeAllLists();
		LoadTextureBank(SYSTEM_TEX_BANK);
		LoadTextureBank(TITLESGENERIC_TEX_BANK);

		dev = CreateAndAddTextOverlay(30,24,"DEVELOPMENT MENU",NO,255,smallFont,0,0);
		dev = CreateAndAddTextOverlay(32,26,"DEVELOPMENT MENU",NO,255,smallFont,0,0);
		dev->r = 0;	dev->g = 0;	dev->b = 0;

		objView	= CreateAndAddTextOverlay(40,60,"object viewer",NO,255,smallFont,0,0);
		sndView	= CreateAndAddTextOverlay(40,80,"sound player",NO,255,smallFont,0,0);
		watView	= CreateAndAddTextOverlay(40,100,"water stuff",NO,255,smallFont,0,0);
		prcView	= CreateAndAddTextOverlay(40,120,"procedural stuff",NO,255,smallFont,0,0);

		// top pane
		CreateAndAddSpriteOverlay(25,20,"tippane.bmp",270,25,191,0);

		ResetParameters();
	}

	button = controllerdata[ActiveController].button;

	if((button & CONT_UP) && !(lastbutton & CONT_UP))
    {
		if(currentSelection > 0)
			currentSelection--;
	}
	    
	if((button & CONT_DOWN) && !(lastbutton & CONT_DOWN))
    {
		if(currentSelection < (MAXMENUOPTIONS-1))
			currentSelection++;
	}

	if(frameCount > 15)
	{
		if((button & CONT_B) && !(lastbutton & CONT_B))
		{
			FreeMenuItems();
			developmentMode = -1;
			gameState.mode = MENU_MODE;
			gameState.menuMode = TITLE_MODE;

			frameCount = 0;
			lastbutton = 0;
			return;
		}

		if (((button & CONT_A) && !(lastbutton & CONT_A)) ||
		   ((button & CONT_START) && !(lastbutton & CONT_START)))
		{
			frameCount = 0;
			lastbutton = 0;

			switch(currentSelection)
			{
				case 0:   // object viewer
					developmentMode = OBJVIEW_MODE;
					FreeMenuItems();
					return;
				case 1:  // sound player
					developmentMode = SNDVIEW_MODE;
					FreeMenuItems();
					return;
				case 2:  // water stuff
					developmentMode = WATERVIEW_MODE;
					FreeMenuItems();
					return;
				case 3:  // procedural stuff
					developmentMode = PROCVIEW_MODE;
					FreeMenuItems();
					return;
			}
		}

		if ((button & CONT_G) && !(lastbutton & CONT_G))
		{
			FreeMenuItems();
			developmentMode = -1;
			gameState.mode = MENU_MODE;
			gameState.menuMode = TITLE_MODE;

			frameCount = 0;
			lastbutton = 0;
		}			
	}

	switch(currentSelection)
	{
		case 0:
			objView->a = 255;
			sndView->a = 91;
			watView->a = 91;
			prcView->a = 91;
			break;
			
		case 1:
			objView->a = 91;
			sndView->a = 255;
			watView->a = 91;
			prcView->a = 91;
			break;

		case 2:
			objView->a = 91;
			sndView->a = 91;
			watView->a = 255;
			prcView->a = 91;
			break;

		case 3:
			objView->a = 91;
			sndView->a = 91;
			watView->a = 91;
			prcView->a = 255;
			break;
	}

	lastbutton = button;
}


/*	--------------------------------------------------------------------------------
	Function		: RunProcDemo
	Purpose			: test routines for procedural textures
	Parameters		: 
	Returns			: void
	Info			: 
*/
void RunProcDemo()
{
	static u16 button,lastbutton;
	static s16 stickX,stickY;
	int j;
	static SPRITEOVERLAY *sOv1 = NULL;

	if(frameCount == 1)
	{
		TEXTOVERLAY *txtTmp;
		
		FreeMenuItems();
		LoadTextureBank(SYSTEM_TEX_BANK);

		CreateAndAddTextOverlay(30,24,"procedural stuff",NO,255,smallFont,0,0);
		txtTmp = CreateAndAddTextOverlay(32,26,"procedural stuff",NO,255,smallFont,0,0);
		txtTmp->r = 0;	txtTmp->g = 0;	txtTmp->b = 0;

		sOv1 = CreateAndAddSpriteOverlay(50,60,"testfire.bmp",64,64,192,0);
		CreateAndAddProceduralTexture(sOv1->frames[0],"prc_fire1");
		for(j=0; j<1024; j++)
			sOv1->frames[0]->data[j] = 0;

		CreateAndAddSpriteOverlay(25,20,"tippane.bmp",270,25,191,0);
	}

	// read controller
	button = controllerdata[ActiveController].button;
	stickX = controllerdata[ActiveController].stick_x;
	stickY = controllerdata[ActiveController].stick_y;

	if((button & CONT_UP))
		sOv1->yPos--;

	if((button & CONT_DOWN))
		sOv1->yPos++;

	if((button & CONT_LEFT))
		sOv1->xPos--;

	if((button & CONT_RIGHT))
		sOv1->xPos++;

	if((button & CONT_C))
		sOv1->width--;

	if((button & CONT_F))
		sOv1->width++;

	if((button & CONT_D))
		sOv1->height--;

	if((button & CONT_E))
		sOv1->height++;
	

	if((button & CONT_G) && !(lastbutton & CONT_G))
	{
		FreeAllLists();
		developmentMode = 0;
		gameState.mode = MENU_MODE;
		gameState.menuMode = TITLE_MODE;

		frameCount = 0;
		lastbutton = 0;
		return;
	}

	lastbutton = button;
}

