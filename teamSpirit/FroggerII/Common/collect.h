/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.


	File		: collect.h
	Programmer	: Andrew Eder
	Date		: 11/27/98

----------------------------------------------------------------------------------------------- */

#ifndef COLLECT_H_INCLUDED
#define COLLECT_H_INCLUDED

// ----- [ DEFINES ] --------------- //

#define PICKUP_RADIUS		25.0F

#define	FRAMES_PER_SEC		20

#define	TIME_SECS_1			FRAMES_PER_SEC * 1
#define	TIME_SECS_2			FRAMES_PER_SEC * 2
#define	TIME_SECS_3			FRAMES_PER_SEC * 3
#define	TIME_SECS_4			FRAMES_PER_SEC * 4
#define	TIME_SECS_5			FRAMES_PER_SEC * 5
#define	TIME_SECS_10		FRAMES_PER_SEC * 10
#define	TIME_SECS_20		FRAMES_PER_SEC * 20
#define	TIME_SECS_30		FRAMES_PER_SEC * 30

#define GARIB_SCALE			64

#define GARIB_DROP_BOUNCE	( 1 << 0 )

typedef struct TAGGARIB
{
	struct TAGGARIB		*prev,*next;
	BYTE				active;
	UBYTE				type;
	float				scale;
	float				scaleAim;
	SPRITE				sprite;
	ACTOR_SHADOW		shadow;
	float				distanceFromFrog;
	unsigned long		flags;
	GAMETILE			*gameTile;
	float				dropSpeed;
} GARIB;

typedef struct TAGGARIBLIST
{
	GARIB				head;
	int					numEntries;

} GARIBLIST;

typedef struct TAG_SCREENSPAWN
{
	float x,y;
	float ax,ay;
	
	SPRITEOVERLAY *sp;
	struct TAG_SCREENSPAWN *next,*prev;

} SCREENSPAWN;


enum
{
	SPAWN_GARIB,			//0
	EXTRAHEALTH_GARIB,		//1
	EXTRALIFE_GARIB,		//2
	AUTOHOP_GARIB,			//3
	LONGHOP_GARIB,			//4
	LONGTONGUE_GARIB,		//5
	WHOLEKEY_GARIB,			//6
	HALFLKEY_GARIB,			//7
	HALFRKEY_GARIB,			//8

	NUM_GARIB_TYPES
};


// ----- [ GLOBAL VARIABLES ] --------------- //

extern unsigned long autoHop;
extern unsigned long longHop;
extern unsigned long longTongue;
extern unsigned long superHop;
extern unsigned long superFrog;
extern unsigned long croakFloat;


extern SPRITE_ANIMATION_TEMPLATE garibAnimation[NUM_GARIB_TYPES];


// ----- [ FUNCTION PROTOTYPES ] --------------- //

extern void InitCollectablesForLevel(unsigned long worldID,unsigned long levelID);
extern void CreateLevelCollectables(unsigned long *tileList, int type);

extern GARIB *GaribIsInTongueRange();
extern ACTOR2 *BabyFrogIsInTongueRange();
extern GAMETILE *GrapplePointInTongueRange();

extern void CheckTileForCollectable(GAMETILE *tile);

extern void ProcessCollectables();
extern void PickupCollectable(GARIB *garib);
extern void PickupBabyFrog(ACTOR2 *baby);


// GARIB RELATED

extern GARIBLIST garibCollectableList;

extern void InitGaribLinkedList();
extern void FreeGaribLinkedList();
extern void AddGarib(GARIB *garib);
extern void SubGarib(GARIB *garib);

extern void InitGaribSprite(GARIB *garib);
extern GARIB *CreateNewGarib(VECTOR pos,int type, GAMETILE* gameTile, float dropSpeed );
extern void UpdateGaribs();

extern void CreateAndAddSpawnScoreSprite(VECTOR *pos,char scoreType);


#endif
