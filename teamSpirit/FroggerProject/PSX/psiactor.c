/************************************************************************************
	ISL PSX LIBRARY	(c) 1999 Interactive Studios Ltd.

	actor.c			Skinned model control routines

************************************************************************************/

#include <islutil.h>
#include <islmem.h>
//#include "global.h"
#include "actor2.h"
#include "Main.h"
#include "Shell.h"

ACTORLIST	actorList;


#include "bbtimer.h"
#include "temp_psx.h"

//#define min(a,b) (((a) < (b)) ? (a) : (b))
//#define max(a,b) (((a) > (b)) ? (a) : (b))
//#define Bound(a,x,b) min(b,max(a,x))


void actorInitialise()
{
	actorList.numEntries = 0;
	actorList.head.next = actorList.head.prev = &actorList.head;
}



/**************************************************************************
	Function 	: actorAdd()
	Purpose 	: adds an actor to the linked list
	Parameters 	: pointer to actor
	Returns 	: none
	Info 		:
**************************************************************************/

void actorAdd(ACTOR *actor)
{
	ACTOR *ptr;

	ptr = actorList.head.next;

	if(actor->next == NULL)
	{
		actorList.numEntries++;
		actor->next = ptr;
		actor->prev = ptr->prev;
		ptr->prev->next = actor;
		ptr->prev = actor;		
	}
}


/**************************************************************************
	Function 	: actorSub()
	Purpose 	: removes an actor from the linked list
	Parameters 	: pointer to actor
	Returns 	: none
	Info 		:
**************************************************************************/

void actorSub(ACTOR *actor)
{
	if(actor->next != NULL)
	{
		actor->prev->next = actor->next;
		actor->next->prev = actor->prev;
		actorList.numEntries--;
		actor->next = NULL;
	}

}


/**************************************************************************
	FUNCTION:	actorFree
	PURPOSE:	free all memory used by actor structure
	PARAMETERS:	ACTOR *
	RETURNS:	
**************************************************************************/

void actorFree(ACTOR *actor)
{
	FREE(actor->psiData.objectTable);
	if (actor->shadow)
	{
		FREE((UBYTE **)&actor->shadow);//IF SHADOWS ARE ADDED...mm
	}
	actorSub(actor);
	FREE(actor);
}



static ULONG 	*segTable;
static PSIOBJECT *psiobNext;

static void actorSetTree(PSIOBJECT *obj,PSIMESH *mesh)
{

		*segTable = (ULONG)obj;
		segTable++;

		obj->meshdata = mesh;

		if (mesh->child)
		{
			obj->child	  = psiobNext;
	   		actorSetTree(psiobNext++,mesh->child);
		}
		if (mesh->next)
		{
			obj->next  = psiobNext;
			actorSetTree(psiobNext++,mesh->next);
		}
}


/**************************************************************************
	FUNCTION:	CreateActor
	PURPOSE:	create an actor structure from PSIModel data
	PARAMETERS:	PSIModel*
	RETURNS:	ACTOR*
**************************************************************************/

ACTOR *actorCreate(PSIMODEL *psiModel)
{

	ULONG len,parts,i;
	ULONG *amem;
	ACTOR *actor;
	PSIOBJECT *psiobRoot;
	PSIMESH *meshRoot;
	char	actorName[32];

	if (psiModel==NULL)
		return 0;

	parts = psiModel->noofmeshes;			// number of objects in this model

	utilPrintf("Number of Objects in Model : %d\n",parts);

	len = parts * sizeof(PSIOBJECT);
	len += sizeof (ACTOR);

	utilPrintf("New actor: %d bytes\n",len);
	
	sprintf(actorName,"ACTOR:%s",(char *)psiModel+4);

	actor = (ACTOR *)MALLOC0(len);
	
	// flush memory
	amem = (ULONG *)actor;
	for (i=0; i<(len/4); i++)
		*amem++ = 0;

	actor->psiData.flags = 0;

	if (psiModel->flags & 1)
	{
		utilPrintf("Skinned Model..............");
		actor->psiData.flags = ACTOR_BONED;

		//ScalePsi(actor->psiData.object->meshdata);
	}

	actor->psiData.modelName = &psiModel->name[0];

	actor->psiData.numObjects = parts;

	// list of pointers to model objects/meshes
	actor->psiData.objectTable = (ULONG *)MALLOC0(parts * sizeof(int));

	// set primitive pointer
	actor->psiData.primitiveList = psiModel->primOffset;

	// set prim counter
	actor->psiData.noofPrims = psiModel->noofPrims;

	utilPrintf ( "Name : %s, Number of polys: %d", actor->psiData.modelName, actor->psiData.noofPrims );

	actor->psiData.noofVerts = psiModel->noofVerts;


	meshRoot = (PSIMESH *)psiModel->firstMesh;

	//	set anim frame pointers
	actor->animation.numAnimations = psiModel->animSegments;
	actor->animSegments = (USHORT*)psiModel->animSegmentList;	// offest to frame list

	// set up actor structure

	psiobRoot = (PSIOBJECT *)((char*)actor + sizeof(ACTOR));		// object structs follow actor struct
	psiobNext = psiobRoot + 1;

	actor->psiData.object = psiobRoot;									// pointer to 1st object

	// fill in next & child pointers
	segTable = actor->psiData.objectTable;
	actorSetTree(psiobRoot,meshRoot);

	actorAdd(actor);											// add actor to actorList

	actorInitAnim(actor);										// initialise animation structure

	actor->animFrames = psiModel->animEnd;						// total noof frames of animation

	actor->radius = psiModel->radius;
	
	psiInitSortList( (actor->radius * 2) + 8 );	 					// make enough room to z sort polys
	actor->size.vx = actor->size.vy = actor->size.vz = 4096;

	if (psiObjectScan(actor->psiData.object,"MOTION"))
	{
		utilPrintf("motion bone found\n");
		actor->psiData.flags |= ACTOR_MOTIONBONE;
	}
	else
		utilPrintf("motion bone NOT found\n");

	actor->psiData.flags |= ACTOR_DYNAMICSORT;							// by default

	actorSetBoundingRotated(actor,0,0,0,0);						// fill in bounding box info

	return actor;
}

