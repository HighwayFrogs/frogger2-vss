/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.
	
	  
	File		: cam.c
	Programmer	: Andrew Eder
	Date		: 11/10/98
	
----------------------------------------------------------------------------------------------- */

#define F3DEX_GBI_2

#include <ultra64.h>

#include "incs.h"

VECTOR	cameraUpVect = { 0,1,0 };
VECTOR	camVect;

VECTOR	currCamSource[4] = {{ 0,0,-100 },{ 0,0,-100 },{ 0,0,-100 },{ 0,0,-100 }};
VECTOR	currCamTarget[4] = {{ 0,0,0 },{ 0,0,0 },{ 0,0,0 },{ 0,0,0 }};

VECTOR	actualCamSource[2];
VECTOR	actualCamTarget[2];

VECTOR currCamDist = {0,0,10};

float	xFOVNew				= 320.0F / 240.0F;
float	yFOVNew				= 45.0F;
float	xFOV				= 320.0F / 240.0F;
float	yFOV				= 45.0F;
float	precScaleFactor		= 1.0F;

VECTOR camSource[4]			= {{ 0,569,-200 },{ 0,569,-200 },{ 0,569,-200 },{ 0,569,-200 }};
VECTOR camTarget[4];
float camLookOfs			= 50;
float camLookOfsNew			= 50;

VECTOR camDist				= { 0,680,192 };
float camSpeed				= 15; 
float camSpeed2				= 15; 
float camSpeed3				= 20;	// source & target interp.
float camSpeed4				= 15;
float fovSpd				= 2;
float transCamSpeedMult		= 1.0F;

int	camFacing				= 0;
int nextCamFacing			= 0;
float scaleV				= 1.1F;

char controlCamera			= 0;
char fixedDir = 0;
char fixedPos = 0;
char firstPerson = 0;
char fixedUp = 0;
TRANSCAMERA *transCameraList = NULL;

extern long idleCamera;
VECTOR idleCamDist	= { 0,100,102 };

float sideSwaySpeed = 0.005,sideSwayAmt=50,swayModifier = 1.0f;

float camSideOfs = 0;
GAMETILE *lastTile = NULL;
long prevCamFacing = 0;

CAM_BOX_LIST cameraBoxes;
CAM_BOX *currCamBox;
float cam_edge_spacing = 100;

float cam_shakiness = 0.0f;
float cam_shake_falloff = 0.1f;

// functions

float FindMaxInterFrogDistance( );
void CameraSetSource(void);
int CameraBoundPosition(VECTOR *v, CAM_BOX *box, float edge);
void CalcSPCamera( VECTOR *target );

/*	--------------------------------------------------------------------------------
	Function		: CameraBoundPosition
	Purpose			: limits a vector to a camera space, with edge padding
	Parameters		: VECTOR *, CAM_BOX *, float
	Returns			: 

int CameraBoundPosition(VECTOR *v, CAM_BOX *box, float edge)
{
	int p;
	float dist, foo;
	CAM_PLANE *plane;
	VECTOR a, new_v;
	int count;

	if (!box)
		return 0;

	count = 0;
	plane = box->planes;
	SetVector(&new_v, v);
	foo = edge;

	for (p = box->numPlanes; p; p--, plane++)
	{
		if (!plane->status) continue;

		dist = DotProduct(&new_v, &plane->normal) - plane->k;

		if (dist < edge)
		{
			if (dist < foo)
				foo = dist;

			dist -= edge;

			SetVector(&a, &plane->normal);
			ScaleVector(&a, -dist);
			AddToVector(&new_v, &a);
			
			count++;
		}
	}

	SetVector(v, &new_v);

	return count;
}
*/

int GetCamLimitVector(VECTOR *out, VECTOR *v, CAM_BOX *box, float edge)
{
	int p;
	float dist, foo;
	CAM_PLANE *plane;
	VECTOR a, res;
	int count;

	if (!box)
		return 0;

	count = 0;
	plane = box->planes;
	ZeroVector(&res);
	foo = edge;

	for (p = box->numPlanes; p; p--, plane++)
	{
		if (!plane->status) continue;

		dist = DotProduct(v, &plane->normal) - plane->k;

		if (dist < edge)
		{
			if (dist < foo)
				foo = dist;

			dist -= edge;

			SetVector(&a, &plane->normal);
			ScaleVector(&a, -dist);
			AddToVector(&res, &a);
			
			count++;
		}
	}

	SetVector(out, &res);

	return count;
}


