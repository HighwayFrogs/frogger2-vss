
#include "particle.h"
#include "specfx.h"
#include "Eff_Draw.h"
#include "cam.h"
#include "enemies.h"
#include "platform.h"
#include "collect.h"
#include "frogmove.h"
#include "main.h"
#include "tongue.h"
#include "map_draw.h"
#include "actor2.h"
#include "fixed.h"
#include "layout.h"


#define gte_stotz_cpu(r)\
asm(\
  "mfc2 %0, $7;"\
  :\
  : "r" (r)\
)

#define addPrimLen(ot,p,len,tempvar) \
{\
	tempvar = *(u_long *)(ot); \
    (*(u_long *)(ot) = (u_long)(p)); \
	tempvar |= (len<<24); \
	*(unsigned long *)(p) = tempvar; \
}

void DrawSpecialFX ( void )
{
	int i;

	ProcessShadows();
	
	if( sfxList.count )
	{
		SPECFX *fx;
		for( fx = sfxList.head.next; fx != &sfxList.head; fx = fx->next )
			if( fx->Draw )
				fx->Draw( fx );
	}

//	for( i=0; i<NUM_FROGS; i++ )
//		if( tongue[i].flags & TONGUE_BEINGUSED )
//			DrawTongue( i );
}




/*	--------------------------------------------------------------------------------
	Function		: ProcessShadows
	Purpose			: processes the shadows
	Parameters		: 
	Returns			: void
	Info			: 
*/
void ProcessShadows()
{
	FVECTOR vec, up;
	SVECTOR pos, tilePos;
	ENEMY *nme;
	PLATFORM *plat;
	long tex, i, scale;
	fixed height, size;
	short alpha;
	
	tex = (long*)txtrSolidRing;

	for( i=0; i<NUM_FROGS; i++ )
		if( frog[i]->actor->shadow && frog[i]->draw && frog[i]->actor->shadow->draw )
		{
			// Get proper positions if a frog is on a platform
			if( currPlatform[i] )
			{
				SetVectorSS( &tilePos, &currPlatform[i]->pltActor->actor->position );
				SetVectorFF( &up, &currPlatform[i]->inTile[0]->normal );
			}
			else
			{
				SetVectorSS( &tilePos, &currTile[i]->centre );
				SetVectorFF( &up, &currTile[i]->normal );
			}

			// Calculate height above ground
			SetVectorSS( &pos, &frog[i]->actor->position );
			SubVectorFSS( &vec, &pos, &tilePos );
			height = DotProductFF( &vec, &up );

			scale = max( FMul(height,8), 4096 );

			size = FDiv( frog[i]->actor->shadow->radius, scale );
			alpha = FDiv( ToFixed(frog[i]->actor->shadow->alpha), scale )>>13;

			DrawShadow( &pos, &up, size>>12, -(height>>12)+10, alpha, tex );
		}

	//------------------------------------------------------------------------------------------------

	// process enemy shadows
	for(nme = enemyList.head.next; nme != &enemyList.head; nme = nme->next)
	{
		if( !nme->active || !nme->nmeActor )
			continue;

		if(nme->nmeActor->actor->shadow && nme->inTile && !nme->nmeActor->clipped )
		{
			SetVectorSS( &pos, &nme->nmeActor->actor->position );
			SetVectorSS( &tilePos, &nme->inTile->centre );
			SetVectorFF( &up, &nme->inTile->normal );

			SubVectorFSS( &vec, &pos, &tilePos );
			height = DotProductFF( &vec, &up );

			scale = max( FMul(height,8), 4096 );

			size = FDiv( nme->nmeActor->actor->shadow->radius, scale );
			alpha = FDiv( ToFixed(nme->nmeActor->actor->shadow->alpha), scale )>>13;

			if (nme->path->nodes[nme->path->fromNode].worldTile==nme->inTile)
				DrawShadow( &pos, &up, size>>12, -(height>>12)+10, alpha, tex );
			else
				DrawShadow( &pos, &up, size>>12, -(height>>12)+10, alpha, tex );
		}
	}

	// process platform shadows
	for(plat = platformList.head.next; plat != &platformList.head; plat = plat->next)
	{
		if( !plat->active || !plat->pltActor )
			continue;

		if(plat->pltActor->actor->shadow && plat->inTile[0] && !plat->pltActor->clipped )
		{
			SetVectorSS( &pos, &plat->pltActor->actor->position );
			SetVectorSS( &tilePos, &plat->inTile[0]->centre );
			SetVectorFF( &up, &plat->inTile[0]->normal );

			SubVectorFSS( &vec, &pos, &tilePos );
			height = DotProductFF( &vec, &up );

			scale = max( FMul(height,8), 4096 );

			size = FDiv( plat->pltActor->actor->shadow->radius, scale );
			alpha = FDiv( ToFixed(plat->pltActor->actor->shadow->alpha), scale )>>13;

			DrawShadow( &pos, &up, size>>12, -(height>>12)+10, alpha, tex );
		}
	}
}







