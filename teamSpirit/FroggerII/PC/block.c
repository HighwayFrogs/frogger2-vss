/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: block.c
	Programmer	: Andy Eder
	Date		: 28/06/99 10:19:37

----------------------------------------------------------------------------------------------- */

#include <ultra64.h>
#include "incs.h"
#include "editor.h"
#include <crtdbg.h>
#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <wtypes.h>
#include <crtdbg.h>
#include <commctrl.h>
#include <cguid.h>
#include <dplay.h>
#include <dplobby.h>
#include "..\resource.h"
#include "network.h"
#include "netchat.h"
#include "software.h"
#include "mavis.h"

void AnimateTexturePointers(void);

#define DEBUG_FILE "C:\\frogger2.log"

unsigned long actFrameCount,
			lastActFrameCount = 0,
			currentFrameTime,
			actTickCount;

unsigned long actTickCountModifier = 0;
unsigned long modFactor = 100;

unsigned long speedKill = 0;
float gameSpeed = 1;
char dprintbuf[255] = "---";

HBITMAP appBackgnd;

WININFO winInfo;
BYTE lButton = 0, rButton = 0;
int runQuit = 0;
long totalFacesDrawn;
extern long numFacesDrawn;
extern long numPixelsDrawn;
extern long runHardware;
extern unsigned long USE_MENUS;
long keyInput = 1;

extern long displayingTile;

long winMode = 1;
long scaleMode = 0;

char baseDirectory[MAX_PATH] = "x:\\teamspirit\\pcversion\\";
char editorOk = 0;
long drawTimers = 0;
float keyDelay;

char outputMessageBuffer[256];

char	transparentSurf		= 0;
char	xluSurf				= 0;		
char	aiSurf				= 0;
char	UseAAMode			= 0;
char	UseZMode			= 1;

char	desiredFrameRate	= 2;
char	newDesiredFrameRate = 2;

int		appActive		= 0;

#define REGISTRY_KEY "Software\\Atari\\Frogger2"

static GUID     guID;
extern TEXTURE *frogEyeOpen,*frogEyeClosed;

/*	--------------------------------------------------------------------------------
	Function		: debugPrintf(int num)
	Purpose			: prints debug information to the file server output window
	Parameters		:
	Returns			: none
	Info			:
*/
void debugPrintf(int num)
{
	FILE *f;

//	_CrtDbgReport(_CRT_WARN,NULL,NULL,"FroggerII",outputMessageBuffer);

	f = fopen(DEBUG_FILE, "a+");
	if (f)
	{
		fputs(outputMessageBuffer, f);
		fclose(f);
	}
}

/*	--------------------------------------------------------------------------------
	Function		: Crash(char *mess)
	Purpose			: prints a message in file server output window if system crash
	Parameters		:
	Returns			: none
	Info			:
*/
void Crash(char *mess)
{
	dprintf"\n\nCRASHED %s!!!!!!!!!!!!!!!\n",mess));
}


/*	--------------------------------------------------------------------------------
	Function		: GetRegistryInformation(void)
	Parameters		: 
	Returns			: int
	Info			: Gets setup stuff (e.g. install dir) from the registry
*/

int GetRegistryInformation(void)
{
	HKEY hkey;
	DWORD len = MAX_PATH;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGISTRY_KEY, 0, KEY_READ, &hkey) != ERROR_SUCCESS)
	{
		dprintf"Couldn't open registry key\n")); return 0;
	}
	else
	{
		if (RegQueryValueEx(hkey, "InstallDir", NULL, NULL, baseDirectory, &len) == ERROR_SUCCESS)
		{
			if (baseDirectory[strlen(baseDirectory) - 1] != '\\')
				strcat(baseDirectory, "\\");
		}
		else
			dprintf"Couldn't read InstallDir value from registry\n"));

		RegCloseKey(hkey);
	}

	return 1;
}

/*	--------------------------------------------------------------------------------
	Function		: SaveRegistryInformation(void)
	Parameters		: 
	Returns			: int
	Info			: Puts some stuff in the registry
*/

