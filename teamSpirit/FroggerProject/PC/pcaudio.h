/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: audio.h
	Programmer	: Jim Hubbard
	Date		: 24/03/00

----------------------------------------------------------------------------------------------- */

#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include <windows.h>
#include <dsound.h>
#include <mdx.h>

#include "types.h"
#include "actor.h"



#ifdef __cplusplus
extern "C"
{
#endif


//***********************************
// Defines

#define SFXFLAGS_3D_SAMPLE		(1<<0)
#define SFXFLAGS_LOOP			(1<<1)

#define AMBIENT_VOLUME	255
#define SAMPLE_VOLUME	255


enum
{
	NOTRACK = 0,
	GARDEN_CDAUDIO = 2,
	ANCIENTS_CDAUDIO,
	SPACE_CDAUDIO,
	CITY_CDAUDIO,
	SUBTERRANEAN_CDAUDIO,
	LABORATORY_CDAUDIO,
	HALLOWEEN_CDAUDIO,
	SUPERRETRO_CDAUDIO,
	FRONTEND_CDAUDIO,
	LEVELSELECT_CDAUDIO,

	NUM_CD_TRACKS,
};

// All generic sfx
enum
{
	GEN_FROG_HOP,
	GEN_SUPER_HOP,
	GEN_DOUBLE_HOP,
	GEN_COLLECT_BABY,
	GEN_FROG_TONGUE,
	GEN_COLLECT_COIN,
	GEN_CROAK,
	GEN_DROWN,
	GEN_BABYHAPPY,
	GEN_BABYSAD,
	GEN_BABYCRY,

	NUM_GENERIC_SFX,
};

//***********************************
// Type Defines

typedef struct _SAMPLE
{
	struct _SAMPLE			*next, *prev;

//	char					*idName;
	unsigned long			flags;
	unsigned long			uid;

	LPWAVEFORMATEX			lpWavFmt;
	BYTE					*data;
	DWORD					len;

	LPDIRECTSOUNDBUFFER		lpdsBuffer;	
    LPDIRECTSOUND3DBUFFER   lpds3DBuffer;	// DirectSound 3D buffer interface

} SAMPLE;


typedef struct _SOUNDLIST
{
	int				numEntries;
	SAMPLE			head;

} SOUNDLIST;


typedef struct _BUFSAMPLE
{
	struct _BUFSAMPLE *next, *prev;
	LPDIRECTSOUNDBUFFER lpdsBuffer;

} BUFSAMPLE;


typedef struct _BUFFERLIST
{
	int			numEntries;
	BUFSAMPLE	head;

} BUFFERLIST;


typedef struct TAG_AMBIENT_SOUND
{
	struct TAG_AMBIENT_SOUND *next,*prev;

	SVECTOR		pos;
	MDX_ACTOR	*follow;

	struct _SAMPLE *sample;
	LPDIRECTSOUNDBUFFER		lpdsBuffer;

	short		volume;
	short		pitch;
	long		radius;

	long		freq;
	long		randFreq;
	long		counter;

}AMBIENT_SOUND;


typedef struct
{
	AMBIENT_SOUND head;
	int numEntries;

}AMBIENT_SOUND_LIST;


extern SAMPLE *genSfx[];

//***********************************
// Function Prototypes

extern void LoadSfxMapping( int world, int level );
extern void FindSfxMapping( unsigned long uid, ACTOR *actor );

extern void LoadSfx( unsigned long worldID );

extern void InitSampleList( );
extern void FreeSampleList( );

extern void InitAmbientSoundList( );
extern void FreeAmbientSoundList( );

extern void FreeBufSampleList( );

extern SAMPLE *FindSample( unsigned long uid );

extern void CleanBufferSamples ( void );
extern void SetSampleFormat ( SAMPLE *sample );


extern int PlaySample(SAMPLE *sample,SVECTOR *pos,long radius,short volume,short pitch);
void PlaySfxMappedSample( MDX_ACTOR *act );
extern void UpdateAmbientSounds();


extern void PrepareSong( );

#ifdef __cplusplus
}
#endif

#endif
