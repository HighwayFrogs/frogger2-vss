/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.
	
	  
	File		: eventfuncs.c
	Programmer	: Jim Hubbard
	Date		: 30/06/99

	Info		: Trigger and event functions for the event system, and setups for 
					level specific events.

----------------------------------------------------------------------------------------------- */

#include <ultra64.h>
#include "incs.h"



/*----- [ TRIGGER FUNCTIONS ] ------------------------------------------------------------------*/

/*	--------------------------------------------------------------------------------
	Function 	: EnemyOnTile
	Purpose 	: Check if an enemy in the trigger structure is on the given tile
	Parameters 	: Pointer to trigger structure
	Returns 	: Boolean
	Info 		:
*/
int EnemyOnTile( TRIGGER *trigger )
{
	ENEMY *nme = (ENEMY *)trigger->data[0];
	GAMETILE *tile = (GAMETILE *)trigger->data[1];

	if( nme->inTile == tile ) return 1;
	else return 0;
}


/*	--------------------------------------------------------------------------------
	Function 	: FrogOnTile
	Purpose 	: Check if a frog in the trigger structure is on the given tile
	Parameters 	: Pointer to trigger structure
	Returns 	: Boolean
	Info 		:
*/
int FrogOnTile( TRIGGER *trigger )
{
	int frognum = *(int *)trigger->data[0];
	GAMETILE *tile = (GAMETILE *)trigger->data[1];

	if( currTile[frognum] == tile ) return 1;
	else return 0;
}


/*	--------------------------------------------------------------------------------
	Function 	: ActorWithinRadius
	Purpose 	: Check if the actor in the trigger structure is within a distance of a point
	Parameters 	: Pointer to trigger structure
	Returns 	: Boolean
	Info 		:
*/
int ActorWithinRadius( TRIGGER *trigger )
{
	ACTOR *actor = (ACTOR *)trigger->data[0];
	VECTOR *pos = (VECTOR *)trigger->data[1];
	float radius = *(float *)trigger->data[2];

	if( DistanceBetweenPoints(&actor->pos, pos) < radius ) return 1;
	else return 0;
}


/*	--------------------------------------------------------------------------------
	Function 	: OnTimeout
	Purpose 	: Trigger when counter reaches zero
	Parameters 	: Pointer to trigger structure
	Returns 	: Boolean
	Info 		:
*/
int OnTimeout( TRIGGER *trigger )
{
	int count = *(int *)trigger->data[0];  // Counter
	int start = *(int *)trigger->data[1];  // Count down from this

	if( count-- == -1 ) count = start;
	else if( count-- == 0 ) return 1;
	else count--;
}



/*----- [ EVENT FUNCTIONS ] --------------------------------------------------------------------*/

/*	--------------------------------------------------------------------------------
	Function 	: ChangeActorScale
	Purpose 	: Alters the scale property of the actor
	Parameters 	: Pointer to event structure
	Returns 	: 
	Info 		:
*/
void ChangeActorScale( EVENT *event )
{
	ACTOR *actor = (ACTOR *)event->data[0];
	VECTOR *scale = (VECTOR *)event->data[1];

	actor->scale = *scale;
}



/*----- [ LEVEL SETUPS ] -----------------------------------------------------------------------*/

/*	--------------------------------------------------------------------------------
	Function 	: SetupEventsForLevel
	Purpose 	: Adds all triggers and events for the current level
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void InitEventsForLevel( unsigned long worldID, unsigned long levelID )
{
	void **args;
	TRIGGER *trigger;
	EVENT *event;
	VECTOR *pos = (VECTOR *)JallocAlloc( sizeof(VECTOR),YES,"Vector" );
	VECTOR *scale = (VECTOR *)JallocAlloc( sizeof(VECTOR),YES,"Vector" );
	float *rad;

	if ( worldID == WORLDID_GARDEN )
	{
		if ( levelID == LEVELID_GARDENLAWN )
		{
		}
		else if( levelID == LEVELID_GARDENMAZE )
		{
		}
		else if( levelID == LEVELID_GARDENVEGPATCH )
		{
			return;

			InitTriggerList( );

			pos->v[0] = -75.0;
			pos->v[1] = 0.0;
			pos->v[2] = -375.0;
			
			rad = (float *)JallocAlloc( sizeof(float),YES,"Float" );
			*rad = 100;

			// Test trigger
			args = (void **)JallocAlloc( (sizeof(void *)*3),YES,"EventData" );
			args[0] = (void *)frog[0]->actor;
			args[1] = (void *)pos;
			args[2] = (void *)rad;

			trigger = MakeTrigger( ActorWithinRadius, 3, args );

			scale->v[0] = 2.0;
			scale->v[1] = 2.0;
			scale->v[2] = 2.0;

			// Test event
			args = (void **)JallocAlloc( (sizeof(void *)*2),YES,"EventData" );
			args[0] = (void *)frog[0]->actor;
			args[1] = (void *)scale;

			event = MakeEvent( ChangeActorScale, 2, args );

			// Attach event to trigger
			AttachEvent( trigger, event );
		}
	} // etc
}