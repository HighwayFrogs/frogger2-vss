/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.


	File		: babyfrog.c
	Programmer	: James Healey
	Date		: 17/03/99

----------------------------------------------------------------------------------------------- */

#define F3DEX_GBI

#include <ultra64.h>

#include "incs.h"


GAMETILE **bTStart;

ACTOR2 *babies[NUM_BABIES];
ACTOR2 *nearestBaby		= NULL;
ACTOR2 *lastBabySaved	= NULL;

SPRITEOVERLAY *babyIcons[6];

unsigned long numBabies = 0;


void CreateBabies(unsigned long createActors,unsigned long createOverlays)
{
	unsigned long i,j;

	nearestBaby		= NULL;
	lastBabySaved	= NULL;
	babiesSaved		= 0;
	
	if ( createActors )
	{
		for (i=0; i<numBabies; i++)
		{
			babies[i] = CreateAndAddActor("froglet.obe",0,0,200.0,INIT_ANIMATION | INIT_SHADOW,0,0);

			babies[i]->actor->shadow->radius	= 15;
			babies[i]->actor->shadow->alpha		= 191;

			InitActorAnim ( babies[i]->actor );
			AnimateActor ( babies[i]->actor, 0, YES, NO, 0.667, 10, 0 );
			babies[i]->actor->scale.v[0] = 0.1;
			babies[i]->actor->scale.v[1] = 0.1;
			babies[i]->actor->scale.v[2] = 0.1;
			babies[i]->action.isSaved = 0;
			babies[i]->actor->pos.v[X] = bTStart[i]->centre.v[X];
			babies[i]->actor->pos.v[Y] = bTStart[i]->centre.v[Y];
			babies[i]->actor->pos.v[Z] = bTStart[i]->centre.v[Z];

//			babies[i]->flags |= ACTOR_DRAW_ALWAYS;
		}
	}

	if ( createOverlays )
	{
		for(i=0; i<numBabies; i++)
		{
			switch(i)
			{
				case 0:
					babyIcons[i] = CreateAndAddSpriteOverlay(280-(i*20),205,"frog001.bmp",16,16,255,255,255,91,ANIMATION_FORWARDS | ANIMATION_CYCLE);
					AddFrameToSpriteOverlay(babyIcons[i],"frog002.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"frog003.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"frog004.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"frog005.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"frog006.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"frog007.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"frog008.bmp");
					SetSpriteOverlayAnimSpeed(babyIcons[i],1.0F);
					babyIcons[i]->animTime = i;
					break;

				case 1:
					babyIcons[i] = CreateAndAddSpriteOverlay(280-(i*20),205,"ylfrg001.bmp",16,16,255,255,255,91,ANIMATION_FORWARDS | ANIMATION_CYCLE);
					AddFrameToSpriteOverlay(babyIcons[i],"ylfrg002.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"ylfrg003.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"ylfrg004.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"ylfrg005.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"ylfrg006.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"ylfrg007.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"ylfrg008.bmp");
					SetSpriteOverlayAnimSpeed(babyIcons[i],1.0F);
					babyIcons[i]->animTime = i;
					break;

				case 2:
					babyIcons[i] = CreateAndAddSpriteOverlay(280-(i*20),205,"blfrog001.bmp",16,16,255,255,255,91,ANIMATION_FORWARDS | ANIMATION_CYCLE);
					AddFrameToSpriteOverlay(babyIcons[i],"blfrog002.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"blfrog003.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"blfrog004.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"blfrog005.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"blfrog006.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"blfrog007.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"blfrog008.bmp");
					SetSpriteOverlayAnimSpeed(babyIcons[i],1.0F);
					babyIcons[i]->animTime = i;
					break;

				case 3:
					babyIcons[i] = CreateAndAddSpriteOverlay(280-(i*20),205,"prfrg001.bmp",16,16,255,255,255,91,ANIMATION_FORWARDS | ANIMATION_CYCLE);
					AddFrameToSpriteOverlay(babyIcons[i],"prfrg002.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"prfrg003.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"prfrg004.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"prfrg005.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"prfrg006.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"prfrg007.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"prfrg008.bmp");
					SetSpriteOverlayAnimSpeed(babyIcons[i],1.0F);
					babyIcons[i]->animTime = i;
					break;

				case 4:
					babyIcons[i] = CreateAndAddSpriteOverlay(280-(i*20),205,"rdfrg001.bmp",16,16,255,255,255,91,ANIMATION_FORWARDS | ANIMATION_CYCLE);
					AddFrameToSpriteOverlay(babyIcons[i],"rdfrg002.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"rdfrg003.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"rdfrg004.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"rdfrg005.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"rdfrg006.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"rdfrg007.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"rdfrg008.bmp");
					SetSpriteOverlayAnimSpeed(babyIcons[i],1.0F);
					babyIcons[i]->animTime = i;
					break;

				case 5:
					babyIcons[i] = CreateAndAddSpriteOverlay(280-(i*20),205,"frog001.bmp",16,16,255,255,255,91,ANIMATION_FORWARDS | ANIMATION_CYCLE);
					AddFrameToSpriteOverlay(babyIcons[i],"frog002.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"frog003.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"frog004.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"frog005.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"frog006.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"frog007.bmp");
					AddFrameToSpriteOverlay(babyIcons[i],"frog008.bmp");
					SetSpriteOverlayAnimSpeed(babyIcons[i],1.0F);
					babyIcons[i]->animTime = i;
					break;
			}
		}
	}
}


/*	--------------------------------------------------------------------------------
	Function		: RunBabySavedSequence
	Purpose			: runs sequence when baby is saved
	Parameters		: ACTOR2 *
	Returns			: void
	Info			: 
*/
void RunBabySavedSequence(ACTOR2 *baby)
{
	static VECTOR pos;
	FX_EXPLODEPARTICLE *explode;
	PLANE2 babyPlane;
	int i;

	babySaved--;
	if(!babySavedText->draw)
	{
		EnableTextOverlay(babySavedText);
		SetVector(&pos,&baby->actor->pos);
	}
	else
	{
		if(babySaved)
		{
			babySavedText->a -= 8;
		}
		else
		{
			DisableTextOverlay(babySavedText);
			babySavedText->a = 255;
		}

		if((babySaved & 3) == 0)
		{
			pos.v[Y] += 20;
			CreateAndAddFXRipple(RIPPLE_TYPE_RING,&pos,&upVec,0.1,2,1,15);

			SetVector(&babyPlane.point,&baby->actor->pos);
			SetVector(&babyPlane.normal,&upVec);

			explode = CreateAndAddFXExplodeParticle(EXPLODEPARTICLE_TYPE_COLOURBURST,&baby->actor->pos,&upVec,12,16,&babyPlane,20);
			i = MAX_EXPLODE_PARTICLES;
			while(i--)
			{
				explode->sprite[i].r = 255;
				explode->sprite[i].g = 255;
				explode->sprite[i].b = 0;
			}
		}
	}
}


/*	--------------------------------------------------------------------------------
	Function		: GetNearestBabyFrog
	Purpose			: determines the nearest baby frog
	Parameters		: 
	Returns			: ACTOR2 *
	Info			: used for, say, determining
*/
ACTOR2 *GetNearestBabyFrog()
{
	ACTOR2 *nearest = NULL;
	float t,distance = 99999;
	int i;

	i = numBabies;
	while(i--)
	{
		if(!babies[i]->action.isSaved)
		{
			t = DistanceBetweenPoints(&frog[0]->actor->pos,&babies[i]->actor->pos);
			if(t < distance)
			{
				distance	= t;
				nearest		= babies[i];
			}
		}
	}

	if(nearest && (distance < CROAK_SOUND_RANGE))
		PlaySample(218,NULL,255,128 + (128 - (distance / 4)));

	return nearest;
}

