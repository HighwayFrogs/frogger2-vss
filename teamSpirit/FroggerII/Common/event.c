/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.
	
	  
	File		: event.c
	Programmer	: Jim Hubbard
	Date		: 30/06/99

	Info		: A trigger has a condition function that is checked every iteration.
					It also has an array of associated data objects that are needed
					by the function, and a list of events whose action functions are 
					triggered when the condition function is true.

				: An event has a pointer to an action function, and an array of 
					associated data objects.

				: An event is associated with a trigger. More than one event can be
					attached to a trigger, and more than one trigger can have the same
					event attached to it.
	
----------------------------------------------------------------------------------------------- */

#include <ultra64.h>
#include "incs.h"

//#include <stdarg.h>

TRIGGERLIST triggerList;


/*	--------------------------------------------------------------------------------
	Function 	: CreateTrigger
	Purpose 	: Make a trigger structure 
	Parameters 	: Pointer to checking function, a number of pointers to data objects
	Returns 	: Created trigger structure
	Info 		:
*/
TRIGGER * MakeTrigger( int (*func)(TRIGGER *t), unsigned int numargs, void ** args )
{
	//va_list args;
	int i;
	TRIGGER *trigger = (TRIGGER *)JallocAlloc(sizeof(TRIGGER),YES,"Trigger");
	
	trigger->prev = trigger->next = NULL;

	InitEventList( &trigger->events );
	
	trigger->func = func;
	trigger->data = args;
	
	/*trigger->data = (void **)JallocAlloc( (sizeof(void *)*numargs),YES,"EventData" );

	// Get variable number of arguments
	va_start( args, numargs );

	for( i=0; i < numargs; i++ )
		trigger->data[i] = va_arg(args,void*);

	va_end( args );*/

	return trigger;
}


/*	--------------------------------------------------------------------------------
	Function 	: CreateEvent
	Purpose 	: Make an event structure
	Parameters 	: As for Create trigger, except function performs action
	Returns 	: Event structure
	Info 		:
*/
EVENT * MakeEvent( void (*func)(EVENT *e), unsigned int numargs, void **args )
{
	//va_list args;
	int i = 0;
	EVENT *event = (EVENT *)JallocAlloc(sizeof(EVENT),YES,"Event");
	
	event->prev = event->next = NULL;

	event->func = func;
	event->data = args;
	
	/*event->data = (void **)JallocAlloc( (sizeof(void *)*numargs),YES,"EventData" );

	// Get variable number of arguments
	va_start( args, numargs );

	for( i=0; i < numargs; i++ )
		event->data[i] = va_arg(args,void*);

	va_end( args );*/

	return event;
}


/*	--------------------------------------------------------------------------------
	Function 	: AttachEvent
	Purpose 	: Adds trigger to list if it doesn't have any events attached already,
					and adds the event to a list in the trigger.
	Parameters 	: Trigger, event to attach to trigger
	Returns 	: 
	Info 		:
*/
void AttachEvent( TRIGGER *trigger, EVENT *event )
{
	TRIGGER *tp;
	EVENT *ep;

	// If this trigger has not had an event attached to it before, add it to the active list.
	if( !(trigger->events.numEntries) )
	{
		tp = triggerList.head.next;
		
		trigger->prev = tp;
		trigger->next = tp->next;
		tp->next->prev = trigger;
		tp->next = trigger;
		triggerList.numEntries++;
	}

	// Attach the event to the trigger
	ep = trigger->events.head.next;
	event->prev = ep;
	event->next = ep->next;
	ep->next->prev = event;
	ep->next = event;
	trigger->events.numEntries++;
}


/*	--------------------------------------------------------------------------------
	Function 	: InitTriggerList
	Purpose 	: Initialise the main list of triggers
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void InitTriggerList( )
{
	triggerList.head.next = triggerList.head.prev = &triggerList.head;
	triggerList.numEntries = 0;
}


/*	--------------------------------------------------------------------------------
	Function 	: InitEventList
	Purpose 	: Initialise the main list of triggers
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void InitEventList( EVENTLIST *eventList )
{
	eventList->head.next = eventList->head.prev = &eventList->head;
	eventList->numEntries = 0;
}


/*	--------------------------------------------------------------------------------
	Function 	: KillAllTriggers
	Purpose 	: Remove all triggers from list
	Parameters 	: 
	Returns 	: 
	Info 		: Deletes the triggers, not just removes them. Check your data pointers!
*/
void KillAllTriggers( )
{
	TRIGGER *trigger, *tmp;

	for( trigger = triggerList.head.next; trigger != &triggerList.head; )
	{
		if( trigger != NULL )
		{
			SubTrigger( trigger );

			KillAllEvents( &trigger->events );

			tmp = trigger->next;
			JallocFree( (UBYTE **)&trigger );
			trigger = tmp;
		}
	}
}


/*	--------------------------------------------------------------------------------
	Function 	: KillAllEvents
	Purpose 	: Remove all events from list
	Parameters 	: 
	Returns 	: 
	Info 		: Deletes the events, not just removes them. Check your data pointers!
*/
void KillAllEvents( EVENTLIST *eventList )
{
	EVENT *event, *tmp;

	for( event = eventList->head.next; event != &eventList->head; )
	{
		if( event != NULL )
		{
			event->prev->next = event->next;
			event->next->prev = event->prev;
			eventList->numEntries--;

			/*
			Probably need to free the data separately
			*/

			tmp = event->next;
			JallocFree( (UBYTE **)&event );
			event = tmp;
		}
	}
}


/*	--------------------------------------------------------------------------------
	Function 	: SubTrigger
	Purpose 	: Remove trigger from list
	Parameters 	: trigger to remove
	Returns 	: 
	Info 		:
*/
void SubTrigger( TRIGGER *t )
{
	t->prev->next = t->next;
	t->next->prev = t->prev;
	triggerList.numEntries--;
}


/*	--------------------------------------------------------------------------------
	Function 	: UpdateEvents
	Purpose 	: Manage events for this turn
	Parameters 	: 
	Returns 	: 
	Info 		: 
*/
void UpdateEvents( )
{
	TRIGGER *trigger;
	EVENT *event;
	
	for( trigger = triggerList.head.next; trigger != &triggerList.head; trigger = trigger->next )
	{
		/* Check if the trigger condition(s) are true */
		if( trigger->func(trigger) )
		{
			/* If so, do correct responses */
			for( event = trigger->events.head.next; event != &trigger->events.head; event = event->next )
				event->func(event);
		}
	}
}