/**************************************************************************
	FUNCTION:	
	PURPOSE:	
	PARAMETERS:	
	RETURNS:	
**************************************************************************/

void actorInitAnim(ACTOR *actor)
{
	actor->animation.currentAnimation = -1;

	actor->animation.queueAnimation[0] = -1;
	actor->animation.queueAnimation[1] = -1;
	actor->animation.queueAnimation[2] = -1;
	actor->animation.queueAnimation[3] = -1;
	actor->animation.queueAnimation[4] = -1;
	actor->animation.loopAnimation = 1;
	actor->animation.numberQueued = 0;
	actor->animation.animTime = 0;
	actor->animation.reachedEndOfAnimation = 0;
}


/**************************************************************************
	FUNCTION:	
	PURPOSE:	
	PARAMETERS:	
	RETURNS:	
**************************************************************************/

void actorFlushQueue(ACTOR *actor)
{
	int i;

	actor->animation.numberQueued = 0;

	for(i = 0;i < ANIM_QUEUE_LENGTH;i++)
	{
		actor->animation.queueAnimation[i] = -1;
		actor->animation.queueAnimationSpeed[i] = 1;
	}
}


/**************************************************************************
	FUNCTION:	
	PURPOSE:	
	PARAMETERS:	
	RETURNS:	
**************************************************************************/
void actorSetAnimationSpeed(ACTOR *actor, int speed)
{
	actor->animation.animationSpeed = speed;
}


/**************************************************************************
	FUNCTION:	
	PURPOSE:	
	PARAMETERS:	
	RETURNS:	
**************************************************************************/

void actorAdjustPosition(ACTOR *actor)
{

	actor->oldPosition.vx = 0;//actor->position.vx;// actor->accumulator.vx;
	actor->oldPosition.vy = 0;//actor->position.vy;//actor->accumulator.vy;
	actor->oldPosition.vz = 0;//actor->position.vz;//actor->accumulator.vz;
	

	actor->accumulator.vx = 0;
	actor->accumulator.vy = 0;
	actor->accumulator.vz = 0;

//	actor->position = actor->oldPosition;

//	actor->position.vx = actor->position.vy = actor->position.vz = 0;

}


/**************************************************************************
	FUNCTION:	
	PURPOSE:	draw all actors in actorList
	PARAMETERS:	
	RETURNS:	
**************************************************************************/

void actorDrawAll()
{

	int loop;
	ACTOR *tempActor;

	loop = actorList.numEntries;
	tempActor = actorList.head.next;
	
	while (loop)
	{
		actorMove(tempActor);

		actorDraw(tempActor);

		tempActor = tempActor->next;
		loop--;
	}
}


/**************************************************************************
	FUNCTION:	
	PURPOSE:	move actor at actor->position
	PARAMETERS:	ACTOR *
	RETURNS:	
**************************************************************************/

void actorMove(ACTOR *actor)
{
	int i,v,rotcalc,rotate;
	PSIOBJECT *world;
	register PSIMODELCTRL	*modctrl = &PSImodelctrl;

   	world = actor->psiData.object;

	if ( !(actor->psiData.flags & ACTOR_DYNAMICSORT) )
	{
		v = 0;
		if (-PSImodelctrl.whichcamera->vpy < actor->position.vy)
		{
			v=4;
		}

		rotcalc=utilCalcAngle(	PSImodelctrl.whichcamera->vpx - actor->position.vx,
							PSImodelctrl.whichcamera->vpz - actor->position.vz)&0xffff;
		rotate=world->rotate.vy+2048; 

		i = utilCalcAngle(	PSImodelctrl.whichcamera->vpx - actor->position.vx,
						PSImodelctrl.whichcamera->vpz - actor->position.vz)&0xffff;
		i=((((rotcalc+rotate)&4095)/1024))&3;
		i += v;
		modctrl->sorttable=i;
	}

	actorSetAnimation(actor, actor->animation.frame);


	if (actor->psiData.flags&ACTOR_MOTIONBONE)
	{
		if (actor->psiData.flags&ACTOR_HOLDMOTION)
		{
			world->matrix.t[0] = actor->position.vx;
			world->matrix.t[1] = actor->position.vy;
			world->matrix.t[2] = actor->position.vz;
		}
		else
		{
			world->matrix.t[0] += actor->position.vx;
			world->matrix.t[1] += actor->position.vy;
			world->matrix.t[2] += actor->position.vz;
			actor->position.vx = world->matrix.t[0];
			actor->position.vy = world->matrix.t[1];
			actor->position.vz = world->matrix.t[2];
		}
	}
}


