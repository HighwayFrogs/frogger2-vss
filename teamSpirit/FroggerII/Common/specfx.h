/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: specfx.h
	Programmer	: Andy Eder
	Date		: 14/05/99 09:32:16

----------------------------------------------------------------------------------------------- */

#ifndef SPECFX_H_INCLUDED
#define SPECFX_H_INCLUDED


//----- [ DEFINES & ENUMS ] --------------------------------------------------------------------//

#define FADE_IN				0
#define FADE_OUT			1

#define NUM_RINGSEGS 16

extern char doScreenFade;
extern char	fadeDir;
extern short fadeOut;
extern short fadeStep;

extern char pauseMode;


// These are used in the editor and translated into effect parameters later
#define EF_RIPPLE_RINGS					(1 << 0)	// Lillypad thing
#define EF_SMOKE_STATIC					(1 << 1)	// Smoke that doesn't grow
#define EF_SPARKBURST					(1 << 2)	// Explosion of smoke
#define EF_FLAMES						(1 << 3)	// Fire
#define EF_FLYSWARM						(1 << 4)	// Crappy fly textures
#define EF_BUBBLES						(1 << 5)	// From the fishes
#define EF_SMOKE_GROWS					(1 << 6)	// Grows as it fades
#define EF_SMOKEBURST					(1 << 7)	// Explosion of smoke
#define EF_FIERYSMOKE					(1 << 8)	// Red at base, goes black
#define EF_BUTTERFLYSWARM				(1 << 9)	// Swarm of butterflies. Uses models

#define EF_FAST							(1 << 26)
#define EF_MEDIUM						(1 << 27)
#define EF_SLOW							(1 << 28)
#define EF_TINTRED						(1 << 29)
#define EF_TINTGREEN					(1 << 30)
#define EF_TINTBLUE						(1 << 31)


// Preset types. Any other can be made by selecting custom and specifying all the stucture members by hand
enum
{
	FXTYPE_CUSTOM,
	FXTYPE_WATERRIPPLE,
	FXTYPE_GARIBCOLLECT,
	FXTYPE_JUMPBLUR,
	FXTYPE_FROGSTUN,
	FXTYPE_SMOKE_STATIC,
	FXTYPE_SMOKE_GROWS,
	FXTYPE_BASICRING,
	FXTYPE_SPLASH,
	FXTYPE_SMOKEBURST,
	FXTYPE_FLAMES,
	FXTYPE_FLYSWARM,
	FXTYPE_BUBBLES,
	FXTYPE_SPARKBURST,
	FXTYPE_FIERYSMOKE,
	FXTYPE_POLYRING,
	FXTYPE_BUTTERFLYSWARM,

	FXTYPE_NUMTYPES
};

typedef struct
{
	VECTOR pos, vel;
	unsigned char r, g, b, a, bounce;

} PARTICLE;


typedef struct TAGSPECFX
{
	struct TAGSPECFX *next, *prev;

	VECTOR normal, origin, vel, scale;
	PLANE2 *rebound;
	PARTICLE *particles;						// For swarm, explosions etc.
	ACTOR2 **act;								// For models

	Vtx *verts;									// Persistent vertices on N64

	short type, fade;
	float speed, accn, angle, spin, tilt;
	long lifetime, deadCount, numP;				// numP is number of particles

	unsigned char r, g, b, a, gravity;

	SPRITE *sprites;
	TEXTURE *tex;
	ACTOR *follow;								// Go where it goes

	int (*Update) ();							// Just like C++
	void (*Draw) ();							// Update and draw functions, specified for different types.

} SPECFX;


typedef struct
{
	SPECFX head;
	int numEntries;

} SPECFXLIST;


extern SPECFXLIST specFXList;
extern D3DTLVERTEX *ringVtx;


extern SPECFX *CreateAndAddSpecialEffect( short type, VECTOR *origin, VECTOR *normal, int size, float speed, float accn, float lifetime );

extern void UpdateSpecialEffects( );

extern void AddSpecFX( SPECFX *fx );
extern void SubSpecFX( SPECFX *fx );
extern void InitSpecFXList( );
extern void FreeSpecFXList( );

extern void SetFXColour( SPECFX *fx, unsigned char r, unsigned char g, unsigned char b );
extern void SetAttachedFXColour( SPECFX *fx, int effects );
extern void ProcessAttachedEffects( void *entity, int type );

extern void CreateTeleportEffect( VECTOR *pos, VECTOR *normal );

#endif

