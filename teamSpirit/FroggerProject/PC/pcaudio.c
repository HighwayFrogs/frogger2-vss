/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: audio.c
	Programmer	: Random Healey
	Date		: 28/06/99 10:37:45

----------------------------------------------------------------------------------------------- */

#include "..\resource.h"
#include <windows.h>
#include <ddraw.h>
#include <d3d.h>
#include <dsound.h>
#include <mdx.h>

#include <islmem.h>
#include <islutil.h>
#include <islfile.h>

#include "enemies.h"
#include "frogger.h"
#include "frogmove.h"
#include "platform.h"
#include "memload.h"
#include "Main.h"
#include "dx_sound.h"
#include "pcaudio.h"
#include "pcmisc.h"
#include "menus.h"
#include "cam.h"
#include "game.h"
#include "controll.h"

#define MAX_AMBIENT_SFX		50
#define DEFAULT_SFX_DIST	7000

#define VOLUME_MIN		-5000.0
#define VOLUME_PERCENT (VOLUME_MIN/255.0)
#define PITCH_STEP		(DSBFREQUENCY_MAX/256)

int AMBIENT_VOLUME = 255;
int SAMPLE_VOLUME = 255;

SAMPLE voices[4];
SAMPLE *genSfx[NUM_GENERIC_SFX];

MCIDEVICEID mciDevice = 0;

SOUNDLIST soundList;					// Actual Sound Samples List
AMBIENT_SOUND_LIST	ambientSoundList;
BUFFERLIST bufferList;					// Buffered Sound Samples List


void AddSample( SAMPLE *sample );
void RemoveSample( SAMPLE *sample );
void AddBufSample( BUFSAMPLE *sample );
void RemoveBufSample( BUFSAMPLE *sample );

AMBIENT_SOUND *AddAmbientSound(SAMPLE *sample,SVECTOR *pos,long radius,short volume,short pitch,int freq,int rFreq,MDX_ACTOR *follow);
void SubAmbientSound(AMBIENT_SOUND *ambientSound);
int UpdateLoopingSample( AMBIENT_SOUND *sample );

SAMPLE *CreateSample( char *data, SAMPLE_HEADER *shdr );

void SetSampleFormat ( SAMPLE *sample );
void CleanBufferSamples( void );

DWORD playCDTrack( HWND hWndNotify, BYTE bTrack );
DWORD stopCDTrack( HWND hWndNotify );

// CD Audio variables

static int		auxVolume, oldVolume;
static DWORD	volumecontrolid, cdaudiovalid;

int cdTrack = 0;	// cd is not playing

static char		errStr[128];
static int		mixerid;

static char *MCIerrorToString(int err)
{
#ifndef DOUBLEBYTE
	mciGetErrorString(err, errStr,128);
	return errStr;
#else
	return "DByte";
#endif
}

long bytetoDB[256] = 
{
	//0-15
   -10000,-7992,-6992,-6407,-5992,-5670,-5407,-5185,-4992,-4823,-4671,-4533,-4408,-4292,-4185,-4086,
	//16-31
	-3993,-3905,-3823,-3745,-3671,-3601,-3533,-3470,-3408,-3350,-3293,-3239,-3186,-3135,-3086,-3039,
	//32-47
	-2993,-2949,-2906,-2864,-2823,-2784,-2745,-2708,-2671,-2636,-2601,-2567,-2534,-2501,-2470,-2439,
	//48-63
	-2408,-2378,-2349,-2321,-2293,-2265,-2238,-2212,-2186,-2160,-2135,-2111,-2086,-2063,-2039,-2016,
	//64-79
	-1993,-1971,-1949,-1927,-1906,-1885,-1864,-1844,-1823,-1804,-1785,-1765,-1745,-1727,-1708,-1690,
	//80-95
	-1671,-1654,-1636,-1618,-1601,-1584,-1567,-1550,-1534,-1518,-1502,-1486,-1470,-1454,-1439,-1424,
	//96-111
	-1408,-1394,-1379,-1364,-1350,-1335,-1321,-1307,-1293,-1279,-1266,-1252,-1239,-1225,-1212,-1199,
	//112-127
	-1186,-1173,-1161,-1148,-1136,-1123,-1111,-1099,-1087,-1075,-1063,-1051,-1039,-1028,-1016,-1005,
	//128-143
	-994,-982,-971,-960,-950,-938,-928,-917,-906,-896,-885,-875,-864,-854,-844,-834,
	//144-159
	-824,-814,-804,-794,-784,-774,-765,-755,-746,-736,-727,-718,-708,-699,-690,-681,
	//160-175
	-672,-663,-654,-645,-636,-627,-619,-610,-601,-593,-585,-576,-568,-560,-551,-543,
	//176-191
	-535,-527,-518,-510,-502,-494,-486,-478,-471,-463,-455,-447,-440,-432,-424,-417,
	//192-207
	-409,-402,-394,-387,-380,-372,-365,-358,-350,-343,-336,-329,-322,-315,-308,-301,
	//208-223
	-294,-287,-280,-273,-266,-260,-253,-246,-240,-233,-226,-220,-213,-206,-200,-193,
	//224-239
	-187,-181,-174,-168,-161,-155,-149,-142,-136,-130,-124,-118,-112,-106,100,-93,
	//240-255
	-87,-81,-75,-70,-64,-58,-52,-46,-40,-34,-28,-22,-17,-11,-5,0
};

enum
{
	NOTRACK = 0,
	GARDEN_CDAUDIO			= 2,
	ANCIENTS_CDAUDIO,
	SPACE_CDAUDIO,
//	CITY_CDAUDIO,
	SUBTERRANEAN_CDAUDIO,
	LABORATORY_CDAUDIO,
	HALLOWEEN_CDAUDIO,
	SUPERRETRO_CDAUDIO,
	FRONTEND_CDAUDIO,
	LEVELCOMPLETE_CDAUDIO,
	GAMEOVER_CDAUDIO,
	LEVELCOMPLETELOOP_CDAUDIO,

	NUM_CD_TRACKS,
};

/*	--------------------------------------------------------------------------------
	Function		: LoadSfx
	Purpose			: 
	Parameters		: 
	Returns			: 
	Info			: 
*/
void LoadSfxSet( char *path )
{
	SAMPLE_HEADER shdr;
	unsigned long length, numFiles;
	char *stak, *ptr;

	if( !(stak = (char *)fileLoad( path, &length )) )
	{
		utilPrintf( "Failed to load sample bank %s\n", path );
		return;
	}

	numFiles = (int)stak[0];
	ptr = &stak[4];

	while( numFiles-- )
	{
		// Length of the buffer, not including format info
		memcpy( &shdr, ptr, sizeof(SAMPLE_HEADER) );
		ptr += sizeof(SAMPLE_HEADER);

		AddSample( CreateSample( ptr, &shdr) );

		// Sample plus format data plus loop flag
		ptr += shdr.length + sizeof(WAVEFORMATEX);
	}

	FREE(stak);
}


