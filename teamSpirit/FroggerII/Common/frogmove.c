/*
	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.


	File		: frogmove.c
	Programmer	: Andrew Eder
	Date		: 11/13/98

-----------------------------------------------------------------------------------------------*/


#define F3DEX_GBI_2

#include <ultra64.h>

#include "incs.h"

long numHops_TOTAL = 0;
long speedHops_TOTAL = 0;
long numHealth_TOTAL = 0;

GAMETILE *destTile[4]			= {0,0,0,0};
GAMETILE *currTile[4]			= {0,0,0,0};
GAMETILE *prevTile				= NULL;

static float frogAnimSpeed		= 0.4F;
static float frogAnimSpeed2		= 0.9F;

int	frogFacing[4]				= {0,0,0,0};
int nextFrogFacing[4]			= {0,0,0,0};

unsigned long standardHopFrames = 7;
unsigned long superHopFrames	= 32;
unsigned long doubleHopFrames	= 44;
unsigned long quickHopFrames	= 4;
unsigned long floatFrames       = 30;
unsigned long iceFrames			= 10;
unsigned long conveyorFrames[3] = { 60, 30, 15 };

unsigned long standardHopJumpDownDivisor	= 10;
unsigned long superHopJumpDownDivisor		= 12;

struct {
	int lastHopOn;
	int freq;
	int Time;
	int Addr;
	int Subr;
	int Max;
	int Min;
} frogPitch = { 0, 128, 15, 2, 8, 64,50 };


#define MAX_HOP_HEIGHT			26.0f	// +1 for rounding :o)
#define MAX_SUPERHOP_HEIGHT		51.0f
#define DROP_HURT_HEIGHT		100.0f
#define DROP_KILL_HEIGHT		150.0f

float	hopHeight		= 30;
float	superhopHeight	= 50;
float	doublehopHeight = 75;

float	floatMultiply	= 0.25f;

BOOL cameoMode			= FALSE;

/* ---------------- Local functions -------------- */

void CalculateFrogJump(VECTOR *startPos, VECTOR *endPos, VECTOR *normal, float height, long time, long player);
void CheckForFroggerLanding(long pl);


/*	--------------------------------------------------------------------------------
	Function		: SetFroggerStartPos
	Purpose			: sets initial position of Frogger on the world
	Parameters		: GAMETILE *,long
	Returns			: none
	Info			:
*/
void SetFroggerStartPos(GAMETILE *startTile,long p)
{
	unsigned long i;
	VECTOR tmpVec;

	// Change frog's position to reflect that of the start tile
	SetVector(&frog[p]->actor->pos,&startTile->centre);

	autoHop			= 0;
	longTongue		= 0;

	camFacing		= 0;
	controlCamera	= 0;

	InitActorAnim(frog[p]->actor);
	AnimateActor(frog[p]->actor,FROG_ANIM_DANCE1,YES,NO,0.25F,0,0);

	currTile[p]		= startTile;

	destTile[p]		= NULL;
	currPlatform[p]	= NULL;
	destPlatform[p]	= NULL;

	player[p].healthPoints	= 3;
	GTInit( &player[p].isCroaking, 0 );
	GTInit( &player[p].isOnFire, 0 );
	GTInit( &player[p].stun, 0 );
	GTInit( &player[p].safe, 0 );
	GTInit( &player[p].dead, 0 );

	// set frog action movement variables
	ZeroVector(&frog[p]->actor->vel);

	player[p].canJump			= 1;
	player[p].frogState			= 0;
	player[p].isSuperHopping	= 0;
	player[p].isSinking			= 0;
	player[p].isQuickHopping	= 0;
	player[p].idleTime			= MAX_IDLE_TIME;
	player[p].idleEnable		= 1;
	player[p].heightJumped		= 0;
	player[p].jumpTime			= -1;

	SitAndFace(frog[p],currTile[p],frogFacing[p]);
}

/*	--------------------------------------------------------------------------------
	Function		: UpdateFroggerControls
	Parameters		: player number
	Returns			: void
	Info			:
*/

BOOL UpdateFroggerControls(long pl)
{
	/* ----------------------- Frog wants to HOP u/d/l/r ----------------------------- */

	//if (!player[pl].canJump) return;

	if(player[pl].frogState & (FROGSTATUS_ISWANTINGU|FROGSTATUS_ISWANTINGL|FROGSTATUS_ISWANTINGR|FROGSTATUS_ISWANTINGD))
	{
		int dir;
		long actF;

		if(player[pl].frogState & FROGSTATUS_ISWANTINGU)		dir = MOVE_UP;
		else if(player[pl].frogState & FROGSTATUS_ISWANTINGD)	dir = MOVE_DOWN;
		else if(player[pl].frogState & FROGSTATUS_ISWANTINGL)	dir = MOVE_LEFT;
		else dir = MOVE_RIGHT;

		AnimateFrogHop((dir + camFacing) & 3,pl);
		frogFacing[pl] = (camFacing + dir) & 3;

		//nextFrogFacing[pl] = (nextFrogFacing[pl] + ((camFacing + dir) - frogFacing[pl])) & 3;
		
		
		if ((actFrameCount-frogPitch.lastHopOn)<frogPitch.Time)
		{
			if (frogPitch.freq<frogPitch.Max)
				frogPitch.freq+=frogPitch.Addr;
		}
		else
			frogPitch.freq = frogPitch.Min;
		
		actF = frogPitch.freq+Random(10);
		
		PlaySample(GEN_FROG_HOP,&frog[pl]->actor->pos,0,100-Random(15),actF);
		frogPitch.lastHopOn = actFrameCount;

		if (!MoveToRequestedDestination(dir,pl))
		{
			AnimateActor(frog[pl]->actor, FROG_ANIM_BREATHE, NO, YES, 1.0f, NO, NO);	// queue a "landed" anim
		}
	}

  	/* ----------------------- Frog wants to SUPERHOP u/d/l/r ----------------------------- */

	else if(player[pl].frogState & (FROGSTATUS_ISWANTINGSUPERHOPU|FROGSTATUS_ISWANTINGSUPERHOPL|FROGSTATUS_ISWANTINGSUPERHOPR|FROGSTATUS_ISWANTINGSUPERHOPD))
	{
		int dir;
		if(player[pl].frogState & FROGSTATUS_ISWANTINGSUPERHOPU)		dir = MOVE_UP;
		else if(player[pl].frogState & FROGSTATUS_ISWANTINGSUPERHOPD)	dir = MOVE_DOWN;
		else if(player[pl].frogState & FROGSTATUS_ISWANTINGSUPERHOPL)	dir = MOVE_LEFT;
		else dir = MOVE_RIGHT;

		AnimateFrogHop((dir + camFacing) & 3,pl);

		PlaySample(GEN_SUPER_HOP,&frog[pl]->actor->pos,0,255,64);

		player[pl].frogState |= FROGSTATUS_ISSUPERHOPPING;

		if (!MoveToRequestedDestination(dir,pl))
		{
			AnimateActor(frog[pl]->actor, FROG_ANIM_BREATHE, NO, YES, 1.0f, NO, NO);	// queue a "landed" anim
		}

	}
	else
		return FALSE;	// nope, we didn't do nuffink

	return TRUE;	// yep, we did somefink
}