void DrawShadow( SVECTOR *pos, FVECTOR *normal, long size, long offset, long alpha, long tex )
{
	FVECTOR tempV[4];
	SVECTOR vT[4];
	IQUATERNION cross, q;
	long i, colour = 8421504;
	fixed t;
	static SPECFX fx;

//PUTS THE SPRITES RGB'S IN COLOUR, FIRST HALVING THEIR VALUES
//	colour = 255>>1;// 191;;
//	colour += 255>>1 <<8;//255<<8;
//	colour += 255>>1 <<16;//0<<16;

	vT[0].vx = size;
	vT[0].vy = offset;
	vT[0].vz = size;
	
	vT[1].vx = size;
	vT[1].vy = offset;
	vT[1].vz = -size;

	vT[2].vx = -size;
	vT[2].vy = offset;
	vT[2].vz = -size;
	
	vT[3].vx = -size;
	vT[3].vy = offset;
	vT[3].vz = size;

	// Rotate to be around normal
	CrossProductFFF( (FVECTOR *)&cross, normal, &upVec );
	MakeUnit( (FVECTOR *)&cross );
	t = DotProductFF( normal, &upVec );
	cross.w = -arccos(t);
	fixedGetQuaternionFromRotation( &q, &cross );

	for( i=3; i>=0; i-- )
	{
		SetVectorFS(&tempV[i],&vT[i]);
	   	RotateVectorByQuaternionFF(&tempV[i],&tempV[i],&q);
	  	SetVectorSF(&vT[i],&tempV[i]);								

		AddToVectorSS( &vT[i], pos );
	}

	fx.tex = tex;
	fx.flags = SPRITE_SUBTRACTIVE;
	fx.a = alpha;
	fx.zDepthOff = -18;	//bring the shadow closer to the camera
	fx.r = fx.g = fx.b = 0;

	Print3D3DSprite( &fx, vT );
}


		   
void DrawFXDecal( SPECFX *ripple )
{
	FVECTOR tempV[4];
	SVECTOR vT[4];
	IQUATERNION q1, q2, q3;
	fixed t;
	unsigned long colour;
	long i;

//PUTS THE SPRITES RGB'S IN COLOUR, FIRST HALVING THEIR VALUES
//	colour = ripple->r>>1;
//	colour += (ripple->g>>1)<<8;
//	colour += (ripple->b>>1)<<16;

//set the size of the effect
	vT[0].vx = (ripple->scale.vx>>12);
	vT[0].vy = 0;
	vT[0].vz = (ripple->scale.vz>>12);

	vT[1].vx = (ripple->scale.vx>>12);
	vT[1].vy = 0;
	vT[1].vz = (-ripple->scale.vz>>12);

	vT[2].vx = (-ripple->scale.vx>>12) ;
	vT[2].vy = 0;
	vT[2].vz = (-ripple->scale.vz>>12) ;
	
	vT[3].vx = (-ripple->scale.vx>>12);
	vT[3].vy = 0;
	vT[3].vz = (ripple->scale.vz>>12) ;


// Rotate to be around normal
	CrossProductFFF((FVECTOR *)&q1, &ripple->normal, &upVec ); 
	MakeUnit((FVECTOR *)&q1 );								   
	t = DotProductFF( &ripple->normal, &upVec );			  	   
	q1.w = -arccos(t);								   
	fixedGetQuaternionFromRotation( &q2, &q1 );		   

	if( ripple->type == FXTYPE_GARIBCOLLECT )		   
	{												   
		// Rotate around axis						   
		SetVectorFF((FVECTOR *)&q1, &ripple->normal ); 
		q1.w = ripple->angle;						   
		fixedGetQuaternionFromRotation( &q3, &q1 );	   
		fixedQuaternionMultiply( &q1, &q2, &q3 );	   
	}												   
	else SetQuaternion( &q1, &q2 );					   


	for( i=3; i>=0; i-- )
	{

		SetVectorFS(&tempV[i],&vT[i]);
	   	RotateVectorByQuaternionFF(&tempV[i],&tempV[i],&q1);
	  	SetVectorSF(&vT[i],&tempV[i]);								

	//add world coords 
		vT[i].vx += ripple->origin.vx;
		vT[i].vy += ripple->origin.vy;
		vT[i].vz += ripple->origin.vz;

	}

	Print3D3DSprite ( ripple, vT );
}




