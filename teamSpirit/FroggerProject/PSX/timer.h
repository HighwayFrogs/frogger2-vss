/************************************************************************************
	ACTION MAN PS	(c) 1998-9 ISL

	timer.h:		Debug timing functions

************************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

//*** TIMER SET 0 - GAME LOOP ***/
#define TIMER_SET_0
#define TIMER_START0(x) 	 TIMER_START(x)
#define TIMER_STOP0(x)  	 TIMER_STOP(x)
#define TIMER_STOP_ADD0(x) TIMER_STOP_ADD(x)
// #define TIMER_START0(x)
// #define TIMER_STOP0(x)
// #define TIMER_STOP_ADD0(x)

//*** TIMER SET 0 - DrawActorList ***//
//#define TIMER_SET_1
//#define TIMER_START1(x)    TIMER_START(x)
//#define TIMER_STOP1(x) 	   TIMER_STOP(x)
//#define TIMER_STOP_ADD1(x) TIMER_STOP_ADD(x)
#define TIMER_START1(x)
#define TIMER_STOP1(x)
#define TIMER_STOP_ADD1(x)

//*** TIMER SET 2 - actorDraw ***/
//#define TIMER_SET_2
//#define TIMER_START2(x) 	 TIMER_START(x)
//#define TIMER_STOP2(x)  	 TIMER_STOP(x)
//#define TIMER_STOP_ADD2(x) TIMER_STOP_ADD(x)
#define TIMER_START2(x)
#define TIMER_STOP2(x)
#define TIMER_STOP_ADD2(x)




//*** TIMER SET 0 - GAME LOOP ***//
#ifdef TIMER_SET_0
enum {
	TIMER_TOTAL,
	TIMER_GAMELOOP,
	TIMER_UPDATE_WATER,
	TIMER_DRAW_WORLD,
	TIMER_DRAW_SPECFX,
	TIMER_PRINT_SPRITES,
	TIMER_DRAW_SCENICS,
	TIMER_DRAW_WATER,
	TIMER_ACTOR_DRAW,
	TIMER_PRINT_OVERS,
	TIMER_PROCTEX,
	TIMER_DRAWSYNC,
	TIMER_INTERPRET,
	TIMER_TIMERS,

	TIMER_NUMTIMERS
};

#define TIMER_NAMES	   	"TOTAL", \
						"GAMELOOP", \
						"UPDATE_WATER", \
						"DRAW_WORLD", \
						"DRAW_SPECFX", \
						"PRINT_SPRITES", \
						"DRAW_SCENICS", \
						"DRAW_WATER", \
						"ACTOR_DRAW", \
						"PRINT_OVERS", \
						"PROCTEX", \
						"DRAWSYNC", \
						"INTERPRET", \
						"TIMERS"
#endif //TIMER_SET_0


//*** TIMER SET 1 - DrawActorList ***//
#ifdef TIMER_SET_1
enum {
	TIMER_TOTAL,
	TIMER_DIST,
	TIMER_CLIP,
	TIMER_UPANI,
	TIMER_SETANI,
	TIMER_DRAW,
	TIMER_TIMERS,

	TIMER_NUMTIMERS
};

#define TIMER_NAMES	   	"TOTAL", \
						"DIST", \
						"CLIP", \
						"UPANI", \
						"SETANI", \
						"DRAW", \
						"TIMERS"
#endif //TIMER_SET_1


//*** TIMER SET 2 - actorDraw ***//
#ifdef TIMER_SET_2
enum {
	TIMER_TOTAL,
	TIMER_ACTDR_QUAT,
	TIMER_ACTDR_MTX,
	TIMER_ACTDR_SEG,
	TIMER_TIMERS,

	TIMER_NUMTIMERS
};

#define TIMER_NAMES	   	"TOTAL", \
						"QUAT", \
						"MTX", \
						"SEG", \
						"TIMERS"
#endif //TIMER_SET_1











/*
//for RunGameLoop
enum {
	TIMER_TOTAL,
	TIMER_PROC_CONT,
	TIMER_CAM_POS,
	TIMER_SET_CAM,
	TIMER_FROG_POS,
	TIMER_UP_PLAT,
	TIMER_UP_ENEM,
	TIMER_UP_SPEC,
	TIMER_UP_EVEN,
	TIMER_UP_AMBI,
	TIMER_COLLECT,
	TIMER_ONSCREEN,
	TIMER_TILENUM,
	TIMER_TIMERS,

	TIMER_NUMTIMERS
};


#define TIMER_NAMES	   	"TOTAL", \
						"PROC_CONT", \
						"CAM_POS", \
						"SET_CAM", \
						"fROG_POS", \
						"UP_PLAT", \
						"UP_ENEM", \
						"UP_SPEC", \
						"UP_EVEN", \
						"UP_AMBI", \
						"COLLECT", \
						"ONSCREEN", \
						"TILENUM", \
						"TIMERS"
*/						







extern char	timerActive;
extern char	*timerName[TIMER_NUMTIMERS];
extern int	globalTimer[TIMER_NUMTIMERS];
extern int	startTimer[TIMER_NUMTIMERS];
extern int	prevTimer[TIMER_NUMTIMERS];


/**************************************************************************
	FUNCTION:	TIMER_START()
	PURPOSE:	Start timing
	PARAMETERS:	Timer number
	RETURNS:	
**************************************************************************/

//#define TIMER_START(n)		globalTimer[n] = VSync(1)
#define TIMER_START(n)		startTimer[n] = VSync(1)


/**************************************************************************
	FUNCTION:	TIMER_STOP()
	PURPOSE:	Stop timing
	PARAMETERS:	Timer number
	RETURNS:	
**************************************************************************/

//#define TIMER_STOP(n)		globalTimer[n] = VSync(1)-globalTimer[n]
#define TIMER_STOP(n)		globalTimer[n] = VSync(1)-startTimer[n]

#define TIMER_STOP_ADD(n)	globalTimer[n] += VSync(1)-startTimer[n]


/**************************************************************************
	FUNCTION:	TIMER_ENDFRAME()
	PURPOSE:	Start timing
	PARAMETERS:	Timer number
	RETURNS:	
**************************************************************************/

#define TIMER_ENDFRAME		memcpy(prevTimer, globalTimer, sizeof(globalTimer))

//need to zero, cos globalTimer may not be reset if only using TIMER_STOP_ADD
#define TIMER_ZERO			memset(globalTimer, 0, sizeof(globalTimer))


/**************************************************************************
	FUNCTION:	timerDisplay()
	PURPOSE:	Display timer readings if enabled
	PARAMETERS:	
	RETURNS:	
**************************************************************************/

void timerDisplay();


#endif