void LoadSfx( unsigned long worldID )
{
	char path[128];
	int len, load=1;
	
	len = strlen(baseDirectory) + strlen(SFX_BASE);

	strcpy( path, baseDirectory );
	strcat( path, SFX_BASE );

	// Load all generic samples first and put in array
	strcat( path, "generic.wus" );
	LoadSfxSet(path);

	genSfx[GEN_FROG_HOP] = FindSample(UpdateCRC("hopongrass"));
	genSfx[GEN_SUPER_HOP] = FindSample(UpdateCRC("hop2"));
	genSfx[GEN_DOUBLE_HOP] = FindSample(UpdateCRC("doublehop"));
	genSfx[GEN_COLLECT_BABY] = FindSample(UpdateCRC("getbabyfrog"));
	genSfx[GEN_COLLECT_COIN] = FindSample(UpdateCRC("pickupcoin"));
	genSfx[GEN_BABYHAPPY] = FindSample(UpdateCRC("babyhappy"));
	genSfx[GEN_BABYSAD] = FindSample(UpdateCRC("babysad"));
	genSfx[GEN_BABYCRY] = FindSample(UpdateCRC("babycry"));
	genSfx[GEN_BABYREPLY] = FindSample(UpdateCRC("babyreply"));
	genSfx[GEN_TELEPORT] = FindSample(UpdateCRC("teleport"));
	genSfx[GEN_POWERUP] = FindSample(UpdateCRC("powerup"));
	genSfx[GEN_CLOCKTICK] = FindSample(UpdateCRC("clocktick"));
	genSfx[GEN_POWERTICK] = FindSample(UpdateCRC("puptick"));

	genSfx[GEN_DEATHDROWN] = FindSample(UpdateCRC("frogdrown1"));
	genSfx[GEN_DEATHCRUSH] = FindSample(UpdateCRC("frogcrush"));
	genSfx[GEN_DEATHEXPLODE] = FindSample(UpdateCRC("frogexplode"));
	genSfx[GEN_DEATHFALL] = FindSample(UpdateCRC("frogfall"));
	genSfx[GEN_DEATHMOWED] = FindSample(UpdateCRC("frogmowed"));
	genSfx[GEN_DEATHGIB] = FindSample(UpdateCRC("frog_gib"));
	genSfx[GEN_DEATHCHOP] = FindSample(UpdateCRC("frogchop"));
	genSfx[GEN_DEATHELECTRIC] = FindSample(UpdateCRC("electrocute"));
	genSfx[GEN_DEATHFIRE] = FindSample(UpdateCRC("burnbum"));

	InitVoices( path, len );

	if( gameState.multi != SINGLEPLAYER )
	{
		strcat( path, "multi.wus" );
		LoadSfxSet( path );
		path[len] = '\0';
	}

	switch( worldID )
	{
	case WORLDID_GARDEN: strcat( path, "garden.wus" ); break;
	case WORLDID_ANCIENT: strcat( path, "ancients.wus" ); break;
	case WORLDID_SPACE: strcat( path, "space.wus" ); break;
	case WORLDID_CITY: strcat( path, "city.wus" ); break;
	case WORLDID_SUBTERRANEAN: strcat( path, "sub.wus" ); break;
	case WORLDID_LABORATORY: strcat( path, "lab.wus" ); break;
	case WORLDID_HALLOWEEN: strcat( path, "halloween.wus" ); break;
	case WORLDID_SUPERRETRO: strcat( path, "superretro.wus" ); break;
	case WORLDID_FRONTEND: strcat( path, "frontend.wus" ); break;
	case WORLDID_TEST: strcat(path, "test.wus"); break;
	default: load = 0; break;
	}

	if( load )
		LoadSfxSet( path );
}


/*	--------------------------------------------------------------------------------
	Function		: CreateAndAddSample
	Purpose			: Make a sample from a mapping
	Parameters		: 
	Returns			: 
	Info			: 
*/
SAMPLE *CreateSample( char *data, SAMPLE_HEADER *shdr )
{
	SAMPLE *sfx;
	int i=0;
	
	if( !lpDS )
	{
		utilPrintf("Could not create sample, no sound device\n");
		return NULL;
	}

	sfx = (SAMPLE *)MALLOC0(sizeof(SAMPLE));

	sfx->uid = shdr->uid;
	if( shdr->loop ) 
		sfx->flags |= SFXFLAGS_LOOP;

	if( !(LoadWav(sfx,data,shdr->length)) )
	{
		FREE( sfx );
		utilPrintf("Could not load wave file\n");
		return NULL;
	}

	return sfx;
}