int SaveRegistryInformation(void)
{
	HKEY hkey;
	DWORD len = MAX_PATH;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGISTRY_KEY, 0, KEY_WRITE, &hkey) != ERROR_SUCCESS)
	{
		dprintf"Couldn't open registry key\n")); return 0;
	}
	else
	{
		//RegSetValueEx(hkey, "DeviceGUID", 0, REG_SZ, guidStr, len);
		
		RegCloseKey(hkey);
	}

	return 1;
}

void GetArgs(char *arglist)
{
	int i;
	for (i=0; arglist[i]!=0; i++)
	{
		if ((arglist[i]=='+') || (arglist[i]=='-') || (arglist[i]=='/'))
		{
			switch(arglist[i+1])
			{
				case 'W':
				case 'w':
					winMode = 1;
					break;
				case 'S':
				case 's':
					scaleMode = 1;
					break;
				case 'C':
				case 'c':
					swingCam = 0;
					break;
				case 'M':
				case 'm':
					USE_MENUS = 1;
					break;

			}
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function		: WinMain
	Purpose			: entry point
	Parameters		: 
	Returns			: int
	Info			: 
*/
int PASCAL WinMain2(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow);
int PASCAL WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	winMode = 0;
	scaleMode = 0;
	GetArgs(lpCmdLine);
//	__try
//	{
		WinMain2(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
//	} 
/*	__except (1) 
	{
		dprintf"--------------------------------------------------------\n"));
		dprintf"- *** Unhandled exception ***                          -\n"));
		dprintf"--------------------------------------------------------\n"));
		///////////////////////////
		
	//	_exception_info();

		///////////////////////////
		dprintf"--------------------------------------------------------\n"));

	}*/
}

long InitOneOverTable(void);
extern float camSideOfs;

int PASCAL WinMain2(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	SYSTEMTIME currTime;
	ULONG memSizeInBytes = 0;
	UBYTE *memPtr = NULL;
    MSG msg;
	FILE *fpp;
	int ok = 1;
	long i;
	long zero = 0;
	HRESULT	dsrVal, hRes;
	unsigned long newTickCount;
	SAMPLE *test;

	GetLocalTime(&currTime);
//	fpp = fopen("c:\frog.ini","rt");
//	if (fpp)
//	{
//		fgets(fpp,&baseDirectory);
//		fclose(fpp);
//	}
	
	dprintf"\n------------- Starting Frogger II ----------------\n"
		"Session started %02d/%02d/%d %02d:%02d:%02d\n",
		currTime.wDay, currTime.wMonth, currTime.wYear,
		currTime.wHour, currTime.wMinute, currTime.wSecond));

	GetRegistryInformation();
	// create and initialise app window
	if(!InitialiseWindows(hInstance,nCmdShow))
		ok = 0;

	// create area for memory Jalloc's
	// 16MB () for now - ANDYE
	memSizeInBytes = (16 * 1024 * 1024);
	dprintf"\nAllocating %lu bytes memory.....",memSizeInBytes));
	memPtr = (UBYTE *)malloc(memSizeInBytes);
	if(memPtr == NULL)
	{
		dprintf"ERROR !\n"));
		ok = 0;
	}
	else
	{
		JallocInit((long)memPtr,memSizeInBytes);
		dprintf"DONE !\n"));
	}

	// initialisation...
	dprintf"Init CRC table...\n"));
	InitCRCTable();
	dprintf"Init matrix stack...\n"));
	InitMatrixStack();

	if(!InitInputDevices())
		ok = 0;
	//FreeAllLists
	//InitSaveData();

	//PrepareSong ( 1 );