/**************************************************************************
	FUNCTION:	
	PURPOSE:	draw actor at actor->position
	PARAMETERS:	ACTOR *
	RETURNS:	
**************************************************************************/

BBTIMER tActorDraw, tDrawSegments, tCalcWorldMatrix;

void actorDraw(ACTOR *actor)
{
	PSIOBJECT *world;

	TimerStart(&tActorDraw);
	world = actor->psiData.object;

	//convert qRot to matrix

// 	if(actor->qRot.x==0 && actor->qRot.y==0 && actor->qRot.z==0 && actor->qRot.w==0)
// 		utilPrintf("ZERO QUAT !\n");

	QuatToPSXMatrix(&actor->qRot, &actor->psiData.object->matrix);

	if(actor->psiData.flags & ACTOR_MOTIONBONE)
	{
		world->matrix.t[0] = -actor->position.vx;
		world->matrix.t[1] = -actor->position.vy;
		world->matrix.t[2] = actor->position.vz;
	}
	else
	{
		world->matrix.t[0] += -actor->position.vx;
		world->matrix.t[1] += -actor->position.vy;
		world->matrix.t[2] += actor->position.vz;
	}

	PSIrootScale = &actor->psiData.object->scale;
	
	TimerStart(&tCalcWorldMatrix);
	psiCalcWorldMatrix(actor->psiData.object);
	TimerStop(&tCalcWorldMatrix);

	TimerStart(&tDrawSegments);
	psiDrawSegments(&actor->psiData);
	TimerStop(&tDrawSegments);

	TimerStop(&tActorDraw);
}

void actorDraw2(ACTOR *actor)
{
	MATRIX		scaleMatrix;
	PSIOBJECT	*world;

	world = actor->psiData.object;

	// create out rotation matrix
	RotMatrixYXZ_gte(&world->rotate,&world->matrix);

	// create our scale matrix
	scaleMatrix = GsIDMATRIX;
	ScaleMatrix(&scaleMatrix, &actor->size);

	gte_MulMatrix0(&GsWSMATRIX, &world->matrix, &world->matrixscale);
	gte_MulMatrix0(&world->matrixscale, &scaleMatrix, &world->matrixscale);

	// get our position in camera space
	gte_SetRotMatrix(&GsWSMATRIX);
	gte_SetTransMatrix(&GsWSMATRIX);
	gte_ldlvl(&actor->position);
	gte_rtirtr();
	gte_stlvl(&world->matrixscale.t);

	psiDrawSegments(&actor->psiData);
}
/*
void actorDraw2(ACTOR *actor)
{
	MATRIX		scaleMatrix;
	PSIOBJECT	*world;

	world = actor->psiData.object;

	// create out rotation matrix
	RotMatrixYXZ_gte(&world->rotate,&world->matrix);

	// create our scale matrix
	scaleMatrix = GsIDMATRIX;
	ScaleMatrix(&scaleMatrix, &actor->size);

	// multiply camera matrix by our scale matrix
	gte_MulMatrix0(&GsWSMATRIX, &scaleMatrix, &world->matrixscale);

	// then by our rotation matrix
	gte_MulMatrix0(&world->matrixscale, &world->matrix, &world->matrixscale);

	// get our position in camera space
	gte_SetRotMatrix(&GsWSMATRIX);
	gte_SetTransMatrix(&GsWSMATRIX);
	gte_ldlvl(&actor->position);
	gte_rtirtr();
	gte_stlvl(&world->matrixscale.t);

	psiDrawSegments(&actor->psiData);

}
*/

void drawBones(PSIOBJECT *world, PSIOBJECT *parent)
{
	LINE_F2	*line;
	VERT	worldPos, parentPos;
	long	worldXY, parentXY;

	while(world)
	{
		worldPos.vx = world->matrixscale.t[0];
		worldPos.vy = world->matrixscale.t[1];
		worldPos.vz = world->matrixscale.t[2];
		
		parentPos.vx = parent->matrixscale.t[0];
		parentPos.vy = parent->matrixscale.t[1];
		parentPos.vz = parent->matrixscale.t[2];

		gte_ldv0(&worldPos);
		gte_rtps();
		gte_stsxy(&worldXY);

		gte_ldv0(&parentPos);
		gte_rtps();
		gte_stsxy(&parentXY);

		BEGINPRIM(line, LINE_F2);
		*(u_long *)&line->x0 = *(u_long *)&worldXY;
		*(u_long *)&line->x1 = *(u_long *)&parentXY;
		setRGB0(line, 255, 255, 255);
		setLineF2(line);
		ENDPRIM(line, 0, LINE_F2);

		if(world->child)
			drawBones(world->child, world);

		world = world->next;
	}
}



