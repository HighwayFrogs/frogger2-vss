/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.


	File		: frogger.h
	Programmer	: James Healey
	Date		: 23/03/99

----------------------------------------------------------------------------------------------- */

#ifndef FROGGER_H_INCLUDED
#define FROGGER_H_INCLUDED


#define SPAWN_SCOREUPTIMER		15

#define MAX_FROGS 4
extern long NUM_FROGS;

//----- [ FROG ANIMATION ENUMS ] -----------------------------------------------------------------

enum
{
	FROG_ANIM_STDJUMP,
	FROG_ANIM_BREATHE,
	FROG_ANIM_FWDSOMERSAULT,
	FROG_ANIM_LANDONHEAD,
	FROG_ANIM_ASSONFIRE,
	FROG_ANIM_GOTOFLOAT,
	FROG_ANIM_JUMPBEFOREFALL,
	FROG_ANIM_DROWNING,
	FROG_ANIM_TRYTOFLY,
	FROG_ANIM_FLOATFALL,
	FROG_ANIM_GETUPFROMFLOAT,
	FROG_ANIM_UNKNOWN2,
	FROG_ANIM_LOOKLEFTANDRIGHT,
	FROG_ANIM_LOOKDOWN,
	FROG_ANIM_GALLOPINGFROG,
	FROG_ANIM_FORWARDSOMERSAULT,
	FROG_ANIM_USINGTONGUE,
	FROG_ANIM_SCRATCHHEAD,
	FROG_ANIM_DANCE1,
	FROG_ANIM_DANCE2,
	FROG_ANIM_DANCE3,
	FROG_ANIM_UNKNOWN4,

	NUM_FROG_ANIMS,
};

//------------------------------------------------------------------------------------------------

typedef struct _PLAYER
{
	char			name[3];
	unsigned long	score;
	short			lives;
	short			numSpawn;
	short			timeSec;

	unsigned char	inputPause;			// used for controller input

	short			spawnTimer;
	short			spawnScoreLevel;

	short		 	worldNum;
	short			levelNum;
	unsigned long	frogState;
	short			saveSlot;

	// frog movement related stuff
	unsigned char 	canJump;
	unsigned char	isSuperHopping;
	unsigned char	isLongHopping;

	unsigned char	extendedHopDir;

	VECTOR			jumpVelocity;

} PLAYER;

extern PLAYER player[];
							

extern float CROAK_SOUND_RANGE;

extern float globalFrogScale;

extern long isLong;
extern float longSpeed;
extern float longAmt;

extern float changeLongSpeed;
extern float startLongSpeed;


extern ACTOR2	*frog[];
extern SPRITEOVERLAY *sprHeart[3];

extern float	croakRadius;
extern float	croakVelocity;
extern char		croakFade;
extern char		croakFadeDec;
extern char		croakR,croakG,croakB;



extern void CreateFrogger ( unsigned long createFrogActor, unsigned long createFrogOverlays,
							 unsigned long createBabyActors, unsigned long createBabyOverlays );

#endif