/*	InitSaveData();
	SaveGameData();*/
	gameState.mode		= INGAME_MODE;
	//frontEndState.mode	= TITLE_MODE;

	// initialise PC stuff and DirectX / Direct3D
	InitPCSpecifics();
	if(!DirectXInit(winInfo.hWndMain,1))
		ok = 0;
	if (runQuit)
		exit (0);

	InitSampleList();

	InitFont();
	InitEditor();
	
	gameState.mode		= FRONTEND_MODE;
	//frontEndState.mode	= DEMO_MODE;
	frameCount = 1;
	actTickCount = GetTickCount();

	lastActFrameCount = actFrameCount = (GetTickCount()/(1000.0/60.0));
	InitOneOverTable();	
	
	if (!runHardware)
		SoftwareInit(SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	if (!USE_MENUS)
	{
		FreeAllLists();
		InitLevel(WORLDID_FRONTEND,LEVELID_FRONTEND1);
	}
	else
	{
		gameState.mode = MENU_MODE;
		gameState.menuMode = TITLE_MODE;
	}

    while(ok)
	{
        while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				ok = 0;
				break;
			}

			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
		if(appActive)
		{
			if (KEYPRESS(DIK_F1))
				camDist.v[1]+=2*gameSpeed;

				
			if (KEYPRESS(DIK_F2))
				camDist.v[1]-=2*gameSpeed;

			/*if (KEYPRESS(DIK_F1))
				speedKill++;

			if (KEYPRESS(DIK_F2))
				if (speedKill>0)
					speedKill--;
*/
			if (KEYPRESS(DIK_F3))
			{
				if (KEYPRESS(DIK_LCONTROL))
					camSideOfs+=2*gameSpeed;
				else
					camDist.v[2]+=2*gameSpeed;
			}
		
			if (KEYPRESS(DIK_F4))
			{
				if (KEYPRESS(DIK_LCONTROL))
					camSideOfs-=2*gameSpeed;
				else
					camDist.v[2]-=2*gameSpeed;
			}

		
			if (KEYPRESS(DIK_F8))
			{
				if (KEYPRESS(DIK_LCONTROL))
					dumpScreen = 1;
				else
					displayingTile=!displayingTile;
			}

			if (keyDelay<1)
			{
				if (KEYPRESS(DIK_F9))
				{
					actTickCountModifier -= modFactor;
					keyDelay = 30;
				}
			
				if (KEYPRESS(DIK_F11))
				{
					modFactor *= 2;
					keyDelay = 30;
				}

				if( KEYPRESS(DIK_F7) && chatFlags )
				{
					if( chatFlags & CHAT_INPUT )
					{
						chatFlags &= ~CHAT_INPUT;
					}
					else
					{
						if( !chatInput.msg )
							chatInput.msg = (char *)JallocAlloc(MAX_CSLENGTH,YES,"string");

						chatInput.msgLen = 0;
						chatFlags |= CHAT_INPUT;
						keyDelay = 20;
					}
				}

//				if( KEYPRESS(DIK_F8) )
//				fog.mode = 1;//!(fog.mode);

				if( KEYPRESS(DIK_F10) )
				{
					editorOk = !editorOk; 
					keyDelay = 20;
				}
			
				if (KEYPRESS(DIK_F5))
				{
					if (drawTimers<3)
						drawTimers++;
					else
						drawTimers = 0;

					keyDelay = 20;
				}


			}
			else
				keyDelay-=gameSpeed;
			
			if (editorOk)
				RunEditor();

			if( gameState.multi == MULTIREMOTE && gameState.mode == GAME_MODE )
				RefreshMPFrogs( );

			GameLoop();

			keysEnabled = !(editorOk || (chatFlags & CHAT_INPUT));
			ProcessUserInput(winInfo.hWndMain);

			DrawGraphics();

			//if (speedKill)
			//	Sleep(speedKill);

			DirectXFlip();

			CleanBufferSamples();

			//Update3DListener ( currCamSource[0].v[X], currCamSource[0].v[Y], currCamSource[0].v[Z]);

			newTickCount = GetTickCount()-actTickCountModifier;
			gameSpeed = (newTickCount-actTickCount)/(1000.0/60.0);
			actTickCount = newTickCount;
			actFrameCount = (actTickCount/(1000.0/60.0));
			

		}
	}

	// Save stuff in the registry
	SaveRegistryInformation();

	// clean up
	FreeAllLists();
	DeInitPCSpecifics();
	DeInitInputDevices();
	ShutdownEditor();
	UnInitMPDirectPlay();

	free(memPtr);
	memPtr = NULL;

    return msg.wParam;
}


