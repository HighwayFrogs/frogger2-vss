/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: printgfx.h
	Programmer	: Andy Eder
	Date		: 19/04/99 11:12:30

----------------------------------------------------------------------------------------------- */

#ifndef PRINTGFX_H_INCLUDED
#define PRINTGFX_H_INCLUDED


/*#define G_CC_DECALRGB_MODULATEPRIMA 0,0,0,TEXEL0,TEXEL0,0,PRIMITIVE,0
#define G_CC_MODULATEPRIMRGBA TEXEL0,0,PRIMITIVE,0,TEXEL0,0,PRIMITIVE,0
#define G_CC_MODULATERGBPRIMA TEXEL0,0,SHADE,0,0,0,0,PRIMITIVE

#define G_CC_COLOURBLEND_SPRITE		ENVIRONMENT,TEXEL0,ENV_ALPHA,TEXEL0,TEXEL0,0,PRIMITIVE,0
#define G_CC_COLOURBLEND_SPRITE2	COMBINED,0,PRIMITIVE,0,0,0,0,COMBINED

#define G_CC_COLOURBLEND_SPRITE_AFTERLIGHT	TEXEL0,0,PRIMITIVE,0,TEXEL0,0,PRIMITIVE,0
#define G_CC_COLOURBLEND_SPRITE_AFTERLIGHT2	ENVIRONMENT,COMBINED,ENV_ALPHA,COMBINED,0,0,0,COMBINED
*/

#define	G_CC_MYTRILERP		TEXEL1, TEXEL0, LOD_FRACTION, TEXEL0, 0, 0, 0, PRIMITIVE

#define G_CC_MODULATERGB_MODULATEPRIMA  TEXEL0,0,SHADE,0,TEXEL0,0,PRIMITIVE,0
#define G_CC_MODULATERGB_MODULATEPRIMA2 COMBINED,0,SHADE,0,COMBINED,0,PRIMITIVE,0

#define G_CC_MODULATEPRIMRGBA TEXEL0,0,PRIMITIVE,0,TEXEL0,0,PRIMITIVE,0
#define G_CC_MODULATERGBPRIMA TEXEL0, 0, SHADE, 0, 0, 0, 0, PRIMITIVE

#define G_CC_TEXTUREBLEND TEXEL1, TEXEL0, SHADE_ALPHA, TEXEL0, 0, 0, 0, PRIMITIVE
#define G_CC_TEXTUREBLENDENV TEXEL1, TEXEL0, ENV_ALPHA, TEXEL0, 0, 0, 0, PRIMITIVE
#define G_CC_TEXTUREBLEND2 COMBINED, 0, SHADE, 0, 0, 0, 0, PRIMITIVE

#define G_CC_MODULATEPRIM	PRIMITIVE,0,SHADE,0,PRIMITIVE,0,SHADE,0
#define G_CC_MODULATEPRIM_ENVALPHA	PRIMITIVE,0,SHADE,0,ENVIRONMENT,0,SHADE,0

#define G_CC_HILITEFACECOLOUR1	SHADE,0,PRIMITIVE,0,TEXEL0,0,ENVIRONMENT,0
#define G_CC_HILITEFACECOLOUR2	ENVIRONMENT, COMBINED, TEXEL0, COMBINED, 0, 0, 0, COMBINED

#define G_CC_MODULATERGBPRIMA2 COMBINED, 0, SHADE, 0, 0, 0, 0, PRIMITIVE

#define G_CC_HILITERGB_PRIMA	PRIMITIVE, SHADE, TEXEL0, SHADE, TEXEL0, 0, PRIMITIVE,0
#define G_CC_HILITERGB_PRIMA2	ENVIRONMENT, COMBINED, TEXEL0, COMBINED, 0, 0, 0, COMBINED

#define G_CC_COLOURBLEND  ENVIRONMENT,TEXEL0,ENV_ALPHA,TEXEL0,TEXEL0,0,PRIMITIVE,0
#define G_CC_COLOURBLEND2 COMBINED,0,SHADE,0,0,0,0,COMBINED

#define G_CC_COLOURBLEND_AFTERLIGHT  TEXEL0,0,SHADE,0,TEXEL0,0,PRIMITIVE,0
#define G_CC_COLOURBLEND_AFTERLIGHT2 ENVIRONMENT,COMBINED,ENV_ALPHA,COMBINED,0,0,0,COMBINED

#define G_CC_COLOURBLEND_SPRITE  ENVIRONMENT,TEXEL0,ENV_ALPHA,TEXEL0,TEXEL0,0,PRIMITIVE,0
#define G_CC_COLOURBLEND_SPRITE2 COMBINED,0,PRIMITIVE,0,0,0,0,COMBINED

#define G_CC_COLOURBLEND_SPRITE_AFTERLIGHT2 ENVIRONMENT,COMBINED,ENV_ALPHA,COMBINED,0,0,0,COMBINED
#define G_CC_COLOURBLEND_SPRITE_AFTERLIGHT  TEXEL0,0,PRIMITIVE,0,TEXEL0,0,PRIMITIVE,0

#define G_CC_FACECOLOURBLEND  ENVIRONMENT,PRIMITIVE,ENV_ALPHA,PRIMITIVE,0,0,0,PRIMITIVE
#define G_CC_FACECOLOURBLEND2 COMBINED,0,SHADE,0,0,0,0,PRIMITIVE

#define G_CC_FACECOLOURBLEND_AFTERLIGHT  PRIMITIVE,0,SHADE,0,0,0,0,PRIMITIVE
#define G_CC_FACECOLOURBLEND_AFTERLIGHT2 ENVIRONMENT,COMBINED,ENV_ALPHA,COMBINED,0,0,0,PRIMITIVE

#define G_CC_DECALI_MODULATEA_PRIM    TEXEL0, 0, 1, SHADE, TEXEL0, 0, PRIMITIVE, 0

extern Vtx shadowVtx[4];
extern Sprite template_sprite;

extern Gfx rspInitForSprites_dl[];
extern Gfx rdpInitForSprites_dl[];
extern Gfx rspInitForOverlays_dl[];
extern Gfx rdpInitForOverlays_dl[];
extern Gfx croakRing_dl[];
extern Gfx polyNoZ_dl[];

void PrintBackdrops();
void PrintTextAsOverlay(TEXTOVERLAY *tOver);
void PrintOverlays();
void PrintSpriteOverlays();

void DrawSpecialFX();
void DrawFXRipple(SPECFX *ripple);

void ProcessShadows();
void DrawShadow(VECTOR *pos,PLANE *plane,float size,float altitude,short alpha,Vtx *vert,VECTOR *lightDir);


SPRITE *PrintSpritesOpaque();
void PrintSpritesTranslucent(SPRITE *sprite);
void TileRectangle(Gfx **glistp,SPRITE *sprite,f32 x0,f32 y0,int z,int scaleX,int scaleY);
void PrintSprite(SPRITE *sprite);

char IsPointVisible(VECTOR *p);

void ScreenShot();


#endif
