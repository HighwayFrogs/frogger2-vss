/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.


	File		: mavis.cpp
	Programmer	: Matthew Cloy
	Date		: 

----------------------------------------------------------------------------------------------- */

#include <windows.h>
#include <ddraw.h>
#include <d3d.h>
#include <math.h>
#include "mgeReport.h"
#include "mdxDDraw.h"
#include "mdxD3D.h"
#include "mdxInfo.h"
#include "mdxTiming.h"
#include "mdxCRC.h"
#include "mdxTexture.h"
#include "mdxMath.h"
#include "mdxObject.h"
#include "mdxActor.h"
#include "mdxLandscape.h"
#include "mdxRender.h"
#include "mdxPoly.h"
#include "mdxDText.h"
#include "mdxProfile.h"
#include "mdxWindows.h"
#include "gelf.h"

extern "C"
{

// Instance of FRAME_INFO, for storing... Surprisingly.. The frame information./
// Would be ideal to choose what to alloc when render hardware/software is chosen!

FRAME_INFO frameInfo[MA_MAX_FRAMES];
FRAME_INFO *cFInfo = &frameInfo[0], *oFInfo = &frameInfo[1];

unsigned long numSoftPolys,numSoftVertex;
SOFTPOLY softPolyBuffer[MA_MAX_FACES];
SOFTPOLY *softDepthBuffer[MA_SOFTWARE_DEPTH];
D3DTLVERTEX softV[MA_MAX_VERTICES];

unsigned long numHaloPoints;

short haloZVals[MA_MAX_HALOS];
MDX_VECTOR haloPoints[MA_MAX_HALOS];

unsigned long lightingMapRS[] = 
{
	D3DRENDERSTATE_ZFUNC,D3DCMP_EQUAL,
	D3DRENDERSTATE_ZWRITEENABLE,FALSE,	
	//
	D3DRENDERSTATE_FORCE_DWORD,			NULL
};

unsigned long normalAlphaCmpRS[] = 
{
	D3DRENDERSTATE_ALPHAREF,			0,
	D3DRENDERSTATE_ALPHAFUNC,			D3DCMP_NOTEQUAL,
	//
	D3DRENDERSTATE_FORCE_DWORD,			NULL
};

unsigned long tightAlphaCmpRS[] = 
{
	D3DRENDERSTATE_ALPHAREF,			0x64,
	D3DRENDERSTATE_ALPHAFUNC,			D3DCMP_GREATER,
	//
	D3DRENDERSTATE_FORCE_DWORD,			NULL
};

unsigned long normalZRS[] = 
{
	D3DRENDERSTATE_ZFUNC,D3DCMP_LESSEQUAL,
	D3DRENDERSTATE_ZWRITEENABLE,TRUE,	
	D3DRENDERSTATE_ZENABLE,TRUE,
	//
	D3DRENDERSTATE_FORCE_DWORD,			NULL
};

unsigned long xluZRS[] = 
{
	D3DRENDERSTATE_ZFUNC,D3DCMP_LESSEQUAL,
	D3DRENDERSTATE_ZWRITEENABLE,FALSE,	
	D3DRENDERSTATE_ZENABLE,TRUE,
	//
	D3DRENDERSTATE_FORCE_DWORD,			NULL
};

unsigned long xluSubRS[] = 
{
	D3DRENDERSTATE_SRCBLEND,D3DBLEND_ZERO,
	D3DRENDERSTATE_DESTBLEND,D3DBLEND_INVSRCCOLOR,
	//
	D3DRENDERSTATE_FORCE_DWORD,			NULL
};

unsigned long xluAddRS[] = 
{
	D3DRENDERSTATE_SRCBLEND,D3DBLEND_SRCALPHA,
	D3DRENDERSTATE_DESTBLEND,D3DBLEND_ONE,	
	//
	D3DRENDERSTATE_FORCE_DWORD,			NULL
};

unsigned long xluSemiRS[] = 
{
	D3DRENDERSTATE_SRCBLEND,D3DBLEND_SRCALPHA,
	D3DRENDERSTATE_DESTBLEND,D3DBLEND_INVSRCALPHA,
	//
	D3DRENDERSTATE_FORCE_DWORD,			NULL
};

/*  --------------------------------------------------------------------------------
    Function      : DrawBatchedPolys
	Purpose       :	-
	Parameters    : -
	Returns       : -
	Info          : -
*/

void ClearSoftwareSortBuffer(void)
{
	numSoftPolys = 0;
	numSoftVertex = 0;
	for (int i=0; i<MA_SOFTWARE_DEPTH; i++)
		softDepthBuffer[i] = 0;
}

/*  --------------------------------------------------------------------------------
    Function      : DrawBatchedPolys
	Purpose       :	-
	Parameters    : -
	Returns       : -
	Info          : -
*/

void InitFrames(void)
{
	for (int i=0; i<MA_MAX_FRAMES; i++)
	{
		SwapFrame(i);
		BlankFrame;
	}
	SwapFrame(0);
	numHaloPoints = 0;
	ClearSoftwareSortBuffer();
}

/*  --------------------------------------------------------------------------------
    Function      : DrawBatchedPolys
	Purpose       :	-
	Parameters    : -
	Returns       : -
	Info          : -
*/

void PushPolys_Software( D3DTLVERTEX *v, int vC, short *fce, long fC, LPDIRECTDRAWSURFACE7 tSrf )
{
	long i,zVal;
	SOFTPOLY *m;

	for (i=0; i<fC; i+=3)
	{
		m = &softPolyBuffer[numSoftPolys++];
		m->f[0] = fce[i]+numSoftVertex;
		m->f[1] = fce[i+1]+numSoftVertex;
		m->f[2] = fce[i+2]+numSoftVertex;
		m->t = tSrf;

		zVal = v[fce[i]].sz * MA_SOFTWARE_DEPTH;		
		zVal += v[fce[i+1]].sz * MA_SOFTWARE_DEPTH;		
		zVal >>= 1;

		if (softDepthBuffer[zVal])
			m->next = softDepthBuffer[zVal];
		else
			m->next = NULL;
		
		softDepthBuffer[zVal] = m;
	}
}

/*  --------------------------------------------------------------------------------
    Function      : DrawBatchedPolys
	Purpose       :	-
	Parameters    : -
	Returns       : -
	Info          : -
*/

void PushPolys( D3DTLVERTEX *v, int vC, short *fce, long fC, LPDIRECTDRAWSURFACE7 tSrf )
{
	long cnt;
	short *mfce = fce;

	// Push for software/sorting...
/*	PushPolys_Software(v,vC,fce,fC,tSrf );
	memcpy(&softV[numSoftVertex],v,vC*sizeof(D3DTLVERTEX));
	numSoftVertex+=vC;
*/	
	// discard excess polys
	if ((cFInfo->nV + vC) > MA_MAX_VERTICES || (cFInfo->nF + fC) > MA_MAX_FACES)
		return;

	for (cnt=0;cnt<fC; cnt++)
	{
		*cFInfo->cF = (unsigned short)((*mfce) + cFInfo->nV);
		*cFInfo->cT = tSrf;
		cFInfo->cF++;
		cFInfo->cT++;
		mfce++;
	}

	memcpy(cFInfo->cV,v,vC*sizeof(D3DTLVERTEX));
	
	cFInfo->cV+=vC;
	cFInfo->nV+=vC;
	cFInfo->nF+=fC;
}

//#endif

/*  --------------------------------------------------------------------------------
    Function      : DrawBatchedPolys
	Purpose       :	-
	Parameters    : -
	Returns       : -
	Info          : -
*/

void AddHalo(MDX_VECTOR *point)
{
	SetVector(&haloPoints[numHaloPoints++], point);
}

/*  --------------------------------------------------------------------------------
    Function      : DrawBatchedPolys
	Purpose       :	-
	Parameters    : -
	Returns       : -
	Info          : -
*/


void DrawSoftwarePolys (void)
{
	SOFTPOLY *cur;
	for (int i=MA_SOFTWARE_DEPTH-1; i>0; i--)
	{
		cur = (SOFTPOLY *)softDepthBuffer[i];
		while (cur)
		{
			pDirect3DDevice->SetTexture(0,cur->t);
		
			while(pDirect3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,D3DFVF_TLVERTEX,softV,numSoftVertex,cur->f,3,D3DDP_WAIT)!=D3D_OK)
			{
			}

			pDirect3DDevice->SetTexture(0,0);
			cur = cur->next;
		}
	}
}

