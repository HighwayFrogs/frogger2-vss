/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: objects.c
	Programmer	: 
	Date		: 29/04/99 11:14:23

----------------------------------------------------------------------------------------------- */

#define F3DEX_GBI

#include <ultra64.h>

#include "incs.h"


OBJECT_BANK	objectBanks[MAX_OBJECT_BANKS];
OBJECT *tempObject;

char message[256];

int offsetVtx = TRUE;

extern OBJECT sec02;
/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
u32 Rom2Ram(u32 x, u32 addressOffset)
{
	u32	temp;
	temp = (u32)x & 0x00ffffff;
	temp += addressOffset;
	return temp;
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void RestoreDrawList(Gfx *dl, u32 offset)
{
	u8	*temp;
	u32	address;
	u32 *addrP;
	TEXTURE	*tempTex;

	temp = (u8 *)dl; 

	while(*temp != 0xB8)
	{
		switch(*temp)
		{
			//loadvertices
			case 0x04:
				//if(offsetVtx)
				//{
					addrP = (u32 *)(temp + 4);
					address = *addrP;
					address = Rom2Ram(address, offset);
					*addrP = address;
				//}
				// ENDIF
				temp += 8;
				break;

			//load texture block
			case 0xFD:		
							//load texture and load texture palette share the same first byte

							//this appears to be the code for a texture load
							if(*(temp + 8) == 0xf5)
							{
								addrP = (u32 *)(temp + 4);
								address = *addrP;
								FindTexture(&tempTex, address,YES,"");
								if(tempTex)
									*addrP = (u32)tempTex->data;
								temp += 56;
							}
							else if(*(temp + 8) == 0xe8)
							{
								addrP = (u32 *)(temp + 4);
								address = *addrP;
								FindTexture(&tempTex, address,YES,"");
								if(tempTex)
									*addrP = (u32)tempTex->palette;
								temp += 48;
							}
							else
								temp += 16;
									
							break;


			default:		temp += 8;
							break;

		}

	}
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void SwapVtxReferencesInDrawlist(OBJECT_CONTROLLER *objectC)
{
	Gfx *dl = objectC->drawList;
//	u32	address;
//	u32 *addrP;
	s32 offset;


	if(objectC->vtxBuf)
		offset = objectC->vtx[0] - objectC->vtx[1];
	else
		offset = objectC->vtx[1] - objectC->vtx[0];

	offset *= sizeof(Vtx);

	AddOffsetToVertexLoads(offset, dl);

	objectC->vtxBuf = 1 - objectC->vtxBuf;



/*
	Gfx *dl = objectC->drawList;
	u8	*temp;
	u32	address;
	u32 *addrP;
	s32 offset;
	Vtx *vtx = objectC->Vtx[objectC->vtxBuf];


	if(objectC->vtxBuf)
		offset = objectC->Vtx[0] - objectC->Vtx[1];
	else
		offset = objectC->Vtx[1] - objectC->Vtx[0];

	offset *= sizeof(Vtx);



	temp = (u8 *)dl; 

	while(*temp != 0xDF)
	{
		switch(*temp)
		{
			//loadvertices
			case 0x01:
				addrP = (u32 *)(temp + 4);
				address = *addrP;
				*addrP += (u32)offset;
				temp += 8;
				break;

			default:
				temp += 8;
				break;
		}
	}

	objectC->vtxBuf = 1 - objectC->vtxBuf; */
	
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void AddOffsetToVertexLoads(int offset, Gfx *dl)
{
	u8	*temp = (u8 *)dl; 
	u32	address;
	u32 *addrP;

	while(*temp != 0xB8)
	{
		switch(*temp)
		{
			//loadvertices
			case 0x04:
				addrP = (u32 *)(temp + 4);
				address = *addrP;
				*addrP += (u32)offset;
				temp += 8;
				break;

			default:
				temp += 8;
				break;
		}
	}



}



/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void DoubleBufferSkinVtx ( void )
{
	ACTOR2 *cur;

	while ( gfxTasks );	//must wait for the graphics tasks to be finished so that we don't disturb the drawlists

	StartTimer ( 0, "code" );

	cur = actList;

	while ( cur )
	{
		if ( cur->actor->objectController )
			if ( cur->actor->objectController->drawList )
				SwapVtxReferencesInDrawlist ( cur->actor->objectController );
//				cur->actor->objectController->vtxBuf = 1 - cur->actor->objectController->vtxBuf;
			// ENDIF
		// ENDIF
		cur = cur->next;
	}
	// ENDWHILE

	EndTimer(0);
}


/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
int numoftc = 0;
void RestoreObjectPointers(OBJECT *obj, u32 memoryOffset)
{
	u32	tempInt;
	int x, y;
	char mess[16];

////////////initialise mesh
	tempInt = (u32)obj->mesh;
	if(tempInt)
	{
		obj->mesh = (MESH *)Rom2Ram((u32)obj->mesh, memoryOffset);

		obj->mesh->vertices = (VECTOR *)Rom2Ram((u32)obj->mesh->vertices, memoryOffset);
		
		if(obj->mesh->numFaces)
		{
			obj->mesh->faceIndex = (SHORTVECTOR *)Rom2Ram((u32)obj->mesh->faceIndex, memoryOffset);

			if(obj->mesh->faceNormals)
				obj->mesh->faceNormals = (BYTEVECTOR *)Rom2Ram((u32)obj->mesh->faceNormals, memoryOffset);
			if(obj->mesh->faceTC)
			{
				obj->mesh->faceTC = (USHORT2DVECTOR *)Rom2Ram((u32)obj->mesh->faceTC, memoryOffset);
				sprintf(mess,"OFTC%d",numoftc++);
				obj->mesh->originalFaceTC = (USHORT2DVECTOR *)JallocAlloc(sizeof(USHORT2DVECTOR) * 3 * obj->mesh->numFaces, YES,mess);
				memcpy(obj->mesh->originalFaceTC, obj->mesh->faceTC, sizeof(USHORT2DVECTOR) * 3 * obj->mesh->numFaces);
			}

			if(obj->mesh->faceFlags)
				obj->mesh->faceFlags = (char *)Rom2Ram((u32)obj->mesh->faceFlags, memoryOffset);
			if(obj->mesh->textureIDs)
			{
				obj->mesh->textureIDs = (TEXTURE **)Rom2Ram((u32)obj->mesh->textureIDs, memoryOffset);

				for(x = 0; x < obj->mesh->numFaces; x++)
					FindTexture(&(obj->mesh->textureIDs[x]), (u32)obj->mesh->textureIDs[x],YES, "");
			}
		}
		if(obj->mesh->vertexNormals)
			obj->mesh->vertexNormals = (BYTEVECTOR *)Rom2Ram((u32)obj->mesh->vertexNormals, memoryOffset);
	}

//////////initialise all sprites within object
	if(obj->phongTex)
		FindTexture(&(obj->phongTex), (u32)obj->phongTex,YES, "");
	tempInt = (u32)obj->sprites;
	if(tempInt)
	{
		obj->sprites = (OBJECTSPRITE *)Rom2Ram((u32)obj->sprites, memoryOffset);
		for(x = 0; x < obj->numSprites; x++)
		{
			FindTexture(&(obj->sprites[x].textureID), (u32)obj->sprites[x].textureID,YES, "");
		}
	}

	tempInt = (u32)obj->effectedVerts;
	if(tempInt)
	{
		obj->effectedVerts = (SKINVTX *)Rom2Ram((u32)obj->effectedVerts, memoryOffset);
		for(x = 0; x < obj->numVerts; x++)
		{
			obj->effectedVerts[x].verts = (s16*)Rom2Ram((u32)obj->effectedVerts[x].verts, memoryOffset);
		}
	}

	obj->originalVerts = (DUELVECTOR *)Rom2Ram((u32)obj->originalVerts, memoryOffset);


	tempInt = (u32)obj->drawList;
	if(tempInt)
		obj->drawList = (Gfx *)Rom2Ram((u32)obj->drawList, memoryOffset);


	tempInt = (u32)obj->scaleKeys;
	if(tempInt)
		obj->scaleKeys = (VKEYFRAME *)Rom2Ram((u32)obj->scaleKeys, memoryOffset);

	tempInt = (u32)obj->moveKeys;
	if(tempInt)
		obj->moveKeys = (VKEYFRAME *)Rom2Ram((u32)obj->moveKeys, memoryOffset);

	tempInt = (u32)obj->rotateKeys;
	if(tempInt)
		obj->rotateKeys = (QKEYFRAME *)Rom2Ram((u32)obj->rotateKeys, memoryOffset);

	tempInt = (u32)obj->children;
	if(tempInt)
		obj->children = (OBJECT *)Rom2Ram((u32)obj->children, memoryOffset);

	tempInt = (u32)obj->next;
	if(tempInt)
		obj->next = (OBJECT *)Rom2Ram((u32)obj->next, memoryOffset);


	if(obj->drawList)
		RestoreDrawList(obj->drawList, memoryOffset);


	if(obj->children)
		RestoreObjectPointers(obj->children, memoryOffset);
	if(obj->next)
		RestoreObjectPointers(obj->next, memoryOffset);
/*	u32	tempInt;
	int x;
	char mess[16];

////////////initialise mesh
	tempInt = (u32)obj->mesh;
	if(tempInt)
	{
		obj->mesh = (MESH *)Rom2Ram((u32)obj->mesh, memoryOffset);

		obj->mesh->vertices = (VECTOR *)Rom2Ram((u32)obj->mesh->vertices, memoryOffset);
		
		if(obj->mesh->numFaces)
		{

			obj->mesh->faceIndex = (SHORTVECTOR *)Rom2Ram((u32)obj->mesh->faceIndex, memoryOffset);

			if(obj->mesh->faceNormals)
				obj->mesh->faceNormals = (BYTEVECTOR *)Rom2Ram((u32)obj->mesh->faceNormals, memoryOffset);
			if(obj->mesh->faceTC)
			{
				obj->mesh->faceTC = (SHORT2DVECTOR *)Rom2Ram((u32)obj->mesh->faceTC, memoryOffset);
				sprintf(mess,"OFTC%d",numoftc++);
				obj->mesh->originalFaceTC = (SHORT2DVECTOR *)JallocAlloc(sizeof(SHORT2DVECTOR) * 3 * obj->mesh->numFaces, YES,mess);
				memcpy(obj->mesh->originalFaceTC, obj->mesh->faceTC, sizeof(SHORT2DVECTOR) * 3 * obj->mesh->numFaces);
			}

			if(obj->mesh->faceFlags)
				obj->mesh->faceFlags = (char *)Rom2Ram((u32)obj->mesh->faceFlags, memoryOffset);
			if(obj->mesh->textureIDs)
			{
				obj->mesh->textureIDs = (TEXTURE **)Rom2Ram((u32)obj->mesh->textureIDs, memoryOffset);

				for(x = 0; x < obj->mesh->numFaces; x++)
					FindTexture(&(obj->mesh->textureIDs[x]), (u32)obj->mesh->textureIDs[x],YES,"");
			}

		}
		if(obj->mesh->vertexNormals)
			obj->mesh->vertexNormals = (BYTEVECTOR *)Rom2Ram((u32)obj->mesh->vertexNormals, memoryOffset);
	}

//////////initialise all sprites within object
	tempInt = (u32)obj->sprites;
	if(tempInt)
	{
		obj->sprites = (OBJECTSPRITE *)Rom2Ram((u32)obj->sprites, memoryOffset);
		for(x = 0; x < obj->numSprites; x++)
		{
			FindTexture(&(obj->sprites[x].textureID), (u32)obj->sprites[x].textureID,YES,"");
		}
	}

	tempInt = (u32)obj->drawList;
	if(tempInt)
		obj->drawList = (Gfx *)Rom2Ram((u32)obj->drawList, memoryOffset);


	tempInt = (u32)obj->scaleKeys;
	if(tempInt)
		obj->scaleKeys = (KEYFRAME *)Rom2Ram((u32)obj->scaleKeys, memoryOffset);

	tempInt = (u32)obj->moveKeys;
	if(tempInt)
		obj->moveKeys = (KEYFRAME *)Rom2Ram((u32)obj->moveKeys, memoryOffset);

	tempInt = (u32)obj->rotateKeys;
	if(tempInt)
		obj->rotateKeys = (KEYFRAME *)Rom2Ram((u32)obj->rotateKeys, memoryOffset);

	tempInt = (u32)obj->children;
	if(tempInt)
		obj->children = (OBJECT *)Rom2Ram((u32)obj->children, memoryOffset);

	tempInt = (u32)obj->next;
	if(tempInt)
		obj->next = (OBJECT *)Rom2Ram((u32)obj->next, memoryOffset);


	if(obj->drawList)
		RestoreDrawList(obj->drawList, memoryOffset,obj);


	if(obj->children)
		RestoreObjectPointers(obj->children, memoryOffset);
	if(obj->next)
		RestoreObjectPointers(obj->next, memoryOffset);*/
}

/*	--------------------------------------------------------------------------------
	Function 	: LoadObjectBank
	Purpose 	: dma's an object bank from rom into ram.
	Parameters 	: char *objectbank
	Returns 	: none
	Info 		:
*/
void LoadObjectBank(int num)
{
	char				*objectBank;
	u32					bankRomStart, bankRomEnd, bankSize;
	short				x = -1, y = 0;
	OBJECT				*obj;
	OBJECT_CONTROLLER	*objCont;

	switch(num)
	{

		case GARDENMASTER_OBJ_BANK:
			bankRomStart	= (u32)&_objBank1SegmentRomStart;
			bankRomEnd		= (u32)&_objBank1SegmentRomEnd;
			sprintf(message, "GAR_MAS");
			break;

		case GARDENLEV1_OBJ_BANK:
			bankRomStart	= (u32)&_objBank2SegmentRomStart;
			bankRomEnd		= (u32)&_objBank2SegmentRomEnd;
			sprintf(message, "GAR_LEV1");
			break;

		case GARDENLEV2_OBJ_BANK:
			bankRomStart	= (u32)&_objBank3SegmentRomStart;
			bankRomEnd		= (u32)&_objBank3SegmentRomEnd;
			sprintf(message, "GAR_LEV2");
			break;

		case GARDENLEV3_OBJ_BANK:
			bankRomStart	= (u32)&_objBank4SegmentRomStart;
			bankRomEnd		= (u32)&_objBank4SegmentRomEnd;
			sprintf(message, "GAR_LEV3");
			break;

		case GARDENBOSSA_OBJ_BANK:
			bankRomStart	= (u32)&_objBank5SegmentRomStart;
			bankRomEnd		= (u32)&_objBank5SegmentRomEnd;
			sprintf(message, "GAR_BOSA");
			break;

		case GARDENBOSSB_OBJ_BANK:
			bankRomStart	= (u32)&_objBank6SegmentRomStart;
			bankRomEnd		= (u32)&_objBank6SegmentRomEnd;
			sprintf(message, "GAR_BOSB");
			break;

		case GARDENBONUS_OBJ_BANK:
			bankRomStart	= (u32)&_objBank7SegmentRomStart;
			bankRomEnd		= (u32)&_objBank7SegmentRomEnd;
			sprintf(message, "GAR_BON");
			break;

		case GARDENMULTI_OBJ_BANK:
			bankRomStart	= (u32)&_objBank8SegmentRomStart;
			bankRomEnd		= (u32)&_objBank8SegmentRomEnd;
			sprintf(message, "GAR_MUL");
			break;

		case ANCIENTMASTER_OBJ_BANK:
			bankRomStart	= (u32)&_objBank9SegmentRomStart;
			bankRomEnd		= (u32)&_objBank9SegmentRomEnd;
			sprintf(message, "ANC_MAS");
			break;

		case ANCIENTLEV1_OBJ_BANK:
			bankRomStart	= (u32)&_objBank10SegmentRomStart;
			bankRomEnd		= (u32)&_objBank10SegmentRomEnd;
			sprintf(message, "ANC_LEV1");
			break;

		case ANCIENTLEV2_OBJ_BANK:
			bankRomStart	= (u32)&_objBank11SegmentRomStart;
			bankRomEnd		= (u32)&_objBank11SegmentRomEnd;
			sprintf(message, "ANC_LEV2");
			break;

		case ANCIENTLEV3_OBJ_BANK:
			bankRomStart	= (u32)&_objBank12SegmentRomStart;
			bankRomEnd		= (u32)&_objBank12SegmentRomEnd;
			sprintf(message, "ANC_LEV3");
			break;

		case ANCIENTBOSSA_OBJ_BANK:
			bankRomStart	= (u32)&_objBank13SegmentRomStart;
			bankRomEnd		= (u32)&_objBank13SegmentRomEnd;
			sprintf(message, "ANC_BOSA");
			break;

		case ANCIENTBONUS_OBJ_BANK:
			bankRomStart	= (u32)&_objBank14SegmentRomStart;
			bankRomEnd		= (u32)&_objBank14SegmentRomEnd;
			sprintf(message, "ANC_BON");
			break;

		case ANCIENTMULTI_OBJ_BANK:
			bankRomStart	= (u32)&_objBank15SegmentRomStart;
			bankRomEnd		= (u32)&_objBank15SegmentRomEnd;
			sprintf(message, "ANC_MUL");
			break;

		default:
			dprintf"ERROR: no object bank specified....\n"));
			for(;;);
			break;
	}

	bankSize = bankRomEnd - bankRomStart;

	objectBank = (char *)JallocAlloc(DMAGetSize(bankRomStart,bankRomEnd),YES,message);

//start download from rom

	DMAMemory(objectBank, bankRomStart, bankRomEnd);
	if(objectBank)
		dprintf"Loaded object bank %s (size %d)\n",message,(int)bankSize));
	else
	{
		dprintf"Unable to load object bank %s\n",message));
		return;
	}