/*	--------------------------------------------------------------------------------
	Function		: IsPointInCameraBox
	Purpose			: checks if a 3D point is within a camera space
	Parameters		: 
	Returns			: 
*/

int IsPointInCameraBox(VECTOR v, CAM_BOX *box)
{
	int p;
	CAM_PLANE *plane = box->planes;

	for (p = box->numPlanes; p; p--, plane++)
	{
		if ((DotProduct(&v, &plane->normal) - plane->k) < 0)
			return 0;
	}
	return 1;
}

/*	--------------------------------------------------------------------------------
	Function		: CheckCameraBoxes
	Purpose			: checks the current camera box, handles transitions between boxes
	Parameters		: 
	Returns			: 
*/

void CheckCameraBoxes(void)
{
	if (!currCamBox || !IsPointInCameraBox(camTarget[0], currCamBox))
	{
		CAM_BOX *box = cameraBoxes.boxes;
		int b, c;

		if (currCamBox)
			dprintf"Frogger leaves cam box (%08x)\n", box));

		currCamBox = NULL;
	
		for (b = cameraBoxes.numBoxes, c = 0; b; b--, c++, box++)
		{
			if (IsPointInCameraBox(camTarget[0], box))
			{
				currCamBox = box;
				dprintf"Frogger enters cam box #%d (%08x)\n", c, box));
				break;
			}
		}
	}
}



/*	--------------------------------------------------------------------------------
	Function		: CreateAndAddTransCamera
	Purpose			: adds a dynamic camera angle change for a specified game tile
	Parameters		: GAMETILE *,unsigned long,float,float,float
	Returns			: TRANSCAMERA *
	Info			: 
*/
TRANSCAMERA *CreateAndAddTransCamera(GAMETILE *tile,unsigned long dirCamMustFace, VECTOR *v, unsigned long flags)
{
	TRANSCAMERA *newItem = (TRANSCAMERA *)JallocAlloc(sizeof(TRANSCAMERA),YES,"TRNSCAM");

	newItem->tile			= tile;
	newItem->dirCamMustFace	= dirCamMustFace;
	SetVector(&newItem->camOffset, v);
	newItem->flags			= flags;
	newItem->FOV			= 45.0;
	newItem->speed			= 1.0;

	newItem->next			= transCameraList;
	transCameraList			= newItem;

	return newItem;
}


/*	--------------------------------------------------------------------------------
	Function		: CheckForDynamicCameraChange
	Purpose			: checks current tile to see if a camera change is needed
	Parameters		: GAMETILE *
	Returns			: void
	Info			: 
*/
void CheckForDynamicCameraChange(GAMETILE *tile)
{
	char specialCaseTile = 0;
	TRANSCAMERA *cur;
	static VECTOR stdVector = { 0,480,192 };
	
	cur = transCameraList;
	while(cur)
	{
		if(cur->tile == tile)
		{
			switch (cur->flags)
			{
			case LOOK_AT_FROG:
				SetVector(&camDist,&cur->camOffset);
				specialCaseTile = 1;
				fixedDir = 0;
				fixedPos = 0;
				fixedUp	 = 0;
				break;

			case LOOK_AT_FROG_NOTILT:
				SetVector(&camDist,&cur->camOffset);
				specialCaseTile = 1;
				fixedDir = 0;
				fixedPos = 0;
				fixedUp	 = 1;
				break;

			case STATIC_CAMERA:
				SetVector(&camSource[0], &cur->camOffset);
				fixedDir = 1;
				fixedPos = 1;
				fixedUp = 0;
				break;

			case FIXED_SOURCE:
				SetVector(&camSource[0], &cur->camOffset);
				fixedDir = 0;
				fixedPos = 1;
				fixedUp = 0;
			break;

			case LOOK_AT_TILE:
				SetVector(&camSource[0], &cur->camOffset);
				SetVector(&camTarget[0], &tile->centre );
				fixedDir = 1;
				fixedPos = 1;
				fixedUp = 0;
				break;

			case LOOK_IN_DIR:
				SetVector(&camSource[0], &cur->camOffset );
				
				SetVector(&camTarget[0],&cur->camLookAt);
				ScaleVector(&camTarget[0],200);
				AddToVector(&camTarget[0], &camSource[0]);
				
				fixedDir = 1;
				fixedPos = 1;
				fixedUp = 1;
				break;
			}

			if (cur->dirCamMustFace)
				camFacing = cur->dirCamMustFace - 1;

			if( cur->speed )
				transCamSpeedMult = cur->speed;
			else
				transCamSpeedMult = 1.0F;

			yFOVNew = cur->FOV;

			break;
		}
		cur = cur->next;
	}

//	if(!specialCaseTile)
//		SetVector(&camDist,&stdVector);
}