/*  --------------------------------------------------------------------------------
    Function      : DrawBatchedPolys
	Purpose       :	-
	Parameters    : -
	Returns       : -
	Info          : -
*/

unsigned long numSeperates;

void DrawBatchedPolys (void)
{
	unsigned long i;
	unsigned long nFace;
	LPDIRECTDRAWSURFACE7 lSurface;

	cFInfo->cF = cFInfo->f;
	cFInfo->cT = cFInfo->t;

	nFace = 3;
	i=0;

	numSeperates = 0;

	while (i<cFInfo->nF)
	{
		lSurface = *cFInfo->cT;
		nFace = 0;

		while (((*(cFInfo->cT)) == lSurface) && (i<cFInfo->nF))
		{
			cFInfo->cT+=3;
			nFace+=3;
			i+=3;
		}
		
		pDirect3DDevice->SetTexture(0,lSurface);
		
		numSeperates++;

		while(pDirect3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,D3DFVF_TLVERTEX,cFInfo->v,cFInfo->nV,cFInfo->cF,nFace,D3DDP_WAIT)!=D3D_OK)
		{
			// Idle Time
		}

		pDirect3DDevice->SetTexture(0,0);
		
		cFInfo->cF+=nFace;
	}

}

/*	--------------------------------------------------------------------------------
	Function		: DrawFlatRect
	Purpose			: draw a flat rectangle
	Parameters		: 
	Returns			: 
	Info			: 
*/

