
#include "specfx.h"
#include "Eff_Draw.h"
#include "cam.h"





void DrawSpecialFX()
{
	if ( ( gameState.mode == INGAME_MODE ) || ( gameState.mode == PAUSE_MODE ) ||
		 ( gameState.mode == CAMEO_MODE ) )
	{
		ProcessShadows();
		
		if( specFXList.numEntries )
		{
			SPECFX *fx;
			for( fx = specFXList.head.next; fx != &specFXList.head; fx = fx->next )
				if( fx->Draw )
					fx->Draw( fx );
		}

// 		for( i=0; i<NUM_FROGS; i++ )
// 			if( tongue[i].flags & TONGUE_BEINGUSED )
// 				DrawTongue( &tongue[i] );
	}
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
// 	VECTOR vec;
// 	ENEMY *nme;
// 	PLATFORM *plat;
// 	GARIB *garib;
// 	int i;
// 	long tex;
// 	float height;
// 	
// 	tex = (long)((TEXENTRY *)txtrSolidRing)->hdl;
// 
// 	for( i=0; i<NUM_FROGS; i++ )
// 		if( frog[i]->actor->shadow && frog[i]->draw && frog[i]->actor->shadow->draw )
// 		{
// 			SubVector( &vec, &frog[i]->actor->pos, &currTile[i]->centre );
// 			height = DotProduct( &vec, &currTile[i]->normal );
// 			DrawShadow( &frog[i]->actor->pos, &currTile[i]->normal, frog[i]->actor->shadow->radius/max(height*0.02, 1), -height+1, frog[i]->actor->shadow->alpha/max(height*0.02, 1), tex );
// 		}
// 
// 	//------------------------------------------------------------------------------------------------
// 
// 	// process enemy shadows
// 	for(nme = enemyList.head.next; nme != &enemyList.head; nme = nme->next)
// 	{
// 		if( !nme->active || !nme->nmeActor )
// 			continue;
// 
// 		if(nme->nmeActor->actor->shadow && nme->inTile && nme->nmeActor->distanceFromFrog < ACTOR_DRAWDISTANCEINNER)
// 		{
// 			SubVector( &vec, &nme->nmeActor->actor->pos, &nme->inTile->centre );
// 			height = DotProduct( &vec, &nme->inTile->normal );
// 
// 			if (nme->path->nodes[nme->path->fromNode].worldTile==nme->inTile)
// 				DrawShadow( &nme->nmeActor->actor->pos, &nme->inTile->normal, nme->nmeActor->actor->shadow->radius/max(height*0.02, 1), -height+1, nme->nmeActor->actor->shadow->alpha/max(height*0.02, 1), tex );
// 			else
// 				DrawShadow( &nme->nmeActor->actor->pos, &nme->inTile->normal, nme->nmeActor->actor->shadow->radius/max(height*0.02, 1), -height+1, nme->nmeActor->actor->shadow->alpha/max(height*0.02, 1), tex );
// 		}
// 	}
// 
// 	// process platform shadows
// 	for(plat = platformList.head.next; plat != &platformList.head; plat = plat->next)
// 	{
// 		if( !plat->active || !plat->pltActor )
// 			continue;
// 
// 		if(plat->pltActor->actor->shadow && plat->inTile && plat->pltActor->distanceFromFrog < ACTOR_DRAWDISTANCEINNER)
// 		{
// 			SubVector( &vec, &plat->pltActor->actor->pos, &plat->inTile[0]->centre );
// 			height = DotProduct( &vec, &plat->inTile[0]->normal );
// 			DrawShadow( &plat->pltActor->actor->pos, &plat->inTile[0]->normal, plat->pltActor->actor->shadow->radius/max(height*0.02, 1), -height+1, plat->pltActor->actor->shadow->alpha/max(height*0.02, 1), tex );
// 		}
// 	}
// 
// 	// process garib shadows
// /*	for(garib = garibCollectableList.head.next; garib != &garibCollectableList.head; garib = garib->next)
// 	{
// 		if(garib->distanceFromFrog < SPRITE_DRAWDISTANCE)
// 		{
// 			vec.v[X] = garib->sprite.pos.v[X];
// 			vec.v[Y] = garib->sprite.pos.v[Y] + garib->sprite.offsetY;
// 			vec.v[Z] = garib->sprite.pos.v[Z];
// 			DrawShadow( &vec, &garib->gameTile->normal, garib->shadow.radius, 0, garib->shadow.alpha, tex );
// 		}
// 	}*/
}




 
 