void actorDrawBones(ACTOR *actor)
{
	PSIOBJECT *world;

   	world = actor->psiData.object;

	if(actor->psiData.flags & ACTOR_MOTIONBONE)
	{
		world->matrix.t[0] = actor->position.vx;
		world->matrix.t[1] = actor->position.vy;
		world->matrix.t[2] = actor->position.vz;
	}
	else
	{
		world->matrix.t[0] += actor->position.vx;
		world->matrix.t[1] += actor->position.vy;
		world->matrix.t[2] += actor->position.vz;
	}

	PSIrootScale = &world->scale;
	
	psiCalcLocalMatrix(world);

	if(world->child)
	{
		gte_SetRotMatrix(&GsWSMATRIX);
		gte_SetTransMatrix(&GsWSMATRIX);
		drawBones(world->child, world);
	}
}


/**************************************************************************
	FUNCTION:	actorUpdateAnimation()
	PURPOSE:	Set keyframe information for current object frame
	PARAMETERS:	Model structure, frame number
	RETURNS:	
**************************************************************************/

void actorUpdateAnimation()
{
	int loop;
	ACTOR *actor;
	
	loop = actorList.numEntries;
	actor = actorList.head.next;
	while (loop)
	{

		actorUpdateAnimations(actor);
		actor = actor->next;
		loop--;

	}	
}


/**************************************************************************
	FUNCTION:	actorUpdateAnimations
	PURPOSE:	
	PARAMETERS:	
	RETURNS:	
**************************************************************************/

void actorUpdateAnimations(ACTOR *actor)
{
	ACTOR_ANIMATION *actorAnim = &actor->animation;
	ANIMATION *anim;
	int i;

	//reject all objects which have no animation
	if(actorAnim->numAnimations == 0)
		return;

	if(actorAnim->currentAnimation < 0)
		return;

	anim = (ANIMATION*) (actor->animSegments + (actorAnim->currentAnimation*2));
	
	actorAnim->reachedEndOfAnimation = 0;
	actorAnim->animTime += actorAnim->animationSpeed;

	actorAnim->frame = actorAnim->animTime >> ANIMSHIFT;
	
	if( ((actorAnim->frame > anim->animEnd) || (actorAnim->frame < anim->animStart)) &&
	 (actorAnim->loopAnimation == 0))
	{
		actorAnim->frame = Bound(actorAnim->frame,anim->animStart,anim->animEnd);
		actorAnim->animTime = actorAnim->frame << ANIMSHIFT;
		actorAnim->reachedEndOfAnimation = actorAnim->currentAnimation+1;

		//queue stuff - do this later
		if(actorAnim->queueAnimation[0] != -1)
		{
			actorAnim->currentAnimation = actorAnim->queueAnimation[0];//actorAnim;
			actorAnim->loopAnimation = actorAnim->queueLoopAnimation[0];//loop;
			actorAnim->animationSpeed = actorAnim->queueAnimationSpeed[0];//speed;
			if(actorAnim->animationSpeed < 0)
			{
				actorAnim->animTime = actor->animSegments[actorAnim->currentAnimation*2 +1] <<ANIMSHIFT;
			}
			else
			{
				actorAnim->animTime = actor->animSegments[actorAnim->currentAnimation*2] <<ANIMSHIFT;
			}

			for(i = 0;i < ANIM_QUEUE_LENGTH - 1;i++)
			{
				actorAnim->queueAnimation[i] = actorAnim->queueAnimation[i + 1];
				actorAnim->queueLoopAnimation[i] = actorAnim->queueLoopAnimation[i + 1];
				actorAnim->queueAnimationSpeed[i] = actorAnim->queueAnimationSpeed[i + 1];
			}
			actorAnim->queueAnimation[ANIM_QUEUE_LENGTH - 1] = -1;
			actorAnim->queueLoopAnimation[ANIM_QUEUE_LENGTH - 1] = -1;
			actorAnim->queueAnimationSpeed[ANIM_QUEUE_LENGTH - 1] = -1;
			if(actorAnim->numberQueued >0) 
			{
				actorAnim->numberQueued--;
			}
		}
	}
	else
	{
		if(actorAnim->frame > anim->animEnd)
		{
			actorAnim->frame = anim->animStart;
			actorAnim->animTime = anim->animStart<<ANIMSHIFT;
			actorAdjustPosition(actor);
		}
		else 
		{
			if(actorAnim->frame < anim->animStart)
			{
				actorAnim->frame = anim->animEnd;
				actorAnim->animTime = anim->animEnd <<ANIMSHIFT;
				actorAdjustPosition(actor);
			}
		}
	}
}

/**************************************************************************
	FUNCTION:	
	PURPOSE:	
	PARAMETERS:	
	RETURNS:	
**************************************************************************/