void DrawFXRing(SPECFX *fx)
{
	unsigned long vx, i, j, vxj;
	VERT vT[5], vTPrev[2];
	TextureType *tEntry;
	FVECTOR scale, normal;
	VECTOR tempVect;
	SVECTOR pos, m;
	IQUATERNION q1, q3, cross;
	SVECTOR fxpos;
	fixed tilt, tilt2, t;
	int zeroZ = 0;
	int r,g,b;

	MATRIX tMtrx, rMtrx, sMtrx, tempMtrx;

	if( !(tEntry = fx->tex) )
		return;

	SetVectorFF(&scale, &fx->scale);
	SetVectorFF(&normal, &fx->normal);
	SetVectorSS(&pos, &fx->origin);

//	ScaleVectorFF( &scale, 410 );
//	ScaleVectorFF( &pos, 410 );


	// Translate to current fx pos and push
//	guTranslateF( tMtrx, pos.vx, pos.vy, pos.vz );
//	PushMatrix( tMtrx );
	tMtrx = GsIDMATRIX;
	tMtrx.t[0] = -pos.vx;
	tMtrx.t[1] = -pos.vy;
	tMtrx.t[2] = pos.vz;

	// Rotate around axis
//	SetVector( (MDX_VECTOR *)&q1, &normal );
	SetVectorFF((FVECTOR*)&q1, &normal);

	// Rotate to be around normal
	CrossProductFFF((FVECTOR*)&cross, (FVECTOR*)&q1, &upVec);
	MakeUnit((FVECTOR *)&cross);
	t = DotProductFF((FVECTOR*)&q1, &upVec);
	cross.w = -arccos(t);
//	GetQuaternionFromRotation( &q3, &cross );
	fixedGetQuaternionFromRotation(&q3, &cross);

	// Combine the rotations and push
//	QuaternionToMatrix( &q3,(MDX_MATRIX *)rMtrx);
//	PushMatrix( rMtrx );
	QuatToPSXMatrix(&q3, &rMtrx);

//	if(fx->type == FXTYPE_CROAK)
//		SwapFrame(3);

//	tilt2 = (float)fx->tilt*0.000244;
	tilt2 = fx->tilt;


/*	vT[0].tu = tEntry->u0;
	vT[0].tv = tEntry->v1;
	vT[1].tu = tEntry->u0;
	vT[1].tv = tEntry->v0;
	vT[2].tu = tEntry->u1;
	vT[2].tv = tEntry->v0;
	vT[3].tu = tEntry->u1;
	vT[3].tv = tEntry->v1;
*/
	r = (fx->r*fx->a) >>8;
	g = (fx->g*fx->a) >>8;
	b = (fx->b*fx->a) >>8;

	vT[0].tu = tEntry->u2;
	vT[0].tv = tEntry->v2;
	vT[1].tu = tEntry->u0;
	vT[1].tv = tEntry->v0;
	vT[2].tu = tEntry->u1;
	vT[2].tv = tEntry->v1;
	vT[3].tu = tEntry->u3;
	vT[3].tv = tEntry->v3;


	for( i=0,vx=0; i < NUM_RINGSEGS; i++,vx+=2 )
	{
		// Transform to proper coords
		for( j=0,zeroZ=0; j<4; j++ )
		{
			if(i && j<2 && vTPrev[0].vz && vTPrev[1].vz)
				memcpy(vT, vTPrev, sizeof(VERT)*2);
			else
			{
				FVECTOR tempFVect;
				int flg;

				vxj = (vx+j)%(NUM_RINGSEGS<<1);
//				vT[j].sx = ringVtx[vxj].vx*0.000244;
//				vT[j].sy = ringVtx[vxj].vy*0.000244;
//				vT[j].sz = ringVtx[vxj].vz*0.000244;
				vT[j].vx = ringVtx[vxj].vx;//*0.000244;
				vT[j].vy = ringVtx[vxj].vy;//*0.000244;
				vT[j].vz = ringVtx[vxj].vz;//*0.000244;

				// Slant the polys
//				tilt = (!(i&1)?(j==0||j==3):(j==1||j==2)) ? 1 : tilt2;
				tilt = (!(i&1)?(j==0||j==3):(j==1||j==2)) ? 4096 : tilt2;

//				vT[j].tv = 1-vT[j].tv;
				vT[j].tv = tEntry->v2-(vT[j].tv-tEntry->v0);


				// Scale and push
//				guScaleF( sMtrx, tilt*scale.vx, tilt*scale.vy, tilt*scale.vz );
//				PushMatrix( sMtrx );
				//bb - scale was causeing overflows
//				tempFVect.vx = FMul(tilt,scale.vx);
//				tempFVect.vy = FMul(tilt,scale.vy);
//				tempFVect.vz = FMul(tilt,scale.vz);
				tempFVect.vx = FMul(tilt,scale.vx>>4);
				tempFVect.vy = FMul(tilt,scale.vy>>4);
				tempFVect.vz = FMul(tilt,scale.vz>>4);
				sMtrx = GsIDMATRIX;
				ScaleMatrix(&sMtrx, &tempFVect);

				// Transform point by combined matrix
//				MatrixSet( &dMtrx, &matrixStack.stack[matrixStack.stackPosition] );
//				guMtxXFMF( dMtrx, vT[j].sx, vT[j].sy, vT[j].sz, &tempVect.vx, &tempVect.vy, &tempVect.vz );
//				tempVect = vT[j];
				tempVect.vx = -vT[j].vx;
				tempVect.vy = -vT[j].vy;
				tempVect.vz =  vT[j].vz;
				ApplyMatrixLV(&sMtrx, &tempVect, &tempVect);
				ApplyMatrixLV(&rMtrx, &tempVect, &tempVect);
//				ApplyMatrixLV(&tMtrx, &tempVect, &tempVect);
				tempVect.vx -= pos.vx;
				tempVect.vy -= pos.vy;
				tempVect.vz += pos.vz;

				if(j==3)
					SetVectorSV(&fxpos, &tempVect);

//				XfmPoint( &m, &tempVect, NULL );
				gte_SetTransMatrix(&GsWSMATRIX);
				gte_SetRotMatrix(&GsWSMATRIX);
				gte_ldlv0(&tempVect);
				gte_rtps();
				gte_stsxy(&m.vx);
//				gte_stsz(&m.vz);	//screen z/4 as otz
				gte_stszotz(&m.vz);	//screen z/4 as otz
				m.vz>>=2;
				gte_stflg(&flg);	//screen z/4 as otz

				// Assign back to vT array
				vT[j].vx = m.vx;
				vT[j].vy = m.vy;
//				if(!m.vz)
				if( (m.vz<60) || (m.vz>1000) )
//				if(flg)
					zeroZ++;
				else
//					vT[j].sz = (m.vz+DIST)*0.00025;
					vT[j].vz = m.vz;

//				PopMatrix( ); // Pop scale
			}
		}

		if(!zeroZ)
		{
/*			if( (vT[0].vx>-256) && (vT[0].vx<256)
			 && (vT[0].vy>-120) && (vT[0].vy<120)
			 &&	(vT[1].vx>-256) && (vT[1].vx<256)
			 && (vT[1].vy>-120) && (vT[1].vy<120)
			 &&	(vT[2].vx>-256) && (vT[2].vx<256)
			 && (vT[2].vy>-120) && (vT[2].vy<120)
			 &&	(vT[3].vx>-256) && (vT[3].vx<256)
			 && (vT[3].vy>-120) && (vT[3].vy<120) )
*/			{
			
				POLY_FT4 *ft4;
				int width;

				memcpy( vTPrev, &vT[2], sizeof(VERT)*2 );
				memcpy( &vT[4], &vT[0], sizeof(VERT) );
	//			Clip3DPolygon( vT, tEntry );
	//			Clip3DPolygon( &vT[2], tEntry );

			
				//set up a poly
				BEGINPRIM(ft4, POLY_FT4);
				setPolyFT4(ft4);
				ft4->x0 = vT[0].vx;
				ft4->y0 = vT[0].vy;
				ft4->x1 = vT[1].vx;
				ft4->y1 = vT[1].vy;
				ft4->x2 = vT[3].vx;
				ft4->y2 = vT[3].vy;
				ft4->x3 = vT[2].vx;
				ft4->y3 = vT[2].vy;
				ft4->r0 = r;
				ft4->g0 = g;
				ft4->b0 = b;
				ft4->u0 = vT[0].tu;
				ft4->v0 = vT[0].tv;
				ft4->u1 = vT[1].tu;
				ft4->v1 = vT[1].tv;
				ft4->u2 = vT[3].tu;
				ft4->v2 = vT[3].tv;
				ft4->u3 = vT[2].tu;
				ft4->v3 = vT[2].tv;
				ft4->tpage = tEntry->tpage;
				ft4->clut  = tEntry->clut;
				ft4->code  |= 2;//semi-trans on
 				ft4->tpage |= 32;//add
				ENDPRIM(ft4, 1, POLY_FT4);
//				ENDPRIM(ft4, (vT[0].vz+vT[1].vz+vT[2].vz+vT[3].vz)>>3, POLY_FT4);

			
				
				if((i&1) && (actFrameCount&1))
				{
					SPECFX *trail;

					ScaleVector(&fxpos, 10);

					if((trail = CreateSpecialEffect(FXTYPE_TWINKLE, &fxpos, &fx->normal, 81920, 0, 0, 4096)))
					{
						trail->tilt = 8192;
						if(i&2)
							SetFXColour(trail, 180, 220, 180);
						else
							SetFXColour(trail, fx->r, fx->g, fx->b);
					}
				}
			}
		}
		else
		{
			vTPrev[0].vz = vTPrev[1].vz = 0;
		}
	}

//	PopMatrix( ); // Rotation
//	PopMatrix( ); // Translation

//	if( fx->type == FXTYPE_CROAK )
//		SwapFrame(0);
}

 