//store a pointer to the object bank so it may be free'd at a later date
	while(objectBanks[++x].freePtr != 0);
	if(x >= MAX_OBJECT_BANKS)
	{
		dprintf"ERROR:too many object banks\n"));
		return;
	}

	objectBanks[x].freePtr = objectBank;
	objectBanks[x].objList = (OBJECT_DESCRIPTOR *)objectBank;
	y = 0;
	/*while(objectBanks[x].objList[y].objCont)
	{
		obj = (OBJECT *)objectBanks[x].objList[y].objCont;
		objectBanks[x].objList[y].objCont = (OBJECT_CONTROLLER *)Rom2Ram((u32)obj, (u32)objectBank);
		objCont = objectBanks[x].objList[y].objCont;
				
		objCont->object = (OBJECT *)Rom2Ram((u32)objCont->object, (u32)objectBank);
		
		//restore animation pointers
		if(objCont->animation)
		{
			objCont->animation = (ACTOR_ANIMATION *)Rom2Ram((u32)objCont->animation, (u32)objectBank);
			if(objCont->animation->anims)
				objCont->animation->anims = (animation *)Rom2Ram((u32)objCont->animation->anims, (u32)objectBank);
		}
		RestoreObjectPointers(objCont->object, (u32)objectBank);
//		if((objCont->object->numScaleKeys == 1) && (objCont->object->numMoveKeys == 1) && (objCont->object->numRotateKeys == 1))
//			objCont->radius = CalcRadius(objCont->object->mesh);

		y++;
	}*/
	while(objectBanks[x].objList[y].objCont)
	{
		obj = (OBJECT *)objectBanks[x].objList[y].objCont;
		objectBanks[x].objList[y].objCont = (OBJECT_CONTROLLER *)Rom2Ram((u32)obj, (u32)objectBank);
		objCont = objectBanks[x].objList[y].objCont;
				
		objCont->object = (OBJECT *)Rom2Ram((u32)objCont->object, (u32)objectBank);
		
		//restore animation pointers
		if(objCont->animation)
		{
			objCont->animation = (ACTOR_ANIMATION *)Rom2Ram((u32)objCont->animation, (u32)objectBank);
			if(objCont->animation->anims)
				objCont->animation->anims = (animation *)Rom2Ram((u32)objCont->animation->anims, (u32)objectBank);
		}
		RestoreObjectPointers(objCont->object, (u32)objectBank);
		if(objCont->drawList)
		{
			/*objCont->drawList = (Gfx *)Rom2Ram((u32)objCont->drawList, (u32)objectBank);
			if(objCont->drawList)
				offsetVtx = FALSE;
			else
				offsetVtx = TRUE;		
			RestoreDrawList(objCont->drawList, (u32)objectBank);
			offsetVtx = TRUE;
  			*/
			objCont->drawList = (Gfx *)Rom2Ram((u32)objCont->drawList, (u32)objectBank);
			RestoreDrawList(objCont->drawList, (u32)objectBank);
		}
		if(objCont->vtx[0])
			objCont->vtx[0] = (Vtx *)Rom2Ram((u32)objCont->vtx[0], (u32)objectBank);
		if(objCont->vtx[1])
			objCont->vtx[1] = (Vtx *)Rom2Ram((u32)objCont->vtx[1], (u32)objectBank);

		if(objCont->LOCMesh)
		{
			objCont->LOCMesh = (MESH *)Rom2Ram((u32)objCont->LOCMesh,(u32)objectBank);
		}

		y++;
	}
	objectBanks[x].numObjects = y;	