void actorSetAnimation(ACTOR *actor, ULONG frame)
{
	PSIOBJECT *world;
	ACTOR_ANIMATION *actorAnim = &actor->animation;
	ANIMATION *anim;
	long temp0,temp1,temp2;
	VECTOR result;

	world = actor->psiData.object;
	
	PSIactorScale = &actor->size;

	psiSetKeyFrames(world, frame);

	/*
		
	actor->accumulator.vx = world->matrix.t[0];
	actor->accumulator.vy = world->matrix.t[1];
	actor->accumulator.vz = world->matrix.t[2];

	world->matrix.t[0] -= actor->oldPosition.vx;
	world->matrix.t[1] -= actor->oldPosition.vy;
	world->matrix.t[2] -= actor->oldPosition.vz;

	actor->oldPosition = actor->accumulator;

	temp0 = world->matrix.t[0];
	temp1 = world->matrix.t[1];
	temp2 = world->matrix.t[2];
	
	actorRotate(world->rotate.vx,world->rotate.vy,world->rotate.vz,temp0,temp1,temp2,&result);

	world->matrix.t[0] = result.vx;
	world->matrix.t[1] = result.vy;
	world->matrix.t[2] = result.vz;
	*/
	
	if (actorAnim->exclusive)  
	{
		anim = (ANIMATION*) (actor->animSegments + (actorAnim->currentAnimation*2));
		if(actorAnim->frame >= anim->animEnd)
		{
			actorAnim->frame = anim->animStart;
			actorAnim->animTime = anim->animStart<<ANIMSHIFT;
			actorAdjustPosition(actor);
		}
		else 
		{
			if(actorAnim->frame < anim->animStart)
			{
				actorAnim->frame = anim->animEnd;
				actorAnim->animTime = anim->animEnd <<ANIMSHIFT;
				actorAdjustPosition(actor);
			}
		}
	}		
		// Store Root Bone movement
		// Use Basic Position - IE Set root bone to zero
}


/**************************************************************************
	FUNCTION:	actorSetAnimation2()
	PURPOSE:	Set keyframe information for current object ((4096 - blend) * frame0) + (blend * frame1)
	PARAMETERS:	actor, frame0, frame1, blend
	RETURNS:	
**************************************************************************/

void actorAnimate(ACTOR *actor, int animNum, char loop, char queue, int speed, char skipendframe)
{
	ACTOR_ANIMATION *actorAnim = &actor->animation;
	ANIMATION *anim;
	int	actualSpeed;

	if(actorAnim->numAnimations == 0)
	{
		return;
	}

	if( animNum >= actorAnim->numAnimations )
	{
		utilPrintf("anim index number out of range.\n");
		return;
	}

	
	if( (actorAnim->currentAnimation == animNum) &&
		(queue == 0) && 
		(actorAnim->reachedEndOfAnimation == 0) &&
		( speed == actorAnim->animationSpeed) )
	{
		actorAnim->loopAnimation = loop;
		actorFlushQueue(actor);
		return;
	}

	if(speed == 0)
	{
		actualSpeed = 256;
	}
	else
	{
		actualSpeed = speed;
	}

	actorAnim->exclusive = skipendframe;
	
	if(queue == 0)
	{
		actorAdjustPosition(actor);
		actorAnim->currentAnimation = animNum;
		actorAnim->loopAnimation = loop;
		actorAnim->animationSpeed = actualSpeed;
		actorAnim->reachedEndOfAnimation = 0;
		anim = (ANIMATION*)(actor->animSegments + (actorAnim->currentAnimation*2));
		actorAnim->animTime = anim->animStart << ANIMSHIFT;
		actorAnim->frame = anim->animStart;
		actorFlushQueue(actor);
		if(actualSpeed < 0)
		{
			actorAnim->animTime = anim->animEnd << ANIMSHIFT;
		}
	}
	else
	{
		if(actorAnim->currentAnimation == -1)
		{
			actorAnim->currentAnimation = animNum;
			actorAnim->loopAnimation = loop;
			actorAnim->animationSpeed = actualSpeed;
		}
		else
		{
			if(actorAnim->numberQueued == 5)
			{
				return;
			}

			actorAnim->queueAnimation[actorAnim->numberQueued] = animNum;
			actorAnim->queueLoopAnimation[actorAnim->numberQueued] = loop;
			actorAnim->queueAnimationSpeed[actorAnim->numberQueued] = actualSpeed;
			actorAnim->numberQueued++;
		}
	}
}



/**************************************************************************
	FUNCTION:	actorSetAnimation()
	PURPOSE:	Set keyframe information for current object frame
	PARAMETERS:	Model structure, frame number
	RETURNS:	
**************************************************************************/