/*	--------------------------------------------------------------------------------
	Function		: PlaySample
	Purpose			: plays a sample
	Parameters		: ID, position, radius, volume, pitch
	Returns			: success?
	Info			: Pass in a valid vector to get attenuation, and a radius to override the default
*/
int PlaySample( SAMPLE *sample, SVECTOR *pos, long radius, short volume, short pitch )
{
	BUFSAMPLE *bufSample=NULL;
	long vol = (volume*globalSoundVol)/MAX_SOUND_VOL;
	int pan = 0;
	float att, dist;
	SVECTOR diff;
	unsigned long flags=0;
	LPDIRECTSOUNDBUFFER lpdsBuffer;
	MDX_VECTOR mdxPos,scrPos;

	if(!lpDS || !sample || !vol) return FALSE;	// No DirectSound object!

/*	if( sample->flags & SFXFLAGS_3D_SAMPLE )
	{
		if( sample->lpds3DBuffer )
		{
			sample->lpds3DBuffer->lpVtbl->SetMode ( sample->lpds3DBuffer, DS3DMODE_NORMAL, DS3D_IMMEDIATE );
			Set3DPosition ( sample->lpds3DBuffer, pos->vx, pos->vy, pos->vz );
		}
	}
	else*/ 
	if( pos )
	{
		att = (radius)?radius:DEFAULT_SFX_DIST;
//		att *= 2;
//		SetVectorSS( &check, currPlatform[0]?&currPlatform[0]->pltActor->actor->position:&currTile[0]->centre );

//		SubVectorSSS( &diff, pos, &check );

		SubVectorSSF(&diff, pos, &currCamSource);

		// Volume attenuation - check also for radius != 0 and use instead of default
		dist = (float)MagnitudeS( &diff )/4096.0;
		if( dist > att )
			return FALSE;

		vol *= (att-dist)/att;

		//work out pan
		mdxPos.vx = (float)pos->vx/10.0;
		mdxPos.vy = (float)pos->vy/10.0;
		mdxPos.vz = (float)pos->vz/10.0;
		guMtxXFMF(vMatrix.matrix,mdxPos.vx,mdxPos.vy,mdxPos.vz,&scrPos.vx,&scrPos.vy,&scrPos.vz);

		dist = mdxMagnitude(&scrPos);
		if(dist)
			pan = (scrPos.vx*255)/dist;
		else
			pan = 0;
	}

	if( sample->flags & SFXFLAGS_LOOP )
		flags |= DSBPLAY_LOOPING;


	//	What we need to do here is create an instance of the buffer and store it in the buffer list.
	//	Have a clean buffer function that will go though and check if the sample is playing or not,
	//	if the sample is not playing then remove it from the list.
	if( !(bufSample = (BUFSAMPLE *)MALLOC0(sizeof(BUFSAMPLE))) )
		return 0;

	lpDS->lpVtbl->DuplicateSoundBuffer( lpDS, sample->lpdsBuffer, &(bufSample->lpdsBuffer) );

	AddBufSample( bufSample );
	lpdsBuffer = bufSample->lpdsBuffer;

	lpdsBuffer->lpVtbl->Play( lpdsBuffer, 0, 0, flags );
	lpdsBuffer->lpVtbl->SetFrequency( lpdsBuffer, (pitch==-1)?(DSBFREQUENCY_ORIGINAL):(pitch*PITCH_STEP) );

	lpdsBuffer->lpVtbl->SetVolume( lpdsBuffer, bytetoDB[vol] );
	if(pan <= 0)
		lpdsBuffer->lpVtbl->SetPan( lpdsBuffer, -bytetoDB[255+pan] );
	else
		lpdsBuffer->lpVtbl->SetPan( lpdsBuffer, bytetoDB[255-pan] );
	// HAAACCK! Bwahahahahahah!
	return (int)lpdsBuffer;
}


/*	--------------------------------------------------------------------------------
	Function		: StopSample
	Purpose			: stops a playing sample
	Parameters		: sample
	Returns			: success?
	Info			: Only works for unique samples - any buffered copies will keep playing
*/
int StopSample( SAMPLE *sample )
{
	unsigned long bufStatus;

	sample->lpdsBuffer->lpVtbl->GetStatus( sample->lpdsBuffer, &bufStatus );

	if( bufStatus & DSBSTATUS_PLAYING )
	{
		sample->lpdsBuffer->lpVtbl->Stop( sample->lpdsBuffer );
		return TRUE;
	}

	return FALSE;
}

/*	--------------------------------------------------------------------------------
	Function		: PlaySample
	Purpose			: plays a sample
	Parameters		: ID, position, radius, volume, pitch
	Returns			: success?
	Info			: Pass in a valid vector to get attenuation, and a radius to override the default
*/
void PlaySfxMappedSample( MDX_ACTOR *act )
{
	SAMPLE **map, *sample;
	SVECTOR pos;

	if( !act || !act->animation )
		return;

	if( !(map = (SAMPLE **)act->animation->sfxMapping) )
		return;

	if( !(sample = map[act->animation->currentAnimation]) )
		return;

	SetVectorSR( &pos, &act->pos );
	ScaleVector( &pos, 10 );
	// If looping, add ambient sound and remove sample from mapping
	if( sample->flags & SFXFLAGS_LOOP )
	{
		if( !act->animation->loopFlags[act->animation->currentAnimation] )
		{
			AddAmbientSound( sample, &pos, DEFAULT_SFX_DIST, SAMPLE_VOLUME, -1, 0, 0, act );
			act->animation->loopFlags[act->animation->currentAnimation] = 1;
		}
		return;
	}

	PlaySample( sample, &pos, DEFAULT_SFX_DIST, SAMPLE_VOLUME, -1 );
}


/*	--------------------------------------------------------------------------------
	Function		: PlaySample
	Purpose			: plays a sample
	Parameters		: ID, position, radius, volume, pitch
	Returns			: success?
	Info			: Pass in a valid vector to get attenuation, and a radius to override the default
*/
int UpdateLoopingSample( AMBIENT_SOUND *sample )
{
	unsigned long bufStatus;
	long vol=(sample->volume*globalSoundVol)/MAX_SOUND_VOL;
	int pan = 0;
	float att, dist;
	SVECTOR diff;
	MDX_VECTOR mdxPos,scrPos;

	if(!lpDS || !sample) return FALSE;	// No DirectSound object!

	if( sample->pos.vx || sample->pos.vy || sample->pos.vz )
	{
		att = (sample->radius)?sample->radius:DEFAULT_SFX_DIST;
//		att *= 2;
//		SetVectorSS( &check, currPlatform[0]?&currPlatform[0]->pltActor->actor->position:&currTile[0]->centre );

//		SubVectorSSS( &diff, &sample->pos, &check );

		SubVectorSSF(&diff, &sample->pos, &currCamSource);

		// Volume attenuation - check also for radius != 0 and use instead of default
		dist = (float)MagnitudeS( &diff )/4096.0;
		if( dist > att )
			vol=0; //return FALSE;

		vol *= (att-dist)/att;

		//work out pan
		mdxPos.vx = (float)sample->pos.vx/10.0;
		mdxPos.vy = (float)sample->pos.vy/10.0;
		mdxPos.vz = (float)sample->pos.vz/10.0;
		guMtxXFMF(vMatrix.matrix,mdxPos.vx,mdxPos.vy,mdxPos.vz,&scrPos.vx,&scrPos.vy,&scrPos.vz);

		dist = mdxMagnitude(&scrPos);
		if(dist)
			pan = (scrPos.vx*255)/dist;
		else
			pan = 0;
	}


	// Now test if the sample is playing - if it is then make a buffered instance of it to play.
	sample->lpdsBuffer->lpVtbl->GetStatus( sample->lpdsBuffer, &bufStatus );

	sample->lpdsBuffer->lpVtbl->SetFrequency( sample->lpdsBuffer, (sample->pitch==-1)?(DSBFREQUENCY_ORIGINAL):(sample->pitch*PITCH_STEP) );
	sample->lpdsBuffer->lpVtbl->SetVolume( sample->lpdsBuffer, bytetoDB[vol] );
	if(pan <= 0)
		sample->lpdsBuffer->lpVtbl->SetPan( sample->lpdsBuffer, -bytetoDB[255+pan] );
	else
		sample->lpdsBuffer->lpVtbl->SetPan( sample->lpdsBuffer, bytetoDB[255-pan] );

	return TRUE;
}


