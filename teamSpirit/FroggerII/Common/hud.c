/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.


	File		: hud.c
	Programmer	: James Healey
	Date		: 31/03/99

----------------------------------------------------------------------------------------------- */

#define F3DEX_GBI

#include <ultra64.h>

#include "incs.h"


char scoreText[32]	= "10000000";
char livesText[8]	= "xxxx";
char timeText[32]	= "00:00";

char countdownTimer	= 1;
char displayFullScreenPoly = 0;

//long timeMin	 = 0, timeSec = 0;
//long timeMin	 = 2, timeSec = (90*30);

//long score		 = 0l
//long lives   = 0;
long gameIsOver  = 0;
long levelIsOver = 0;

ANIM_STRUCTURE *livesIcon;

TEXTOVERLAY	*livesTextOver,*timeTextOver,*scoreTextOver;
TEXTOVERLAY	*gameOver1;
TEXTOVERLAY	*gameOverScore;
TEXTOVERLAY	*babySavedText;

TEXTOVERLAY *keyCollected;

TEXTOVERLAY *pauseTitle;

TEXTOVERLAY *continueText;
TEXTOVERLAY *quitText;
TEXTOVERLAY *nextLev1;
TEXTOVERLAY *nextLev2;

TEXTOVERLAY *wholeKeyText = NULL;

void UpDateOnScreenInfo ( void )
{
	int i;

	sprintf(livesText,"*%lu",player[0].lives);	
	sprintf(scoreText,"%lu",player[0].score);

	if(scoreTextOver->a && !levelIsOver)
	{
		scoreTextOver->a -= 8;
		if(scoreTextOver->a < 8)
			scoreTextOver->a = 0;
	}


	if(countdownTimer)
	{
		player[0].timeSec--;
		if (player[0].timeSec < 0)
		{
			countdownTimer = 0;
			PlaySample(28,NULL,255,128);
			sprintf(timeText,"NO BONUS !");
			timeTextOver->draw = 50;
		}

		if((player[0].timeSec < (11*30)) && ((player[0].timeSec%30) == 0))
			PlaySample(93,NULL,255,128);
								   
		if(player[0].timeSec >= 0)
			sprintf(timeText,"%02lu",(player[0].timeSec/30));
	}
	else
	{
		if(timeTextOver->draw)
			timeTextOver->draw--;
		timeTextOver->a -= 16;
		timeTextOver->a &= 255;
	} 


	i = 3;
	while(i--)
	{
		if ( frog[0] )
		{
			if(frog[0]->action.lives <= i)
				sprHeart[i]->a = 64;
			else
				sprHeart[i]->a = 255;
		}
	}
}


/*	--------------------------------------------------------------------------------
	Function		: UpdateScore
	Purpose			: updates the current score
	Parameters		: ACTOR2 *,long
	Returns			: void
	Info			: 
*/
void UpdateScore(ACTOR2 *act,long scoreUpdate)
{
	player[0].score += scoreUpdate;
	scoreTextOver->a = 255;
}