void actorSetAnimation2(ACTOR *actor, ULONG frame0, ULONG frame1, ULONG blend)
{
	PSIOBJECT *world;
	ACTOR_ANIMATION *actorAnim = &actor->animation;
	ANIMATION *anim;
	long temp0,temp1,temp2;
	VECTOR result;

	world = actor->psiData.object;
	
	PSIactorScale = &actor->size;

	psiSetKeyFrames2(world, frame0, frame1, blend);
	
	/*
	actor->accumulator.vx = world->matrix.t[0];
	actor->accumulator.vy = world->matrix.t[1];
	actor->accumulator.vz = world->matrix.t[2];

	world->matrix.t[0] -= actor->oldPosition.vx;
	world->matrix.t[1] -= actor->oldPosition.vy;
	world->matrix.t[2] -= actor->oldPosition.vz;

//	utilPrintf("add %d (%d)\n",world->matrix.t[1],actor->oldPosition.vy);

	actor->oldPosition = actor->accumulator;

	temp0 = world->matrix.t[0];
	temp1 = world->matrix.t[1];
	temp2 = world->matrix.t[2];
	
	actorRotate(world->rotate.vx,world->rotate.vy,world->rotate.vz,temp0,temp1,temp2,&result);

	world->matrix.t[0] = result.vx;
	world->matrix.t[1] = result.vy;
	world->matrix.t[2] = result.vz;
	*/

	if (actorAnim->exclusive)  
	{
		anim = (ANIMATION*) (actor->animSegments + (actorAnim->currentAnimation*2));
		if(actorAnim->frame >= anim->animEnd)
		{
			actorAnim->frame = anim->animStart;
			actorAnim->animTime = anim->animStart<<ANIMSHIFT;
			actorAdjustPosition(actor);
		}
		else 
		{
			if(actorAnim->frame < anim->animStart)
			{
				actorAnim->frame = anim->animEnd;
				actorAnim->animTime = anim->animEnd <<ANIMSHIFT;
				actorAdjustPosition(actor);
			}
		}
	}		
		// Store Root Bone movement
		// Use Basic Position - IE Set root bone to zero
}


void actorRotate(short angx, short angy, short angz, long movex, long movey, long movez, VECTOR *result)
{																
	SVECTOR  MATHS_ang;											
	VECTOR   MATHS_move;										
	MATRIX   MATHS_rotMat;										
																
	MATHS_ang.vx = (angx);										
	MATHS_ang.vy = (angy);										
	MATHS_ang.vz = (angz);										
																
	MATHS_move.vx = (movex);								
	MATHS_move.vy = (movey);									
	MATHS_move.vz = (movez);									
																
	MATHS_rotMat = GsIDMATRIX;									
	RotMatrixYXZ_gte(&MATHS_ang, &MATHS_rotMat);				
	ApplyMatrixLV(&MATHS_rotMat, &MATHS_move, (result));		
}

/**************************************************************************
	FUNCTION:	actorIsVisible
	PURPOSE:	using actor's preset radius, see if it's screen vivible
	PARAMETERS:	
	RETURNS:	true / false
**************************************************************************/

UBYTE actorIsVisible(ACTOR *actor)
{
	VERT	v[3];
	DVECTOR	scrXY[3];
	long	scrZ[3];
	LINE_F2	*line;

	gte_SetRotMatrix(&GsWSMATRIX);		// load viewpoint matrices
	gte_SetTransMatrix(&GsWSMATRIX);

	v[0].vx = actor->position.vx;		// use actor's world position
	v[0].vy = actor->position.vy - actor->radius;
	v[0].vz = actor->position.vz;

	v[1].vx = v[0].vx + actor->radius;
	v[1].vy = v[0].vy + actor->radius;
	v[1].vz = v[0].vz;

	v[2].vx = v[0].vx - actor->radius;
	v[2].vy = v[0].vy - actor->radius;
	v[2].vz = v[0].vz;

	gte_ldv3c(&v[0]);
	gte_rtpt();
	gte_stsxy3c(&scrXY[0]);
	gte_stsz3c(&scrZ[0]);

	if (scrZ[0] <= 0)
	{
		return 0;					// return if 'behind' camera
	}

	if ((scrZ[0]-actor->radius)>PSImodelctrl.farclip)
	{
		return 0;					// Too far away
	}

	if( (scrXY[1].vx < -256) || (scrXY[1].vy < -128) || (scrXY[2].vx > 256) || (scrXY[2].vy > 128))
	{
		return 0;					// Off screen
	}

	return 1;
}