/*	--------------------------------------------------------------------------------
	Function		: InitVoices
	Purpose			: Load and store voice sets for all frogs
	Parameters		: 
	Returns			: void
	Info			: 
*/
void InitVoices( char *path, int len )
{
	SAMPLE *voice;
	SAMPLE_HEADER shdr;
	unsigned long numFiles, i, length;
	char *stak, *ptr;

	for( i=0; i<NUM_FROGS; i++ )
	{
		path[len] = '\0'; // ds

		strcat( path, frogPool[player[i].character].fileName );
		strcat( path, ".wus" );

		if( !(stak = (char *)fileLoad( path, &length )) )
		{
			utilPrintf( "Failed to load voice bank for %s\n", frogPool[player[i].character].fileName );
			continue;
		}

		numFiles = (int)stak[0];
		ptr = &stak[4];

		while( numFiles-- )
		{
			// Length of the buffer, not including format info
			memcpy( &shdr, ptr, sizeof(SAMPLE_HEADER) );
			ptr += sizeof(SAMPLE_HEADER);

			if( (voice = CreateSample(ptr, &shdr)) )
			{
				// Add to special voice list
				voice->next = voices[i].next;
				voice->prev = &voices[i];
				voice->prev->next = voice;
				voice->next->prev = voice;
			}

			// Sample plus format data
			ptr += shdr.length + sizeof(WAVEFORMATEX);
		}
	}

	FREE(stak);
	path[len] = '\0';
}


/*	--------------------------------------------------------------------------------
	Function		: SetSampleFormat
	Purpose			: Changes the format of the sound
	Parameters		: SAMPLE *sample : Contains the sample to change the format for.
	Returns			: void
	Info			: 
*/
void SetSampleFormat( SAMPLE *sample )
{
	WAVEFORMATEX	wfx;

	memset ( &wfx, 0, sizeof ( WAVEFORMATEX ) ); 
	wfx.wFormatTag		= WAVE_FORMAT_PCM; 
	wfx.nChannels		= 1;
	wfx.nSamplesPerSec	= 22050;
	wfx.wBitsPerSample	= 16;
	wfx.nBlockAlign		= wfx.wBitsPerSample / 8 * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	sample->lpdsBuffer->lpVtbl->SetFormat ( sample->lpdsBuffer, &wfx );
}


/*	--------------------------------------------------------------------------------
	Function		: Clean Buffer Samples
	Purpose			: I have no idea
	Parameters		: 
	Returns			: 
	Info			: 
*/
void CleanBufferSamples ( void )
{
	BUFSAMPLE *cur, *next;

	unsigned long bufStatus;

	if ( bufferList.numEntries == 0)
		return;

	for ( cur = bufferList.head.next; cur != &bufferList.head; cur = next )
	{
		next = cur->next;

		// Check if sample is playing
		cur->lpdsBuffer->lpVtbl->GetStatus ( cur->lpdsBuffer, &bufStatus );

		if ( !(bufStatus & DSBSTATUS_PLAYING) && !(bufStatus & DSBSTATUS_LOOPING) )
		{
			RemoveBufSample( cur );

			if ( bufferList.numEntries == 0 )
				return;
		}
	}
}


/*	--------------------------------------------------------------------------------
	Function 	: AddAmbientSound
	Purpose 	: Create an ambient sound
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
AMBIENT_SOUND *AddAmbientSound(SAMPLE *sample, SVECTOR *pos, long radius, short vol, short pitch, int freq, int randFreq, MDX_ACTOR *follow )
{
	AMBIENT_SOUND *ptr;
	AMBIENT_SOUND *ambientSound;

	if( !sample ) return NULL;

	ambientSound = (AMBIENT_SOUND *)MALLOC0(sizeof(AMBIENT_SOUND));
	ptr = &ambientSoundList.head;

	if( pos ) SetVectorSS( &ambientSound->pos, pos );
	ambientSound->follow = follow;

	ambientSound->volume = vol;
	ambientSound->sample = sample;
	ambientSound->pitch = pitch;
	ambientSound->radius = radius;

	ambientSound->freq = freq*60;
	ambientSound->randFreq = randFreq*60;

	ambientSound->prev = ptr;
	ambientSound->next = ptr->next;
	ptr->next->prev = ambientSound;
	ptr->next = ambientSound;
	ambientSoundList.numEntries++;

	return ambientSound;
}


/*	--------------------------------------------------------------------------------
	Function 	: UpdateAmbientSounds
	Purpose 	: Start sound effect if close enough, if timeout, etc
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void UpdateAmbientSounds()
{
	static int vol = 20000;
	AMBIENT_SOUND *amb,*amb2;
	SVECTOR *pos;

	// Update each ambient in turn
	for( amb = ambientSoundList.head.next; amb != &ambientSoundList.head; amb = amb2 )
	{
		amb2 = amb->next;

		// Timeout, so play a new sound
		if( actFrameCount < amb->counter )
			continue;

		// If it is attached to a platform, make ambient follow that platform
		if( amb->follow )
		{
			SetVectorSR( &amb->pos, &amb->follow->pos );
			ScaleVector( &amb->pos, 10 );
		}

		// If sound doesn't have a source
		if( !(amb->pos.vx || amb->pos.vy || amb->pos.vz) )
			pos = NULL;
		else
			pos = &amb->pos;

		if( (amb->sample->flags & SFXFLAGS_LOOP) && amb->lpdsBuffer )
			UpdateLoopingSample( amb );
		else
			amb->lpdsBuffer = (LPDIRECTSOUNDBUFFER)PlaySample( amb->sample, &amb->pos, amb->radius, amb->volume, amb->pitch );

		// Freq and randFreq are cunningly pre-multiplied by 60
		amb->counter = actFrameCount + amb->freq + ((amb->randFreq)?Random(amb->randFreq):0);
	}

/*
	if (KEYPRESS(DIK_NUMPADPLUS))
	{
		vol += gameSpeed>>4;
		SetCDVolume(vol);
	}
	else if (KEYPRESS(DIK_NUMPADMINUS))
	{
		vol -= gameSpeed>>4;
		SetCDVolume(vol);
	}
*/
}


