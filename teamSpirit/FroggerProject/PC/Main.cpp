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

float camY = 100,camZ = 100;
extern "C" {MDX_LANDSCAPE *world;}

long DrawLoop(void)
{
	POINT t;
	D3DSetupRenderstates(D3DDefaultRenderstates);
	
	// Just to get functionality... ;)
	StartTimer (2,"DrawActorList (old)");
	DrawActorList();
	EndTimer(2);

	guLookAtF (vMatrix.matrix,
		currCamTarget[0].vx/40960.0,currCamTarget[0].vy/40960.0,currCamTarget[0].vz/40960.0,
		currCamSource[0].vx/40960.0,currCamSource[0].vy/40960.0,currCamSource[0].vz/40960.0, 0,1,0);
	
	pDirect3DDevice->BeginScene();
	BlankAllFrames();
	SwapFrame(MA_FRAME_NORMAL);
	
	if (world)
		DrawLandscape(world);

	StartTimer(1,"Actors");
	ActorListDraw();
	EndTimer(1);
	
	PrintTextOverlays();
	pDirect3DDevice->EndScene();
	
	DrawAllFrames();
	
	

	EndTimer(0);
	if (consoleDraw)
		PrintConsole();
	if (timerDraw)
		PrintTimers();
	ClearTimers();	
	StartTimer(0,"Everything");
	
	DDrawFlip();
	D3DClearView();

	GetCursorPos(&t);
	camZ = t.x*8;
	camY = t.y*8;

	
	return 0;
}

long LoopFunc(void)
{
	ACTOR2 *c;

	actFrameCount = timeInfo.frameCount;
	gameSpeed = 4096*timeInfo.speed;

	GameLoop();

	c = actList;
	while (c)
	{
		((MDX_ACTOR *)(c->actor->actualActor))->pos.vx = c->actor->position.vx / 10.0;
		((MDX_ACTOR *)(c->actor->actualActor))->pos.vy = c->actor->position.vy / 10.0;
		((MDX_ACTOR *)(c->actor->actualActor))->pos.vz = c->actor->position.vz / 10.0;

		if (c->actor->qRot.w || c->actor->qRot.x || c->actor->qRot.y || c->actor->qRot.z)
		{
			((MDX_ACTOR *)(c->actor->actualActor))->qRot.x = c->actor->qRot.x / 4096.0;
			((MDX_ACTOR *)(c->actor->actualActor))->qRot.y = c->actor->qRot.y / 4096.0;
			((MDX_ACTOR *)(c->actor->actualActor))->qRot.z = c->actor->qRot.z / 4096.0;
			((MDX_ACTOR *)(c->actor->actualActor))->qRot.w = c->actor->qRot.w / 4096.0;			
		}

		c = c->next;
	}

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

	// Clear the timers for the initial frame
	ClearTimers();

	InitInputDevices();

	CommonInit();

	pcFont = InitFont("FontA",baseDirectory);

	InitTiming(60.0);

	RunWindowsLoop(&LoopFunc);

	// Byeeeeeeeeeee
	DeInitInputDevices();
	D3DShutdown();
	DDrawShutdown();	
	gelfShutdown();

	return 0;
}
