#ifndef _MAIN_H_INCLUDE
#define _MAIN_H_INCLUDE

#include <math.h>
#include <windows.h>
#include "fixed.h"
#include "sonylibs.h"
#include <malloc.h>

#define COLLISION_BASE	"collision\\"
#define SCENIC_BASE		"scenics\\"
#define OBJECT_BASE		"objects\\"
#define TEXTURE_BASE	"textures\\"
#define ENTITY_BASE		"maps\\"
#define SAVEDATA_BASE	"savedata\\"
#define SFX_BASE		"sfx\\"

//#ifndef __cplusplus
//#define sinf(x)			((float)sin(x))
//#define cosf(x)			((float)cos(x))
//#define sqrtf(x)		((float)sqrt(x))
//#endif


//#define MALLOC0(S)	memoryAllocateZero(S, __FILE__, __LINE__)
#define MALLOC0(S)	calloc((S),1)




typedef struct TAGWININFO
{
	HWND	hWndMain;
	HINSTANCE	hInstance;

} WININFO;




extern GsRVIEW2	camera;

extern fixed gameSpeed;
extern unsigned long actFrameCount, lastActFrameCount;
extern char UseAAMode;
extern char UseZMode;



extern long winMode;
extern WININFO winInfo;
//mm extern BYTE lButton, rButton;
extern u8 lButton, rButton;
extern char baseDirectory[MAX_PATH];
extern char outputMessageBuffer[256];
extern unsigned long actFrameCount, lastActFrameCount;
extern unsigned long currentFrameTime;
extern long keyInput;

extern char	transparentSurf;
extern char	xluSurf;
extern char	aiSurf;
extern char	UseAAMode;
extern char	UseZMode;
extern char	desiredFrameRate;
extern char	newDesiredFrameRate;
extern int runQuit;
extern fixed gameSpeed;

void debugPrintf(int num);
void Crash(char *mess);

int InitialiseWindows(HINSTANCE hInstance,int nCmdShow);
long FAR PASCAL WindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

void DrawGraphics();

#endif