/*	---------------------------------------------------------------------------------
	Notes on Frog Movement
	by Dave S.

	The frog's vertical motion during a jump is described by the simple parabolic curve equation

	y = h(1-(2mt-1)�)

	where h is the height of the curve and t is the 'time' variable (see below). m is a multiplier
	calculated using the formula

	m = (1+sqrtf(1-d))/2

	where d is the difference in height between the start and end position. height is displacement
	parallel to the starting normal vector. Horizontal motion is simply linear,	parallel to the
	plane described by the starting position and its normal. (phew)

	x = A(1-t) + Bt

	The variable t ranges from 0 (zero) at the start of the jump and 1 (one) at the end of the
	jump. This makes calculating the end of the jump nice and easy!
*/

/*	--------------------------------------------------------------------------------
	Function		: FroggerHop
	Parameters		: player
	Returns			: void
	Info			: See notes above for the maths

	Note that we also check for falling through platforms here! I know it's stupid, shut up.
*/
void FroggerHop(long pl)
{
	VECTOR up, fwd, pos;
	float p, t, delta;

	if (player[pl].jumpTime < 0.0f) return;

	delta = (player[pl].jumpSpeed * gameSpeed);

	if (player[pl].frogState & FROGSTATUS_ISFLOATING)
		delta *= floatMultiply;

	player[pl].jumpTime += delta;

	t = player[pl].jumpTime;

	// Calculate frog's position (see above for the maths)
	// horizontal (parallel to plane)
	
	SetVector(&fwd, &player[pl].jumpFwdVector);
	ScaleVector(&fwd, t);
	
	AddVector(&pos, &fwd, &player[pl].jumpOrigin);

	// vertical

	if (player[pl].jumpMultiplier)
	{
		p = (2*t*player[pl].jumpMultiplier - 1);
		p = 1 - (p*p);
		SetVector(&up, &player[pl].jumpUpVector);
		ScaleVector(&up, p);
		AddToVector(&pos, &up);

		// Check for falling through platforms
		// TODO: Test, test, test. And possibly find somewhere more sensible for this code.
		if (!destPlatform[pl] && nearestPlatform[pl])
		{
			VECTOR v, plt;
			float before, after;	// height above platform before and after moving

			SetVector(&plt, &nearestPlatform[pl]->pltActor->actor->pos);

			SubVector(&v, &frog[pl]->actor->pos, &plt);
			before = DotProduct(&v, &currTile[pl]->normal);

			SubVector(&v, &pos, &plt);
			after = DotProduct(&v, &currTile[pl]->normal);

			if (before >= 0 && after <= 0)
			{
				destPlatform[pl] = nearestPlatform[pl];
				player[pl].frogState = FROGSTATUS_ISJUMPINGTOPLATFORM;
				player[pl].jumpTime = 1.0f;	// aaand land.
			}
		}
	}

	SetVector(&frog[pl]->actor->pos, &pos);
}



/*	--------------------------------------------------------------------------------
	Function		: UpdateFroggerPos
	Parameters		: player number
	Returns			: void
	Info			:
*/