void DrawFlatRect(RECT r, D3DCOLOR colour)
{
	D3DLVERTEX v[4] = {
		{
			r.left,r.top,0,0,
			colour,D3DRGBA(0,0,0,1),
			0,0
		},
		{
			r.left,r.bottom,0,0,
			colour,D3DRGBA(0,0,0,1),
			0,0
		},
		{
			r.right,r.bottom,0,0,
			colour,D3DRGBA(0,0,0,1),
			0,0
		},
		{
			r.right,r.top,0,0,
			colour,D3DRGBA(0,0,0,1),
			0,0
	}};


	pDirect3DDevice->SetTexture(0,NULL);
	pDirect3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE,0);
	pDirect3DDevice->SetRenderState(D3DRENDERSTATE_CULLMODE,D3DCULL_NONE);
	pDirect3DDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE,0);

	pDirect3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,TRUE);

	if (pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,D3DFVF_TLVERTEX,v,4,D3DDP_WAIT)!=D3D_OK)
		dp("Could not draw flat rectangle\n");
	
//	pDirect3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,FALSE);
}

/*	--------------------------------------------------------------------------------
	Function		: DrawFlatRect
	Purpose			: draw a flat rectangle
	Parameters		: 
	Returns			: 
	Info			: 
*/

void DrawTexturedRect(RECT r, D3DCOLOR colour, LPDIRECTDRAWSURFACE7 tex, float u0, float v0, float u1, float v1)
{

	if ((r.left>clx1) || (r.top>cly1) || (r.right<clx0) || (r.bottom<cly0))
		return;
		
	if (r.left<clx0)
	{
		u0 += ((clx0 - r.left)/((float)r.right-r.left)) * (u1-u0);
		r.left = clx0;
		
	}

	if (r.top<cly0)
	{
		v0 += ((cly0 - r.top)/((float)r.bottom-r.top)) * (v1-v0);
		r.top = cly0;
		
	}

	if (r.right>clx1)
	{
		u1 += ((clx1 - r.right)/((float)r.right-r.left)) * (u1-u0);
		r.right = clx1;		
	}

	if (r.bottom>cly1)
	{
		v1 += ((cly1 - r.bottom)/((float)r.bottom-r.top))*(v1-v0);
		r.bottom = cly1;		
	}

	D3DLVERTEX v[4] = {
		{
			r.left,r.top,0,0,
			colour,D3DRGBA(0,0,0,0),
			u0,v0
		},
		{
			r.left,r.bottom,0,0,
			colour,D3DRGBA(0,0,0,0),
			u0,v1
			},
		{
			r.right,r.bottom,0,0,
			colour,D3DRGBA(0,0,0,0),
			u1,v1
		},
		{
			r.right,r.top,0,0,
			colour,D3DRGBA(0,0,0,0),
			u1,v0
	}};

	
	pDirect3DDevice->SetTexture(0,tex);
	pDirect3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE,0);
	pDirect3DDevice->SetRenderState(D3DRENDERSTATE_CULLMODE,D3DCULL_NONE);
	pDirect3DDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE,0);

	pDirect3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,TRUE);
	
	pDirect3DDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTFN_POINT);  
	pDirect3DDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTFN_POINT);

	while ((pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,D3DFVF_TLVERTEX,v,4,D3DDP_WAIT)!=D3D_OK));
			
	pDirect3DDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTFN_LINEAR);  
	pDirect3DDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTFN_LINEAR);
	
//	pDirect3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,FALSE);
	pDirect3DDevice->SetTexture(0,NULL);
}

/*	--------------------------------------------------------------------------------
	Function		: DrawFlatRec
	Purpose			: draw a flat rectangle
	Parameters		: 
	Returns			: 
	Info			: 
*/

void BlankAllFrames(void)
{	
	for (int i=MA_FRAME_NORMAL; i<MA_MAX_FRAMES; i++)
	{
		SwapFrame(i);
		BlankFrame;
	}	
	numHaloPoints = 0;
	
	ClearSoftwareSortBuffer();
}

/*	--------------------------------------------------------------------------------
	Function		: DrawFlatRect
	Purpose			: draw a flat rectangle
	Parameters		: 
	Returns			: 
	Info			: 
*/

