/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: block.c
	Programmer	: Andy Eder
	Date		: 28/06/99 10:19:37

----------------------------------------------------------------------------------------------- */

#include <windows.h>
#include <ddraw.h>
#include <d3d.h>
#include <math.h>
#include <islutil.h>
#include <islpad.h>

extern "C"
{

#include <anim.h>
#include <stdio.h>

#include "game.h"
//#include "netgame.h"
#include "types2d.h"
//#include "controll.h"
#include "frogger.h"
#include "levplay.h"
#include "frogmove.h"
#include "cam.h"
#include "tongue.h"
#include "babyfrog.h"
#include "hud.h"
#include "frontend.h"
//#include "objects.h"
#include "textover.h"
//#include "3dtext.h"
#include "multi.h"
#include "layout.h"
#include "platform.h"
#include "enemies.h"
//#include "audio.h"
//#include "ptexture.h"
#include "levprog.h"
#include "event.h"
//#include "newstuff.h"
#include "main.h"
#include "newpsx.h"
#include "Main.h"
#include "actor2.h"
#include "bbtimer.h"
#include "maths.h"

#include <temp_pc.h>
}

#include "mdx.h"

extern "C"
{
#include "controll.h"
psFont *font;
MDX_FONT *pcFont;
}


extern char baseDirectory[MAX_PATH] = "X:\\TeamSpirit\\pcversion\\";

long DrawLoop(void)
{
	D3DSetupRenderstates(D3DDefaultRenderstates);
	
	pDirect3DDevice->BeginScene();
	PrintTextOverlays();
	pDirect3DDevice->EndScene();

	EndTimer(0);
	if (consoleDraw)
		PrintConsole();
	if (timerDraw)
		PrintTimers();
	ClearTimers();	
	StartTimer(0,"Everything");
	
	DDrawFlip();
	D3DClearView();
	return 0;
}

long LoopFunc(void)
{
	GameLoop();
	DrawLoop();
	ProcessUserInput();
	return 0;
}

int PASCAL WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
		// Init common controls
	InitCommonControls();

	// Init windows
	if (!WindowsInitialise(hInstance,"Frogger2",1))
		return 1;

	// Init DDraw Object
	if (!DDrawInitObject (NULL))
		return 1;

	// Setup our sufaces
	if (!DDrawCreateSurfaces (mdxWinInfo.hWndMain, 640, 480, 16,TRUE, 16))
		return 2;

	// Setup D3D
	if (!D3DInit())
		return 3;
	
	// Initialise the matrix stack
	MatrixStackInitialise();
	
	// Initialise Johns BMP loader
	gelfInit();

	// Init the CRC table
	InitCRCTable();

	// Init a table for 1/n (Optimisation)
	InitOneOverTable();

	// Init mavis frameset
	InitFrames();
	
	// Init the font system
	InitFontSystem();

	// Clear the spare suface (Ready for lotsa nice fx)
	DDrawClearSurface(SPARE_SRF, 0, DDBLT_COLORFILL);

	// Clear the timers for the initial frame
	ClearTimers();

	InitInputDevices();

	CommonInit();

	pcFont = InitFont("FontA",baseDirectory);

	RunWindowsLoop(&LoopFunc);

	// Byeeeeeeeeeee
	DeInitInputDevices();
	D3DShutdown();
	DDrawShutdown();	
	gelfShutdown();

	return 0;
}
