/*

	Trigger and event codes used by Frogger2 scripting system

	Programmer: David Swift

	NOTE: This file is SHARED in sourcesafe between the Frogger2/Common
	project	and the ETC (Compiler) project. 
*/

#ifndef _INCLUDED_CODES_H
#define _INCLUDED_CODES_H

enum EVENTTYPES
{
	EV_DEBUG = 0,
	EV_CHANGEACTORSCALE,
	EV_TOGGLEPLATFORMMOVE,
	EV_TOGGLEENEMYMOVE,
	EV_TOGGLETILELINK,
	EV_PLAYSFX,
	EV_CHANGELEVEL,
	EV_ANIMATEACTOR,
	EV_ON,
};

enum TRIGGERTYPES
{
	TR_ENEMYONTILE = 0,
	TR_FROGONTILE,
	TR_FROGONPLATFORM,
	TR_PLATNEARPOINT,
	TR_ENEMYNEARPOINT,
	TR_FROGNEARPOINT,
	TR_FROGNEARPLAT,
	TR_FROGNEARENEMY,
	TR_ENEMYATFLAG,
	TR_PLATATFLAG
};

#endif