//	if(num == SYSTEM_OBJ_BANK)
//		InitFadeOverlay();
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
float CalcRadius(MESH *mesh)
{
	int i;
	float radius = 0,temp;
	VECTOR *vertex;

	for(i = 0;i < mesh->numVertices;i++)
	{
		vertex = mesh->vertices+i;
		if((temp = Magnitude(vertex)) > radius)
			radius = temp;
	}
	return radius;
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void FindObject(OBJECT_CONTROLLER **objCPtr, int objID, char *name,BOOL vital)
{
	int x, y;
	char *temp;
			
	for(x = 0; x < MAX_OBJECT_BANKS; x++)
	{
		if(objectBanks[x].freePtr)
		{
			temp = objectBanks[x].data;
			for(y = 0; y < objectBanks[x].numObjects; y++)
			{
				if(objectBanks[x].objList[y].objectID == objID)
				{
					(*objCPtr) = objectBanks[x].objList[y].objCont;
//					if(name)
//						dprintf"Found object %s\n", name));
					return;
				}

			}
		}
	}
	*objCPtr = NULL;
	if(name)
	{
		if(vital)
		{
			dprintf"Object '%s' not found\n", name));
//			sprintf(gameInfo.error,"LEVEL UNPLAYABLE - %s MISSING",name);
		}
//		Crash();
	}
	else
	{
		dprintf"Whoops, object '%X' not found!\n", objID));