void DrawFXTrail( SPECFX *trail )
{
 	unsigned long colour, i = trail->start;
 	SVECTOR vT[4], vTPrev[2];
 
 	if( trail->start == trail->end )
 		return;


//PUTS THE SPRITES RGB'S IN COLOUR, FIRST HALVING THEIR VALUES
//	colour = trail->r>>1;
//	colour += (trail->g>>1)<<8;
//	colour += (trail->b>>1)<<16; 
 
 	do
 	{
 		/*********-[ First 2 points ]-********/
 		if( i != trail->start && vTPrev[0].vz && vTPrev[1].vz )
 			memcpy( vT, vTPrev, sizeof(SVECTOR)*2 );			// Previously transformed vertices
 		else
 			CalcTrailPoints( vT, trail, i );
 
 		/*********-[ Next 2 points ]-********/
 		CalcTrailPoints( &vT[2], trail, (i+1)%trail->numP );
 		memcpy( vTPrev, &vT[2], sizeof(SVECTOR)*2 ); 			// Store first 2 vertices of the next segment
 
//add world coords 
		vT[0].vx += trail->origin.vx;
		vT[0].vy += trail->origin.vy;
		vT[0].vz += trail->origin.vz;

		vT[1].vx += trail->origin.vx;
		vT[1].vy += trail->origin.vy;
		vT[1].vz += trail->origin.vz;

		vT[2].vx += trail->origin.vx;
		vT[2].vy += trail->origin.vy;
		vT[2].vz += trail->origin.vz;

		vT[3].vx += trail->origin.vx;
		vT[3].vy += trail->origin.vy;
		vT[3].vz += trail->origin.vz;

		Print3D3DSprite ( trail, vT );
 
 		if( ++i >= trail->numP ) i=0;
 
 	} while( i != trail->end );
}