/**************************************************************************
	FUNCTION:	
	PURPOSE:	
	PARAMETERS:	
	RETURNS:	
**************************************************************************/
/*
void actorDrawRadiusBox(ACTOR *actor)
{

	BOUNDINGBOX *corners;
	SVECTOR v;
	//long lx,rx,ty,by,ison;
	DVECTOR scrxy1,scrxy2;

//	PSISetBoundingRotated(actor,actor->animation.frame,actor->object->rotate.vx,actor->object->rotate.vy,actor->object->rotate.vz);
//	PSISetBounding(actor,0);

	gte_SetRotMatrix(&GsWSMATRIX);
	gte_SetTransMatrix(&GsWSMATRIX);

	corners = &actor->bounding;
//////////////
	corners->maxX = actor->radius;
	corners->maxY = actor->radius;
	corners->maxZ = actor->radius;

	corners->minX = -actor->radius;
	corners->minY = -actor->radius;
	corners->minZ = -actor->radius;

////////////

	polyLine.r0 = 240;
	polyLine.g0 = 0;
	polyLine.b0 = 240;

	v.vx = corners->minX+actor->position.vx;
	v.vy = corners->minY+actor->position.vy;
	v.vz = corners->minZ+actor->position.vz;
	gte_ldv0(&v);
	gte_rtps();
	gte_stsxy(&scrxy1);
	polyLine.x0 = scrxy1.vx;
	polyLine.y0 = scrxy1.vy;

		v.vx = corners->minX+actor->position.vx;
		v.vy = corners->minY+actor->position.vy;
		v.vz = corners->maxZ+actor->position.vz;
		gte_ldv0(&v);
		gte_rtps();
		gte_stsxy(&scrxy2);
		polyLine.x1 = scrxy2.vx;
		polyLine.y1 = scrxy2.vy;
		PSIDrawLine(0);

		v.vx = corners->maxX+actor->position.vx;
		v.vy = corners->minY+actor->position.vy;
		v.vz = corners->minZ+actor->position.vz;
		gte_ldv0(&v);
		gte_rtps();
		gte_stsxy(&scrxy2);
		polyLine.x1 = scrxy2.vx;
		polyLine.y1 = scrxy2.vy;
		PSIDrawLine(0);
	
		v.vx = corners->minX+actor->position.vx;
		v.vy = corners->maxY+actor->position.vy;
		v.vz = corners->minZ+actor->position.vz;
		gte_ldv0(&v);
		gte_rtps();
		gte_stsxy(&scrxy2);
		polyLine.x1 = scrxy2.vx;
		polyLine.y1 = scrxy2.vy;
		PSIDrawLine(0);

	v.vx = corners->maxX+actor->position.vx;
	v.vy = corners->maxY+actor->position.vy;
	v.vz = corners->minZ+actor->position.vz;
	gte_ldv0(&v);
	gte_rtps();
	gte_stsxy(&scrxy1);
	polyLine.x0 = scrxy1.vx;
	polyLine.y0 = scrxy1.vy;

		v.vx = corners->minX+actor->position.vx;
		v.vy = corners->maxY+actor->position.vy;
		v.vz = corners->minZ+actor->position.vz;
		gte_ldv0(&v);
		gte_rtps();
		gte_stsxy(&scrxy2);
		polyLine.x1 = scrxy2.vx;
		polyLine.y1 = scrxy2.vy;
		PSIDrawLine(0);

		v.vx = corners->maxX+actor->position.vx;
		v.vy = corners->minY+actor->position.vy;
		v.vz = corners->minZ+actor->position.vz;
		gte_ldv0(&v);
		gte_rtps();
		gte_stsxy(&scrxy2);
		polyLine.x1 = scrxy2.vx;
		polyLine.y1 = scrxy2.vy;
		PSIDrawLine(0);
	
		v.vx = corners->maxX+actor->position.vx;
		v.vy = corners->maxY+actor->position.vy;
		v.vz = corners->maxZ+actor->position.vz;
		gte_ldv0(&v);
		gte_rtps();
		gte_stsxy(&scrxy2);
		polyLine.x1 = scrxy2.vx;
		polyLine.y1 = scrxy2.vy;
		PSIDrawLine(0);


	v.vx = corners->maxX+actor->position.vx;
	v.vy = corners->minY+actor->position.vy;
	v.vz = corners->maxZ+actor->position.vz;
	gte_ldv0(&v);
	gte_rtps();
	gte_stsxy(&scrxy1);
	polyLine.x0 = scrxy1.vx;
	polyLine.y0 = scrxy1.vy;

		v.vx = corners->maxX+actor->position.vx;
		v.vy = corners->minY+actor->position.vy;
		v.vz = corners->minZ+actor->position.vz;
		gte_ldv0(&v);
		gte_rtps();
		gte_stsxy(&scrxy2);
		polyLine.x1 = scrxy2.vx;
		polyLine.y1 = scrxy2.vy;
		PSIDrawLine(0);

		v.vx = corners->minX+actor->position.vx;
		v.vy = corners->minY+actor->position.vy;
		v.vz = corners->maxZ+actor->position.vz;
		gte_ldv0(&v);
		gte_rtps();
		gte_stsxy(&scrxy2);
		polyLine.x1 = scrxy2.vx;
		polyLine.y1 = scrxy2.vy;
		PSIDrawLine(0);
	
		v.vx = corners->maxX+actor->position.vx;
		v.vy = corners->maxY+actor->position.vy;
		v.vz = corners->maxZ+actor->position.vz;
		gte_ldv0(&v);
		gte_rtps();
		gte_stsxy(&scrxy2);
		polyLine.x1 = scrxy2.vx;
		polyLine.y1 = scrxy2.vy;
		PSIDrawLine(0);

	v.vx = corners->minX+actor->position.vx;
	v.vy = corners->maxY+actor->position.vy;
	v.vz = corners->maxZ+actor->position.vz;
	gte_ldv0(&v);
	gte_rtps();
	gte_stsxy(&scrxy1);
	polyLine.x0 = scrxy1.vx;
	polyLine.y0 = scrxy1.vy;

		v.vx = corners->maxX+actor->position.vx;
		v.vy = corners->maxY+actor->position.vy;
		v.vz = corners->maxZ+actor->position.vz;
		gte_ldv0(&v);
		gte_rtps();
		gte_stsxy(&scrxy2);
		polyLine.x1 = scrxy2.vx;
		polyLine.y1 = scrxy2.vy;
		PSIDrawLine(0);

		v.vx = corners->minX+actor->position.vx;
		v.vy = corners->maxY+actor->position.vy;
		v.vz = corners->minZ+actor->position.vz;
		gte_ldv0(&v);
		gte_rtps();
		gte_stsxy(&scrxy2);
		polyLine.x1 = scrxy2.vx;
		polyLine.y1 = scrxy2.vy;
		PSIDrawLine(0);
	
		v.vx = corners->minX+actor->position.vx;
		v.vy = corners->minY+actor->position.vy;
		v.vz = corners->maxZ+actor->position.vz;
		gte_ldv0(&v);
		gte_rtps();
		gte_stsxy(&scrxy2);
		polyLine.x1 = scrxy2.vx;
		polyLine.y1 = scrxy2.vy;
		PSIDrawLine(0);


}
*/