/*	--------------------------------------------------------------------------------
	Function		: FreeTransCameraList
	Purpose			: frees the camera list
	Parameters		: 
	Returns			: void
	Info			: 
*/
void FreeTransCameraList()
{
	TRANSCAMERA *cur,*next;

	cur = transCameraList;
	while(cur)
	{
		next = cur->next;
		JallocFree((UBYTE**)&cur);
		cur = next;
	}

	transCameraList = NULL;
	lastTile = NULL;

	fixedDir = 0;
	fixedPos = 0;
	firstPerson = 0;
}


/* --------------------------------------------------------------------------------
	Function	: CameraLookAtFrog
	Purpose		: Sets camTarget to the frog, or an average position
	Parameters	: (void)
	Returns		: void 
*/
void CameraLookAtFrog(void)
{
	if(frog[0] && !fixedDir && !controlCamera)
	{
		// Average frog position	
		VECTOR target;

		if( gameState.multi == SINGLEPLAYER )
			CalcSPCamera( &target );
		else
			CalcMPCamera( &target );

/*		if (!idleCamera)
		{
			VECTOR v;
			SetVector(&v, currTile[0]->dirVector[frogFacing[0]]);
			ScaleVector(&v, camLookOfs);
			AddToVector(&target, v);
		}
*/
		SetVector(&camTarget[0], &target);
	}
	
}

void CalcSPCamera( VECTOR *target )
{
	VECTOR v;
	float t = player[0].jumpTime;
	
	if (t > 0)	// jumping; calculate linear position
	{
		SetVector(target, &player[0].jumpOrigin);

		// horizontal
		SetVector(&v, &player[0].jumpFwdVector);
		ScaleVector(&v, t);
		AddToVector(target, &v);

		// vertical
		//SetVector(&v, &player[i].jumpUpVector);
		//ScaleVector(&v, player[i].heightJumped);
		//AddToVector(&target, &v);
	}										
	else
		SetVector(target, &frog[0]->actor->pos);
}