/*	--------------------------------------------------------------------------------
	FUNCTION:	PauseAudio
	PURPOSE:	Stop all sounds when in pause mode
	PARAMETERS:	
	RETURNS:	
	INFO:		
*/
void PauseAudio( )
{
	BUFSAMPLE *s;
	AMBIENT_SOUND *a;
	unsigned long stat;

	for( s=bufferList.head.next; s!=&bufferList.head; s=s->next )
	{
		s->lpdsBuffer->lpVtbl->GetStatus( s->lpdsBuffer, &stat );

		if( stat & DSBSTATUS_PLAYING )
			s->lpdsBuffer->lpVtbl->Stop( s->lpdsBuffer );
	}

	for( a=ambientSoundList.head.next; a!=&ambientSoundList.head; a=a->next )
		a->lpdsBuffer = NULL;

	if( mciDevice )
		mciSendCommand(mciDevice, MCI_STOP, 0, (DWORD)NULL);
}


/*	--------------------------------------------------------------------------------
	FUNCTION:	UnPauseAudio
	PURPOSE:	Resume playing sounds
	PARAMETERS:	
	RETURNS:	
	INFO:		
*/
void UnPauseAudio( )
{
	BUFSAMPLE *s;
	AMBIENT_SOUND *a;
	unsigned long stat;

	for( s=bufferList.head.next; s!=&bufferList.head; s=s->next )
	{
		s->lpdsBuffer->lpVtbl->GetStatus( s->lpdsBuffer, &stat );

		if( stat & DSBSTATUS_PLAYING )
			s->lpdsBuffer->lpVtbl->Play( s->lpdsBuffer, 0, 0, 0 );
	}

	for( a=ambientSoundList.head.next; a!=&ambientSoundList.head; a=a->next )
		a->lpdsBuffer = NULL;

	if( mciDevice )
	{
		MCI_PLAY_PARMS mciPlayParms;

		mciPlayParms.dwCallback = (DWORD)mdxWinInfo.hWndMain;
		mciSendCommand(mciDevice, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciPlayParms);
	}
}


/*	--------------------------------------------------------------------------------
	FUNCTION:	InitCDaudio
	PURPOSE:	Open the CD audio device
	PARAMETERS:	
	RETURNS:	0/1
	INFO:		
*/
int InitCDaudio()
{
	int					noofmixers, l;
	MIXERCAPS			mixcaps;
	MIXERLINE			mixline;
	MIXERLINECONTROLS	linecontrols;
	DWORD				linetype, lineid;
	MIXERCONTROL		mixercontrol;

	utilPrintf("\n");
	noofmixers = mixerGetNumDevs();
	utilPrintf("Found %d mixer devices\n", noofmixers);
	utilPrintf("\n");

	lineid = 0;
	volumecontrolid = 0;
	cdaudiovalid = 0;
	for(l = 0; l < noofmixers; l++)
		{
		mixerGetDevCaps(l, &mixcaps, sizeof(MIXERCAPS));
		utilPrintf("  Mixer name : '%s'\n", mixcaps.szPname);
		utilPrintf("  #Destinations : %d\n", mixcaps.cDestinations);

		mixline.cbStruct = sizeof(MIXERLINE);
		mixline.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;
		if (mixerGetLineInfo((HMIXEROBJ)l, &mixline, MIXER_GETLINEINFOF_COMPONENTTYPE | MIXER_OBJECTF_MIXER) == MMSYSERR_NOERROR)
			{
			utilPrintf("    _COMPACTDISC\n");
			utilPrintf("    #Channels : %d\n", mixline.cChannels);
			utilPrintf("    Line ID : %d\n", mixline.dwLineID);
			utilPrintf("    Short name : %s\n", mixline.szShortName);
			utilPrintf("    Name : %s\n", mixline.szName);
			utilPrintf("    Product name : %s\n", mixline.Target.szPname);
			linetype = MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;
			lineid = mixline.dwLineID;
			mixerid = l;
			break;
			}
		else
			utilPrintf("No _COMPACTDISC line found\n");

		mixline.cbStruct = sizeof(MIXERLINE);
		mixline.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_DIGITAL;
		if (mixerGetLineInfo((HMIXEROBJ)l, &mixline, MIXER_GETLINEINFOF_COMPONENTTYPE | MIXER_OBJECTF_MIXER) == MMSYSERR_NOERROR)
			{
			utilPrintf("    _DIGITAL\n");
			utilPrintf("    #Channels : %d\n", mixline.cChannels);
			utilPrintf("    Line ID : %d\n", mixline.dwLineID);
			utilPrintf("    Short name : %s\n", mixline.szShortName);
			utilPrintf("    Name : %s\n", mixline.szName);
			utilPrintf("    Product name : %s\n", mixline.Target.szPname);
			linetype = MIXERLINE_COMPONENTTYPE_SRC_DIGITAL;
			lineid = mixline.dwLineID;
			mixerid = l;
			break;
			}
		else
			utilPrintf("No _DIGITAL line found\n");

		mixline.cbStruct = sizeof(MIXERLINE);
		mixline.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_AUXILIARY;
		if (mixerGetLineInfo((HMIXEROBJ)l, &mixline, MIXER_GETLINEINFOF_COMPONENTTYPE | MIXER_OBJECTF_MIXER) == MMSYSERR_NOERROR)
			{
			utilPrintf("    _AUXILIARY\n");
			utilPrintf("    #Channels : %d\n", mixline.cChannels);
			utilPrintf("    Line ID : %d\n", mixline.dwLineID);
			utilPrintf("    Short name : %s\n", mixline.szShortName);
			utilPrintf("    Name : %s\n", mixline.szName);
			utilPrintf("    Product name : %s\n", mixline.Target.szPname);
			linetype = MIXERLINE_COMPONENTTYPE_SRC_AUXILIARY;
			lineid = mixline.dwLineID;
			mixerid = l;
			break;
			}
		else
			utilPrintf("No _AUXILIARY line found\n");
		}

	if (l != noofmixers)
		{
		mixline.cbStruct = sizeof(MIXERLINE);
		mixline.dwComponentType = linetype;
		mixerGetLineInfo((HMIXEROBJ)mixerid, &mixline, MIXER_GETLINEINFOF_COMPONENTTYPE | MIXER_OBJECTF_MIXER);

		mixercontrol.cbStruct = sizeof(MIXERCONTROL);
		linecontrols.cbStruct = sizeof(MIXERLINECONTROLS);
		linecontrols.dwLineID = lineid;
		linecontrols.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
		linecontrols.cControls = 1;
		linecontrols.cbmxctrl = sizeof(MIXERCONTROL);
		linecontrols.pamxctrl = &mixercontrol;
		if (mixerGetLineControls((HMIXEROBJ)mixerid, &linecontrols, MIXER_GETLINECONTROLSF_ONEBYTYPE | MIXER_OBJECTF_MIXER) == MMSYSERR_NOERROR)
			{
			utilPrintf("      Control name : '%s' / '%s'\n", mixercontrol.szShortName, mixercontrol.szName);
			utilPrintf("      Control range : %u -> %u\n", mixercontrol.Bounds.dwMinimum, mixercontrol.Bounds.dwMaximum);
			utilPrintf("      Control ID : %u\n", mixercontrol.dwControlID);

			cdaudiovalid = 1;
			volumecontrolid = mixercontrol.dwControlID;

			utilPrintf("        Current CD volume = %d\n", GetCDVolume()); 
			}
		else
			utilPrintf("mixerGetLineControls failed\n");
		}

	oldVolume = auxVolume = GetCDVolume();
	return 0;
}

