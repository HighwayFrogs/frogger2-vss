/*

	This file is part of the M libraries,

	File		: mliReport.h
	Programmer	: Matthew Cloy
	Date		: 

----------------------------------------------------------------------------------------------- */

#ifndef MDXFONT_H_INCLUDED
#define MDXFONT_H_INCLUDED

#ifdef __cplusplus

extern "C"
{
#endif

typedef struct _MDX_FONT MDX_FONT;

void InitFontSystem(void);
MDX_FONT *InitFont(const char *filename);
long DrawFontCharAtLoc(long x,long y,char c,unsigned long color, MDX_FONT *font,float scale);
long DrawFontStringAtLoc(long x,long y,char *c,unsigned long color, MDX_FONT *font, float scale,long centredX,long centredY);
long CalcStringWidth(const char *string,MDX_FONT *font, float scale);

long WrapStringToArray(const char* str, long maxWidth, char* buffer, long bufferSize, char** array, long arraySize, MDX_FONT *font);

#ifdef __cplusplus
}
#endif

#endif