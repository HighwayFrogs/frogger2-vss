/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: fadeout.cpp
	Programmer	: David
	Date		: 
	Purpose		: Transition effects 'n shit

----------------------------------------------------------------------------------------------- */

#include "fadeout.h"
#include "layout.h"

long fadeoutStart, fadeoutLength;
int fadingOut = 0, keepFade = 0;

int (*fadeProc)(void);

int startIntensity = 0, endIntensity = 255;

/*	-------------------------------------------------------------------------------
	Function:	ScreenFade
	Params:		start, end intensity (0-255), time in frames
	returns:	
*/
int DrawScreenFade(void)
{
//	RECT r = { 0, 0, rXRes, rYRes };
	POLY_F4 *f4;
	int col;
	int halfX=256, halfY;
	
	if (actFrameCount > (fadeoutStart+fadeoutLength))
		col = endIntensity;
	else
		col = startIntensity + ((endIntensity-startIntensity)*(long)(actFrameCount-fadeoutStart))/fadeoutLength;

//		col = startIntensity + ((endIntensity-startIntensity)*(actFrameCount-fadeoutStart))/fadeoutLength;

//	D3DSetupRenderstates(xluSubRS);
//	DrawFlatRect(r, RGBA_MAKE(col, col, col, 255));
//	D3DSetupRenderstates(xluSemiRS);

	#if PALMODE==1
		halfY = 128;
	#else
		halfY = 120;
	#endif

	BEGINPRIM(f4, POLY_F4);
	setPolyF4(f4);
	f4->x0 = -halfX;
	f4->y0 = -halfY
	f4->x1 = halfX;
	f4->y1 = -halfY;
	f4->x2 = -halfX;
	f4->y2 = halfY;
	f4->x3 = halfX;
	f4->y3 = halfY;
	f4->r0 = col;
	f4->g0 = col;
	f4->b0 = col;
	f4->code |= 2;
	f4->code |= 32;
	ENDPRIM(f4, 1, POLY_F4);
	

	return (actFrameCount > (fadeoutStart+fadeoutLength));
}


/*	-------------------------------------------------------------------------------
	Function:	ScreenFade
	Params:		start, end intensity (0-255), time in frames
	returns:	
*/
void ScreenFade(int start, int end, long time)
{
	fadeProc = DrawScreenFade;
	fadeoutStart = actFrameCount;
	fadeoutLength = time;
	fadingOut = 1;

	// cause we're using subtractive
	startIntensity = 255-start;
	endIntensity = 255-end;
}


void StopFadeout()
{
	fadeProc = NULL;
	fadingOut = 0;
}

/*	-------------------------------------------------------------------------------
	Function:	MainWndProc
	Params:		As WNDPROC
	returns:	0 for success, other to pass on to mdx default handler
*/
void DrawScreenTransition(void)
{
	if (KEYPRESS(DIK_F))
		ScreenFade(0, 255, 60);

	if (fadeProc)
		if (fadeProc())
		{
			fadingOut = 0;
			
			if (!keepFade)
				fadeProc = NULL;
		}
}
