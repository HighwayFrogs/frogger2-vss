/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.

	File		: graphic.c
	Programmer	: Andy Eder
	Date		: 06/08/99 12:37:57

----------------------------------------------------------------------------------------------- */

#define F3DEX_GBI_2

#include "incs.h"

TIMECOUNT timers[10];
TIMECOUNT printTimer[10];
unsigned int frameStart;
unsigned int printFrameStart[10];
char timerMode=0;
char message[256];

/*	--------------------------------------------------------------------------------
	Function 	: TIMER_InitTimers
	Purpose 	: initialises the timers
	Parameters 	: 
	Returns 	: void
	Info 		:
*/
void TIMER_InitTimers()
{
	int i,j;

	for (i=0;i<10;i++)
	{
		timers[i].status = 0;

		for (j=0;j<MAX_SLICES;j++)
		{
			timers[i].start[j] = 0;
			timers[i].end[j] = 0;
			timers[i].total[j] = 0;
		}

		timers[i].name[0] = NULL;
		timers[i].slice = 0;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: TIMER_ClearTimers
	Purpose 	: clears the timers
	Parameters 	: 
	Returns 	: void
	Info 		:
*/
void TIMER_ClearTimers()
{
	int i,j;
	char overrun=0;

	for (i=0;i<10;i++)
	{
		if (timers[i].status==0) printTimer[i] = timers[i];
		else overrun=1;
	}
	if (overrun==1) return;
	for (i=0;i<10;i++)
	{
		//printTimer[i] = timers[i];
		timers[i].slice=0;
		timers[i].name[0] = NULL;
		timers[i].frameStart = frameStart;

		for (j=0;j<MAX_SLICES;j++)
		{
			timers[i].start[j] = 0;
			timers[i].end[j] = 0;
			timers[i].total[j] = 0;
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: TIMER_StartTimer
	Purpose 	: starts a timer
	Parameters 	: int,char *
	Returns 	: void
	Info 		:
*/
void TIMER_StartTimer(int number,char *name)
{
	int slice;

	slice = timers[number].slice;

	if (slice>(MAX_SLICES-1)) return;

	timers[number].start[slice] = osGetCount();
	timers[number].status = 1;
	//timers[number].frameStart = frameStart;
//	strcpy(timers[number].name, name);
	sprintf(timers[number].name, name);
}

/*	--------------------------------------------------------------------------------
	Function 	: TIMER_EndTimer
	Purpose 	: ends a timer
	Parameters 	: int
	Returns 	: void
	Info 		:
*/
void TIMER_EndTimer(int number)
{
	unsigned int maxCount = -1;
	int slice;

	slice = timers[number].slice;

	if (slice>(MAX_SLICES-1)) return;

	timers[number].end[slice] = osGetCount();
	if (timers[number].end[slice] > timers[number].start[slice])
	{
		timers[number].total[slice] += timers[number].end[slice] - timers[number].start[slice];
	}
	else
	{
		timers[number].total[slice] += maxCount-timers[number].start[slice]+timers[number].end[slice];
	}
	timers[number].status = 0;

	if (slice<MAX_SLICES) timers[number].slice++;
}

/*	--------------------------------------------------------------------------------
	Function 	: TIMER_PrintTimers
	Purpose 	: prints the timers
	Parameters 	: 
	Returns 	: void
	Info 		:
*/
void TIMER_PrintTimers()
{
	int i,j;
	unsigned int ulx,uly,lrx,lry,size,start;
	unsigned int maxCount = -1;

	if(!timerMode)
		return;

	gDPPipeSync(glistp++);
	gDPSetRenderMode(glistp++,G_RM_OPA_SURF , G_RM_OPA_SURF2);      
	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_FILL);

	gDPSetFillColor(glistp++, GPACK_RGBA5551(255,255,255,1) << 16 | 
		GPACK_RGBA5551(255,255,255,1));

		
	gDPFillRectangle(glistp++,100,30,101,195);
	gDPFillRectangle(glistp++,100+((TicksPerFrame)/10000),30,101+((TicksPerFrame)/10000),195);
	gDPFillRectangle(glistp++,100+((TicksPerFrame)/20000),30,101+((TicksPerFrame)/20000),195);
	gDPFillRectangle(glistp++,100+((TicksPerFrame)/1),30,101+((TicksPerFrame)/1),195);

	ulx = 100;
	uly = 30;
	lry = 35;
	for (i=0;i<10;i++)
	{
		if (timerMode==2)
		{
//			sprintf(message,"  %d",printTimer[i].total[0]);
//			sprintf(message,"%s %d",printTimer[i].name, printTimer[i].total[0]);
//			NEW_TEXT(20,30+(i*16),255,255,255,255,1,1,1,message, NO, SMALL_FONT, 0);
		}
		for (j=0;j<printTimer[i].slice;j++)
		{
			if (printTimer[i].start[j]!=0)
			{
				gDPPipeSync(glistp++);
				switch(j&3)
				{
				case 0:
					gDPSetFillColor(glistp++, GPACK_RGBA5551(240,0,0,1) << 16 | 
						GPACK_RGBA5551(240,0,0,1));
					break;
				case 1:
					gDPSetFillColor(glistp++, GPACK_RGBA5551(0,240,0,1) << 16 | 
						GPACK_RGBA5551(0,240,0,1));
					break;
				case 2:
					gDPSetFillColor(glistp++, GPACK_RGBA5551(0,0,240,1) << 16 | 
						GPACK_RGBA5551(0,0,240,1));
					break;
				case 3:
					gDPSetFillColor(glistp++, GPACK_RGBA5551(240,240,10,1) << 16 | 
						GPACK_RGBA5551(240,240,10,1));
					break;
				}
				

				size = printTimer[i].total[j]/20000;
				if (printTimer[i].start[j] > printTimer[i].frameStart)
				{
					start = (printTimer[i].start[j]-printTimer[i].frameStart)/20000;
				}
				else
				{
					start = (maxCount-printTimer[i].frameStart+printTimer[i].start[j])/20000;
				}
				lrx = 100+size;
				if(timerMode)
					gDPFillRectangle(glistp++,ulx+start,uly,lrx+start,lry);
			}
		}
		uly += 16;
		lry += 16;
	}


	j = 0;

//	sprintf(message,"  %d : %f",(int)framesPerSec,GAME_SPEED);
//	NEW_TEXT(160,200,255,255,255,255,1,1,1,message, NO, SMALL_FONT, 0);

	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_1CYCLE);      
	gDPPipeSync(glistp++);
	gDPSetRenderMode(glistp++,G_RM_AA_ZB_OPA_SURF , G_RM_AA_ZB_OPA_SURF2);
}