void CalcTrailPoints( SVECTOR *vT, SPECFX *trail, int i )
{
	FVECTOR tempV[2];
	IQUATERNION q = {0,0,0,4096};
	SVECTOR pos;

	AddVectorSSS( &pos, &trail->origin, &trail->particles[i].pos );

	if( trail->type == FXTYPE_BILLBOARDTRAIL )	// Calculate screen align rotation
	{
		FVECTOR normal;
		IQUATERNION cross;
		fixed t;

		SubVectorFFS( &normal, &currCamSource, &pos );
		MakeUnit( &normal );
		CrossProductFFF( (FVECTOR *)&cross, &normal, &upVec );
		MakeUnit( (FVECTOR *)&cross );
		t = DotProductFF( &normal, &upVec );
		cross.w = arccos(t);
		fixedGetQuaternionFromRotation( &q, &cross );
	}

	vT[0].vx = trail->particles[i].poly[0].vx;
	vT[0].vy = trail->particles[i].poly[0].vy;
	vT[0].vz = trail->particles[i].poly[0].vz;
	vT[1].vx = trail->particles[i].poly[1].vx;
	vT[1].vy = trail->particles[i].poly[1].vy;
	vT[1].vz = trail->particles[i].poly[1].vz;


	SetVectorFS(&tempV[0], &vT[0]);

	RotateVectorByQuaternionFF(&tempV[0],&tempV[0],&q);

	SetVectorSF(&vT[0],&tempV[0]);

	SetVectorFS(&tempV[1], &vT[1]);

	RotateVectorByQuaternionFF(&tempV[1],&tempV[1],&q);

	SetVectorSF(&vT[1],&tempV[1]);

 	ScaleVector(&vT[0],SCALE);
 	ScaleVector(&vT[1],SCALE);

}



							   