void StoreHaloPoints(void)
{
	DDSURFACEDESC2		ddsd;
	MDX_VECTOR v;

	DDINIT(ddsd);
	while (surface[ZBUFFER_SRF]->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_NOSYSLOCK,0)!=DD_OK);
	
	ddsd.lPitch /= sizeof(short);

	for (int i=0; i<numHaloPoints; i++)
	{
		XfmPoint(&v,&haloPoints[i],NULL);
		haloPoints[i].vx = (unsigned long)v.vx;
		haloPoints[i].vy = (unsigned long)v.vy;
		haloPoints[i].vz = v.vz;
		
		if (haloPoints[i].vz)
			haloZVals[i] = ((short *)ddsd.lpSurface)[(unsigned long)(haloPoints[i].vx+(haloPoints[i].vy*ddsd.lPitch))];
	}

	surface[ZBUFFER_SRF]->Unlock(NULL);
}

/*	--------------------------------------------------------------------------------
	Function		: DrawFlatRect
	Purpose			: draw a flat rectangle
	Parameters		: 
	Returns			: 
	Info			: 
*/

void CheckHaloPoints(void)
{
	DDSURFACEDESC2		ddsd;
	MDX_VECTOR v;
	unsigned long i;

	DDINIT(ddsd);
	while (surface[ZBUFFER_SRF]->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR| DDLOCK_NOSYSLOCK,0)!=DD_OK);
	
	ddsd.lPitch /= sizeof(short);

	for (i=0; i<numHaloPoints; i++)
		if (haloPoints[i].vz)
			if (haloZVals[i] == ((short *)ddsd.lpSurface)[(unsigned long)(haloPoints[i].vx+(haloPoints[i].vy*ddsd.lPitch))])
				haloPoints[i].vz = 0;
	
	surface[ZBUFFER_SRF]->Unlock(NULL);
}

/*	--------------------------------------------------------------------------------
	Function		: DrawFlatRect
	Purpose			: draw a flat rectangle
	Parameters		: 
	Returns			: 
	Info			: 
*/

LPDIRECTDRAWSURFACE7 haloS;

void DrawHalos(void)
{
	RECT r;
	D3DSetupRenderstates(xluAddRS);

	for (int i=0; i<numHaloPoints; i++)
	{
		if (haloPoints[i].vz)
		{
			r.left = haloPoints[i].vx - 30;
			r.right = haloPoints[i].vx + 30;

			r.top = haloPoints[i].vy - 30;
			r.bottom = haloPoints[i].vy + 30;

			DrawTexturedRect(r,D3DRGBA(1,1,1,1),haloS,FULL_TEXTURE);
		}
	}

}

/*	--------------------------------------------------------------------------------
	Function		: DrawFlatRect
	Purpose			: draw a flat rectangle
	Parameters		: 
	Returns			: 
	Info			: 
*/

unsigned long drawLighting = 0;
unsigned long drawPhong = 0;

void DrawAllFrames(void)
{
/*	pDirect3DDevice->BeginScene();
	DrawSoftwarePolys();
	pDirect3DDevice->EndScene();
*/
	pDirect3DDevice->BeginScene();
	
	// Draw Normal Polys
	SwapFrame(MA_FRAME_NORMAL);
	DrawBatchedPolys();
	
	pDirect3DDevice->EndScene();
	
	// Need to test the ZBuffer on these polys...
	StoreHaloPoints();

	pDirect3DDevice->BeginScene();

	SwapFrame(MA_FRAME_GLOW);
	DrawBatchedPolys();

	pDirect3DDevice->EndScene();

	CheckHaloPoints();
	
	pDirect3DDevice->BeginScene();

	D3DSetupRenderstates(tightAlphaCmpRS);

	if (drawLighting || drawPhong)
	{
		// Draw Lightingmap polys
		D3DSetupRenderstates(lightingMapRS);
	
		if (drawLighting)
		{
			// First normal lighting
			D3DSetupRenderstates(xluSubRS);
			SwapFrame(MA_FRAME_LIGHTMAP);
			DrawBatchedPolys();
		}

		if (drawPhong)
		{
			// Then Phong
			D3DSetupRenderstates(xluAddRS);
			SwapFrame(MA_FRAME_PHONG);
			DrawBatchedPolys();
		}

		// Switch Back to normal
		
	}
	
	D3DSetupRenderstates(xluZRS);
	D3DSetupRenderstates(xluSemiRS);
	SwapFrame(MA_FRAME_XLU);
	DrawBatchedPolys();	

	D3DSetupRenderstates(xluAddRS);	
	SwapFrame(MA_FRAME_ADDITIVE);
	DrawBatchedPolys();	
	
	D3DSetupRenderstates(normalZRS);
	D3DSetupRenderstates(normalAlphaCmpRS);	

	DrawHalos();
	D3DSetupRenderstates(xluSemiRS);
	
	pDirect3DDevice->EndScene();
}

}