/**************************************************************************
	FUNCTION:	PSISetBoundingRotated
	PURPOSE:	set bounding box values for animation frame & rotations
	PARAMETERS:	
	RETURNS:	sets values in actor structure's bounding box
**************************************************************************/

void actorSetBoundingRotated(ACTOR *actor,int frame,int rotX,int rotY, int rotZ)
{
	int x,y,z;

	// save rotations
	
	x = actor->psiData.object->rotate.vx;	
	y = actor->psiData.object->rotate.vy;	
	z = actor->psiData.object->rotate.vz;	

	actor->psiData.object->rotate.vx = rotX;		
	actor->psiData.object->rotate.vy = rotY;
	actor->psiData.object->rotate.vz = rotZ;

	// set the model 'pose'
	actorSetAnimation(actor, frame);

	PSIrootScale = &actor->psiData.object->scale;
	psiCalcLocalMatrix(actor->psiData.object);

	actorCalcSegments(actor);

	// restore rotations
		
	actor->psiData.object->rotate.vx = x;		
	actor->psiData.object->rotate.vy = y;
	actor->psiData.object->rotate.vz = z;
}

/**************************************************************************
	FUNCTION:
	PURPOSE:	find min and max xyz from model 'pose'
	PARAMETERS:	
	RETURNS:	
**************************************************************************/

int actorCalcSegments(ACTOR *actor)
{
	register PSIOBJECT	*world;
	int loop,obs,i,j,q,x,y,z,s;
	long flg;
	VECTOR	tfv;
	short *verts;
	SVECTOR v;
	BOUNDINGBOX *corners;

	q = 0;
	obs = actor->psiData.numObjects;
	corners = &actor->bounding;

	corners->minX = corners->maxX = 0;
	corners->minY = corners->maxY = 0;
	corners->minZ = corners->maxZ = 0;

	for (loop = 0; loop < obs; loop++)
	{
		world = (PSIOBJECT*)actor->psiData.objectTable[loop];

	 	gte_SetRotMatrix(&world->matrixscale);
	   	gte_SetTransMatrix(&world->matrixscale);

		verts=(short*)world->meshdata->vertop;
		j = world->meshdata->vern;

		for (i=0; i < j; i++)
		{

			v.vx = *(verts++);
			v.vy = *(verts++);
			v.vz = *(verts++);

			RotTrans(&v,&tfv,&flg);
			
			verts++;

			x = tfv.vx;
			y = tfv.vy;
			z = tfv.vz;


			if ( x < corners->minX )
				corners->minX = x;
			if ( x > corners->maxX )
				corners->maxX = x;
			if ( y < corners->minY )
				corners->minY = y;
			if ( y > corners->maxY )
				corners->maxY = y;
			if ( z < corners->minZ )
				corners->minZ = z;
			if ( z > corners->maxZ )
				corners->maxZ = z;
							 

			s = abs(x*x) + abs(y*y) + abs(z*z);

			if (s>q)
			{
				q = s;
			}
		}
	}

  	j = utilSqrt(q) >> 16;

	return j;	
}


/**************************************************************************
	FUNCTION:	actorSetBounding
	PURPOSE:	as actorSetBoundingRotated but forces 0 rotation
	PARAMETERS:	
	RETURNS:	sets values in actor structure's bounding box
**************************************************************************/

void actorSetBounding(ACTOR *actor,int frame)
{
	actorSetBoundingRotated(actor,frame,0,0,0);
}


void ScalePsi(PSIMESH* pMesh)
{
	int m,v;
	SVKEYFRAME* pK;
	int i;

	//loop verts
	for(v=pMesh->vern-1; v>=0; v--)
	{
		pMesh->vertop[v].vx *= SCALE;
		pMesh->vertop[v].vy *= SCALE;
		pMesh->vertop[v].vz *= SCALE;
	}


	pK = pMesh->movekeys;
	for(i=0; i<pMesh->numMoveKeys; i++, pK++)
	{
		pK->vect.x *= SCALE;
		pK->vect.y *= SCALE;
		pK->vect.z *= SCALE;
	}

		//loop scalekeys
// 		for(v=curMesh->numScaleKeys-1; v>=0; v--)
// 		{
// 			curMesh->scalekeys[v].vect.x *= SCALE;
// 			curMesh->scalekeys[v].vect.y *= SCALE;
// 			curMesh->scalekeys[v].vect.z *= SCALE;
// 		}

	if(pMesh->child)
		ScalePsi(pMesh->child);

	if(pMesh->next)
		ScalePsi(pMesh->next);
}