void DrawFXLightning( SPECFX *fx )
{
	SVECTOR vT[4], vTPrev[2], tempSvect;
	TextureType *tEntry;
	PARTICLE *p;
	long i=0, otz, clipped, sz;
	POLY_FT4 *ft4;

	if( !(tEntry = (fx->tex)) )
		return;

	if(gameState.mode == LEVELCOMPLETE_MODE)
		return;

	gte_SetTransMatrix(&GsWSMATRIX);
	gte_SetRotMatrix(&GsWSMATRIX);

	p = fx->particles;
	while( i < fx->numP-1 )
	{
		clipped = 0;
		// Copy in previous transformed vertices, if they exist
		if( i && vTPrev[0].vz && vTPrev[1].vz )
			lmemcpy( (long *)vT, (long *)vTPrev, 4 );
		else
		{
			// Otherwise, transform them again
			SetVectorSS(&tempSvect, &p->poly[0]);
			tempSvect.vx = -tempSvect.vx;
			tempSvect.vy = -tempSvect.vy;
			gte_ldv0(&tempSvect);
			gte_rtps();
			gte_stsxy(&vT[0].vx);
			gte_stszotz(&otz);
			vT[0].vz = otz;
			gte_stsz(&sz);
			clipped += (sz<20 || sz>fog.max);

			SetVectorSS(&tempSvect, &p->poly[1]);
			tempSvect.vx = -tempSvect.vx;
			tempSvect.vy = -tempSvect.vy;
			gte_ldv0(&tempSvect);
			gte_rtps();
			gte_stsxy(&vT[1].vx);
			gte_stszotz(&otz);
			vT[1].vz = otz;
			gte_stsz(&sz);
			clipped += (sz<20 || sz>fog.max);
		}

		SetVectorSS(&tempSvect, &p->next->poly[1]);
		tempSvect.vx = -tempSvect.vx;
		tempSvect.vy = -tempSvect.vy;
		gte_ldv0(&tempSvect);
		gte_rtps();
		gte_stsxy(&vT[2].vx);
		gte_stszotz(&otz);
		vT[2].vz = otz;
		gte_stsz(&sz);
		clipped += (sz<20 || sz>fog.max);

		SetVectorSS(&tempSvect, &p->next->poly[0]);
		tempSvect.vx = -tempSvect.vx;
		tempSvect.vy = -tempSvect.vy;
		gte_ldv0(&tempSvect);
		gte_rtps();
		gte_stsxy(&vT[3].vx);
		gte_stszotz(&otz);
		vT[3].vz = otz;
		gte_stsz(&sz);
		clipped += (sz<20 || sz>fog.max);

		// Store first 2 vertices of the next segment
		lmemcpy( (long *)vTPrev, (long *)&vT[3], 2 );
		lmemcpy( (long *)&vTPrev[1], (long *)&vT[2], 2 );

		// Draw polys, if they're not clipped
		if( !clipped )
		{
			BEGINPRIM(ft4, POLY_FT4);
			setPolyFT4(ft4);
			*((long*)&ft4->x0) = *((long*)&vT[0].vx);
			*((long*)&ft4->x1) = *((long*)&vT[1].vx);
			*((long*)&ft4->x2) = *((long*)&vT[2].vx);
			*((long*)&ft4->x3) = *((long*)&vT[3].vx);

			ft4->r0 = (p->r*p->a)>>8;
			ft4->g0 = (p->g*p->a)>>8;
			ft4->b0 = (p->b*p->a)>>8;
			ft4->u0 = tEntry->u0;
			ft4->v0 = tEntry->v0;
			ft4->u1 = tEntry->u1;
			ft4->v1 = tEntry->v1;
			ft4->u2 = tEntry->u2;
			ft4->v2 = tEntry->v2;
			ft4->u3 = tEntry->u3;
			ft4->v3 = tEntry->v3;
			ft4->tpage = tEntry->tpage;
			ft4->clut  = tEntry->clut;

			ft4->code  |= 2;//semi-trans on
 			ft4->tpage |= 32;//add

			ENDPRIM(ft4, (vT[0].vz+vT[1].vz+vT[2].vz+vT[3].vz)>>4, POLY_FT4);
		}

		i++;
		p = p->next;
	} 
}