/* --------------------------------------------------------------------------------
	Function	: SlurpCamPosition
	Purpose		:
	Parameters	: (void)
	Returns		: void 
*/
void SlurpCamPosition(long cam)
{
	float s1,s2,s3,s4;
	VECTOR v, w;
//	unsigned long afc = actFrameCount;

	if (idleCamera)
	{
		s1 = gameSpeed / (camSpeed * transCamSpeedMult * 6);
		s2 = gameSpeed / (camSpeed2 * transCamSpeedMult * 6);
		s3 = gameSpeed / (camSpeed3 * transCamSpeedMult * 6);
		s4 = gameSpeed / (camSpeed4 * transCamSpeedMult * 6);
	}
	else
	{
		s1 = gameSpeed / (camSpeed * transCamSpeedMult);
		s2 = gameSpeed / (camSpeed2 * transCamSpeedMult);
		s3 = gameSpeed / (camSpeed3 * transCamSpeedMult);
		s4 = gameSpeed / (camSpeed4 * transCamSpeedMult);
	}

//	if( !lastActFrameCount )
//		lastActFrameCount = afc - 1;
	
//	while( lastActFrameCount < afc )
//	{
		/*if (!initialCamera)
		{

			if (idleCamera)
			{
				currCamDist.v[0] -= s4 * (currCamDist.v[0] - idleCamDist.v[0]);
				currCamDist.v[1] -= s4 * (currCamDist.v[1] - idleCamDist.v[1]);
				currCamDist.v[2] -= s4 * (currCamDist.v[2] - idleCamDist.v[2]);
			}
			else
			{
				currCamDist.v[0] -= s4 * (currCamDist.v[0] - camDist.v[0]*scaleV);
				currCamDist.v[1] -= s4 * (currCamDist.v[1] - camDist.v[1]*scaleV);
				currCamDist.v[2] -= s4 * (currCamDist.v[2] - camDist.v[2]*scaleV);
			}
		}
		else
		{*/

	// Cam target

	SubVector(&v, &camTarget[cam], &currCamTarget[cam]);
	ScaleVector(&v, s3);
	AddToVector(&currCamTarget[cam], &v);

//	GetCamLimitVector(&v, &currCamTarget[cam], currCamBox, cam_edge_spacing);
//	ScaleVector(&v, s3);
//	AddToVector(&currCamTarget[cam], &v);

	//CameraSetSource();

	SubVector(&v, &camSource[cam], &currCamSource[cam]);
	ScaleVector(&v, s3);
	AddToVector(&currCamSource[cam], &v);

	// do we actually WANT to limit the camera source?
//	GetCamLimitVector(&v, &currCamSource[cam], currCamBox, cam_edge_spacing);
//	ScaleVector(&v, s3);
//	AddToVector(&currCamSource[cam], &v);

	// camVect (up vector)

	if (!fixedUp)
	{
		SubVector(&v, &currTile[0]->normal, &camVect);
		ScaleVector(&v, s2);
		AddToVector(&camVect, &v);
	}

	// camDist

	currCamDist.v[0] = camDist.v[0]*scaleV;
	currCamDist.v[1] = camDist.v[1]*scaleV;
	currCamDist.v[2] = camDist.v[2]*scaleV;
	
	if ( gameState.mode != CAMEO_MODE )
	{
		VECTOR t = { 0,0,0 };
		int i;
	
		for (i=0; i<NUM_FROGS; i++)
		{
			if( player[i].healthPoints )
			{
				t.v[0]+=currTile[i]->normal.v[0];
				t.v[1]+=currTile[i]->normal.v[1];
				t.v[2]+=currTile[i]->normal.v[2];
			}
		}

		MakeUnit (&t);
		
		if (!fixedUp)
		{
			camVect.v[0] -= s2 * (camVect.v[0] - t.v[0]);
			camVect.v[1] -= s2 * (camVect.v[1] - t.v[1]);
			camVect.v[2] -= s2 * (camVect.v[2] - t.v[2]);
		}
		else
		{
			camVect.v[0] = 0;
			camVect.v[1] = 1;
			camVect.v[2] = 0;
		}
		
		
	}

	camLookOfs	-= s1 * (camLookOfs - camLookOfsNew);
}

void CameraSetSource(void)
{
	VECTOR avgFrogPos;
	VECTOR avgUpVec;

	//float afx,afy,afz;
	float afx2,afy2,afz2;
	float afx3,afy3,afz3;
	long nC;
	int i,l;
	//afx = afy = afz = 0;
	afx2 = afy2 = afz2 = 0;
	l=0;

	CheckCameraBoxes();

	ZeroVector(&avgFrogPos);
	ZeroVector(&avgUpVec);

	// Average for multiplayer
/*
	for (i=0; i<NUM_FROGS; i++)
	{
		if( player[i].healthPoints )
		{
			AddToVector(&avgFrogPos, &frog[i]->actor->pos);

			if (fixedUp)
			{
				avgUpVec.v[1] +=afx2 += 0;
				afy2 += 1*currCamDist.v[1];
				afz2 += 0;
			}
			else

			{

			//AddToVector(avgUpVec, &currTile[i]->normal
				afx2 += currTile[i]->normal.v[0]*currCamDist.v[1];
				afy2 += currTile[i]->normal.v[1]*currCamDist.v[1];
				afz2 += currTile[i]->normal.v[2]*currCamDist.v[1];
			}

			afx2 -= currTile[0]->dirVector[camFacing].v[0]*currCamDist.v[2];
			afy2 -= currTile[0]->dirVector[camFacing].v[1]*currCamDist.v[2];
			afz2 -= currTile[0]->dirVector[camFacing].v[2]*currCamDist.v[2];

			l++;
		}
	}
*/
		SetVector(&avgFrogPos, &camTarget[0]);

		afx2 = currTile[0]->normal.v[0]*currCamDist.v[1] - currTile[0]->dirVector[camFacing].v[0]*currCamDist.v[2]; 
		afy2 = currTile[0]->normal.v[1]*currCamDist.v[1] - currTile[0]->dirVector[camFacing].v[1]*currCamDist.v[2];
		afz2 = currTile[0]->normal.v[2]*currCamDist.v[1] - currTile[0]->dirVector[camFacing].v[2]*currCamDist.v[2];

	nC = (camFacing+1)&3;

	afx3 = currTile[0]->dirVector[nC].v[0]*camSideOfs;
	afy3 = currTile[0]->dirVector[nC].v[1]*camSideOfs;
	afz3 = currTile[0]->dirVector[nC].v[2]*camSideOfs;

	if (l > 1)
	{
		float scale = 1.0f/l;
		ScaleVector(&avgFrogPos, scale);
	}

	SetVector(&camSource[0], &avgFrogPos);

	camSource[0].v[0] += afx2+afx3;
	camSource[0].v[1] += afy2+afy3;
	camSource[0].v[2] += afz2+afz3;
}



