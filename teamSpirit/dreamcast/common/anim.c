/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: anim.c
	Programmer	: Andy Eder
	Date		: 29/04/99 10:30:56

----------------------------------------------------------------------------------------------- */


#define F3DEX_GBI_2

#include "Types.h"

#include "islpsi.h"
#include <islutil.h>

#include "event.h"
#include "evenfunc.h"
#include "anim.h"
#include "main.h"
#include "maths.h"
#include "specfx.h"
#include "frogger.h"
#include "frogmove.h"
#include "platform.h"
#include "layout.h"

#ifdef PSX_VERSION
#include "audio.h"
#else
#include <pcaudio.h>
#endif

fixed hedRotAmt = 0;


void DeathVanish( int pl )
{
	if( frogPool[player[pl].character].anim < FANIM_ALL )
	{
		DeathNormal(pl);
		return;
	}

	ChangeModel( frog[pl]->actor, "nothing" );
}


/*	--------------------------------------------------------------------------------
	Function 	: Death functions
	Purpose 	: Frog is dead
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void DeathNormal( int pl )
{
	player[pl].deathBy = DEATHBY_NORMAL;
	actorAnimate(frog[pl]->actor, FROG_ANIM_BOWLEDOVER, NO, NO, 128, 0);
	GTInit( &player[pl].dead, 3 );

	PlayVoice(pl, "frogdeath");
}

void DeathRunOver( int pl )
{
	DeathNormal(pl);
}

void DeathDrowning( int pl )
{
	GTInit( &player[pl].dead, 4 );
	player[pl].deathBy = DEATHBY_DROWNING;
	player[pl].frogState |= FROGSTATUS_ISDEAD;
	player[pl].isSinking = 0;
	
	actorAnimate(frog[pl]->actor,FROG_ANIM_DROWN,NO,NO,64,0);

	if( Random(2) )
		PlaySample(genSfx[GEN_DEATHDROWN],NULL,0,SAMPLE_VOLUME,-1);
	else
		PlayVoice( pl, "frogdrown2" );
}

void DeathSquashed( int pl )
{
	if( frogPool[player[pl].character].anim < FANIM_ALL )
	{
		DeathNormal(pl);
		return;
	}

	player[pl].deathBy = DEATHBY_SQUASHED;
	player[pl].idleEnable = 0;

	// Drop in replacement model and animate gib explosion
	ChangeModel( frog[pl]->actor, "dth-flat" );

	actorAnimate( frog[pl]->actor, 0, NO, NO, 51, 0 );
	actorAnimate( frog[pl]->actor, 1, YES, YES, 51, 0 );

	GTInit( &player[pl].dead, 3 );

	PlaySample(genSfx[GEN_DEATHCRUSH], NULL, 0, SAMPLE_VOLUME, -1 );
}

void DeathFire( int pl )
{
	TRIGGER *t;

	player[pl].deathBy = DEATHBY_FIRE;
	actorAnimate(frog[pl]->actor, FROG_ANIM_ASSONFIRE, NO, NO, 128, 0 );
	PlaySample( genSfx[GEN_DEATHFIRE], NULL, 0, SAMPLE_VOLUME, -1 );

	BounceFrog( pl, 4096000, 40 );

	GTInit( &player[pl].dead, 3 );

	t = MakeTrigger( OnTimeout, (void *)(actFrameCount + 120), NULL, NULL, NULL );
	AttachEvent( t, TRIGGER_ONCE, 0, DeathAnim, (void *)pl, (void *)DeathNormal, NULL, NULL );
}

void DeathElectric( int pl )
{
	SPECFX *fx;
	FVECTOR pos;
	SVECTOR tp;
	TRIGGER *t;

	if( frogPool[player[pl].character].anim < FANIM_ALL )
	{
		DeathNormal(pl);
		return;
	}

	// So the changing thing works right
	ChangeModel( frog[pl]->actor, frogPool[player[pl].character].singleModel );

	player[pl].deathBy = DEATHBY_ELECTRIC;
	actorAnimate(frog[pl]->actor, FROG_ANIM_ELECTROCUTE, YES, NO, 128, 0 );
	GTInit( &player[pl].dead, 4 );

	t = MakeTrigger( OnTimeout, (void *)(actFrameCount + 80), NULL, NULL, NULL );
	AttachEvent( t, TRIGGER_ONCE, 0, DeathAnim, (void *)pl, (void *)DeathIncinerate, NULL, NULL );

	SetVectorFF( &pos, &currTile[pl]->normal );
	ScaleVector( &pos, 50 );
	AddToVectorFS( &pos, &currTile[pl]->centre );
	SetVectorSF( &tp, &pos );

	if( (fx = CreateSpecialEffectDirect( FXTYPE_SPARKLYTRAIL, &tp, &currTile[pl]->normal, 163840, 8192, 0, 20480 )) )
	{
		fx->gravity = 4096;
		SetFXColour( fx, frogPool[player[pl].character].r, frogPool[player[pl].character].g, frogPool[player[pl].character].b );
	}
	if( (fx = CreateSpecialEffectDirect( FXTYPE_SPARKLYTRAIL, &tp, &currTile[pl]->normal, 163840, 8192, 0, 20480 )) )
	{
		fx->gravity = 4096;
		SetFXColour( fx, 220, 60, 60 );
	}

	PlaySample( genSfx[GEN_DEATHELECTRIC], NULL, 0, SAMPLE_VOLUME, -1 );
}

void DeathFalling( int pl )
{
	DeathNormal(pl);
}

void DeathWhacking( int pl )
{
	if( frog[pl]->actor->shadow ) frog[pl]->actor->shadow->draw = 0;

	player[pl].deathBy = DEATHBY_WHACKING;

	ThrowFrogAtScreen( pl );
	actorAnimate( frog[pl]->actor, FROG_ANIM_SCREENSPLAT, NO, NO, 64, NO);
	actorAnimate( frog[pl]->actor, FROG_ANIM_SCREENSLIDE, NO, YES, 64, NO);

	GTInit( &player[pl].dead, 5 );

	PlayVoice(pl, "frogdeath");
}

void DeathInflation( int pl )
{
	if( frogPool[player[pl].character].anim < FANIM_ALL )
	{
		DeathNormal(pl);
		return;
	}

	player[pl].deathBy = DEATHBY_INFLATION;
	player[pl].idleEnable = 0;

	// Drop in replacement model and animate gib explosion
	ChangeModel( frog[pl]->actor, "dth-ball" );

	// Randomly play different type of inflation anim
	if( Random(2) )
	{
		actorAnimate( frog[pl]->actor, 0, NO, NO, 51, 0 );
		actorAnimate( frog[pl]->actor, 2, YES, YES, 51, 0 );
	}
	else
	{
		actorAnimate( frog[pl]->actor, 1, NO, NO, 51, 0 );
		actorAnimate( frog[pl]->actor, 3, YES, YES, 51, 0 );
	}

	GTInit( &player[pl].dead, 5 );

	PlayVoice(pl, "frogdeath");
}

void DeathPoison( int pl )
{
	SPECFX *fx;

	player[pl].deathBy = DEATHBY_POISON;
	actorAnimate(frog[pl]->actor, FROG_ANIM_STUNG, NO, NO, 128, 0);

	if( (fx = CreateSpecialEffect( FXTYPE_SPARKBURST, &frog[pl]->actor->position, &currTile[pl]->normal, 81920, 16384, 0, 20480 )) )
	{
		SetFXColour( fx, 255, 255, 0 );
		fx->gravity = 4100;
	}

	GTInit( &player[pl].dead, 3 );

	PlayVoice(pl, "frogdeath");
}

void DeathSlicing( int pl )
{
	SPECFX *fx;

	if( frogPool[player[pl].character].anim < FANIM_ALL )
	{
		DeathNormal(pl);
		return;
	}

	player[pl].deathBy = DEATHBY_SLICING;
	player[pl].idleEnable = 0;

	if( frog[pl]->actor->shadow ) frog[pl]->actor->shadow->draw = 0;

	// Drop in replacement model and animate halves falling apart
	ChangeModel( frog[pl]->actor, "dth-half" );

	actorAnimate( frog[pl]->actor, 0, NO, NO, 64, 0 );

	if( (fx = CreateSpecialEffect( FXTYPE_SPARKBURST, &frog[pl]->actor->position, &currTile[pl]->normal, 81920, 16384, 0, 20480 )) )
	{
		SetFXColour( fx, 255, 255, 0 );
		fx->gravity = 4100;
	}
	if( (fx = CreateSpecialEffect( FXTYPE_SPARKBURST, &frog[pl]->actor->position, &currTile[pl]->normal, 81920, 12288, 0, 20480 )) )
	{
		SetFXColour( fx, 255, 255, 130 );
		fx->gravity = 4100;
	}
	if( (fx = CreateSpecialEffect( FXTYPE_DECAL, &frog[pl]->actor->position, &currTile[pl]->normal, 163840, 0, 0, 8192 )) )
	{
		SetFXColour( fx, frogPool[player[pl].character].r, frogPool[player[pl].character].g, frogPool[player[pl].character].b );
		fx->tex = txtrSolidRing;
	}
	GTInit( &player[pl].dead, 3 );

	PlaySample(genSfx[GEN_DEATHCHOP], NULL, 0, SAMPLE_VOLUME, -1 );
}

void DeathExplosion( int pl )
{
	TRIGGER *t;
	SPECFX *fx;

	if( frogPool[player[pl].character].anim < FANIM_ALL )
	{
		DeathNormal(pl);
		return;
	}

	if( frog[pl]->actor->shadow ) frog[pl]->actor->shadow->draw = 0;

	player[pl].deathBy = DEATHBY_EXPLOSION;
	player[pl].idleEnable = 0;

	// Drop in replacement model and animate gib explosion
	ChangeModel( frog[pl]->actor, "dth-gib" );

	actorAnimate( frog[pl]->actor, 0, NO, NO, 51, 0 );
	actorAnimate( frog[pl]->actor, 2, YES, YES, 51, 0 );

	if( (fx = CreateSpecialEffect( FXTYPE_SPARKBURST, &frog[pl]->actor->position, &currTile[pl]->normal, 81920, 16384, 0, 20480 )) )
	{
		SetFXColour( fx, 255, 255, 0 );
		fx->gravity = 4100;
	}
	if( (fx = CreateSpecialEffect( FXTYPE_SPARKBURST, &frog[pl]->actor->position, &currTile[pl]->normal, 81920, 12288, 0, 20480 )) )
	{
		SetFXColour( fx, 255, 255, 130 );
		fx->gravity = 4100;
	}
	if( (fx = CreateSpecialEffect( FXTYPE_DECAL, &frog[pl]->actor->position, &currTile[pl]->normal, 163840, 0, 0, 8192 )) )
	{
		SetFXColour( fx, frogPool[player[pl].character].r, frogPool[player[pl].character].g, frogPool[player[pl].character].b );
		fx->tex = txtrSolidRing;
	}
	GTInit( &player[pl].dead, 3 );

	// Make invisibly after explosion
	t = MakeTrigger( OnTimeout, (void *)(actFrameCount + 25), NULL, NULL, NULL );
	AttachEvent( t, TRIGGER_ONCE, 0, DeathAnim, (void *)pl, (void *)DeathVanish, NULL, NULL );

	PlaySample(genSfx[GEN_DEATHEXPLODE], NULL, 0, SAMPLE_VOLUME, -1 );
}

void DeathGibbing( int pl )
{
	SPECFX *fx;

	if( frogPool[player[pl].character].anim < FANIM_ALL )
	{
		DeathNormal(pl);
		return;
	}

	if( frog[pl]->actor->shadow ) frog[pl]->actor->shadow->draw = 0;

	player[pl].deathBy = DEATHBY_GIBBING;
	player[pl].idleEnable = 0;

	// Drop in replacement model and animate gib explosion
	ChangeModel( frog[pl]->actor, "dth-gib" );

	actorAnimate( frog[pl]->actor, 1, NO, NO, 51, 0 );
	actorAnimate( frog[pl]->actor, 3, YES, YES, 51, 0 );

	if( (fx = CreateSpecialEffect( FXTYPE_SPARKBURST, &frog[pl]->actor->position, &currTile[pl]->normal, 81920, 16384, 0, 20480 )) )
	{
		SetFXColour( fx, 255, 255, 0 );
		fx->gravity = 4100;
	}
	if( (fx = CreateSpecialEffect( FXTYPE_SPARKBURST, &frog[pl]->actor->position, &currTile[pl]->normal, 81920, 12288, 0, 20480 )) )
	{
		SetFXColour( fx, 255, 255, 130 );
		fx->gravity = 4100;
	}
	if( (fx = CreateSpecialEffect( FXTYPE_DECAL, &frog[pl]->actor->position, &currTile[pl]->normal, 163840, 0, 0, 8192 )) )
	{
		SetFXColour( fx, frogPool[player[pl].character].r, frogPool[player[pl].character].g, frogPool[player[pl].character].b );
		fx->tex = txtrSolidRing;
	}
	GTInit( &player[pl].dead, 3 );

	if( player[0].worldNum != WORLDID_GARDEN )
		PlaySample(genSfx[GEN_DEATHGIB], NULL, 0, SAMPLE_VOLUME, -1 );
	else
		PlaySample(genSfx[GEN_DEATHMOWED], NULL, 0, SAMPLE_VOLUME, -1 );
}

void DeathIncinerate( int pl )
{
	if( frogPool[player[pl].character].anim < FANIM_ALL )
	{
		DeathNormal(pl);
		return;
	}

	player[pl].deathBy = DEATHBY_INCINERATE;
	player[pl].idleEnable = 0;

	if( frog[pl]->actor->shadow ) frog[pl]->actor->shadow->draw = 0;

	// Drop in replacement model and animate frog becoming a pile of ash
	ChangeModel( frog[pl]->actor, "dth-ash" );

	actorAnimate( frog[pl]->actor, 0, NO, NO, 80, 0 );
	actorAnimate( frog[pl]->actor, 1, YES, YES, 80, 0 );

	CreateSpecialEffect( FXTYPE_SMOKEBURST, &frog[pl]->actor->position, &currTile[pl]->normal, 204800, 8192, 0, 6963 );

	GTInit( &player[pl].dead, 3 );

	PlayVoice(pl, "frogdeath");
}

void DeathBursting( int pl )
{
	TRIGGER *t;

	if( frogPool[player[pl].character].anim < FANIM_ALL )
	{
		DeathNormal(pl);
		return;
	}

	DeathInflation( pl );
	GTInit( &player[pl].dead, 4 );

	t = MakeTrigger( OnTimeout, (void *)(actFrameCount + 120), NULL, NULL, NULL );
	AttachEvent( t, TRIGGER_ONCE, 0, DeathAnim, (void *)pl, (void *)DeathExplosion, NULL, NULL );
}

void DeathSpiked( int pl )
{
	TRIGGER *t;

	if( frogPool[player[pl].character].anim < FANIM_ALL )
	{
		DeathNormal(pl);
		return;
	}

	// So the changing thing works right
	ChangeModel( frog[pl]->actor, frogPool[player[pl].character].singleModel );

	player[pl].deathBy = DEATHBY_SPIKED;
	actorAnimate(frog[pl]->actor, FROG_ANIM_ASSONFIRE, NO, NO, 128, 0 );
	PlayVoice(pl, "frogdeath");

	CalculateFrogJump( &frog[pl]->actor->position, &currTile[pl]->centre, &currTile[pl]->normal, 5000000, 40, pl );

	GTInit( &player[pl].dead, 3 );

	t = MakeTrigger( OnTimeout, (void *)(actFrameCount + 35), NULL, NULL, NULL );
	AttachEvent( t, TRIGGER_ONCE, 0, DeathAnim, (void *)pl, (void *)DeathGibbing, NULL, NULL );
}

void DeathMulti( int pl )
{
	SPECFX *fx;
	FVECTOR pos;
	SVECTOR tp;

	// A standard multiplayer death
	SetVectorFF( &pos, &currTile[pl]->normal );
	ScaleVector( &pos, 50 );
	AddToVectorFS( &pos, &currTile[pl]->centre );
	SetVectorSF( &tp, &pos );
	fx = CreateSpecialEffectDirect( FXTYPE_SMOKEBURST, &tp, &currTile[pl]->normal, 262144, 8192, 410, 6963 );
	SetFXColour( fx, frogPool[player[pl].character].r, frogPool[player[pl].character].g, frogPool[player[pl].character].b );
	fx = CreateSpecialEffectDirect( FXTYPE_SMOKEBURST, &tp, &currTile[pl]->normal, 327680, 8192, 410, 6963 );
	SetFXColour( fx, frogPool[player[pl].character].r, frogPool[player[pl].character].g, frogPool[player[pl].character].b );

	fx = CreateSpecialEffectDirect( FXTYPE_WAKE, &tp, &currTile[pl]->normal, 81920, 410, 205, 2048 );
	SetFXColour( fx, frogPool[player[pl].character].r, frogPool[player[pl].character].g, frogPool[player[pl].character].b );

	PlaySample( genSfx[GEN_DEATHEXPLODE], NULL, 0, SAMPLE_VOLUME, -1 );

	player[pl].frogState |= FROGSTATUS_ISDEAD;
	GTInit( &player[pl].dead, 3 );
	frog[pl]->draw = 0;
}

void DeathLightning( int pl )
{
	SPECFX *fx;
	FVECTOR pos;
	SVECTOR tp;

	SetVectorFF( &pos, &currTile[pl]->normal );
	ScaleVector( &pos, 50 );
	AddToVectorFS( &pos, &currTile[pl]->centre );
	SetVectorSF( &tp, &pos );
	fx = CreateSpecialEffectDirect( FXTYPE_SMOKEBURST, &tp, &currTile[pl]->normal, 262144, 8192, 410, 6963 );
	SetFXColour( fx, 200, 200, 255 );
	fx = CreateSpecialEffectDirect( FXTYPE_SMOKEBURST, &tp, &currTile[pl]->normal, 327680, 8192, 410, 6963 );
	SetFXColour( fx, 255, 255, 255 );

	if( (fx = CreateSpecialEffectDirect( FXTYPE_SPARKBURST, &tp, &currTile[pl]->normal, 72200, 8192, 0, 20480 )) )
	{
		fx->gravity = 4096;
		SetFXColour( fx, frogPool[player[pl].character].r, frogPool[player[pl].character].g, frogPool[player[pl].character].b );
	}
	if( (fx = CreateSpecialEffectDirect( FXTYPE_SPARKBURST, &tp, &currTile[pl]->normal, 61149, 8192, 0, 20480 )) )
	{
		fx->gravity = 4096;
		SetFXColour( fx, 220, 60, 60 );
	}

	PlaySample( genSfx[GEN_DEATHELECTRIC], NULL, 0, SAMPLE_VOLUME, -1 );

	player[pl].frogState |= FROGSTATUS_ISDEAD;
	GTInit( &player[pl].dead, 3 );
	frog[pl]->draw = 0;
}