void DrawShadow( FVECTOR *pos, FVECTOR *normal, fixed size, fixed offset, short alpha, long tex )
{
// 	VECTOR tempVect, m, fwd;
// 	D3DTLVERTEX vT[5];
// 	QUATERNION cross, q, up;
// 	long i, zeroZ=0;
// 	float t;
// 
// 	vT[0].sx = size;
// 	vT[0].sy = offset;
// 	vT[0].sz = size;
// 	vT[0].tu = 0;
// 	vT[0].tv = 0;
// 	vT[0].color = D3DRGBA(0,0,0,alpha/255.0);
// 	vT[0].specular = D3DRGB(0,0,0);
// 
// 	vT[1].sx = size;
// 	vT[1].sy = offset;
// 	vT[1].sz = -size;
// 	vT[1].tu = 0;
// 	vT[1].tv = 1;
// 	vT[1].color = vT[0].color;
// 	vT[1].specular = vT[0].specular;
// 
// 	vT[2].sx = -size;
// 	vT[2].sy = offset;
// 	vT[2].sz = -size;
// 	vT[2].tu = 1;
// 	vT[2].tv = 1;
// 	vT[2].color = vT[0].color;
// 	vT[2].specular = vT[0].specular;
// 	
// 	vT[3].sx = -size;
// 	vT[3].sy = offset;
// 	vT[3].sz = size;
// 	vT[3].tu = 1;
// 	vT[3].tv = 0;
// 	vT[3].color = vT[0].color;
// 	vT[3].specular = vT[0].specular;
// 
// 	// Translate to current fx pos and push
// 	guTranslateF( tMtrx, pos->v[X], pos->v[Y], pos->v[Z] );
// 	PushMatrix( tMtrx );
// 
// 	// Rotate to be around normal
// 	CrossProduct( (VECTOR *)&cross, normal, &upVec );
// 	MakeUnit( (VECTOR *)&cross );
// 	t = DotProduct( normal, &upVec );
// 	cross.w = -acos(t);
// 	GetQuaternionFromRotation( &q, &cross );
// 	QuaternionToMatrix( &q,(MATRIX *)rMtrx);
// 	PushMatrix( rMtrx );
// 
// 	// Transform point by combined matrix
// 	SetMatrix( &dMtrx, &matrixStack.stack[matrixStack.stackPosition] );
// 
// 	for( i=0; i<4; i++ )
// 	{
// 		guMtxXFMF( dMtrx, vT[i].sx, vT[i].sy, vT[i].sz, &tempVect.v[X], &tempVect.v[Y], &tempVect.v[Z] );
// 		XfmPoint( &m, &tempVect );
// 
// 		// Assign back to vT array
// 		vT[i].sx = m.v[X];
// 		vT[i].sy = m.v[Y];
// 		if( !m.v[Z] ) zeroZ++;
// 		else vT[i].sz = (m.v[Z]+DIST+4)*0.00025;
// 	}
// 
// 	memcpy( &vT[4], &vT[0], sizeof(D3DTLVERTEX) );
// 
// 	if( tex && !zeroZ )
// 	{
// 		Clip3DPolygon( vT, tex );
// 		Clip3DPolygon( &vT[2], tex );
// 	}
// 
// 	PopMatrix( ); // Rotation
// 	PopMatrix( ); // Translation
}


		   
void DrawFXRipple( SPECFX *ripple )
{
	FVECTOR tempV[4];
	SVECTOR vT[4];
	IQUATERNION q1, q2, q3;
	fixed t;
	unsigned long colour;
	long i;

	if(ripple->deadCount)
		return;

//PUTS THE SPRITES RGB'S IN COLOUR, FIRST HALVING THEIR VALUES
	colour = ripple->r>>1;
	colour += (ripple->g>>1)<<8;
	colour += (ripple->b>>1)<<16;


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

	Print3D3DSprite ( ripple->tex, vT, colour );
}




