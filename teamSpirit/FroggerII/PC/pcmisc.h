/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: pcmisc.h
	Programmer	: Andy Eder
	Date		: 05/05/99 17:43:09

----------------------------------------------------------------------------------------------- */

#ifndef PCMISC_H_INCLUDED
#define PCMISC_H_INCLUDED


#ifdef __cplusplus
extern "C"
{
#endif


#define COLLISION_BASE	"collision\\"
#define SCENIC_BASE		"scenic\\"
#define TEXTURE_BASE	"textures\\hardware\\"
#define OBJECT_BASE		"objects\\"

extern char *baseDirectory;

//----- [ DATA STRUCTURES ] --------------------------------------------------------------------//

typedef struct TAGWININFO
{
	HWND		hWndMain;						// main window handle
	HANDLE		hInstance;						// our instance handle

} WININFO;

//----- [ GLOBALS ] ----------------------------------------------------------------------------//

extern WININFO winInfo;

extern short mouseX;
extern short mouseY;

//----- [ FUNCTION PROTOTYPES ] ----------------------------------------------------------------//

extern void InitPCSpecifics();
extern void DeInitPCSpecifics();
extern short *GetGelfBmpDataAsShortPtr(char *filename);

#ifdef __cplusplus
}
#endif


#endif
