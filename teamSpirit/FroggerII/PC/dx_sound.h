/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.


	File		: dx_sound.h
	Programmer	: James Healey
	Date		: 16/08/99

----------------------------------------------------------------------------------------------- */

#ifndef DSOUND_H_INCLUDED
#define DSOUND_H_INCLUDED



#ifdef __cplusplus
extern "C"
{
#endif

extern LPDIRECTSOUND           lpDS;

//extern int Makebuffer ( SAMPLE *sample );
extern int LoadWav ( LPSTR lpFile, SAMPLE *sample );

extern int InitDirectSound ( LPGUID guid, HINSTANCE hInst,  HWND hWndMain, int prim );
extern BOOL CALLBACK DSEnumDlgProc( HWND , UINT , WPARAM , LPARAM );
extern int DSoundEnumerate ( LPGUID lpGUID, HINSTANCE hInst,  HWND hWndMain );

#ifdef __cplusplus
}
#endif

#endif