/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.
	
	  
	File		: event.h
	Programmer	: Jim Hubbard
	Date		: 30/06/99
	
----------------------------------------------------------------------------------------------- */

#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED


/*----- [ STRUCTURES ] -------------------------------------------------------------------------*/

typedef struct TAGEVENT
{
	struct TAGEVENT *next, *prev;
	void (*func) (), **data;
} EVENT;

typedef struct
{
	EVENT head;	
	unsigned int numEntries;
} EVENTLIST;

typedef struct TAGTRIGGER
{
	struct TAGTRIGGER *next, *prev;
	void **data;
	int (*func) ();
	EVENTLIST events;
} TRIGGER;

typedef struct
{
	TRIGGER head;	
	unsigned int numEntries;
} TRIGGERLIST;


/*----- [ DATA ] ------------------------------------------------------------------------------*/

extern TRIGGERLIST triggerList;


/*----- [ PROTOTYPES ] ------------------------------------------------------------------------*/

extern TRIGGER * MakeTrigger( int (*func)(TRIGGER *t), unsigned int numargs, void **args );
extern EVENT * MakeEvent( void (*func)(EVENT *e), unsigned int numargs, void **args );
extern void AttachEvent( TRIGGER *trigger, EVENT *event );
extern void InitTriggerList( );
extern void InitEventList( );
extern void KillAllTriggers( );
extern void KillAllEvents( );
extern void SubTrigger( TRIGGER *t );
extern void UpdateEvents( );

#endif