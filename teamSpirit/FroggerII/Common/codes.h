#ifndef _CODES_H_INCLUDED
#define _CODES_H_INCLUDED

/* codes.h automatically generated by ETC
   Created: Tue Jan 11 14:59:21 2000

----------------------------------------------------------------*/

/* Commands */

#define C_DEBUG                  255
#define EV_CHANGEFROG            36
#define EV_COLLAPSE_PLAT         35
#define EV_SET_COUNTER           34
#define EV_DEC_COUNTER           33
#define EV_INC_COUNTER           32
#define EV_SCALEPLATSPEED        31
#define EV_SCALENMESPEED         30
#define EV_HURTFROG              29
#define EV_SFX_ENEMY             28
#define EV_SFX_PLATFORM          27
#define EV_SFX_TILE              26
#define EV_RESETFLAG             25
#define EV_SETFLAG               24
#define EV_CAMEOMODE_OFF         23
#define EV_CAMEOMODE_ON          22
#define EV_NO_FOG                21
#define EV_FOG                   20
#define EV_HOP                   18
#define EV_SETSTARTTILE          17
#define EV_CHANGELEVEL           16
#define EV_SPRING                14
#define EV_CHANGEVOLUME          13
#define EV_DROPGARIB             12
#define EV_MOVEPLAT              11
#define EV_MOVEENEMY             10
#define EV_TELEPORT              9
#define EV_SETPLATFORM           8
#define EV_SETENEMY              7
#define EV_ANIMATEACTOR          6
#define EV_RESETPLATFLAG         5
#define EV_SETPLATFLAG           4
#define EV_RESETENEMYFLAG        3
#define EV_SETENEMYFLAG          2
#define EV_SETTILE               1
#define EV_DEBUG                 0
#define EV_IF                    65
#define EV_ON                    64

/* Triggers */

#define TR_NOT                   66
#define TR_OR                    65
#define TR_AND                   64
#define TR_LEVELISOPEN           14
#define TR_FROGISDEAD            13
#define TR_COUNTEREQUALS         12
#define TR_FLAGSET               11
#define TR_WAIT                  10
#define TR_PLATATFLAG            9
#define TR_ENEMYATFLAG           8
#define TR_FROGONPLATFORM        2
#define TR_FROGONTILE            1
#define TR_ENEMYONTILE           0
#endif