/*	--------------------------------------------------------------------------------
	FUNCTION:	ShutdownCDaudio
	PURPOSE:	Shutdown the CD audio device
	PARAMETERS:	
	RETURNS:	0/1
	INFO:		
*/

int ShutdownCDaudio()
{
	StopSong( );
	SetCDVolume(auxVolume);
	return 0;
}

/*	--------------------------------------------------------------------------------
	Function		: PrepareSong
	Purpose			: prepares a song...and plays it...woohoo !
	Parameters		: char
	Returns			: void
	Info			: 
*/
void PrepareSong(short world, short loop)
{
	int track;

#ifdef PC_DEMO
	return;
#endif

	switch (world)
	{
	case WORLDID_GARDEN:		track = GARDEN_CDAUDIO; break;
	case WORLDID_ANCIENT:		track = ANCIENTS_CDAUDIO; break;
	case WORLDID_SPACE:			track = SPACE_CDAUDIO; break;
	case WORLDID_CITY:			track = SUPERRETRO_CDAUDIO; break;
	case WORLDID_SUBTERRANEAN:	track = SUBTERRANEAN_CDAUDIO; break;
	case WORLDID_LABORATORY:	track = LABORATORY_CDAUDIO; break;
	case WORLDID_HALLOWEEN:		track = HALLOWEEN_CDAUDIO; break;
	case WORLDID_SUPERRETRO:	track = SUPERRETRO_CDAUDIO; break;
	case WORLDID_FRONTEND:		track = FRONTEND_CDAUDIO; break;

	case AUDIOTRK_GAMEOVER:		track = GAMEOVER_CDAUDIO; break;
	case AUDIOTRK_LEVELCOMPLETE:track = LEVELCOMPLETE_CDAUDIO; break;
	case AUDIOTRK_LEVELCOMPLETELOOP:track = LEVELCOMPLETELOOP_CDAUDIO; break;

	default:
		return;
	}
	// play cd audio track here....
	playCDTrack ( mdxWinInfo.hWndMain, track);
	
	if (loop)
		cdTrack = track;
	else
		cdTrack = -1;	// playing but not looping
}


void LoopSong()
{
	if (cdTrack>0)
		playCDTrack(mdxWinInfo.hWndMain, cdTrack);
	else
		cdTrack = 0;	// CD is not playing
}


void StopSong( )
{
	stopCDTrack( mdxWinInfo.hWndMain );
	cdTrack = 0;
}


int IsSongPlaying()
{
	return (cdTrack != 0);
}

DWORD stopCDTrack ( HWND hWndNotify )
{
	MCI_GENERIC_PARMS parms;

	// Stop
	mciSendCommand(mciDevice, MCI_STOP, MCI_NOTIFY, (DWORD)(LPMCI_GENERIC_PARMS)&parms);

	// Close device
	mciSendCommand(mciDevice, MCI_CLOSE, MCI_NOTIFY, (DWORD)(LPMCI_GENERIC_PARMS)&parms);

	mciDevice = 0;

	return TRUE;
}

int pauseCDTrack()
{
    return mciSendCommand(mciDevice, MCI_STOP, 0, 0);
}

// Plays a specified audio track using MCI_OPEN, MCI_PLAY. Returns as 
// soon as playback begins. The window procedure function for the 
// specified window will be notified when playback is complete. 
// Returns 0L on success; otherwise, returns an MCI error code.
DWORD playCDTrack ( HWND hWndNotify, BYTE bTrack )
{
    UINT wDeviceID;
    DWORD dwReturn;
    MCI_OPEN_PARMS mciOpenParms;
    MCI_SET_PARMS mciSetParms;
    MCI_PLAY_PARMS mciPlayParms;

	if( !mciDevice )
	{
		// Open the CD audio device by specifying the device name.
		mciOpenParms.lpstrDeviceType = "cdaudio";

		if ( dwReturn = mciSendCommand( 0, MCI_OPEN, MCI_OPEN_TYPE, ( DWORD ) ( LPVOID ) &mciOpenParms ) )
		{
			if( dwReturn == MCIERR_MUST_USE_SHAREABLE )
			{
				utilPrintf("Failed to open device %s, trying shareable...\n", mciOpenParms.lpstrDeviceType );
				if ( dwReturn = mciSendCommand( 0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_SHAREABLE, ( DWORD ) ( LPVOID ) &mciOpenParms ) )
				{
					utilPrintf("Failed to open device %s\n",mciOpenParms.lpstrDeviceType);
					// Failed to open device. Don't close it; just return error.
					return dwReturn;
				}
			}
		}

		// The device opened successfully; get the device ID.
		wDeviceID = mciOpenParms.wDeviceID;

		// Set the time format to track/minute/second/frame (TMSF).
		mciSetParms.dwTimeFormat = MCI_FORMAT_TMSF;

		if (dwReturn = mciSendCommand ( wDeviceID, MCI_SET,MCI_SET_TIME_FORMAT, ( DWORD )  ( LPVOID ) &mciSetParms ) )
		{
			utilPrintf("Failed to set time format for cd device\n");

			mciSendCommand( wDeviceID, MCI_CLOSE, 0, 0 );
			return dwReturn;
		}
	}
	else // Repeat performance
	{
		wDeviceID = mciDevice;
	}
	
	// Begin playback from the given track and play until the beginning 
    // of the next track. 
    mciPlayParms.dwTo		= 0L;
    mciPlayParms.dwFrom		= MCI_MAKE_TMSF ( bTrack, 0, 0, 0 );
    mciPlayParms.dwTo		= MCI_MAKE_TMSF ( bTrack + 1, 0, 0, 0 );
    mciPlayParms.dwCallback = ( DWORD ) hWndNotify;
    if ( dwReturn = mciSendCommand ( wDeviceID, MCI_PLAY,MCI_FROM | MCI_TO | MCI_NOTIFY, (DWORD)(LPVOID) &mciPlayParms ) )
    {
		utilPrintf("Couldn't play cd track %i\n",bTrack);

        mciSendCommand ( wDeviceID, MCI_CLOSE, 0, 0 );
        return dwReturn;
    }

	mciDevice = wDeviceID;

    return 0L;
}