/*	--------------------------------------------------------------------------------
	Function		: InitInGameTextOverlays
	Purpose			: creates and initialises in game text overlay messages
	Parameters		: unsigned long,unsigned long
	Returns			: void
	Info			: 
*/
void InitInGameTextOverlays(unsigned long worldID,unsigned long levelID)
{
	currFont = smallFont;

	pauseTitle		= CreateAndAddTextOverlay ( 50, 70, "pause", YES, NO, 255, 255, 255, 255, bigFont, 0, 0, 0 );
	continueText	= CreateAndAddTextOverlay ( 50, 110, "continue", YES, NO, 255, 255, 255, 255, currFont, 0,0, 0 );
	quitText		= CreateAndAddTextOverlay ( 50, 130, "quit", YES, NO, 255, 255, 255, 255, currFont, 0,0, 0 );
	DisableTextOverlay ( pauseTitle );
	DisableTextOverlay ( continueText );
	DisableTextOverlay ( quitText );

	livesTextOver	= CreateAndAddTextOverlay(50,205,livesText,NO,NO,255,255,255,255,currFont,0,0,0);
	timeTextOver	= CreateAndAddTextOverlay(25,20,timeText,NO,NO,255,255,255,255,currFont,0,0,0);
	scoreTextOver	= CreateAndAddTextOverlay(230,20,scoreText,NO,NO,255,255,255,255,currFont,0,0,0);

	gameOver1 = CreateAndAddTextOverlay(60,239,"game over",NO,NO,255,255,255,255,bigFont,TEXTOVERLAY_WAVECHARS,6,0);
	DisableTextOverlay(gameOver1);
	gameOverScore = CreateAndAddTextOverlay(110,0,"************************",YES,NO,255,255,255,255,smallFont,0,0,0);
	DisableTextOverlay(gameOverScore);

	babySavedText = CreateAndAddTextOverlay(0,110,"BABY SAVED",YES,NO,255,255,255,255,smallFont,TEXTOVERLAY_WAVECHARS,4,0);
	DisableTextOverlay(babySavedText);
	babySaved = 0;

//	keyCollected = CreateAndAddTextOverlay(0,100,"",YES,NO,255,255,255,255,bigFont,TEXTOVERLAY_LIFE,0,0);
//	DisableTextOverlay(keyCollected);


	//  Awards screen
	nextLev1 = CreateAndAddTextOverlay ( 0, 180, "next", YES, NO, 255, 255, 255, 255, smallFont, 0, 0, 0 );
	DisableTextOverlay ( nextLev1 );

	nextLev2 = CreateAndAddTextOverlay ( 0, 200, worldVisualData [ player[0].worldNum ].levelVisualData [ player[0].levelNum+1 ].description, YES, NO, 255, 255, 255, 255, smallFont, 0, 0, 0 );
	DisableTextOverlay ( nextLev2 );

	wholeKeyText = CreateAndAddTextOverlay ( 0, 110, "World Open", YES, NO, 255, 255, 255, 255, smallFont, 0, 0, 0 );;
	wholeKeyText->draw = 0;
}



/*	--------------------------------------------------------------------------------
	Function		: RunGameOverSequence
	Purpose			: runs the game over sequence
	Parameters		: 
	Returns			: void
	Info			: 
*/
void RunGameOverSequence ( void )
{
	if(!gameOver1->draw)
	{
		sprintf(gameOverScore->text,"you scored %s !!",scoreText);
		EnableTextOverlay(gameOver1);
		EnableTextOverlay(gameOverScore);
	}

	if((gameOverScore->yPos < 120) && (gameIsOver > 200))
		gameOverScore->yPos += 8;
	else if(gameIsOver < 50)
	{
		gameOverScore->centred = 0;
		gameOverScore->xPos += 8;
	}

	if((gameOver1->yPos > 80) && (gameIsOver > 150))
	{
		gameOver1->yPos -= 5;
	}
	else if(gameIsOver < 90)
	{
		gameOver1->centred = 0;
		gameOver1->xPos -= 4;
	}
}

void DisableHUD( )
{
	long i;
	livesTextOver->oa = livesTextOver->a;
	scoreTextOver->oa = scoreTextOver->a;
	timeTextOver->oa = timeTextOver->a;

	livesTextOver->a = 0;
	scoreTextOver->a = 0;
	timeTextOver->a = 0;

	for ( i = 0; i < 3; i++ )
		sprHeart[i]->draw = 0;

	for(i=0; i<numBabies; i++)
		babyIcons[i]->draw = 0;
}

void EnableHUD( )
{
	long i;
	livesTextOver->a = livesTextOver->oa;
	timeTextOver->a = timeTextOver->oa;
	scoreTextOver->a = scoreTextOver->oa;

	for ( i = 0; i < 3; i++ )
		sprHeart[i]->draw = 1;

	for(i=0; i<numBabies; i++)
		babyIcons[i]->draw = 1;
}

