/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.


	File		: video.h
	Programmer	: David Swift
	Date		: 28 May 00, a bloody sunday no less

----------------------------------------------------------------------------------------------- */

#include <islutil.h>
#include "story.h"
#include "video.h"

#ifdef USE_BINK_VIDEO
#include <windows.h>
#include <d3d.h>
#include <mdx.h>
#include <stdio.h>

#include "layout.h"
#include "islpad.h"
#include "controll.h"
#include "pcaudio.h"
#include "dx_sound.h"
#include "main.h"
#include "game.h"
#include <bink.h>

static HBINK bink=0;
static s32 surfacetype=0;
bool blitVideo = 0;

#define VIDEOPATH "Video\\"

long (*oldLoop)() = 0;

#define videoSurface surface[PRIMARY_SRF]
long RunVideoPlayback()
{
	if (!BinkWait(bink))
	{
		// decompress a frame
		BinkDoFrame(bink);

		DDSURFACEDESC2 ddsd; ddsd.dwSize = sizeof(DDSURFACEDESC2);
		HRESULT res;
		RECT r;

		if (blitVideo)
		{
			if (!rFullscreen)
			{
				// Here's some scaling fun ..
				// if we're running in a window, we want to keep the right aspect ratio but fit
				// the whole video in the window.
				// 640x400 is 8:5 ratio, so that's what all the 8s and 5s mean

				RECT clientr;
				int width, height;

				GetClientRect(mdxWinInfo.hWndMain, &clientr);

				width = clientr.right;
				height = clientr.bottom;

				ClientToScreen(mdxWinInfo.hWndMain, (POINT*)&clientr.left);
				clientr.right += clientr.left;
				clientr.bottom += clientr.top;
				
				// pick the proportionally biggest dimension (width or height)
				// centre the rectangle in this direction, spanning the other

				if (width*5 < height*8)
				{
					r.bottom = (width*5)/8;
					r.left = clientr.left;
					r.right = clientr.right;
					r.top = ((height-r.bottom)/2)+clientr.top;
					r.bottom += r.top;
				}
				else
				{
					r.right = (height*8)/5;
					r.top = clientr.top;
					r.bottom = clientr.bottom;
					r.left = ((width-r.right)/2)+clientr.left;
					r.right += r.left;
				}
			}
			else
			{
				r.right = rXRes;
				r.bottom = (rXRes*5)/8;

				r.left = 0;
				r.top = (rYRes-r.bottom)/2;
				r.bottom += r.top;
			}

			if ((res = surface[RENDER_SRF]->Lock(NULL, &ddsd, DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR|DDLOCK_WRITEONLY, 0)) != DD_OK)
			{
				utilPrintf("RunVideoPlayback(): Failed locking device surface\n");
				return 1;
			}

			BinkCopyToBuffer(bink, ddsd.lpSurface, ddsd.lPitch, bink->Height,
				0, 0, surfacetype);

			RECT sr;
			sr.left = 0;
			sr.right = bink->Width;
			sr.top = 0;
			sr.bottom = bink->Height;

			surface[RENDER_SRF]->Unlock(NULL);

			surface[PRIMARY_SRF]->Blt(&r, surface[RENDER_SRF], &sr, DDBLT_WAIT, 0);
		}
		else
		{
			if ((res = videoSurface->Lock(NULL, &ddsd, DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR|DDLOCK_WRITEONLY, 0)) != DD_OK)
			{
				utilPrintf("RunVideoPlayback(): Failed locking primary surface\n");
				return 1;
			}

			// copy the data onto the screen .. hm
			BinkCopyToBuffer(bink, ddsd.lpSurface, ddsd.lPitch, bink->Height,
				(rXRes-bink->Width)/2, (rYRes-bink->Height)/2, surfacetype);

			videoSurface->Unlock(NULL);
		}

		ProcessUserInput();

		if (bink->FrameNum == bink->Frames || padData.debounce[0])
		{
			// end playback mode..
			BinkClose(bink); bink=NULL;

			if(padData.debounce[0] & PAD_START)
				quitAllVideo = YES;
			//AppLoop = oldLoop;
			return 1;
		}
		else
			BinkNextFrame(bink);
	}


	return 0;
}


DWORD CALLBACK VideoThreadProc(LPVOID foo)
{
	while (!RunVideoPlayback());
	
	return 0;
}


long StartVideoPlayback(int num)
{
	char path[MAX_PATH];

	if (!cdromDrive[0])
	{
		utilPrintf("No Frogger2 CD-ROM detected; skipping video playback\n");
		return 0;
	}

	BinkSoundUseDirectSound(lpDS);

	sprintf(path, "%s" VIDEOPATH "%s.bik", cdromDrive, fmv[num].name);

	bink = BinkOpen(path, 0);
	if (!bink)
	{
		utilPrintf("StartVideoPlayback(): Bink failed opening '%s'\n", path);
		return 0;
	}
	else
	{
		utilPrintf("StartVideoPlayback(): Starting video: '%s'\n", path);
	}

	surfacetype = BinkDDSurfaceType(videoSurface);

	if (!rFullscreen || !(rXRes == 640 && rYRes == 480))
		blitVideo = 1;

	DDBLTFX bltfx;
	bltfx.dwSize = sizeof(DDBLTFX);
	bltfx.dwFillColor = 0;

	videoSurface->Blt(NULL,NULL,NULL,DDBLT_COLORFILL,&bltfx);

	//oldLoop = AppLoop;
	//AppLoop = RunVideoPlayback;

	DWORD id;
	HANDLE videoThread = CreateThread(0, 0, VideoThreadProc, 0, 0, &id);

	while (WaitForSingleObject(videoThread, 1000) == WAIT_TIMEOUT)
	{
		// do nothing, every second, until the thread is finished.		
	}

	return 1;
}

/* ----------------------------------------------------------------------- */

#else // if we're compiling without video support..

long RunVideoPlayback()
{
	return 1;
}

long StartVideoPlayback(int num)
{
	utilPrintf("StartVideoPlayback(): No video support compiled, skipping '%s'\n", fmv[num].name);
	return 0;
}

#endif // INCLUDE_VIDEO