/*	--------------------------------------------------------------------------------
	Function		: InitialiseWindows
	Purpose			: initialises and creates application window
	Parameters		: HINSTANCE,int
	Returns			: int
	Info			: 
*/

int InitialiseWindows(HINSTANCE hInstance,int nCmdShow)
{
    WNDCLASS wc;
	
	// save instance handle
    winInfo.hInstance = hInstance;

    // set up and register window class
    wc.style			= 0;
    wc.lpfnWndProc		= WindowProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= hInstance;
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FROGGER));
    wc.hCursor			= LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName		= NULL;
    wc.lpszClassName	= "Frogger2PC";
    if(!RegisterClass(&wc))
	{
		dprintf"RegisterClass failed\n"));
        return 0;
	}

	// create the window
    winInfo.hWndMain = CreateWindowEx(
		winMode?0:WS_EX_TOPMOST,
        "Frogger2PC",
        "Frogger2PC",
		winMode?WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE:WS_POPUP,
		//WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE,			// window style
        0,
		0,
		winMode?640:GetSystemMetrics(SM_CXSCREEN), // 640
		winMode?480:GetSystemMetrics(SM_CYSCREEN), // 480
        NULL,				// parent window
        NULL,				// menu handle
        hInstance,			// program handle
        NULL);				// create parms

	
    if(!winInfo.hWndMain)
	{
        dprintf"CreateWindowEx failed\n"));
        return 0;
	}

    appBackgnd=LoadBitmap(winInfo.hInstance,MAKEINTRESOURCE(IDB_BACKGROUND));
	
	appActive = 1;
	ShowWindow(winInfo.hWndMain,SW_SHOW);
	UpdateWindow(winInfo.hWndMain);
//	ShowCursor(0);
	
												
    return 1;
}


/*	--------------------------------------------------------------------------------
	Function		: WindowProc
	Purpose			: handles windows messages
	Parameters		: HWND,UINT,WPARAM,LPARAM
	Returns			: long
	Info			: 
*/


long FAR PASCAL WindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	HDC dc;
	PAINTSTRUCT ps;

    switch(message)
	{
        case WM_DESTROY:
			PostQuitMessage(0);
			appActive = 0;
            break;

		case WM_PAINT:
			dc = BeginPaint(winInfo.hWndMain,&ps);
			EndPaint(winInfo.hWndMain,&ps);
			break;
		
		
		case WM_LBUTTONDOWN:
			lButton = 1;
			break;
		
		case WM_LBUTTONUP:
			lButton = 0;
			break;

		case WM_RBUTTONDOWN:
			rButton = 1;
			break;
		
		case WM_RBUTTONUP:
			rButton = 0;
			break;

		case WM_KEYUP:
			switch( (int)wParam )
			{
			case VK_SHIFT: mod &= ~FRED_SHIFT; break;
			case VK_CONTROL: mod &= ~FRED_CONTROL; break;
			}
			break;

		case WM_KEYDOWN:
			switch( (int)wParam )
			{
			case VK_SHIFT: mod |= FRED_SHIFT; break;
			case VK_CONTROL: mod |= FRED_CONTROL; break;
			case VK_CAPITAL: 
				if( mod & FRED_CAPS ) mod &= ~FRED_CAPS;
				else mod |= FRED_CAPS;
				break; // Caps lock is a toggle
			}
			break;

		case WM_CHAR:
			if( chatFlags & CHAT_INPUT )
			{
				ChatInput((char)wParam);
				return 0;
			}
			else if (editorOk)	// only when editor is set up to "grab" keyboard data
			{
				EditorKeypress((char)wParam);
				return 0;
			}
			break;

		case WM_ERASEBKGND:
		{
			RECT cRect;
			int i,j;
			HDC appBackgndDC;

			GetClientRect (hWnd,&cRect);
			appBackgndDC=CreateCompatibleDC((HDC)wParam);
	
			if (SelectObject (appBackgndDC,appBackgnd)==NULL)
				dp("sharugar");

			for (i=0; i<cRect.right+128; i+=128)
				for (j=0; j<cRect.bottom+128; j+=128)
					if (!BitBlt((HDC)wParam,i,j,128,128,appBackgndDC,0,0,SRCCOPY))
										 dp("bugger");
			DeleteDC(appBackgndDC);
      			
			return TRUE;
		 }

		case WM_ACTIVATE:
		{
			switch (LOWORD(wParam))
			{
				

				case WA_ACTIVE:
				case WA_CLICKACTIVE:
					keyInput = 1;
					break;
				case WA_INACTIVE:
					keyInput = 0;
					break;
			}
			break;
		}


	}

    return DefWindowProc(hWnd,message,wParam,lParam);
}