void DrawFXRing( SPECFX *ring )
{
 	unsigned long  vx, i, j, colour;
 	SVECTOR vT[4];
	FVECTOR tempV[4];
 	IQUATERNION q1, q2, q3, cross;
 	fixed tilt, t;
 
 	if( ring->deadCount )
 		return;

//PUTS THE SPRITES RGB'S IN COLOUR, FIRST HALVING THEIR VALUES
	colour = ring->r>>1;
	colour += (ring->g>>1)<<8;
	colour += (ring->b>>1)<<16;

 
// Rotate around axis
 	SetVectorFF((FVECTOR *)&q1, &ring->normal);
 	q1.w = ring->angle;
 	fixedGetQuaternionFromRotation( &q2, &q1 );
 
// Rotate to be around normal
 	CrossProductFFF((FVECTOR *)&cross, (FVECTOR *)&q1, &upVec );
 	MakeUnit((FVECTOR *)&cross );
	t = DotProductFF( (FVECTOR *)&q1, &upVec );
	cross.w = -arccos(t);
 	fixedGetQuaternionFromRotation( &q3, &cross );

// Combine the rotations
 	fixedQuaternionMultiply( &q1, &q2, &q3 );
 
	for( i=0,vx=0; i < NUM_RINGSEGS; i++,vx+=4 )
	{
		memcpy( tempV, &ringVtx[vx], sizeof(FVECTOR)*4 );

// Transform to proper coords
 		for( j=0; j<4; j++ )
 		{
// Slant the polys
 			tilt = ((j < 2) ? ring->tilt : 4096);

			tempV[j].vx	= FMul(tempV[j].vx,FMul(tilt,ring->scale.vx));
			tempV[j].vy	= FMul(tempV[j].vy,FMul(tilt,ring->scale.vy));
			tempV[j].vz = FMul(tempV[j].vz,FMul(tilt,ring->scale.vz));

			RotateVectorByQuaternionFF(&tempV[j],&tempV[j],&q1);

			SetVectorSF(&vT[j],&tempV[j]);
//add world coords 
			vT[j].vx += ring->origin.vx;
			vT[j].vy += ring->origin.vy;
			vT[j].vz += ring->origin.vz;
 		}
		Print3D3DSprite ( ring->tex, vT, colour );
 	}
}
 




void DrawFXTrail( SPECFX *trail )
{
 	unsigned long colour, i = trail->start;
 	SVECTOR vT[4], vTPrev[2];
 
 	if( trail->deadCount || (trail->start == trail->end) )
 		return;


//PUTS THE SPRITES RGB'S IN COLOUR, FIRST HALVING THEIR VALUES
	colour = trail->r>>1;
	colour += (trail->g>>1)<<8;
	colour += (trail->b>>1)<<16; 
 
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

		Print3D3DSprite ( trail->tex, vT, colour );
 
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

		SubVectorFFS( &normal, &currCamSource[0], &pos );
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


// 	VECTOR tempVect, m;
 	SVECTOR	 vT[5], vTPrev[2];
// 	TEXENTRY *tEntry;
 	long i=0;
 	unsigned long colour;
 
  	if( fx->deadCount )
 		return;

//PUTS THE SPRITES RGB'S IN COLOUR, FIRST HALVING THEIR VALUES
	colour = fx->r>>1;
	colour += (fx->g>>1)<<8;
	colour += (fx->b>>1)<<16; 

 
 	while( i < fx->numP-1 )
 	{
// Copy in previous transformed vertices, if they exist
 		if( i != 0 && vTPrev[0].vz && vTPrev[1].vz )
 			memcpy( vT, vTPrev, sizeof(SVECTOR)*2 );
 		else
 		{
// Otherwise, transform them again 			
	   	vT[0].vx = fx->particles[i].poly[0].vx;
	   	vT[0].vy = fx->particles[i].poly[0].vy;
	   	vT[0].vz = fx->particles[i].poly[0].vz;

		vT[1].vx = fx->particles[i].poly[1].vx;
		vT[1].vy = fx->particles[i].poly[1].vy;
		vT[1].vz = fx->particles[i].poly[1].vz;
 		}
 
 		// Now the next two, to make a quad
    
		vT[2].vx = fx->particles[i+1].poly[1].vx;
		vT[2].vy = fx->particles[i+1].poly[1].vy;
		vT[2].vz = fx->particles[i+1].poly[1].vz;
 
		vT[3].vx = fx->particles[i+1].poly[0].vx;
		vT[3].vy = fx->particles[i+1].poly[0].vy;
		vT[3].vz = fx->particles[i+1].poly[0].vz;
 
 		// Store first 2 vertices of the next segment
 		memcpy( vTPrev, &vT[2], sizeof(SVECTOR)*2 );
 
// 		memcpy( &vT[4], &vT[0], sizeof(D3DTLVERTEX) );
// Draw polys, if they're not clipped

		Print3D3DSprite ( fx->tex, vT, colour );
 
 		i++;
 	} 
 
}
 
 