//		if(vital)
//			sprintf(gameInfo.error,"LEVEL UNPLAYABLE - OBJECT MISSING");
 //		Crash();
	}
	if(vital)
	{
//		StopDrawing("fondobj");
//help		disableGraphics = TRUE;
//		gameInfo.state = ERROR_SCREEN_MODE;
	}

	return;
}

/*	--------------------------------------------------------------------------------
	Function 	: FindSubObjectFromObject
	Purpose 	: called from FindSubObject - recursive search of an object
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
OBJECT *FindSubObjectFromObject(OBJECT *obj, int objID)
{
	OBJECT	*tempObj = NULL;

	tempObj = obj;

	if(tempObj->objID == objID)
		return tempObj;	

	if(obj->children)
	{
		tempObj = FindSubObjectFromObject(obj->children, objID);
		if(tempObj)
		{
			if(tempObj->objID == objID)
				return tempObj;	
		}
	}

	if(obj->next)
	{
		tempObj = FindSubObjectFromObject(obj->next, objID);
		if(tempObj)
		{
			if(tempObj->objID == objID)
				return tempObj;	
		}
	}

	return NULL;
}

/*	--------------------------------------------------------------------------------
	Function 	: FindSubObject
	Purpose 	: searches an object and all its children for a specified object
	Parameters 	: OBJECT **, required id
	Returns 	: none
	Info 		:
*/
void FindSubObject(OBJECT **objPtr, int objID)
{
	int x, y;
	OBJECT	*tempObj, *foundObj;

	for(x = 0; x < MAX_OBJECT_BANKS; x++)
	{
		if(objectBanks[x].freePtr)
		{
			for(y = 0; y < objectBanks[x].numObjects; y++)
			{
				tempObj = objectBanks[x].objList[y].objCont->object;
				foundObj = FindSubObjectFromObject(tempObj, objID);				   		
				if(foundObj)
				{
					(*objPtr) = foundObj;
					return;
				}
					
			}
		}
	}

	*objPtr = NULL;
	dprintf"Whoops, object '%X' not found!\n", objID));
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void WriteObjectDisplayListFlat(OBJECT *obj)
{
/*	int x, y, face = 0,j;
	MESH	*mesh = obj->mesh;
	SHORT2DVECTOR	*tcp = mesh->faceTC;
	VECTOR		*baseVertices = mesh->vertices;
	VECTOR		*vertex;
	SHORTVECTOR	*baseFaceIndex = mesh->faceIndex;
	SHORTVECTOR *faceIndex;
	Vtx			*batchPtr;
	int			fC = mesh->numFaces / 10;
	int			mod = mesh->numFaces % 10, temp;
	TEXTURE		*texture = NULL;
	BYTEVECTOR	*normalPtr;
	BYTEVECTOR	*baseNormalPtr = mesh->faceNormals;

    gDPPipeSync(glistp++);

	for(x = 0; x < fC; x++)
	{
		batchPtr = vtxPtr;

		for(y = 0; y < 10; y++)
		{		
			faceIndex = baseFaceIndex + face;
			normalPtr = baseNormalPtr + face;

			vertex = baseVertices + faceIndex->v[X];
  			V((vtxPtr), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				normalPtr->v[X], normalPtr->v[Y], normalPtr->v[Z], xluFact);

			vertex = baseVertices + faceIndex->v[Y];
			V((vtxPtr+1), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				normalPtr->v[X], normalPtr->v[Y], normalPtr->v[Z], xluFact);

			vertex = baseVertices + faceIndex->v[Z];
			V((vtxPtr+2), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				normalPtr->v[X], normalPtr->v[Y], normalPtr->v[Z], xluFact);



  
			face++;			
			vtxPtr+=3;
//			if(face == mesh->numFaces)
//				break;

		}

		gSPVertex(glistp++, batchPtr , 30, 0);

		for(j = 0;j < 10;j++)
		{
			if(texture != obj->mesh->textureIDs[face-10+j]	)
			{
				texture = obj->mesh->textureIDs[face-10+j];
			    gDPPipeSync(glistp++);
				LoadTexture(texture);
			}
			gSP1Triangle(glistp++, j*3, j*3+1, j*3+2, 2);
		}
	}

//handle remaining faces
	batchPtr = vtxPtr;

	for(y = 0; y < mod; y++)
	{		
		faceIndex = baseFaceIndex + face;
		normalPtr = baseNormalPtr + face;

		vertex = baseVertices + faceIndex->v[0];
		V((vtxPtr), 
			(int)vertex->v[0],
			(int)vertex->v[1],
			(int)vertex->v[2],0,
			tcp->v[0], (tcp++)->v[1],
			normalPtr->v[X], normalPtr->v[Y], normalPtr->v[Z], xluFact);

		vertex = baseVertices + faceIndex->v[1];
		V((vtxPtr+1), 
			(int)vertex->v[0],
			(int)vertex->v[1],
			(int)vertex->v[2],0,
			tcp->v[0], (tcp++)->v[1],
  			normalPtr->v[X], normalPtr->v[Y], normalPtr->v[Z], xluFact);

		vertex = baseVertices + faceIndex->v[2];
  		V((vtxPtr+2), 
			(int)vertex->v[0],
			(int)vertex->v[1],
			(int)vertex->v[2],0,
			tcp->v[0], (tcp++)->v[1],
			normalPtr->v[X], normalPtr->v[Y], normalPtr->v[Z], xluFact);


		face++;			
		vtxPtr+=3;

	}

	if(mod)
	{
		face -= mod;
		gSPVertex(glistp++, batchPtr , mod * 3, 0);
		temp = 0;
		for(x = 0; x < mod; x++)
		{
			if(texture != obj->mesh->textureIDs[face])
			{
				texture = obj->mesh->textureIDs[face];		
				gDPPipeSync(glistp++);
				LoadTexture(texture);
			}
			gSP1Triangle(glistp++, temp++, temp++, temp++, 2);
			face++;
		}
	}*/
	int x, y, face = 0,j;
	MESH	*mesh = obj->mesh;
	USHORT2DVECTOR	*tcp = mesh->faceTC;
	VECTOR		*baseVertices = mesh->vertices;
	VECTOR		*vertex;
	SHORTVECTOR	*baseFaceIndex = mesh->faceIndex;
	SHORTVECTOR *faceIndex;
	Vtx			*batchPtr;
	int			fC = mesh->numFaces / 10;
	int			mod = mesh->numFaces % 10, temp;
	TEXTURE		*texture = NULL;
	BYTEVECTOR	*normalPtr;
	BYTEVECTOR	*baseNormalPtr = mesh->faceNormals;

    gDPPipeSync(glistp++);

	for(x = 0; x < fC; x++)
	{
		batchPtr = vtxPtr;

		for(y = 0; y < 10; y++)
		{		
			faceIndex = baseFaceIndex + face;
			normalPtr = baseNormalPtr + face;

			vertex = baseVertices + faceIndex->v[X];
  			V((vtxPtr), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				normalPtr->v[X], normalPtr->v[Y], normalPtr->v[Z], xluFact);

			vertex = baseVertices + faceIndex->v[Y];
			V((vtxPtr+1), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				normalPtr->v[X], normalPtr->v[Y], normalPtr->v[Z], xluFact);

			vertex = baseVertices + faceIndex->v[Z];
			V((vtxPtr+2), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				normalPtr->v[X], normalPtr->v[Y], normalPtr->v[Z], xluFact);



  
			face++;			
			vtxPtr+=3;
//			if(face == mesh->numFaces)
//				break;

		}

		gSPVertex(glistp++, batchPtr , 30, 0);

		for(j = 0;j < 10;j++)
		{
			if(texture != obj->mesh->textureIDs[face-10+j]	)
			{
				texture = obj->mesh->textureIDs[face-10+j];
			    gDPPipeSync(glistp++);
				LoadTexture(texture);
			}
			gSP1Triangle(glistp++, j*3, j*3+1, j*3+2, 2);
		}
	}

//handle remaining faces
	batchPtr = vtxPtr;

	for(y = 0; y < mod; y++)
	{		
		faceIndex = baseFaceIndex + face;
		normalPtr = baseNormalPtr + face;

		vertex = baseVertices + faceIndex->v[0];
		V((vtxPtr), 
			(int)vertex->v[0],
			(int)vertex->v[1],
			(int)vertex->v[2],0,
			tcp->v[0], (tcp++)->v[1],
			normalPtr->v[X], normalPtr->v[Y], normalPtr->v[Z], xluFact);

		vertex = baseVertices + faceIndex->v[1];
		V((vtxPtr+1), 
			(int)vertex->v[0],
			(int)vertex->v[1],
			(int)vertex->v[2],0,
			tcp->v[0], (tcp++)->v[1],
  			normalPtr->v[X], normalPtr->v[Y], normalPtr->v[Z], xluFact);

		vertex = baseVertices + faceIndex->v[2];
  		V((vtxPtr+2), 
			(int)vertex->v[0],
			(int)vertex->v[1],
			(int)vertex->v[2],0,
			tcp->v[0], (tcp++)->v[1],
			normalPtr->v[X], normalPtr->v[Y], normalPtr->v[Z], xluFact);


		face++;			
		vtxPtr+=3;

	}

	if(mod)
	{
		face -= mod;
		gSPVertex(glistp++, batchPtr , mod * 3, 0);
		temp = 0;
		for(x = 0; x < mod; x++)
		{
			if(texture != obj->mesh->textureIDs[face])
			{
				texture = obj->mesh->textureIDs[face];		
				gDPPipeSync(glistp++);
				LoadTexture(texture);
			}
			gSP1Triangle(glistp++, temp++, temp++, temp++, 2);
			face++;
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void WriteObjectDisplayListGouraud(OBJECT *obj)
{
/**	int x, y, face = 0,j;
	MESH	*mesh = obj->mesh;
	SHORT2DVECTOR	*tcp = mesh->faceTC;
	VECTOR		*baseVertices = mesh->vertices;
	VECTOR		*vertex;
	SHORTVECTOR	*baseFaceIndex = mesh->faceIndex;
	SHORTVECTOR *faceIndex;
	Vtx			*batchPtr;
	int			fC = mesh->numFaces / 10;
	int			mod = mesh->numFaces % 10, temp;
	TEXTURE		*texture = NULL;
	BYTEVECTOR	*vNormalPtr;
	BYTEVECTOR	*baseVNormalPtr = mesh->vertexNormals;

    gDPPipeSync(glistp++);

	for(x = 0; x < fC; x++)
	{
		batchPtr = vtxPtr;

		for(y = 0; y < 10; y++)
		{		
			faceIndex = baseFaceIndex + face;
//			tcp = mesh->faceTC + face;

			vertex = baseVertices + faceIndex->v[X];
			vNormalPtr = baseVNormalPtr + faceIndex->v[X];
  			V((vtxPtr), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

			vertex = baseVertices + faceIndex->v[Y];
			vNormalPtr = baseVNormalPtr + faceIndex->v[Y];
			V((vtxPtr+1), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

			vertex = baseVertices + faceIndex->v[Z];
			vNormalPtr = baseVNormalPtr + faceIndex->v[Z];
			V((vtxPtr+2), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

  
			face++;			
			vtxPtr+=3;

		}

		gSPVertex(glistp++, batchPtr , 30, 0);
	    gDPPipeSync(glistp++);


		for(j = 0;j < 10;j++)
		{
			if(texture != obj->mesh->textureIDs[face-10+j]	)
			{
				texture = obj->mesh->textureIDs[face-10+j];
			    gDPPipeSync(glistp++);
				LoadTexture(texture);
			}
			gSP1Triangle(glistp++, j*3, j*3+1, j*3+2, 2);
		}
	}

//handle remaining faces
	batchPtr = vtxPtr;

	for(y = 0; y < mod; y++)
	{		
		faceIndex = baseFaceIndex + face;

		vertex = baseVertices + faceIndex->v[0];
		vNormalPtr = baseVNormalPtr + faceIndex->v[X];
		V((vtxPtr), 
			(int)vertex->v[0],
			(int)vertex->v[1],
			(int)vertex->v[2],0,
			tcp->v[0], (tcp++)->v[1],
			vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

		vertex = baseVertices + faceIndex->v[1];
		vNormalPtr = baseVNormalPtr + faceIndex->v[Y];
		V((vtxPtr+1), 
			(int)vertex->v[0],
			(int)vertex->v[1],
			(int)vertex->v[2],0,
			tcp->v[0], (tcp++)->v[1],
			vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

		vertex = baseVertices + faceIndex->v[2];
		vNormalPtr = baseVNormalPtr + faceIndex->v[Z];
  		V((vtxPtr+2), 
			(int)vertex->v[0],
			(int)vertex->v[1],
			(int)vertex->v[2],0,
			tcp->v[0], (tcp++)->v[1],
			vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);


		face++;			
		vtxPtr+=3;

	}

	if(mod)
	{
		face -= mod;
		gSPVertex(glistp++, batchPtr , mod * 3, 0);
		temp = 0;
		for(x = 0; x < mod; x++)
		{
			if(texture != obj->mesh->textureIDs[face])
			{
				texture = obj->mesh->textureIDs[face];
			    gDPPipeSync(glistp++);
				LoadTexture(texture);
			}
			gSP1Triangle(glistp++, temp++, temp++, temp++, 2);
			face++;
		}
	}

    gDPPipeSync(glistp++);*/
	int x, y, face = 0,j;
	MESH	*mesh = obj->mesh;
	USHORT2DVECTOR	*tcp = mesh->faceTC;
	VECTOR		*baseVertices = mesh->vertices;
	VECTOR		*vertex;
	SHORTVECTOR	*baseFaceIndex = mesh->faceIndex;
	SHORTVECTOR *faceIndex;
	Vtx			*batchPtr;
	int			fC = mesh->numFaces / 10;
	int			mod = mesh->numFaces % 10, temp;
	TEXTURE		*texture = NULL;
	BYTEVECTOR	*vNormalPtr;
	BYTEVECTOR	*baseVNormalPtr = mesh->vertexNormals;


    gDPPipeSync(glistp++);

	for(x = 0; x < fC; x++)
	{
		batchPtr = vtxPtr;

		for(y = 0; y < 10; y++)
		{		
			faceIndex = baseFaceIndex + face;
//			tcp = mesh->faceTC + face;

			vertex = baseVertices + faceIndex->v[X];
			vNormalPtr = baseVNormalPtr + faceIndex->v[X];
  			V((vtxPtr), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

			vertex = baseVertices + faceIndex->v[Y];
			vNormalPtr = baseVNormalPtr + faceIndex->v[Y];
			V((vtxPtr+1), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

			vertex = baseVertices + faceIndex->v[Z];
			vNormalPtr = baseVNormalPtr + faceIndex->v[Z];
			V((vtxPtr+2), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

  
			face++;			
			vtxPtr+=3;

		}

		gSPVertex(glistp++, batchPtr , 30, 0);
	    gDPPipeSync(glistp++);


		for(j = 0;j < 10;j++)
		{
			if(texture != obj->mesh->textureIDs[face-10+j]	)
			{
				texture = obj->mesh->textureIDs[face-10+j];
			    gDPPipeSync(glistp++);
				LoadTexture(texture);
			}
			gSP1Triangle(glistp++, j*3, j*3+1, j*3+2, 2);
		}
	}

//handle remaining faces
	batchPtr = vtxPtr;

	for(y = 0; y < mod; y++)
	{		
		faceIndex = baseFaceIndex + face;

		vertex = baseVertices + faceIndex->v[0];
		vNormalPtr = baseVNormalPtr + faceIndex->v[X];
		V((vtxPtr), 
			(int)vertex->v[0],
			(int)vertex->v[1],
			(int)vertex->v[2],0,
			tcp->v[0], (tcp++)->v[1],
			vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

		vertex = baseVertices + faceIndex->v[1];
		vNormalPtr = baseVNormalPtr + faceIndex->v[Y];
		V((vtxPtr+1), 
			(int)vertex->v[0],
			(int)vertex->v[1],
			(int)vertex->v[2],0,
			tcp->v[0], (tcp++)->v[1],
			vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

		vertex = baseVertices + faceIndex->v[2];
		vNormalPtr = baseVNormalPtr + faceIndex->v[Z];
  		V((vtxPtr+2), 
			(int)vertex->v[0],
			(int)vertex->v[1],
			(int)vertex->v[2],0,
			tcp->v[0], (tcp++)->v[1],
			vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);


		face++;			
		vtxPtr+=3;

	}

	if(mod)
	{
		face -= mod;
		gSPVertex(glistp++, batchPtr , mod * 3, 0);
		temp = 0;
		for(x = 0; x < mod; x++)
		{
			if(texture != obj->mesh->textureIDs[face])
			{
				texture = obj->mesh->textureIDs[face];
			    gDPPipeSync(glistp++);
				LoadTexture(texture);
			}
			gSP1Triangle(glistp++, temp++, temp++, temp++, 2);
			face++;
		}
	}

    gDPPipeSync(glistp++);
}


/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void LoadTexture(TEXTURE *texture)
{
	if(texture == 0)
		return;

	switch(texture->pixSize)
	{
		case G_IM_SIZ_16b:
			gDPSetTextureLUT(glistp++,	G_TT_NONE);
			if(mirrorTextures)
			{
				gDPLoadTextureBlock(glistp++, texture->data , texture->format, G_IM_SIZ_16b, texture->sx, texture->sy, 0,G_TX_WRAP | G_TX_MIRROR, G_TX_WRAP | G_TX_MIRROR,texture->TCScaleX, texture->TCScaleY, G_TX_NOLOD, G_TX_NOLOD);
			}
			else
			{
				gDPLoadTextureBlock(glistp++, texture->data , texture->format, G_IM_SIZ_16b, texture->sx, texture->sy, 0,G_TX_WRAP /*| G_TX_MIRROR*/, G_TX_WRAP /*| G_TX_MIRROR*/,texture->TCScaleX, texture->TCScaleY, G_TX_NOLOD, G_TX_NOLOD);
			}
			break;

		case G_IM_SIZ_8b:

			gDPSetTextureLUT(glistp++,	G_TT_RGBA16);
			gDPLoadTLUT_pal256(glistp++, texture->palette);
			if(mirrorTextures)
			{
				gDPLoadTextureBlock(glistp++, texture->data , texture->format, G_IM_SIZ_8b, texture->sx, texture->sy, 0,G_TX_WRAP | G_TX_MIRROR, G_TX_WRAP | G_TX_MIRROR,texture->TCScaleX, texture->TCScaleY, G_TX_NOLOD, G_TX_NOLOD);
			}
			else
			{
				gDPLoadTextureBlock(glistp++, texture->data , texture->format, G_IM_SIZ_8b, texture->sx, texture->sy, 0,G_TX_WRAP/* | G_TX_MIRROR*/, G_TX_WRAP/* | G_TX_MIRROR*/,texture->TCScaleX, texture->TCScaleY, G_TX_NOLOD, G_TX_NOLOD);
			}
			break;

		case G_IM_SIZ_4b:

			gDPSetTextureLUT(glistp++,	G_TT_RGBA16);
			gDPLoadTLUT_pal16(glistp++,0,texture->palette);
			if(mirrorTextures)
			{
				gDPLoadTextureBlock_4b(glistp++, texture->data , texture->format, texture->sx, texture->sy, 0,G_TX_WRAP | G_TX_MIRROR, G_TX_WRAP | G_TX_MIRROR,texture->TCScaleX, texture->TCScaleY, G_TX_NOLOD, G_TX_NOLOD);
			}
			else
			{
				gDPLoadTextureBlock_4b(glistp++, texture->data , texture->format, texture->sx, texture->sy, 0,G_TX_WRAP/* | G_TX_MIRROR*/, G_TX_WRAP/* | G_TX_MIRROR*/,texture->TCScaleX, texture->TCScaleY, G_TX_NOLOD, G_TX_NOLOD);
			}
			break;
	}
}


/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void LoadPhongTexture(TEXTURE *texture)
{
	if(texture == 0)
		return;

	switch(texture->pixSize)
	{
		case G_IM_SIZ_16b:
			gDPSetTextureLUT(glistp++,	G_TT_NONE);
			_gDPLoadTextureBlockTile(glistp++,texture->data,256,1,texture->format,G_IM_SIZ_16b, texture->sx, texture->sy,0,G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, texture->TCScaleX, texture->TCScaleY, G_TX_NOLOD, G_TX_NOLOD);
			break;

		case G_IM_SIZ_8b:

			gDPSetTextureLUT(glistp++,	G_TT_RGBA16);
			gDPLoadTLUT_pal256(glistp++, texture->palette);
			_gDPLoadTextureBlockTile(glistp++,texture->data,0,1,texture->format,G_IM_SIZ_8b, texture->sx, texture->sy,0,G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, texture->TCScaleX, texture->TCScaleY, G_TX_NOLOD, G_TX_NOLOD);
			break;

		case G_IM_SIZ_4b:

			gDPSetTextureLUT(glistp++,	G_TT_RGBA16);
			gDPLoadTLUT_pal16(glistp++,0,texture->palette);
			_gDPLoadTextureBlockTile(glistp++,texture->data,0,1,texture->format,G_IM_SIZ_4b, texture->sx, texture->sy,0,G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, texture->TCScaleX, texture->TCScaleY, G_TX_NOLOD, G_TX_NOLOD);
			break;
	}
}


/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void StopDrawing(char *whereami)
{
	if(codeDrawingRequest == TRUE)
		dprintf"Stop Drawing Requested from '%s'\n", whereami));
	codeDrawingRequest = FALSE;
	while(gfxIsDrawing == TRUE);
	disableGraphics = TRUE;
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void StartDrawing(char *whereami)
{
	if(codeDrawingRequest == FALSE)
		dprintf"Start Drawing Requested from '%s'\n", whereami));
	codeDrawingRequest = TRUE;
	while(gfxIsDrawing == FALSE);
	disableGraphics = FALSE;
	onlyDrawBackdrops = FALSE;
}


/*	--------------------------------------------------------------------------------
	Function		: SetObjectXlu
	Purpose			: sets an object to be translucent
	Parameters		: OBJECT *,BOOL
	Returns			: void
	Info			: 
*/
void SetObjectXlu(OBJECT *object,BOOL xlu)
{
	if(xlu)
		object->flags |= OBJECT_FLAGS_XLU;
	else
		object->flags &= -1 - OBJECT_FLAGS_XLU;
	if(object->children)
		SetObjectXlu(object->children,xlu);
	if(object->next)
		SetObjectXlu(object->next,xlu);
}


/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void SetXluInDrawList(Gfx *dl,UBYTE xlu)
{
	u8 *temp = (u8 *)dl;

	while(*temp != 0xDF)
	{
		if(*temp == 0xFA)
		{
			*(temp+7) = xlu;
		}
		temp += 8;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void ReplaceTextureInDrawList(Gfx *dl,u32 texDataToReplace,u32 newTexData, short once)
{
	u8 *temp = (u8 *)dl; 
	u32 address,*addrP;

	while(*temp != 0xDF)
	{
		if(*temp == 0xFD)
		{
			addrP = (u32 *)(temp + 4);
			address = *addrP;
			if(*(temp + 8) == 0xf5)
			{			
				if((texDataToReplace == NULL) || (address == texDataToReplace))
				{
					*addrP = newTexData;
					if(once)
						return;
				}
			}
		}
		temp += 8;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void ReplaceAnyTextureInDrawList(Gfx *dl,u32 newTexData, short once)
{
	u8 *temp = (u8 *)dl; 
	u32 address,*addrP;

	while(*temp != 0xDF)
	{
		if(*temp == 0xFD)
		{
			addrP = (u32 *)(temp + 4);
			address = *addrP;
			if(*(temp + 8) == 0xf5)
			{
				*addrP = newTexData;
			
				if(once)
					return;
			}
		}
		temp += 8;
	}
}


/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
COLOUR watCol = {60,255,200,255};
void WriteObjectDisplayListGouraudPhong(OBJECT *obj)
{
	int x, y, face = 0,j;
	MESH	*mesh = obj->mesh;
	USHORT2DVECTOR	*tcp = mesh->faceTC;
	VECTOR		*baseVertices = mesh->vertices;
	VECTOR		*vertex;
	SHORTVECTOR	*baseFaceIndex = mesh->faceIndex;
	SHORTVECTOR *faceIndex;
	Vtx			*batchPtr;
	int			fC = mesh->numFaces / 10;
	int			mod = mesh->numFaces % 10, temp;
	BYTEVECTOR	*vNormalPtr;
	BYTEVECTOR	*basevNormalPtr = mesh->vertexNormals;


    gDPPipeSync(glistp++);

	gDPSetPrimColor(glistp++,0,0,watCol.r,watCol.g,watCol.b,xluFact);

	for(x = 0; x < fC; x++)
	{
		batchPtr = vtxPtr;

		for(y = 0; y < 10; y++)
		{		
			faceIndex = baseFaceIndex + face;

			vertex = baseVertices + faceIndex->v[X];
			vNormalPtr = basevNormalPtr + faceIndex->v[X];
  			V((vtxPtr), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

			vertex = baseVertices + faceIndex->v[Y];
			vNormalPtr = basevNormalPtr + faceIndex->v[Y];
			V((vtxPtr+1), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

			vertex = baseVertices + faceIndex->v[Z];
			vNormalPtr = basevNormalPtr + faceIndex->v[Z];
			V((vtxPtr+2), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);
  
			face++;			
			vtxPtr+=3;

		}

		gSPVertex(glistp++, batchPtr , 30, 0);
	    gDPPipeSync(glistp++);


		for(j = 0;j < 10;j++)
		{
			gSP1Triangle(glistp++, j*3, j*3+1, j*3+2, 2);
		}
	}

//handle remaining faces
	batchPtr = vtxPtr;

	if(mod)
	{
		for(y = 0; y < mod; y++)
		{		
			faceIndex = baseFaceIndex + face;

			vertex = baseVertices + faceIndex->v[X];
			vNormalPtr = basevNormalPtr + faceIndex->v[X];
			V((vtxPtr), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

			vertex = baseVertices + faceIndex->v[Y];
			vNormalPtr = basevNormalPtr + faceIndex->v[Y];
			V((vtxPtr+1), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

			vertex = baseVertices + faceIndex->v[Z];
			vNormalPtr = basevNormalPtr + faceIndex->v[Z];
  			V((vtxPtr+2), 
				(int)vertex->v[0],
				(int)vertex->v[1],
				(int)vertex->v[2],0,
				tcp->v[0], (tcp++)->v[1],
				vNormalPtr->v[X], vNormalPtr->v[Y], vNormalPtr->v[Z], xluFact);

			face++;			
			vtxPtr+=3;

		}

		face -= mod;
		gSPVertex(glistp++, batchPtr , mod * 3, 0);
		temp = 0;
		for(x = 0; x < mod; x++)
		{
			gSP1Triangle(glistp++, temp++, temp++, temp++, 2);
			face++;
		}
	}

    gDPPipeSync(glistp++);
}