void UpdateFroggerPos(long pl)
{
	float x,y,z;
	VECTOR effectPos;
	PLANE2 ground;
	VECTOR moveVec;
	float dist,glowRange;
	static float glowSeed = 0.0F;

	if (cameoMode)
	{
		player[pl].frogState &= 
			~(FROGSTATUS_ISWANTINGU | FROGSTATUS_ISWANTINGD |
			FROGSTATUS_ISWANTINGL | FROGSTATUS_ISWANTINGR);
		//player[pl].canJump = 0;
	}
	
	if( player[pl].isQuickHopping )
		player[pl].isQuickHopping--;

	/*	--------------------------------------------------------------------------------------------
		Consider effects of special tile types
	*/

	player[pl].frogState &= ~FROGSTATUS_ISSAFE;

	// But first... platforms
	if (currPlatform[pl])
	{
		currTile[pl] = currPlatform[pl]->inTile[0];
	
/*
		PLATFORM *plat = currPlatform[pl];
		GAMETILE *dest = plat->inTile[0];
		if (dest != currTile[pl])
		{
			camFacing = GetTilesMatchingDirection(currTile[pl], camFacing, dest);
			frogFacing[pl] = GetTilesMatchingDirection(currTile[pl], frogFacing[pl], dest);
			currTile[pl] = dest;
		}
*/
	}
	else if( currTile[pl]->state == TILESTATE_SINK )
	{
		VECTOR dir;
		float J, dist;

		SubVector( &dir, &frog[pl]->actor->pos, &currTile[pl]->centre );
		MakeUnit( &dir );
		J = DotProduct( &currTile[pl]->normal, &dir );
		dist = DistanceBetweenPointsSquared( &frog[pl]->actor->pos, &currTile[pl]->centre );
		if( J < 0 ) dist *= -1;

		if( player[pl].frogState & FROGSTATUS_ALLHOPFLAGS )
		{
			SetVector( &moveVec, &currTile[pl]->normal );
			ScaleVector( &moveVec, gameSpeed*2 );
			AddToVector( &frog[pl]->actor->pos, &moveVec );

			// Frog has broken free!
			if( dist >= 0 )
				player[pl].isSinking = 0;
		}
		else if( player[pl].isSinking )
		{
			SetVector( &moveVec, &currTile[pl]->normal );
			ScaleVector( &moveVec, gameSpeed*-0.2 );
			AddToVector( &frog[pl]->actor->pos, &moveVec );

			// Frog is dead
			if( dist < -900 && !player[pl].dead.time )
			{
				GTInit( &player[pl].dead, 3 );
				player[pl].deathBy = DEATHBY_DROWNING;
				player[pl].frogState |= FROGSTATUS_ISDEAD;
				player[pl].isSinking = 0;
				AnimateActor(frog[pl]->actor,FROG_ANIM_DROWNING,NO,NO,0.25F,0,0);
			}
		}
	}
	else if ( currTile[pl]->state == TILESTATE_SAFE )
	{
		player[pl].frogState |= FROGSTATUS_ISSAFE;
	}
	else if ( currTile[pl]->state & TILESTATE_CONVEYOR )
	{
		/*	If we're on a conveyor and can jump - i.e. we're not already jumping -
			consider the player to be on its destination tile once it goes past half-way
			along its 'jump'.
		*/
		if (destTile[pl] &&
			player[pl].canJump &&
			(player[pl].jumpTime > 0.5))
		{
			currTile[pl] = destTile[pl];
		}
	}

	/*	--------------------------------------------------------------------------------------------
		Calculate frog hop
	*/

	FroggerHop(pl);

	//--------------------------------------------------------------------------------------------
	
	if( NUM_FROGS == 1 && (player[pl].frogState & FROGSTATUS_ISDEAD) )
	{
		CheckForFroggerLanding(pl);
		KillFrog(pl);
		return;
	}
	
	// update frog tongue
	UpdateFrogTongue(pl);

	// frog is croaking
	if(player[pl].frogState & FROGSTATUS_ISCROAKING)
	{
		SPECFX *fx;

		if( !(player[pl].isCroaking.time%2) )
		{
			SetVector(&effectPos,&frog[pl]->actor->pos);
			effectPos.v[Y] += 15;
			fx = CreateAndAddSpecialEffect( FXTYPE_POLYRING, &effectPos, &currTile[pl]->normal, 20, 1, 0.1, 1.5 );
			fx->r = 191;
			fx->g = 255;
			fx->b = 0;
		}

		GTUpdate( &player[pl].isCroaking, -1 );
		if( !player[pl].isCroaking.time )
		{
			player[pl].frogState &= ~FROGSTATUS_ISCROAKING;

			// check for nearest baby frog - do radius check ????
			if(nearestBaby = GetNearestBabyFrog())
			{
//				fx = CreateAndAddSpecialEffect(	FXTYPE_POLYRING, &babies[nearestBaby]->actor->pos, &upVec, 15, 1, 0.1, 1.2 );
//				fx->r = babyList[nearestBaby].fxColour[R];
//				fx->g = babyList[nearestBaby].fxColour[G];
//				fx->b = babyList[nearestBaby].fxColour[B];
			}
		}
	}


	/* ---------------------------------------------------- */

	if (cameoMode || !UpdateFroggerControls(pl))
	{
		CheckForFroggerLanding(pl);
	}

	/* ---------------------------------------------------- */

	// check if frog is on fire, etc.
	if(player[pl].isOnFire.time)
	{
		GTUpdate( &player[pl].isOnFire, -1 );
		if( !(player[pl].isOnFire.time%2) )
		{
			SetVector(&effectPos,&frog[pl]->actor->pos);
			effectPos.v[Y] += 25;
			CreateAndAddSpecialEffect( FXTYPE_SMOKE_GROWS, &effectPos, &currTile[pl]->normal, 30, 5, 0, 2.5 );
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function		: GetNextTile(unsigned long direction)
	Purpose			: determines the next tile
	Parameters		:
	Returns			:
	Info			:
*/

long GetTilesMatchingDirection(GAMETILE *me, long direction, GAMETILE *next)
{
	long dirNumber,i;
	float t,distance = 0;
	
	for(i=0; i<4; i++)
	{
		t = DotProduct(&me->dirVector[direction],&next->dirVector[i]);
		if(t >= distance)
		{
			distance = t;
			dirNumber = i;			
		}
	}

	return dirNumber;
}

/*	--------------------------------------------------------------------------------
	Function		: GetNextTile(unsigned long direction)
	Purpose			: Finds the next tile in a given direction
	Parameters		: direction, player
	Returns			: GAMETILE*
*/
GAMETILE *GetNextTile(unsigned long *pdir,long pl)
{
	GAMETILE *dest = NULL;
	long i, direction;
	float distance, t, t2, at2;
	VECTOR vecUp;
		
	direction = *pdir;

	if(pl == 0)
	{
		i = (direction + camFacing + 2) & 3;
		dest = currTile[pl]->tilePtrs[i]; // hmm...
	}
	else
	{
		/*	Alas, this only works when this player's tile and player 0's tile are orientated the same
			
			destTile[pl] = currTile[pl]->tilePtrs[(direction + camFacing + 2) & 3];

			We have to find the direction on this player's tile corresponding to the direction on
			player 0's tile!
		*/
			
		i = GetTilesMatchingDirection(currTile[0], (direction + camFacing + 2) & 3, currTile[pl]);
		dest = currTile[pl]->tilePtrs[i];
	}	

	*pdir = i;

	if (!dest || dest->state == TILESTATE_BARRED)
		return NULL;

	while (dest->state == TILESTATE_JOIN)	// || dest->state == TILESTATE_SUPERHOP
	{
		GAMETILE *joiningTile = dest;

		// Find the direction closest to "up" or "down" in the opposite direction to where we are now

		SetVector(&vecUp,&currTile[pl]->normal);

		distance = -1000;
		
		for(i=0; i<4; i++)
		{
			if(joiningTile->tilePtrs[i])
			{					
				t2 = DotProduct(&vecUp,&joiningTile->dirVector[i]);
				t = Fabs(t2);
				if(t > distance)
				{
					if(currTile[pl] != joiningTile->tilePtrs[i])
					{
						distance = t;
						direction = i;
						at2 = t2;
					}
				}
			}
		}

		dest = joiningTile->tilePtrs[direction];

		if (!dest)
			 return NULL;

		if (dest->state == TILESTATE_BARRED)
			return NULL;
/*
		if((joiningTile->state == TILESTATE_SUPERHOP))
		{
			if(DotProduct(&vecUp,&joiningTile->dirVector[n]) < 0)
			{
				if(!player[pl].isSuperHopping)
				{
					return NULL;
				}
			}
		}
*/
	}

	// frog is jumping to available tile
	player[pl].frogState |= FROGSTATUS_ISJUMPINGTOTILE;

	return dest;
}

/*	-------------------------------------------------------------------------
	Function:	AnimateFrogHop
	Purpose:	Runs the appropriate hop animation
	Parameters:	direction, player
	Returns:
*/
void AnimateFrogHop( unsigned long direction, long pl )
{
	float speed, speed2;
	int anim;

	if(player[pl].frogState & (FROGSTATUS_ISWANTINGSUPERHOPU|FROGSTATUS_ISWANTINGSUPERHOPL|FROGSTATUS_ISWANTINGSUPERHOPR|FROGSTATUS_ISWANTINGSUPERHOPD))
	{
		anim = FROG_ANIM_SUPERHOP;
		speed = 0.45;
		speed2 = 0.45;
	}
	else
	{
		anim = FROG_ANIM_STDJUMP;
		speed = frogAnimSpeed;
		speed2 = frogAnimSpeed2;
		
	}

	switch( direction )
	{
	case 0:
		switch( frogFacing[pl] )
		{
		case 3:	AnimateActor(frog[pl]->actor,anim,NO,NO,speed2,0,0); break;
		case 1:	AnimateActor(frog[pl]->actor,anim,NO,NO,speed2,0,0); break;
		default: AnimateActor(frog[pl]->actor,anim,NO,NO,speed,0,0); break;
		}
		break;
	case 1:
		switch( frogFacing[pl] )
		{
		case 2:	AnimateActor(frog[pl]->actor,anim,NO,NO,speed2,0,0); break;
		case 0:	AnimateActor(frog[pl]->actor,anim,NO,NO,speed2,0,0); break;
		default: AnimateActor(frog[pl]->actor,anim,NO,NO,speed,0,0); break;
		}
		break;
	case 2:
		switch( frogFacing[pl] )
		{
		case 1:	AnimateActor(frog[pl]->actor,anim,NO,NO,speed2,0,0); break;
		case 3:	AnimateActor(frog[pl]->actor,anim,NO,NO,speed2,0,0); break;
		default: AnimateActor(frog[pl]->actor,anim,NO,NO,speed,0,0); break;
		}
		break;
	case 3:
		switch( frogFacing[pl] )
		{
		case 2:	AnimateActor(frog[pl]->actor,anim,NO,NO,speed2,0,0); break;
		case 0:	AnimateActor(frog[pl]->actor,anim,NO,NO,speed2,0,0); break;
		default: AnimateActor(frog[pl]->actor,anim,NO,NO,speed,0,0); break;
		}
		break;
	}
	//AnimateActor(frog[pl]->actor,FROG_ANIM_BREATHE,YES,YES,0.6F,0,0);
}

/*	--------------------------------------------------------------------------------
	Function		: MoveToRequestedDestination
	Purpose			: Moves a player in the requested destination
	Parameters		: direction, player
	Returns			: TRUE if we did a hop, FALSE otherwise
*/
long speedFrameCount = 0;

BOOL MoveToRequestedDestination(int dir,long pl)
{
	GAMETILE *dest, *from;
	float t,t2,h;
	unsigned long tiledir;
	
/*	TODO: check if this is actually necessary

//	if( tongue[pl].flags & TONGUE_BEINGUSED)
//		RemoveFrogTongue(pl);

	// see if frog is currently on a moving platform (i.e. platform that follows a path)
	if(currPlatform[pl])
	{
		// frog is on a platform - get current tile
		currTile[pl] = currPlatform[pl]->inTile[0];
		currPlatform[pl]->carrying = frog[pl];

		// check if the platform follows a path
		if(currPlatform[pl]->flags & PLATFORM_NEW_FOLLOWPATH)
			player[pl].frogState |= FROGSTATUS_ISONMOVINGPLATFORM;
	}
*/
	SitAndFace(frog[pl],currTile[pl],frogFacing[pl]);

	tiledir = dir;
	dest = GetNextTile(&tiledir, pl);

	// clear all movement flags
	player[pl].frogState &= ~(FROGSTATUS_ALLHOPFLAGS | FROGSTATUS_ISJUMPINGTOTILE | FROGSTATUS_ISJUMPINGTOPLATFORM);

	if (!dest || player[pl].isSinking )
	{
		player[pl].canJump = 1;
		player[pl].isSuperHopping = 0;
		
		return FALSE;
	}

	// check destination tile
	if(GameTileTooHigh(dest, pl))
	{			
		// gametile is too high
		player[pl].canJump = 1;
		player[pl].isSuperHopping = 0;
		return FALSE;
	}
	else if(dest->state == TILESTATE_SMASH)
	{
		// smash tiles

		if(!(player[pl].frogState & FROGSTATUS_ISSUPERHOPPING))
		{
			player[pl].canJump = 1;
			player[pl].isSuperHopping = 0;
			return FALSE;
		}
		else
		{
			dest->state = TILESTATE_NORMAL;
			player[pl].canJump = 1;
			player[pl].isSuperHopping = 0;
			return FALSE;
		}
	}

	// ------------------------------------------------------------------------------------------
	// If we get this far, it's a valid jump (to a tile)

	from = currTile[pl];
	destTile[pl] = dest;
	destPlatform[pl] = NULL;

	player[pl].frogState |= FROGSTATUS_ISJUMPINGTOTILE;

	/* If we're just hopping, check if there's a platform to jump to
	   This should help fix tile->platform and platform->platform jumping

	   note to the PSX guys - apart from the height check, these maths are
	   just a simple radius check and could easily use straight integer maths
	   and still work
	*/
	if (!player[pl].isSuperHopping)
	{
		PLATFORM *plat; int n;
		VECTOR v, w;
		float height;

//		SubVector(&v, &destTile[pl]->centre, &currTile[pl]->centre);
//		SubVector(&v, &frog[pl]->actor->pos, &currTile[pl]->dirVector[tiledir]);

		SetVector(&v, &currTile[pl]->dirVector[tiledir]); ScaleVector(&v, -50.0f);
		AddToVector(&v, &frog[pl]->actor->pos);

		for (n = platformList.numEntries, plat = platformList.head.next; n; n--, plat = plat->next)
		{
			if (!plat->active) continue;

			// line 1 of this test is for platform->platform
			// line 2 is for tile->platform
			if ((currPlatform[pl] && (40*40) > DistanceBetweenPointsSquared(&v, &plat->pltActor->actor->pos)) ||
				(plat->inTile[0] == dest))
			{
				// Check we're not trying to jump too high!
				SubVector(&w, &plat->pltActor->actor->pos, &frog[pl]->actor->pos);
				height = DotProduct(&w, &destTile[pl]->normal);
				
				if (height <= MAX_HOP_HEIGHT)
				{
					destPlatform[pl] = plat;
					player[pl].frogState &= ~FROGSTATUS_ISJUMPINGTOTILE;
					player[pl].frogState |= FROGSTATUS_ISJUMPINGTOPLATFORM;
				}
				break;
			}
		}
	}

	if (currPlatform[pl])
	{
		currPlatform[pl]->carrying = NULL;
		currPlatform[pl]->flags &= ~PLATFORM_NEW_CARRYINGFROG;
		currPlatform[pl] = NULL;
	}

	nextCamFacing = GetTilesMatchingDirection(from, camFacing, dest);
	nextFrogFacing[pl] = GetTilesMatchingDirection(from, frogFacing[pl], dest);

	if( player[pl].hasDoubleJumped )
	{
//		float time = (float)(actFrameCount - player[pl].jumpStartFrame)/(float)(player[pl].jumpEndFrame-player[pl].jumpStartFrame),
//			v = player[pl].vInitialVelocity;
		t = doubleHopFrames;
		h = doublehopHeight;
		// Current velocity is initial velocity of superhop + (acceleration * time)
//		sV = v + (superGravity*time);
	}
	else if(player[pl].isSuperHopping)
	{
//		frogTrail[pl] = CreateAndAddSpecialEffect( FXTYPE_BILLBOARDTRAIL, &frog[pl]->actor->pos, &currTile[pl]->normal, 20, 0.95, 0.0, 1 );
//		frogTrail[pl]->follow = frog[pl]->actor;
//		SetFXColour( frogTrail[pl], 50, 200, 50 );

		t = superHopFrames;
		h = superhopHeight;
	}
	else if(player[pl].isQuickHopping)
	{
		t = quickHopFrames;
		h = hopHeight;
	}
	else
	{
		t = standardHopFrames;
		h = hopHeight;
	}

	// determine where frog is going to land...

    if(player[pl].frogState & FROGSTATUS_ISJUMPINGTOPLATFORM)
	{
		CalculateFrogJump(
			&frog[pl]->actor->pos, &destPlatform[pl]->pltActor->actor->pos, &currTile[pl]->normal,
			h, t, pl);
	}
	else
	{	
		CalculateFrogJump(
			&frog[pl]->actor->pos, &destTile[pl]->centre, &currTile[pl]->normal,
			h, t, pl);
	}

	if( player[pl].heightJumped < -125 )
		AnimateActor(frog[pl]->actor, FROG_ANIM_TRYTOFLY, YES, NO, 0.25, 0, 0 );

	// ------------------------------------------------------------------------------------------

	numHops_TOTAL++;
	if(speedFrameCount)
		speedHops_TOTAL += frameCount - speedFrameCount;	
	speedFrameCount = frameCount;

	return TRUE;
}

/*	--------------------------------------------------------------------------------
	Function		: CheckForFroggerLanding
	Purpose			: determine whether the player has finished its jump
	Parameters		: int
	Returns			: void
	Info			:
*/
void CheckForFroggerLanding(long pl)
{
	VECTOR telePos;
	unsigned long i, j;
	float distance;

	if (player[pl].jumpTime < 0) return;	// we're not even jumping. Duh.

/*		
		// ...yep - check for presence of a platform in the destination tile
		destPlatform[pl] = CheckDestForPlatform(destTile[pl],pl);
		if(!destPlatform[pl])
		{
			player[pl].frogState &= ~FROGSTATUS_ISJUMPINGTOPLATFORM;
			player[pl].frogState |= FROGSTATUS_ISJUMPINGTOTILE;
		}
	}
*/

	if (player[pl].jumpTime < 1) return; // cause we're still jumping

	// Frog has landed - set camera to new rotation, face frog correctly, blahblahblah

	//if (pl == 0)
	//	camFacing = nextCamFacing;

//	player[pl].deathBy = -1;
//	GTInit( &player[pl].dead, 0 );

	player[pl].canJump = 1;
	player[pl].isSuperHopping = 0;
	player[pl].hasDoubleJumped = 0;
	player[pl].jumpTime = -1;

	// Assume for now that if we've landed after being on another frogs head then we're no longer on it.
	if( player[pl].frogon != (char)-1 )
	{
		int f = player[pl].frogon;

		player[f].canJump = 1;
		player[f].frogunder = -1;
		player[pl].frogon = -1;

		player[pl].idleEnable = 1;

		if( player[f].frogon == (char)-1 )
		{
			player[f].idleEnable = 1;
			AnimateActor( frog[f]->actor, FROG_ANIM_BREATHE, YES, NO, 0.6, 0,0 );
		}
		else
			AnimateActor( frog[f]->actor, FROG_ANIM_PINLOOP, YES, NO, 0.5, 0,0 );
	}

	if (currPlatform[pl])
	{
		currPlatform[pl]->carrying = NULL;
		currPlatform[pl] = NULL;
	}

/*	if( frogTrail[pl] && frogTrail[pl]->follow )
	{
		frogTrail[pl]->follow = NULL;
		frogTrail[pl] = NULL;
	}
*/
	// Finish anims
	if (player[pl].frogState & FROGSTATUS_ISFLOATING)
	{
		AnimateActor(frog[0]->actor, FROG_ANIM_GETUPFROMFLOAT, NO, NO, 0.5f, NO, NO);
		AnimateActor(frog[pl]->actor,FROG_ANIM_BREATHE,YES,YES,0.6F,0,0);
	}
	else if( !(player[pl].frogState & FROGSTATUS_ISDEAD) )
	{
		AnimateActor(frog[pl]->actor,FROG_ANIM_BREATHE,YES,NO,0.6F,0,0);
	}
	else if( player[pl].deathBy == DEATHBY_WHACKING )
	{
		VECTOR up, fwd, side;

		// Find the forward and up vectors for camera
		SubVector( &fwd, &currCamSource[0], &currCamTarget[0] );
		MakeUnit( &fwd );

		CrossProduct( &side, &fwd, &upVec );
		CrossProduct( &up, &side, &fwd );
		MakeUnit( &up );

		// Orientate to camera
		Orientate( &frog[pl]->actor->qRot, &fwd, &inVec, &up );
	}

	if(player[pl].frogState & FROGSTATUS_ISJUMPINGTOPLATFORM)
	{
		GAMETILE *tile = destPlatform[pl]->inTile[0];

		// ok - frog has landed
		SetVector(&frog[pl]->actor->pos,&destPlatform[pl]->pltActor->actor->pos);

		frogFacing[pl] = GetTilesMatchingDirection(currTile[pl], frogFacing[pl], tile);
		
		if(pl == 0)
			camFacing = GetTilesMatchingDirection(currTile[pl], camFacing, tile);

		destPlatform[pl]->flags		|= PLATFORM_NEW_CARRYINGFROG;
		player[pl].frogState		|= FROGSTATUS_ISONMOVINGPLATFORM;

		destPlatform[pl]->carrying	= frog[pl];
		
		frog[pl]->actor->scale.v[X] = globalFrogScale;	//0.09F;
		frog[pl]->actor->scale.v[Y] = globalFrogScale;	//0.09F;
		frog[pl]->actor->scale.v[Z] = globalFrogScale;	//0.09F;

		player[pl].frogState &= ~(	FROGSTATUS_ISJUMPINGTOTILE | FROGSTATUS_ISFLOATING |
									FROGSTATUS_ISJUMPINGTOPLATFORM | FROGSTATUS_ISSUPERHOPPING);

		currPlatform[pl] = destPlatform[pl];
		currTile[pl] = tile;
		destPlatform[pl] = NULL;

		if(player[pl].heightJumped < -DROP_KILL_HEIGHT)
		{
			if(!player[pl].dead.time)
			{
				CreateAndAddSpecialEffect( FXTYPE_BASICRING, &destTile[pl]->centre, &destTile[pl]->normal, 25, 1, 0.1, 0.8 );
				player[pl].deathBy = DEATHBY_NORMAL;
				AnimateActor(frog[pl]->actor,FROG_ANIM_BASICSPLAT,NO,NO,0.25F,0,0);

				player[pl].frogState |= FROGSTATUS_ISDEAD;
				GTInit( &player[pl].dead, 3 );

				//PlayActorBasedSample(2,frog[pl]->actor,255,128);
			}
		}
		else if (player[pl].heightJumped < -DROP_HURT_HEIGHT)
		{
			// TODO: Hurt frog!
		}

		CheckTileForCollectable(NULL,0);
	}
	else /*if (player[pl].frogState & FROGSTATUS_ISJUMPINGTOTILE)*/
	{
		GAMETILE *tile;
		int state;
		
		if (!destTile[pl])
			tile = currTile[pl];
		else
		{
			currTile[pl] = tile = destTile[pl];
			destTile[pl] = NULL;

			// set frog to centre of tile
			SetVector(&frog[pl]->actor->pos, &tile->centre);
		}

		//frogFacing[pl] = GetTilesMatchingDirection(currTile[pl], frogFacing[pl], tile);

		frogFacing[pl] = nextFrogFacing[pl];

		state = tile->state;

		frog[pl]->actor->scale.v[X] = globalFrogScale;	//0.09F;
		frog[pl]->actor->scale.v[Y] = globalFrogScale;	//0.09F;
		frog[pl]->actor->scale.v[Z] = globalFrogScale;	//0.09F;

		player[pl].frogState &= ~(FROGSTATUS_ISJUMPINGTOTILE | FROGSTATUS_ISJUMPINGTOPLATFORM |
			FROGSTATUS_ISONMOVINGPLATFORM | FROGSTATUS_ISSUPERHOPPING | FROGSTATUS_ISFLOATING);

		if(player[pl].heightJumped < -DROP_KILL_HEIGHT)
		{
			if(!player[pl].dead.time)
			{
				CreateAndAddSpecialEffect( FXTYPE_BASICRING, &tile->centre, &tile->normal, 25, 1, 0.1, 0.8 );
				player[pl].deathBy = DEATHBY_NORMAL;
				AnimateActor(frog[pl]->actor,FROG_ANIM_BASICSPLAT,NO,NO,1.0f,0,0);

				player[pl].frogState |= FROGSTATUS_ISDEAD;
				GTInit( &player[pl].dead, 3 );

				//PlayActorBasedSample(2,frog[pl]->actor,255,128);
			}
		}
		else if( player[pl].frogState & FROGSTATUS_ISDEAD )
		{
			// We're bouncing along the ground
			if( player[pl].deathBy == DEATHBY_FIRE )
			{
				BounceFrog( pl, 50, 30 );
				AnimateActor( frog[pl]->actor, FROG_ANIM_ASSONFIRE, NO, NO, 0.5F, 0, 0 );
			}
		}
/*		else if (player[pl].heightJumped < -DROP_HURT_HEIGHT)
		{
			// TODO: Hurt frog?
		}*/
		else	// check tile to see if frog has jumped onto a certain tile type

		if(state == TILESTATE_DEADLY)
		{
			if(!player[pl].dead.time)
			{
				SPECFX *fx;
				CreateAndAddSpecialEffect( FXTYPE_WATERRIPPLE, &tile->centre, &tile->normal, 20, 0.8, 0.1, 0.6 );
				player[pl].deathBy = DEATHBY_DROWNING;
				AnimateActor(frog[pl]->actor,FROG_ANIM_DROWNING,NO,NO,0.25F,0,0);

				player[pl].frogState |= FROGSTATUS_ISDEAD;
				GTInit( &player[pl].dead, 3 );
			}
			return;
		}
		else if(state == TILESTATE_SINK)
		{
			player[pl].isSinking = 1;
		}
		if (state & TILESTATE_CONVEYOR)
		{	
			// -------------------------------- Conveyors ----------------------------

			int res = MoveToRequestedDestination(
				((state & (TILESTATE_CONVEYOR-1)) - camFacing) & 3, pl);

			if (res)
			{
				int speed;

				if (state >= TILESTATE_CONVEYOR_FAST)
					speed = 2;
				else if (state >= TILESTATE_CONVEYOR_MED)
					speed = 1;
				else
					speed = 0;

				if (player[pl].frogState & FROGSTATUS_ISJUMPINGTOPLATFORM)
					CalculateFrogJump(
						&frog[pl]->actor->pos, &destPlatform[pl]->pltActor->actor->pos, &tile->normal,
						0, conveyorFrames[speed], pl);
				else
					CalculateFrogJump(
						&frog[pl]->actor->pos, &destTile[pl]->centre, &tile->normal, 
						0, conveyorFrames[speed], pl);
			}
		}
		else if (state == TILESTATE_ICE)
		{
			int res = MoveToRequestedDestination((frogFacing[pl] - camFacing) & 3, pl);

			if (res)
			{
				if (player[pl].frogState & FROGSTATUS_ISJUMPINGTOPLATFORM)
					CalculateFrogJump(
						&frog[pl]->actor->pos, &destPlatform[pl]->pltActor->actor->pos, &tile->normal,
						0, iceFrames, pl);
				else
					CalculateFrogJump(
						&frog[pl]->actor->pos, &destTile[pl]->centre, &tile->normal, 
						0, iceFrames, pl);

				StartAnimateActor(frog[pl]->actor, FROG_ANIM_ICE3, YES, NO, 0.2f, NO, NO);

				player[pl].canJump = FALSE;
			}
		}

		// If we've landed on another frog, sit on his head
		for( i=(pl+1)%NUM_FROGS; i!=pl; i=(i+1)%NUM_FROGS )
			if( currTile[i] == tile )
			{
				if( DistanceBetweenPointsSquared( &frog[pl]->actor->pos, &frog[i]->actor->pos ) > 20*20 ) continue;

				// Face all lower frogs to our direction
				nextFrogFacing[i] = frogFacing[i] = frogFacing[pl];
				SetQuaternion( &frog[i]->actor->qRot, &frog[pl]->actor->qRot );
				SitAndFace( frog[i], currTile[i], frogFacing[i] );

				if( player[i].frogunder != (char)-1 ) continue;

				player[i].canJump = 0;

				player[pl].frogon = i;
				player[i].frogunder = pl;

				player[pl].idleEnable = 0;
				player[i].idleEnable = 0;

				StartAnimateActor( frog[pl]->actor, FROG_ANIM_PINLOOP, YES, NO, 0.5, 0,0 );

				if( player[i].frogon != (char)-1 )
				{
					VECTOR up;
					SetVector( &up, &currTile[i]->normal );
					ScaleVector( &up, 10 );
					AddVector( &frog[i]->actor->pos, &frog[player[i].frogon]->actor->pos, &up );
				}

				SetVector( &frog[pl]->actor->pos, &frog[i]->actor->pos );

				AnimateActor( frog[i]->actor, FROG_ANIM_PINNED, NO, NO, 1.0, 0,0 );
				StartAnimateActor( frog[i]->actor, FROG_ANIM_PINNEDLOOP, YES, YES, 0.5, 0,0 );
			}

		// Check for camera transitions on the tile
		CheckForDynamicCameraChange(tile);

		// Next, check if frog has landed on a collectable
		CheckTileForCollectable(tile, pl);
	}
	/*else
	{
		VECTOR pos;

		SetVector(&pos, &player[pl].jumpOrigin);
		AddToVector(&pos, &player[pl].jumpFwdVector);
		AddToVector(&pos, &player[pl].jumpUpVector);
		SetVector(&frog[pl]->actor->pos, &pos);
	}*/
}


/*	--------------------------------------------------------------------------------
	Function		: GameTileTooHigh
	Purpose			: checks is destination tile is too high to jump to
	Parameters		: GAMETILE *
	Returns			: BOOL
	Info			: 
*/
BOOL GameTileTooHigh(GAMETILE *tile,long pl)
{
	VECTOR vec;
	float height,h;
	VECTOR diff;
		
	SubVector(&diff,&tile->centre,&frog[pl]->actor->pos);
	h = Magnitude(&diff);
	MakeUnit(&diff);
	height = (h * DotProduct(&diff,&tile->normal));

	return
		height > (player[pl].isSuperHopping ? MAX_SUPERHOP_HEIGHT : MAX_HOP_HEIGHT);
}

/*	--------------------------------------------------------------------------------
	Function		: KillFrog
	Purpose			: kills the specified frog - runs the relevant death sequence
	Parameters		: long
	Returns			: BOOL - true if running death 'sequence'
	Info			: 
*/
BOOL KillFrog(long pl)
{
	TEXTURE *txtr1,*txtr2;
	float modifier;
	int rVal;
	
	GTUpdate( &player[pl].dead, -1 );
	if( !player[pl].dead.time )
	{
		numHealth_TOTAL++;
		// lose a life
/*		if( player[pl].worldNum == WORLDID_SUPERRETRO )
		{
			player[pl].numCredits--;
			if(!player[pl].numCredits)
			{
				gameState.mode = GAMEOVER_MODE;
				GTInit( &modeTimer, 10 );
				return FALSE;
			}
		}
		else
		{
*/		
		if(--player[pl].lives == 0)
		{
			StartGameOver();
			return FALSE;
		}

		player[pl].frogState &= ~FROGSTATUS_ISDEAD;

		if(player[pl].deathBy == DEATHBY_ELECTRICSHOCK)
		{
			// reset frog texture !
#ifndef PC_VERSION
			FindTexture(&txtr1,UpdateCRC("fg.bmp"),NO);
			FindTexture(&txtr2,UpdateCRC("ai_wake.bmp"),NO);
			ReplaceTextureInDrawList(frog[pl]->actor->objectController->drawList,(u32)txtr2->data,(u32)txtr1->data,NO);
#endif
		}

		SetFroggerStartPos(gTStart[0],pl);
		ResetPlatformFlags();

		return FALSE;
	}

	switch(player[pl].deathBy)
	{
		case DEATHBY_NORMAL:
			break;

		case DEATHBY_RUNOVER:
			break;

		case DEATHBY_DROWNING:
			if( !(actFrameCount % 5) )
			{
				VECTOR up;
				SPECFX *fx = CreateAndAddSpecialEffect( FXTYPE_BUBBLES, &frog[pl]->actor->pos, &currTile[pl]->normal, 8, 0.8, 0, 0.6 );

				fx->rebound = (PLANE2 *)JallocAlloc( sizeof(PLANE2), YES, "Rebound" );
				SetVector( &up, &currTile[pl]->normal );
				SetVector( &fx->rebound->normal, &up );
				ScaleVector( &up, 30 );
				AddVector( &fx->rebound->point, &frog[pl]->actor->pos, &up );
			}
			break;

		case DEATHBY_SQUASHED:
			break;

		case DEATHBY_FIRE:
			if( !(actFrameCount % 5) )
				CreateAndAddSpecialEffect( FXTYPE_FIERYSMOKE, &frog[pl]->actor->pos, &currTile[pl]->normal, 50, 0.5, 0, 1.5 );
			break;

		case DEATHBY_ELECTRICSHOCK:
			frog[pl]->actor->pos.v[X] += (-0.5F + Random(2));
			frog[pl]->actor->pos.v[Y] += (-0.5F + Random(2));
			frog[pl]->actor->pos.v[Z] += (-0.5F + Random(2));

#ifndef PC_VERSION

			FindTexture(&txtr1,UpdateCRC("fg.bmp"),NO);
			FindTexture(&txtr2,UpdateCRC("ai_wake.bmp"),NO);

			rVal = Random(10);
			if(rVal > 5)
				ReplaceTextureInDrawList(frog[pl]->actor->objectController->drawList,(u32)txtr1->data,(u32)txtr2->data,NO);
			else
				ReplaceTextureInDrawList(frog[pl]->actor->objectController->drawList,(u32)txtr2->data,(u32)txtr1->data,NO);
#endif
			break;

		case DEATHBY_FALLING:
			break;

		case DEATHBY_WHACKING:
			break;

		case DEATHBY_VACUUM:
			break;

		case DEATHBY_CHOCOLATE:
			break;
	}

	return TRUE;
}


/*	--------------------------------------------------------------------------------
	Function		: HopFrogToTile
	Purpose			: Hop the frog to a tile
	Parameters		: 
	Returns			: void
*/

void HopFrogToTile(GAMETILE *tile, long pl)
{
	VECTOR v;
	float maxdot = 0.0, dot;
	int j, dir = 0;

	player[pl].frogState |= FROGSTATUS_ISJUMPINGTOTILE;
	player[pl].canJump = 0;
	destTile[pl] = tile;

	SubVector(&v, &currTile[pl]->centre, &tile->centre);

	for(j=0; j<4; j++)
	{	
		dot = DotProduct(&currTile[pl]->dirVector[j], &v);
		if(dot > maxdot)
		{
			maxdot = dot;
			dir = j; //(j - camFacing + 2) & 3;
		}
	}

	AnimateFrogHop(dir, pl);	
	frogFacing[pl] = nextFrogFacing[pl] = (dir + 2) & 3;

	SitAndFace(frog[pl],currTile[pl],frogFacing[pl]);

	CalculateFrogJump(
		&frog[pl]->actor->pos, &destTile[pl]->centre, &currTile[pl]->normal,
		hopHeight, standardHopFrames, pl);
}

/*	--------------------------------------------------------------------------------
	Function		: HopFrogToTile
	Purpose			: Hop the frog to a tile
	Parameters		: 
	Returns			: void
*/

void SpringFrogToTile(GAMETILE *tile, float height, float time, long pl)
{
	long t = (long)(time * 60);

	player[pl].frogState |= FROGSTATUS_ISJUMPINGTOTILE;
	player[pl].canJump = 0;
	destTile[pl] = tile;

	player[pl].canJump = 0;
	player[pl].frogState = FROGSTATUS_ISJUMPINGTOTILE;
	
	if (currPlatform[pl])
	{
		currPlatform[pl]->carrying = NULL;
		currPlatform[pl]->flags &= ~PLATFORM_NEW_CARRYINGFROG;
		currPlatform[pl] = NULL;
	}
	destTile[pl] = tile;

	CalculateFrogJump(
		&frog[pl]->actor->pos, &destTile[pl]->centre, &currTile[pl]->normal,
		height, t, pl);
}

/*	--------------------------------------------------------------------------------
	Function		: PushFrog
	Purpose			: Push the frog in a given direction
	Parameters		: VECTOR, VECTOR, long
	Returns			: void
*/
void PushFrog(VECTOR *where, VECTOR *direction, long pl)
{
	VECTOR v;
	GAMETILE *dest;
	float maxdot = 0.0, dot;
	int j, dir = 0;

	// Find nearest matching direction vector
	for(j=0; j<4; j++)
	{	
		dot = DotProduct(&currTile[pl]->dirVector[j], direction);
		if(dot > maxdot)
		{
			maxdot = dot;
			dir = j; //(j - camFacing + 2) & 3;
		}
	}

	if (!MoveToRequestedDestination((dir - camFacing) & 3, pl)) return;
	//dest = currTile[pl]->tilePtrs[dir];

	if (player[pl].frogState & FROGSTATUS_ISJUMPINGTOPLATFORM)
		CalculateFrogJump(
			&frog[pl]->actor->pos, &destPlatform[pl]->pltActor->actor->pos, &currTile[pl]->normal,
			20.0f, standardHopFrames, pl);
	else
		CalculateFrogJump(
			&frog[pl]->actor->pos, &destTile[pl]->centre, &currTile[pl]->normal,
			20.0f, standardHopFrames, pl);

	currTile[pl] = destTile[pl];
}

/*	--------------------------------------------------------------------------------
	Function		: ThrowFrogAtScreen
	Purpose			: Throw the frog at the screen
	Parameters		: 
	Returns			: void
*/

#define THROWFROG_FRAMES	12

void ThrowFrogAtScreen(long pl)
{
	VECTOR target, v, w;
	float dist, time, screenDist = 60.0f;

	// Calculate the point to throw the frog to
	SubVector(&v, &currCamTarget[0], &currCamSource[0]);	// v points from screen to screen target
	dist = Magnitude(&v);
	ScaleVector(&v, 1.0f/dist);
	SetVector(&w, &v);
	ScaleVector(&w,	screenDist);
	AddVector(&target, &w, &currCamSource[0]);

	CalculateFrogJump(&frog[pl]->actor->pos, &target, &currTile[pl]->normal, 0, 80, pl);

//	animSpeed = 60.0f * player[pl].jumpSpeed / ((float)THROWFROG_FRAMES * frameTime);

	AnimateActor( frog[pl]->actor, FROG_ANIM_TO_SCREENSPLAT, NO, NO, 0.25, NO, NO );
	AnimateActor( frog[pl]->actor, FROG_ANIM_SCREENSPLAT, NO, YES, 0.25, NO, NO );

	controlCamera = 1;

	player[pl].idleEnable = 0;
}


/*	--------------------------------------------------------------------------------
	Function		: CalculateFrogJump
	Purpose			: Calculates jump curve variables
	Parameters		: VECTOR*, VECTOR*, VECTOR*, float, float, long
	Returns			: void
*/
void CalculateFrogJump(VECTOR *startPos, VECTOR *endPos, VECTOR *normal, float height, long time, long p)
{
	VECTOR F, V, Q;
	float diff, m;
	PLAYER *pl = &player[p];

	SetVector(&pl->jumpOrigin, startPos);
	SubVector(&V, endPos, startPos);
	
	// Forward vector
	diff = DotProduct(normal, &V);		// projection of V onto normal
	SetVector(&Q, normal);
	ScaleVector(&Q, diff);
	SubVector(&F, &V, &Q);			// F = V - N(N.V)
	SetVector(&pl->jumpFwdVector, &F);

	if (diff > 0) height += diff;		// When we're jumping UP, add height

	// Up vector
	SetVector(&pl->jumpUpVector, normal);
	ScaleVector(&pl->jumpUpVector, height);

	// Multiplier
	// Set to zero if we don't actually want to do the vertical bit.
	if (height)
	{
		m = 0.5f * (1 + sqrtf(1 - diff/height));
		pl->jumpSpeed = 1.0f/(m*(float)time); //1/(float)time; 
		//- longer jumps take longer, kind of thing
	}
	else
	{
		m = 0;	// Probably only happens on conveyors/ice, but still.
		pl->jumpSpeed = 1.0f/(float)time;
	}

	pl->jumpMultiplier = m;
	pl->jumpTime = 0;
	pl->heightJumped = diff;
}


/*	--------------------------------------------------------------------------------
	Function		: BounceFrog
	Purpose			: Jump in current direction
	Parameters		: player number, height, time
	Returns			: void
*/
void BounceFrog( int pl, float height, long time )
{
	GAMETILE *bounceTo;
	VECTOR dir;

	// Frog forward vector
	RotateVectorByQuaternion( &dir, &inVec, &frog[pl]->actor->qRot );
	ScaleVector( &dir, 30 );
	AddToVector( &dir, &frog[pl]->actor->pos );

	// And a tile to bounce to
	bounceTo = FindNearestJoinedTile( currTile[pl], &dir );
	CalculateFrogJump( &frog[pl]->actor->pos, &bounceTo->centre, &currTile[pl]->normal, height, time, pl );

	destTile[pl] = bounceTo;
}