/*	--------------------------------------------------------------------------------
	Function		: DrawGraphics
	Purpose			: 
	Parameters		: 
	Returns			: 
	Info			: 

*/
VECTOR oldCCSource, oldCCTarget;
extern long numSprites;
extern long numPixelsDrawn;
char fR,fG,fB;
long useBilerpN = 0;
long useBilerpF = 0;
extern D3DTEXTUREHANDLE lastH;

unsigned long oldTH = 0;

unsigned long blinkTimer = 0;
unsigned long blinkTimer2 = 0;

unsigned long blinkSpeed = 5;
unsigned long blinkRand = 5;

unsigned long blinkSpeed2 = 10;
unsigned long blinkRand2 = 300;

void DrawGraphics() 
{
	totalFacesDrawn = 0;

	StartTimer(1,"Draw Gfx");

	FindTexture(&frogEyeOpen,UpdateCRC("fgeye.bmp"),1);
	FindTexture(&frogEyeClosed,UpdateCRC("fgeye2.bmp"),1);

	if (frogEyeOpen)
	{
		
		if (oldTH==0)
		{
			if (actFrameCount>blinkTimer2)
			{
				unsigned long newH = ((TEXENTRY *)frogEyeClosed)->hdl;
				oldTH = ((TEXENTRY *)frogEyeOpen)->hdl;
				((TEXENTRY *)frogEyeOpen)->hdl = newH;
				blinkTimer = actFrameCount + blinkSpeed + Random(blinkRand);
			}
		}
		else
		{
			if (actFrameCount>blinkTimer)
			{
				((TEXENTRY *)frogEyeOpen)->hdl = oldTH;
				oldTH = 0;
				blinkTimer2 = actFrameCount + blinkSpeed2 + Random(blinkRand2);

			}
		}
	}

	/*if (useBilerpN)
		pDirect3DDevice->lpVtbl->SetRenderState(pDirect3DDevice,D3DRENDERSTATE_TEXTUREMAG,D3DFILTER_LINEAR);
	else
		pDirect3DDevice->lpVtbl->SetRenderState(pDirect3DDevice,D3DRENDERSTATE_TEXTUREMAG,D3DFILTER_NEAREST);

	if (useBilerpF)
		pDirect3DDevice->lpVtbl->SetRenderState(pDirect3DDevice,D3DRENDERSTATE_TEXTUREMIN,D3DFILTER_LINEAR);
	else
		pDirect3DDevice->lpVtbl->SetRenderState(pDirect3DDevice,D3DRENDERSTATE_TEXTUREMIN,D3DFILTER_NEAREST);
*/

//	pDirect3DDevice->lpVtbl->SetRenderState(pDirect3DDevice,D3DRENDERSTATE_FOGCOLOR, D3DRGBA((float)fog.r/256.0,(float)fog.g/256.0,(float)fog.b/256.0,0) );
//	pDirect3DDevice->lpVtbl->SetRenderState(pDirect3DDevice,D3DRENDERSTATE_FOGTABLESTART, *(DWORD *)&fStart );
//	pDirect3DDevice->lpVtbl->SetRenderState(pDirect3DDevice,D3DRENDERSTATE_FOGTABLEEND, *(DWORD *)&fEnd );

	numPixelsDrawn=0;
	numSprites = 0;

	currentFrameTime = timeGetTime();
	
	StartTimer(2,"XformActorList");
	XformActorList();
	EndTimer(2);

	if(spriteList.numEntries)
		AnimateSprites();

	// Actual stuff that draws
	if (runHardware)
		BeginDrawHardware();
	else
		SoftwareBeginFrame();

	
	
	StartTimer(3,"DrawActorList");
	DrawActorList();	
	EndTimer(3);

	if(spriteList.numEntries)
		PrintSpritesOpaque();
	
	pDirect3DDevice->lpVtbl->SetRenderState(pDirect3DDevice,D3DRENDERSTATE_ALPHABLENDENABLE,TRUE);
	pDirect3DDevice->lpVtbl->SetRenderState(pDirect3DDevice,D3DRENDERSTATE_ZWRITEENABLE,FALSE);

	DrawBatchedPolys();
	BlankFrame();
	
	
	pDirect3DDevice->lpVtbl->SetRenderState(pDirect3DDevice,D3DRENDERSTATE_CULLMODE,D3DCULL_NONE);

	DrawSpecialFX();
	
	DrawBatchedPolys();
	BlankFrame();
	
	pDirect3DDevice->lpVtbl->SetRenderState(pDirect3DDevice,D3DRENDERSTATE_CULLMODE,D3DCULL_CW);
	pDirect3DDevice->lpVtbl->SetRenderState(pDirect3DDevice,D3DRENDERSTATE_ZWRITEENABLE,TRUE);
	
	PrintSpriteOverlays();	
	lastH = -1;
	PrintTextOverlays();
	
	if (editorOk)
	{
		DrawBatchedPolys();
		BlankFrame();
	
		DrawEditor();
		DrawBatchedPolys();
		BlankFrame();
	}


	if( chatFlags && gameState.mode == GAME_MODE )
		DrawChatBuffer( 100, 20, 540, 150 );
	
	/* CAMERA SPACE STUFF */
	// Back up currCamSource and currCamTarget
	oldCCSource = currCamSource[screenNum];
	oldCCTarget = currCamTarget[screenNum];
	
	if( text3DList.numEntries )
	{
		Calculate3DText( );
		Print3DText( );
		pDirect3DDevice->lpVtbl->SetRenderState(pDirect3DDevice,D3DRENDERSTATE_ALPHABLENDENABLE,TRUE);
		DrawBatchedPolys();
		BlankFrame();
		pDirect3DDevice->lpVtbl->SetRenderState(pDirect3DDevice,D3DRENDERSTATE_ALPHABLENDENABLE,FALSE);
	}

	// Set them so we don't do any modelling transforms
	currCamSource[screenNum] = zero;
	currCamTarget[screenNum] = inVec;
	
	
	// Restore currCam vectors
	currCamSource[screenNum] = oldCCSource;
	currCamTarget[screenNum] = oldCCTarget;
	/* END CAMERA SPACE STUFF */
	EndTimer(1);
	EndTimer(0);

	if (drawTimers)
	if (drawTimers<3)
	{
		PrintTimers();
		{	
			HDC hdc;
			HRESULT res = IDirectDrawSurface4_GetDC(surface[RENDER_SRF], &hdc);
			char speed[255];
					
			if (res == DD_OK)
			{
				sprintf(speed,"%.2f fps - %lu polys (%lu) - %lu sprites- %lu pixels",(60.0/gameSpeed),numFacesDrawn,totalFacesDrawn,numSprites,numPixelsDrawn);
				
				SetBkMode(hdc, TRANSPARENT);
				SetTextColor(hdc, RGB(255,0,0));
				TextOut(hdc,50,28, speed, strlen(speed));

				IDirectDrawSurface4_ReleaseDC(surface[RENDER_SRF], hdc);
			}
		}
	}
	
	if (KEYPRESS(DIK_F6))
		HoldTimers();
		
	ClearTimers();
	StartTimer(1,"Draw Gfx");
	StartTimer(0,"Everything");

	if (runHardware)
		EndDrawHardware();
	else
		SoftwareEndFrame();

	AnimateTexturePointers();
	ProcessProcTextures( );

	EndTimer(1);
}