/*	--------------------------------------------------------------------------------
	Function : GetCDVolume
	Purpose : Return the current CD volume setting
	Parameters :	
	Returns : volume (0..0xffff)
	Info :
*/

int GetCDVolume()
{
	MIXERCONTROLDETAILS	controldetails;
	DWORD				details[2];
	MMRESULT			mmres;

	if (!cdaudiovalid)
		return 0;

	controldetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
	controldetails.dwControlID = volumecontrolid;
	controldetails.cChannels = 1;
	controldetails.cMultipleItems = 0;
	controldetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	controldetails.paDetails = (void*)details;
	mmres = mixerGetControlDetails((HMIXEROBJ)mixerid, &controldetails, MIXER_GETCONTROLDETAILSF_VALUE | MIXER_OBJECTF_MIXER);
	if (mmres == MMSYSERR_NOERROR)
		return ((int)details[0]);
	else
		{
		utilPrintf("mixerGetControlDetails failed\n");
		switch(mmres)
			{
			case MIXERR_INVALCONTROL:
				utilPrintf("MIXERR_INVALCONTROL\n");
				break;
			case MMSYSERR_BADDEVICEID:
				utilPrintf("MMSYSERR_BADDEVICEID\n");
				break;
			case MMSYSERR_INVALFLAG:
				utilPrintf("MMSYSERR_INVALFLAG\n");
				break;
			case MMSYSERR_INVALHANDLE:
				utilPrintf("MMSYSERR_INVALHANDLE\n");
				break;
			case MMSYSERR_INVALPARAM:
				utilPrintf("MMSYSERR_INVALPARAM\n");
				break;
			case MMSYSERR_NODRIVER:
				utilPrintf("MMSYSERR_NODRIVER\n");
				break;
			default:
				utilPrintf("UNKNOWN\n");
				break;
			}
		}
	return 0;
}


/*	--------------------------------------------------------------------------------
	Function : SetCDVolume
	Purpose : Set the CD volume
	Parameters : volume (0..0xffff)
	Returns : 
	Info :
*/

void SetCDVolume(int vol)
{
	MIXERCONTROLDETAILS	controldetails;
	MMRESULT			mmres;
	DWORD				details[2];

	if (!cdaudiovalid)
		return;
/*
	if ((vol == 0) && (oldVolume))
		cdplCDaudio();
	else if ((vol != 0) && (oldVolume == 0))
		PlayCDaudio(currTrack);
*/

	if (vol<0) vol=0;
	oldVolume = vol;

	details[0] = vol;
	controldetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
	controldetails.dwControlID = volumecontrolid;
	controldetails.cChannels = 1;
	controldetails.cMultipleItems = 0;
	controldetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	controldetails.paDetails = (void*)details;
	mmres = mixerSetControlDetails((HMIXEROBJ)mixerid, &controldetails, MIXER_SETCONTROLDETAILSF_VALUE | MIXER_OBJECTF_MIXER);
	if (mmres != MMSYSERR_NOERROR)
		{
		utilPrintf("** mixerSetControlDetails failed\n");
		switch(mmres)
			{
			case MIXERR_INVALCONTROL:
				utilPrintf("MIXERR_INVALCONTROL\n");
				break;
			case MMSYSERR_BADDEVICEID:
				utilPrintf("MMSYSERR_BADDEVICEID\n");
				break;
			case MMSYSERR_INVALFLAG:
				utilPrintf("MMSYSERR_INVALFLAG\n");
				break;
			case MMSYSERR_INVALHANDLE:
				utilPrintf("MMSYSERR_INVALHANDLE\n");
				break;
			case MMSYSERR_INVALPARAM:
				utilPrintf("MMSYSERR_INVALPARAM\n");
				break;
			case MMSYSERR_NODRIVER:
				utilPrintf("MMSYSERR_NODRIVER\n");
				break;
			default:
				utilPrintf("UNKNOWN\n");
				break;
			}
		}
}

/*	--------------------------------------------------------------------------------
	Function		: FindSample
	Purpose			: Search the list for the required sound
	Parameters		: ID
	Returns			: Sample or NULL
	Info			: 
*/
SAMPLE *FindSample( unsigned long uid )
{
	SAMPLE *cur;

	for( cur = soundList.head.next; cur != &soundList.head; cur = cur->next )
		if( cur->uid == uid )
			return cur;

	return NULL;
}

SAMPLE *FindVoice( unsigned long uid, int pl )
{
	SAMPLE *cur;

	for( cur = voices[pl].next; cur != &voices[pl]; cur = cur->next )
		if( cur->uid == uid )
			return cur;

	return NULL;
}

/*	--------------------------------------------------------------------------------
	Function		: DULL LIST FUNCTIONS
	Purpose			: Init, Add, Sub, Free etc...
	Parameters		: 
	Returns			: 
	Info			: 
*/
void InitSampleList( )
{
	//Init the sample list for the real samples.
	soundList.numEntries	= 0;
	soundList.head.next		= soundList.head.prev = &soundList.head;

	voices[0].next = voices[0].prev = &voices[0];
	voices[1].next = voices[1].prev = &voices[1];
	voices[2].next = voices[2].prev = &voices[2];
	voices[3].next = voices[3].prev = &voices[3];

	// Init the buffer list for samples that are playing
	bufferList.numEntries	= 0;
	bufferList.head.next	= bufferList.head.prev = &bufferList.head;

	// Setup animation callback
	StartAnim = PlaySfxMappedSample;

	sfx_anim_map = NULL;
}


void InitAmbientSoundList()
{
	ambientSoundList.head.next = ambientSoundList.head.prev = &ambientSoundList.head;
	ambientSoundList.numEntries = 0;
}


void AddSample( SAMPLE *sample )
{
	if( sample && !sample->next )
	{
		soundList.numEntries++;
		sample->prev				= &soundList.head;
		sample->next				= soundList.head.next;
		soundList.head.next->prev	= sample;
		soundList.head.next			= sample;
	}
}


