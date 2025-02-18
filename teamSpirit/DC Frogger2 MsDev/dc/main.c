/*
	This file is part of Frogger II, Copyright 2000 Interactive Studios Ltd

	File        : shell.c
	Description : Main core routine and system initialisation
	Date        : 09/03/2000
	Version     : 0.01
*/

#include "include.h"		// so we can use mw_pr() which outputs to the debug window

#include "frogger.h"
#include "sg_Chain.h"
#include "generic.h"
#include <shinobi.h>
#include <kamui2.h>
#include <sg_sd.h>
#include <sg_mw.h>
#include "dc_timer.h"
#include "lang.h"
#include "psxsprite.h"
#include "tongue.h"
#include "menus.h"

#include <ac.h>
#include <a64thunk.h>
#include <am.h>

#include "am_audio.h"
#include "audio.h"
#include "bpamsetup.h"
#include "bpamstream.h"
#include "bpbuttons.h"
#include "bpprintf.h"
#include "bpamsfx.h"
#include "strsfx.h"
#include "islvideo.h"

#include <Machine.h>
#include <math.h>


// *ASL* 09/08/2000 - Faster not to map the stack to cache
//#define USE_STACK_CACHE

#ifdef USE_STACK_CACHE
extern void MapStackToRamCache();
#endif


KMPACKEDARGB 	borderColour;
KMDWORD 		FBarea[24576 + 19456];

// *ASL* 20/07/2000 - Removed 'const' keyword from below var
PDS_PERIPHERAL 	*per;

KMSTRIPCONTEXT 	stripContext00;
KMSTRIPHEAD 	stripHead00;
KMSTRIPCONTEXT 	StripContext;
KMSTRIPHEAD 	StripHead;
KMVERTEX_00		vertices[8];

DCKVERTEX 		*globalVerts;

KMSURFACEDESC	TexList[256];

int				matrixSize = 0;
DCKMATRIX		matrixStack[256];
DCKMATRIX		viewMatrix;

KMPACKEDARGB	colourClampMin,colourClampMax;

displayPageType displayPage[2];
displayPageType *currentDisplayPage  = 0;

psFont 			*font;
psFont 			*fontSmall;

int 			drawLandscape = 1;
long 			textEntry = 0;	
char 			textString[255] = "";

long 			drawGame = 1;

INT				textPosX = 0;
INT				textPosY = 10;

GsRVIEW2		camera;

int				polyCount;
int				mapCount;

TEXTOVERLAY 	*startText;
PSIOBJECT 		*world;
ACTOR2 			*me = NULL;
ACTOR2 			*me2 = NULL;
char			textbuffer[256];
float			myframe = 0;
DCKVECTOR		camPos = {0.0f,0.0f,-50.0f};
DCKVECTOR		camTarg = {0.0f,0.0f,0.0f};
float			viewAngle = 3.34;
float			viewDist = 1000;

extern int		frogFacing[4];
extern int		camFacing;
extern struct 	gameStateStruct gameState;

SYCHAIN      	VblChain;
unsigned long 	*sqrtable;
short			*acostable;

int 			vsyncCounter = 0;
int 			lastframe = 0;
int 			myVsyncCounter = 0;

//AM_BANK_PTR		gMyBank	= KTNULL;
//AM_SOUND_PTR	gOneShot;

long 			turbo = 4096;

unsigned short 	globalClut;
short			actorShiftDepth;
short			timerBars = OFF;

AC_ERROR_PTR	acErr;
AM_ERROR 		*amErr;
AM_BANK_PTR		gBank =	KTNULL;
	
KMDWORD			FogDensity;


// -------
// Globals

// used to abort the game
unsigned int globalAbortFlag = 0;


// ----------
// Prototypes

// gsFs error function callback
static void gdFsErrorCallback(void *obj, Sint32 err);

// initialise kamui
static void InitKamui();











void SetCam(VECTOR src, VECTOR tar)
{
	camera.vpx = src.vx;
	camera.vpy = src.vy;
	camera.vpz = src.vz;

	camera.vrx = tar.vx;
	camera.vry = tar.vy;
	camera.vrz = tar.vz;

	camera.rz  =  0;

	GsSetRefView2(&camera);

	gte_SetRotMatrix(&GsWSMATRIX);
	gte_SetTransMatrix(&GsWSMATRIX);
}