/* --------------------------------------------------------------------------------
	Function	: UpdateCameraPosition
	Purpose		: updates the position of the camera, and, er, does some other stuff
	Parameters	: (void)
	Returns		: void 
*/
void UpdateCameraPosition(long cam)
{
	if(!frog[0] || !currTile[0] || controlCamera)
		return;
	
	if (currTile[0] != lastTile)	//This causes lots of problems with camera transitions..
	{
		if (currTile[0] && lastTile)
		{
			if (camFacing == prevCamFacing)
				camFacing = GetTilesMatchingDirection(lastTile,camFacing,currTile[0]);
		}
		prevCamFacing = camFacing;
		lastTile = currTile[0];
		CheckForDynamicCameraChange(currTile[0]);
	}

	CameraLookAtFrog();

	if ( gameState.mode != CAMEO_MODE && !fixedPos )
		CameraSetSource();

	if (fixedUp)
	{
		camVect.v[0] = 0;
		camVect.v[1] = 1;
		camVect.v[2] = 0;
	}

	SlurpCamPosition(0);

	// Handle shaky camera nonsense
	
	if (cam_shakiness > 0.0)
	{
		int i;
		for (i = 0; i < 3; i++)
		{
			currCamTarget[0].v[i] += (Random(1000)*0.001f - 0.5f)*cam_shakiness;
			currCamSource[0].v[i] += (Random(1000)*0.001f - 0.5f)*cam_shakiness;
		}
		cam_shakiness -= cam_shake_falloff * gameSpeed;
	}

	// edge spacing should probably related to tan(FOV/2)
	cam_edge_spacing = DistanceBetweenPoints(&currCamSource[0], &currCamTarget[0]) * 0.75f;

	if( swingCam )
	{
		// if the player is on their last life - give the swaying camera more 'urgency' - subtle
		// bunch of arse, more like
		if(player[0].lives < 3)
		{
			swayModifier	= 3.0f;
			sideSwayAmt		= 25.0f;
		}
		else
		{
			swayModifier	= 1.0f;
			sideSwayAmt		= 50.0f;
		}

		camSideOfs = ((sinf(actFrameCount*sideSwaySpeed*swayModifier)*sideSwayAmt) * camDist.v[2]) / 350.0;
	}
}


void CameraShake(float amount)
{
	cam_shakiness = amount;
}

/*	--------------------------------------------------------------------------------
	Function		: InitCamera
	Purpose			: 
	Parameters		: 
	Returns			: 
*/

void InitCamera(void)
{
	currCamBox = NULL;
	CheckCameraBoxes();
	
	CheckForDynamicCameraChange(currTile[0]);
	UpdateCameraPosition(0);

	SetVector(&currCamSource[0], &camSource[0]);
	SetVector(&currCamTarget[0], &camTarget[0]);

	cam_shakiness = 0;
}




float FindMaxInterFrogDistance( )
{
	int i, max=0;
	float dist, best=-100000;
	VECTOR sep;

	for( i=0; i<NUM_FROGS; i++ )
	{
		SubVector( &sep, &frog[i]->actor->pos, &frog[0]->actor->pos );
		dist = Magnitude( &sep );

		if( dist > best )
		{
			max = i;
			best = dist;
		}
	}

	if( max ) return best;
	
	return -1;
}
