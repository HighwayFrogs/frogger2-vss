/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.


	File		: mdxReport.cpp
	Programmer	: Matthew Cloy
	Date		: 

----------------------------------------------------------------------------------------------- */

#include <windows.h>
#include <ddraw.h>
#include <d3d.h>
#include <crtdbg.h>
#include <stdio.h>

#include "mdxDDraw.h"
#include "mdxD3D.h"
#include "mdxTexture.h"
#include "mdxCRC.h"
#include "mgeReport.h"
#include "gelf.h"

#ifdef __cplusplus

extern "C"
{
#endif

MDX_TEXENTRY *texList = NULL;
MDX_TEXPAGE	*texPages = NULL;

#define TEX_PAGE_SIZE 32

unsigned long maxTexturesInPage = ((TEX_PAGE_SIZE/32) * (TEX_PAGE_SIZE/32));

float textureAdjustDivider = (TEX_PAGE_SIZE/32);

unsigned long texXCoords[((TEX_PAGE_SIZE/32) * (TEX_PAGE_SIZE/32))];
unsigned long texYCoords[((TEX_PAGE_SIZE/32) * (TEX_PAGE_SIZE/32))];

MDX_TEXENTRY *GetTexEntryFromCRC (long CRC)
{
	MDX_TEXENTRY *me = texList;
	
	while (me)
	{
		if (me->CRC==CRC)
			return me;
		me=me->next;
	}
	return NULL;
}

MDX_TEXPAGE *GetFreeTexturePage(void)
{
	MDX_TEXPAGE *ret;
	unsigned int i,xCrd,yCrd;

	// If we have some texture pases already.
	if (texPages)
	{
		ret = texPages;
		
		// The texture page is full, start another
		if (ret->numTex >= maxTexturesInPage)
		{
			ret = new MDX_TEXPAGE;
			if ((ret->surf = D3DCreateTexSurface(TEX_PAGE_SIZE,TEX_PAGE_SIZE, 0xf81f, 0, 1)) == NULL)
			{
				dp("Could not allocate a texture page!\n");
				delete ret;
				return NULL;
			}

			ret->next = texPages;
			ret->numTex = 0;			
		}
		texPages = ret;
		return ret;
	}
	
	// We need to make our very first texture page (ahhhhhh)
	ret = new MDX_TEXPAGE;
	
	if ((ret->surf = D3DCreateTexSurface(TEX_PAGE_SIZE,TEX_PAGE_SIZE, 0xf81f, 0, 1)) == NULL)
	{
		dp("Could not allocate a texture page!\n");
		delete ret;
		return NULL;
	}
	ret->next = NULL;
	ret->numTex = 0;

	texPages = ret;

	//May as well calculate tex coords here!

	for (i=0,xCrd = 0,yCrd = 0; i<maxTexturesInPage; i++)
	{
		texXCoords[i] = xCrd;
		texYCoords[i] = yCrd;

		xCrd+=32;
		
		if (xCrd>=TEX_PAGE_SIZE)
		{
			xCrd = 0;
			yCrd+=32;
		}
	}
	return ret;				
}

void AddTextureToTexList(char *file, char *shortn, long finalTex)
{
	char mys[255];
	MDX_TEXENTRY *newE;
	MDX_TEXENTRY *cEntry = texList;
	long isAnim = 0,texType = 0;
	int pptr = -1;

	strcpy (mys,shortn);
	strlwr (mys);
	
	if (strncmp(mys,"ai_",3)==0)
		texType = TEXTURE_AI;

	if (strncmp(mys,"lg_",3)==0)
		texType = TEXTURE_LARGE;

	newE = new MDX_TEXENTRY;
	
	newE->next = texList;
	texList = newE;

	strcpy (newE->name,mys);
	newE->CRC  = UpdateCRC (mys);

	newE->data = (short *)gelfLoad_BMP(file,NULL,(void**)&pptr,NULL,NULL,NULL,GELF_IFORMAT_16BPP555,GELF_IMAGEDATA_TOPDOWN);

	if (newE->data)
	{
		switch (texType)
		{
			case TEXTURE_NORMAL:
			{
				LPDIRECTDRAWSURFACE7 temp;
				MDX_TEXPAGE *page = GetFreeTexturePage();

				newE->surf = page->surf;
					
				// Create a temporary surface to hold the texture.
				if ((temp = D3DCreateTexSurface(32,32, 0xf81f, 0, 1)) == NULL)
					return;

				DDrawCopyToSurface(temp,(unsigned short *)newE->data,0,32,32);

				while (newE->surf->BltFast(texXCoords[page->numTex],texYCoords[page->numTex],temp,NULL,NULL)!=DD_OK);
				newE->xPos = texXCoords[page->numTex];
				newE->yPos = texYCoords[page->numTex];
				
				temp->Release();
				page->numTex++;

				newE->type = TEXTURE_NORMAL;
				break;
			}

			case TEXTURE_AI:
			{
				LPDIRECTDRAWSURFACE7 temp;
								
				// Create a temporary surface to hold the texture.
				if ((temp = D3DCreateTexSurface(64,64, 0xf81f, 0, 1)) == NULL)
					return;

				DDrawCopyToSurface(temp,(unsigned short *)newE->data,0,64,64);			
				newE->surf = temp;
				newE->xPos = 0;
				newE->yPos = 0;
				
				newE->type = TEXTURE_AI;
				break;
			}
			
			case TEXTURE_LARGE:
			{
				LPDIRECTDRAWSURFACE7 temp;
								
				// Create a temporary surface to hold the texture.
				if ((temp = D3DCreateTexSurface(128,128, 0xf81f, 0, 1)) == NULL)
					return;

				DDrawCopyToSurface(temp,(unsigned short *)newE->data,0,128,128);			
				newE->surf = temp;
				newE->xPos = 0;
				newE->yPos = 0;
				
				newE->type = TEXTURE_LARGE;
				break;
			}
		}

	}
	else
		dp("Cannot load texture %s\n",shortn);
}


unsigned long LoadTexBank(char *bank, char *baseDir)
{
	char	fName[MAX_PATH];
	char	fPath[MAX_PATH];

	WIN32_FIND_DATA fData;
	HANDLE			fHandle;

	unsigned long numTextures;

	strcpy (fPath,baseDir);
	strcat (fPath,TEXTURE_BASE);
	strcat (fPath,bank);
	strcat (fPath,"\\");

	strcpy (fName,fPath);
	strcat (fName,"*.bmp");

	fHandle = FindFirstFile (fName,&fData);


	if (fHandle != INVALID_HANDLE_VALUE)
	{
		long ret;
		char finalFile[MAX_PATH];
		char finalShort[MAX_PATH];

		numTextures = 0;
		do
		{
			strcpy (finalFile,fPath);
			strcat (finalFile,fData.cFileName);
			strcpy (finalShort,fData.cFileName);
			ret = FindNextFile (fHandle,&fData);

			AddTextureToTexList (finalFile,finalShort,!ret);			
			numTextures++;
		}
		while (ret);
	
		FindClose (fHandle);
	}
	else
		dp("Could not load bank %s\n",fName);

	return 1;
}

#ifdef __cplusplus
}
#endif