void InitCam(void)
{
	// Set up an ambient light
	GsSetAmbient(128*16, 128*16, 128*16);
 	GsSetLightMode(2);

	// Set up a simple camera
	GsSetProjection(475);			// default distance of projection; alters perspective
	SetGeomOffset(640/2,480/2);		// camera focused on middle of screen
//	SetGeomOffset(0,0);				// camera focused on middle of screen

 	bbsrc.vx = 500;
	bbsrc.vy = -350;
 	bbsrc.vz = 11000;
   
	bbtar.vx = bbsrc.vx + (sinf(viewAngle) * viewDist);
	bbtar.vy = bbsrc.vy;// + 200;
	bbtar.vz = bbsrc.vz + (cosf(viewAngle) * viewDist);	

/* 	bbsrc.vx = 0;
 	bbsrc.vy = 00;
 	bbsrc.vz = -300;
  
 	bbtar.vx = 0;
 	bbtar.vy = 0;
 	bbtar.vz = 1300;
*/
	SetCam(bbsrc,bbtar);
}

static void VblCallBack(void *arg)
{
//	MyStreamServer();

	if(vsyncCounter > lastframe)
	{
		lastframe = vsyncCounter;
		myVsyncCounter = 0;
	}
	else
	{
		myVsyncCounter++;
	}
	frame++;
	vsyncCounter++;	
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/

void InitLookupTables()
{
	int			fileLength;

	// *ASL* 20/07/2000 - Re-cast to expected pointers
	sqrtable = (unsigned long *)fileLoad("incbin\\Sqrtable.bin",&fileLength);
	acostable = (short *)fileLoad("incbin\\acostab.bin",&fileLength);
}

/*	--------------------------------------------------------------------------------
	Function 	: main
	Purpose 	: main code loop
	Parameters 	: none
	Returns 	: none
	Info 		:
*/

void BuildFogTable (void)
{
	int				i;
	float			depth;
	KMPACKEDARGB	fogcol;
	KMFLOAT			FogTable[128];
	KMSTATUS		status;
	float			min,max,density;

	// Fog Color
	fogcol.byte.bBlue	= 0;//0.5 * 255;
	fogcol.byte.bGreen	= 0;//0.5 * 255;
	fogcol.byte.bRed	= 0.5 * 255;
	status = kmSetFogTableColor(fogcol);

/*ma
	fog.min = (1100)<<2;
	fog.min = (1100+500)<<2;

	min = 1.0/(float)fog.min;
	max = 1.0/(float)fog.max;
	kmGenerateFogTable(FogTable,min,max,0.1,&density,KM_FOGTYPE_LINEAR);

	kmConvertFogDensity(density,&FogDensity);
	kmSetFogDensity(FogDensity);
	kmSetFogTable(&FogTable[0]);
*/
	// Fog Density
	FogDensity = 0xFF0B;		// FF09 - FF0D
	status = kmSetFogDensity(FogDensity);

	// Fog Table
	for(i = 0; i < 128; i++)
	{
#define	FOG_MIN		(-0.8f)
		FogTable[i] = 1.0f - (float)i * ((1.0f - FOG_MIN) / 128.0f);
		if(FogTable[i] < 0.0f)
			FogTable[i] = 0.0f;
	}
	status = kmSetFogTable(&FogTable[0]);
}

extern int frameCount;
extern int AllowPsiDestroy;
extern int psiActorCount;
extern int fmaActorCount;
extern int numTextureBanks;

char texurestring[256];
char texurestring2[256];


extern KTU32 *gTransferBuffer;
extern KTU32 *gPlayBuffer;
extern KTU32 *gHeaderBuffer;
extern KTU32 gTransferBufferSize;

int	texViewer = 0;

short videoKeyHandler()
{
	return 0;
}

extern StrDataType 	vStream;
extern CurrentData	current[24];




/* ---------------------------------------------------------
   Function : main
   Purpose : entry / exit function
   Parameters : 
   Returns : 
   Info : 
*/

void main()
{
	int					i;
	MWS_PLY_INIT_SFD	iprm;
	Uint32 				*memfree,*memsize;
	unsigned long		hyp,test;
	int					numUsed;
	

#ifdef __GNUC__
	shinobi_workaround();
#endif

	// initialise system for middleware (call before sbInitSystem)
	mwPlyPreInitSofdec();
		
	// initialise video mode
	switch (syCblCheck())
	{
		case SYE_CBL_VGA:
			sbInitSystem((int)KM_DSPMODE_VGA,(int)KM_DSPBPP_RGB565,1);
			displayMode = KM_DSPMODE_VGA;		
			break;
			
		case SYE_CBL_PAL:
			sbInitSystem((int)KM_DSPMODE_PALNI640x480EXT,(int)KM_DSPBPP_RGB565,1);
			displayMode = KM_DSPMODE_PALNI640x480EXT;		
			break;
			
		case SYE_CBL_NTSC:
		default:
			sbInitSystem((int)KM_DSPMODE_NTSCNI640x480,(int)KM_DSPBPP_RGB565,1);
			displayMode = KM_DSPMODE_NTSCNI640x480;		
			break;
	}
	frameBufferFormat = KM_DSPBPP_RGB888;

	// initialise all pads
	for (i=0; i<4; i++)
	{
		switch (i)
		{
			case 0:	
				per = pdGetPeripheral(PDD_PORT_A0);
				break;
			case 1:	
				per = pdGetPeripheral(PDD_PORT_B0);
				break;
			case 2:	
				per = pdGetPeripheral(PDD_PORT_C0);
				break;
			case 3:	
				per = pdGetPeripheral(PDD_PORT_D0);
				break;
		}
		padData.present[i] = TRUE;
		if (strstr(per->info->product_name,"(none)"))
			padData.present[i] = FALSE;
	}

	// initialise KAMUI
	InitKamui();

    // initialise our CRC table
	InitCRCTable();	

	// *ASL* 09/09/2000 - Tray open

	// clear our abort flag
	globalAbortFlag = 0;

	// set our gdFs error callback
	gdFsEntryErrFuncAll(gdFsErrorCallback, (void *)0);

    // malloc alignment
    Init32Malloc();


	// Open the AM system	
	if(!bpAmSetup(AC_DRIVER_DA, KTFALSE, KTNULL))
	{
		acASEBRK(KTTRUE);
	}
	(void)acSetTransferMode(AC_TRANSFER_DMA);
		
	acSystemDelay(500000);

	acErr = acErrorGetLast();
	amErr = amErrorGetLast();

	// Setup the file system
	a64FileInit();

////////

	amHeapAlloc(&gPlayBuffer, gPlayBufferSize, 32,	AM_PURGABLE_MEMORY,KTNULL);
	gTransferBuffer = (KTU32*)syMalloc(gTransferBufferSize);
	gHeaderBuffer = (KTU32*)syMalloc(AM_FILE_BLOCK_SIZE);

/////////

	// setup my vb interrupt
	VblChain = syChainAddHandler(SYD_CHAIN_EVENT_IML6_VBLANK,VblCallBack,0x60,NULL);



#ifdef USE_STACK_CACHE
	MapStackToRamCache();
#endif




//	kmSetCullingRegister(1.0f);
	
	InitLookupTables();
	utilInitCRC();
	utilSeedRandomInt(398623);
	InitCam();
	actorInitialise();

	InitProgressBar();
	InitTimerbar();

	initialisePsxStrips();

	numTextureBanks = 0;

	CommonInit();

	for(i = 0;i < 4;i++)
	{
		switch(i)
		{
			case 0:	
				per = pdGetPeripheral(PDD_PORT_A0);
				break;
			case 1:	
				per = pdGetPeripheral(PDD_PORT_B0);
				break;
			case 2:	
				per = pdGetPeripheral(PDD_PORT_C0);
				break;
			case 3:	
				per = pdGetPeripheral(PDD_PORT_D0);
				break;
		}
		padData.present[i] = TRUE;				
		if(strstr(per->info->product_name,"(none)"))
			padData.present[i] = FALSE;				
	}
	
//ma	BuildFogTable();


	// *ASL* 08/08/2000 - Run around the main loop until the user opens the cd tray
	while (globalAbortFlag == 0)
	{

		Uint32 drawTime;

		DCTIMER_START(0);

		DCTIMER_START(1);		
		polyCount = 0;
		actorCount = 0;		
		mapCount = 0;
		psiActorCount = 0;
		fmaActorCount = 0;
		
		DCTIMER_STOP(1);		

		bpAmStreamServer();
		if (bpAmStreamDone(gStream))
		{
			ReplayStream();
		}

		for(i=0;i<NUM_FROGS;i++)
		{
			switch(i)
			{
				case 0:	
					per = pdGetPeripheral(PDD_PORT_A0);
					break;
				case 1:	
					per = pdGetPeripheral(PDD_PORT_B0);
					break;
				case 2:	
					per = pdGetPeripheral(PDD_PORT_C0);
					break;
				case 3:	
					per = pdGetPeripheral(PDD_PORT_D0);
					break;
			}

			// temp pad emulation
			padData.debounce[i] = 0;
			if(per->press & PDD_DGT_KU)
				padData.debounce[i] |= PAD_UP;
			if(per->press & PDD_DGT_KD)
				padData.debounce[i] |= PAD_DOWN;
			if(per->press & PDD_DGT_KR)	
				padData.debounce[i] |= PAD_RIGHT;
			if(per->press & PDD_DGT_KL)
				padData.debounce[i] |= PAD_LEFT;

			if(per->press & PDD_DGT_TB)
				padData.debounce[i] |= PAD_TRIANGLE;
			if(per->press & PDD_DGT_TA)
				padData.debounce[i] |= PAD_CROSS;
			if(per->press & PDD_DGT_TX)
				padData.debounce[i] |= PAD_SQUARE;
			if(per->press & PDD_DGT_TY)
				padData.debounce[i] |= PAD_CIRCLE;
			if(per->press & PDD_DGT_ST)
				padData.debounce[i] |= PAD_START;

			if((per->press & PDD_DGT_TR)&&(per->press & PDD_DGT_TL))
			{
				padData.debounce[i] |= PAD_R1;
			}
			else
			{
				if(per->press & PDD_DGT_TR)
					padData.debounce[i] |= PAD_R2;
				if(per->press & PDD_DGT_TL)
					padData.debounce[i] |= PAD_L2;
			}
		
			// toggle timer bars on/off
			if((per->press & PDD_DGT_TY) /*&&((per->r <= 128)&&(per->r > 0))&&((per->l <= 128)&&(per->l > 0))*/)
				timerBars = !timerBars;

			// temp pad emulation
			padData.digital[i] = 0;
			if(per->on & PDD_DGT_KU)
				padData.digital[i] |= PAD_UP;
			if(per->on & PDD_DGT_KD)
				padData.digital[i] |= PAD_DOWN;
			if(per->on & PDD_DGT_KR)
				padData.digital[i] |= PAD_RIGHT;
			if(per->on & PDD_DGT_KL)
				padData.digital[i] |= PAD_LEFT;

			if(per->on & PDD_DGT_TB)
				padData.digital[i] |= PAD_TRIANGLE;
			if(per->on & PDD_DGT_TA)
				padData.digital[i] |= PAD_CROSS;
			if(per->on & PDD_DGT_TX)
				padData.digital[i] |= PAD_SQUARE;
			if(per->on & PDD_DGT_TY)
				padData.digital[i] |= PAD_CIRCLE;
			if(per->on & PDD_DGT_ST)
				padData.digital[i] |= PAD_START;
		}
		
		UpdateTextureAnimations();
				
		GameLoop();
		DCTIMER_STOP(1);		
	
		kmBeginScene(&kmSystemConfig);
		kmBeginPass(&vertexBufferDesc);

		DCTIMER_START(2);		
		DrawSpecialFX();
		PrintSprites();
		DCTIMER_STOP(2);		

		// *ASL* 27/07/2000
		PSIDC_SH4XD_SetMatrix(&RotMatrix, &TransVector);

		DCTIMER_START(3);		
		if(drawLandscape && drawGame)
			DrawWorld();		
		DCTIMER_STOP(3);

		DCTIMER_START(4);		
		if(drawLandscape && drawGame)
			DrawScenicObjList();
		if(drawLandscape && drawGame)
			DrawWaterList();			
		drawTime = DCTIMER_STOP(4);		
		
		DCTIMER_START(5);		
		if(drawGame)
			DrawActorList();
		DCTIMER_STOP(5);		

		UpdateFrogTongue(0);
		UpdateFrogCroak(0);

		if(tileTexture[0])
			DrawTiledBackdrop(/*saveInfo.saveFrame ? NO : */YES);

		DCTIMER_START(6);				
		PrintSpriteOverlays(1);
		PrintSpriteOverlays(0);
		PrintTextOverlays();		
		DrawScreenTransition();
		DCTIMER_STOP(6);		

		gte_SetRotMatrix(&GsWSMATRIX);
		gte_SetTransMatrix(&GsWSMATRIX);

		DCTIMER_START(7);
		DCTIMER_STOP(7);


		if(timerBars)
		{
			DCTIMER_PRINTS();
			
		 	numUsed = 0;
			for(i=0; i<24; i++)
			{
				if(current[i].sound.isPlaying)
					numUsed++;
			}
 
//		 	sprintf(textbuffer,"numUsed: %d",numUsed);
//			fontPrint(font, 10,10, textbuffer, 255,255,255);

//			fontPrint(font, textPosX,textPosY, texurestring, 255,255,255);

//			fontPrint(font, textPosX,textPosY+16, texurestring2, 255,255,255);

			sprintf(textbuffer, "map polys: %d", polyCount);
			fontPrint(font, textPosX,textPosY, textbuffer, 255,255,255);

//			sprintf(textbuffer,"Actors: %d (%d,%d)",(psiActorCount+fmaActorCount),psiActorCount, fmaActorCount);
//			fontPrint(font, textPosX,textPosY+16, textbuffer, 255,255,255);

//			sprintf(textbuffer,"DCK: %d (%d)",DCKnumTextures,texViewer);
//			fontPrint(font, textPosX,textPosY+32, textbuffer, 255,255,255);

//			sprintf(textbuffer,"Map: %d",mapCount);
//			fontPrint(font, textPosX,textPosY+16, textbuffer, 255,255,255);

			sprintf(textbuffer,"Draw Time: %d",drawTime);
			fontPrint(font, textPosX,textPosY+32, textbuffer, 255,255,255);

//			// *ASL* 25/07/2000 - Commented out as crashed in un-optimised build
//			//syMallocStat(memfree,memsize);
//			sprintf(textbuffer,"alloc: %d",*memfree);//mallocList.totalMalloc);		
//			fontPrint(font, textPosX,textPosY+48, textbuffer, 255,255,255);						

//			sprintf(textbuffer,"mallocList: %d",mallocList.numEntries);//mallocList.totalMalloc);		
//			fontPrint(font, textPosX,textPosY+48+16, textbuffer, 255,255,255);							

//			sprintf(textbuffer,"fog.max: %d",fog.max);		
//			fontPrint(font, textPosX,textPosY+48+16, textbuffer, 255,255,255);							

/*			if(per->press & PDD_DGT_TR)
			{
				if(texViewer < DCKnumTextures)
					texViewer++;
			}
				
			if(per->press & PDD_DGT_TL)
			{
				if(texViewer > 0)
					texViewer--;
			}
			
			vertices_GT4[0].fX = 32;
			vertices_GT4[0].fY = 32;
			vertices_GT4[0].u.fZ = 10;
			vertices_GT4[0].fU = (float)0;
			vertices_GT4[0].fV = (float)0;
			vertices_GT4[0].uBaseRGB.dwPacked = RGBA(255,255,255,255);	

			vertices_GT4[1].fX = 32+256;
			vertices_GT4[1].fY = 32;
			vertices_GT4[1].u.fZ = 10;
			vertices_GT4[1].fU = (float)1;
			vertices_GT4[1].fV = (float)0;
			vertices_GT4[1].uBaseRGB.dwPacked = RGBA(255,255,255,255);

			vertices_GT4[2].fX = 32;
			vertices_GT4[2].fY = 32+256;
			vertices_GT4[2].u.fZ = 10;
			vertices_GT4[2].fU = (float)0;
			vertices_GT4[2].fV = (float)1;
			vertices_GT4[2].uBaseRGB.dwPacked = RGBA(255,255,255,255);

			vertices_GT4[3].fX = 32+256;
			vertices_GT4[3].fY = 32+256;
			vertices_GT4[3].u.fZ = 10;
			vertices_GT4[3].fU = (float)1;
			vertices_GT4[3].fV = (float)1;
			vertices_GT4[3].uBaseRGB.dwPacked = RGBA(255,255,255,255);

			kmChangeStripTextureSurface(&StripHead_Sprites,KM_IMAGE_PARAM1,&DCKtextureList[texViewer].surface);
			
			kmStartStrip(&vertexBufferDesc, &StripHead_Sprites);	
			kmSetVertex(&vertexBufferDesc, &vertices_GT4[0], KM_VERTEXTYPE_03, sizeof(KMVERTEX_03));
			kmSetVertex(&vertexBufferDesc, &vertices_GT4[1], KM_VERTEXTYPE_03, sizeof(KMVERTEX_03));
			kmSetVertex(&vertexBufferDesc, &vertices_GT4[2], KM_VERTEXTYPE_03, sizeof(KMVERTEX_03));	
			kmSetVertex(&vertexBufferDesc, &vertices_GT4[3], KM_VERTEXTYPE_03, sizeof(KMVERTEX_03));	
			kmEndStrip(&vertexBufferDesc);
*/			
		}
	
		kmEndPass(&vertexBufferDesc);
				
		DCTIMER_START(8);		
		kmRender(KM_RENDER_FLIP);
		kmEndScene(&kmSystemConfig);

		if((gameState.mode != PAUSE_MODE) || (quittingLevel))
		{
			//bb
			lastActFrameCount = actFrameCount;

			gameSpeed = vsyncCounter<<12;

			if(gameSpeed > (5<<12))
				gameSpeed = (5<<12);
					
			actFrameCount += gameSpeed>>12;
			vsyncCounter = 0;
		}
		
		pdExecPeripheralServer();
		DCTIMER_STOP(8);		
		DCTIMER_STOP(0);		
	}


	// *ASL* 09/08/2000 - Restore
	// ** We got this far so we should shutdown all of our resources and drop back into the
	// ** boot ROM

#if 0
	bpAmShutdown();		
	
	syChainDeleteHandler(VblChain);

	// *ASL* 21/07/2000 - Changed from FreeBackDrop()!!!
	FreeBackdrop();
#endif

	sbExitSystem();
	syBtExit();
}





/* ---------------------------------------------------------
   Function : InitKamui
   Purpose : initialise Kamui
   Parameters : 
   Returns : 
   Info : 
*/

static void InitKamui()
{
	int	i;
	
	// create the frame buffers
	pFB[0] = &primarySurfaceDesc;
	pFB[1] = &renderSurfaceDesc;

    // vertex buffer pointer
    dwDataPtr = (PKMDWORD)(((KMDWORD)syMalloc(0x400000) & 0x0FFFFFFFUL) | 0xA0000000);

	// set the system configuration
	kmSystemConfig.dwSize						= sizeof(KMSYSTEMCONFIGSTRUCT);
	kmSystemConfig.flags						= KM_CONFIGFLAG_ENABLE_CLEAR_FRAMEBUFFER;
	// for frame buffer
	kmSystemConfig.ppSurfaceDescArray			= pFB;
	kmSystemConfig.fb.nNumOfFrameBuffer			= 2;
	// for texture memory
    kmSystemConfig.nTextureMemorySize       	= 0x200000 * 2;		// 4 MB for textures
    kmSystemConfig.nNumOfTextureStruct        	= 4096;
    kmSystemConfig.nNumOfSmallVQStruct        	= 0;//1024;
    kmSystemConfig.pTextureWork               	= &FBarea[0];
    // for Vertex buffer
    kmSystemConfig.pBufferDesc                	= &vertexBufferDesc;
    kmSystemConfig.nNumOfVertexBank           	= 2;
    kmSystemConfig.pVertexBuffer              	= dwDataPtr;		// pointer to vertex buffer
    kmSystemConfig.nVertexBufferSize          	= 0x200000 * 2;		// 4MB for vertex buffer
    kmSystemConfig.nPassDepth                 	= 1;
    kmSystemConfig.Pass[0].dwRegionArrayFlag  	= KM_PASSINFO_AUTOSORT;
    kmSystemConfig.Pass[0].nDirectTransferList	= KM_OPAQUE_POLYGON;
	kmSystemConfig.Pass[0].fBufferSize[0]     	= 20.0f;
    kmSystemConfig.Pass[0].fBufferSize[1]     	= 0.0f;
    kmSystemConfig.Pass[0].fBufferSize[2]     	= 80.0f;
    kmSystemConfig.Pass[0].fBufferSize[3]     	= 0.0f;
    kmSystemConfig.Pass[0].fBufferSize[4]     	= 0.0f;

	kmSetSystemConfiguration(&kmSystemConfig);

    // initialise border colour
    borderColour.dwPacked = RGBA(0,0,0,255);	// black border
    kmSetBorderColor(borderColour);
}


/* ---------------------------------------------------------
   Function : gdFsErrorCallback
   Purpose : gsFs error function callback
   Parameters : user object pointer, error code
   Returns : 
   Info : 
*/

static void gdFsErrorCallback(void *obj, Sint32 err)
{
	// was the tray opened?
	if (err == GDD_ERR_TRAYOPEND || err == GDD_ERR_UNITATTENT)
	{
		globalAbortFlag = 1;
	}
}