void AddBufSample( BUFSAMPLE *bufSample )
{
	if( bufSample && !bufSample->next )
	{
		bufferList.numEntries++;
		bufSample->prev				= &bufferList.head;
		bufSample->next				= bufferList.head.next;
		bufferList.head.next->prev	= bufSample;
		bufferList.head.next		= bufSample;
	}
}


void RemoveSample( SAMPLE *sample )
{
	if( !sample->next )
		return;

	sample->prev->next	= sample->next;
	sample->next->prev	= sample->prev;
	sample->next		= NULL;

	if( sample->lpds3DBuffer )
		sample->lpds3DBuffer->lpVtbl->Release(sample->lpds3DBuffer);

	if( sample->lpdsBuffer )
		sample->lpdsBuffer->lpVtbl->Release(sample->lpdsBuffer);

//	if( sample->idName )
//		FREE( sample->idName );

	if( sample->data )
		FREE( sample->data );

	FREE( sample );
}


void RemoveBufSample( BUFSAMPLE *bufSample )
{
	if( !bufSample->next )
		return;

	bufSample->prev->next	= bufSample->next;
	bufSample->next->prev	= bufSample->prev;
	bufSample->next		= NULL;
	bufferList.numEntries--;

	if( bufSample->lpdsBuffer )
		bufSample->lpdsBuffer->lpVtbl->Release(bufSample->lpdsBuffer);

	FREE( bufSample );
}


void FreeSampleList( void )
{
	unsigned long i;

	if( sfx_anim_map ) 
	{
		FREE( sfx_anim_map );
		sfx_anim_map = NULL;
	}

	FreeBufSampleList( );

	utilPrintf("Freeing linked list : SAMPLE : (%d elements)\n",soundList.numEntries);

	// traverse enemy list and free elements
	while( soundList.head.next && soundList.head.next != &soundList.head )
	{
		StopSample( soundList.head.next );
		RemoveSample( soundList.head.next );
		soundList.numEntries--;
	}

	for( i=0; i<NUM_FROGS; i++ )
		while( voices[i].next && voices[i].next != &voices[i] )
		{
			StopSample( voices[i].next );
			RemoveSample( voices[i].next );
		}

	for( i=0; i<NUM_GENERIC_SFX; i++ )
		genSfx[i] = NULL;

	dispSample = NULL;

	// initialise list for future use
	InitSampleList();
}


void FreeBufSampleList ( void )
{
	unsigned long stat;

	utilPrintf("Freeing linked list : BUFSAMPLE : (%d elements)\n",bufferList.numEntries);

	// traverse enemy list and free elements
	while( bufferList.head.next && bufferList.head.next != &bufferList.head )
	{
		bufferList.head.next->lpdsBuffer->lpVtbl->GetStatus( bufferList.head.next->lpdsBuffer, &stat );
		if( stat & DSBSTATUS_PLAYING )
			bufferList.head.next->lpdsBuffer->lpVtbl->Stop( bufferList.head.next->lpdsBuffer );

		RemoveBufSample( bufferList.head.next );
	}
}


void SubAmbientSound(AMBIENT_SOUND *ambientSound)
{
	if( !ambientSound->next )
		return;

	ambientSound->prev->next = ambientSound->next;
	ambientSound->next->prev = ambientSound->prev;
	ambientSoundList.numEntries--;

	FREE( ambientSound );
}


void FreeAmbientSoundList( )
{
	BUFSAMPLE *bufs;

	// traverse enemy list and free elements
	while( ambientSoundList.head.next && ambientSoundList.head.next != &ambientSoundList.head )
	{
		if( ambientSoundList.head.next->sample && ambientSoundList.head.next->lpdsBuffer )
			ambientSoundList.head.next->lpdsBuffer->lpVtbl->Stop( ambientSoundList.head.next->lpdsBuffer );

		SubAmbientSound( ambientSoundList.head.next );
	}

	// initialise list for future use
	InitAmbientSoundList();

	// eat up looping, buffered samples - ds
	for (bufs=bufferList.head.next; bufs!=&bufferList.head; bufs=bufs->next)
	{
		DWORD stat;
		bufs->lpdsBuffer->lpVtbl->GetStatus( bufs->lpdsBuffer, &stat );
		if( stat & DSBSTATUS_PLAYING && stat & DSBSTATUS_LOOPING )
		{
			bufs->lpdsBuffer->lpVtbl->Stop( bufs->lpdsBuffer );
			//RemoveBufSample( bufferList.head.next );
		}
	}

	CleanBufferSamples();
}



/*	--------------------------------------------------------------------------------
	Function 	: FindSfxMapping
	Purpose 	: Load a mapping of animations to sound effects for a list of actors
	Parameters 	: world and level ID's
	Returns 	: Pointer to mapping array
	Info 		: 
*/
void FindSfxMapping( unsigned long uid, ACTOR *actor )
{
	MDX_ACTOR *act = (MDX_ACTOR *)actor->actualActor;
	unsigned long actID, type, index=0, i;

	// Clear all loopflags
	for( i=0; i<NUM_NME_ANIMS; i++ )
		act->animation->loopFlags[i] = 0;

	// First actor uid - if none, return NULL
	if( !sfx_anim_map || !uid || !(actID = (unsigned long)sfx_anim_map[index++]) )
	{
		act->animation->sfxMapping = NULL;
		return;
	}

	do
	{
		// Advance cursor by number depending on actor type
		type = (unsigned long)sfx_anim_map[index++];

		if( actID == uid )
		{
			if( type == 3 )
			{
				// Make an ambient sound if we've attached a sound to a scenic
				AddAmbientSound( sfx_anim_map[index], &actor->position, 12000, AMBIENT_VOLUME, -1/*128*/, 0, 0, act );
				act->animation->sfxMapping = NULL;
				return;
			}
			else
			{
				// Return a run of samples if we've found the actor
				act->animation->sfxMapping = (void **)&sfx_anim_map[index];
				return;
			}
		}

		switch( type )
		{
		case 0: index += NUM_FROG_ANIMS; break;
		case 1: index += MULTI_NUM_ANIMS; break;
		case 2: index += NUM_NME_ANIMS; break;
		case 3: index ++; break; // Number of scenic animations
		}

		// Get next actor uid and stop if end of list
		actID = (unsigned long)sfx_anim_map[index++];

	} while( actID );

	// Not found owt
	act->animation->sfxMapping = NULL;
}
