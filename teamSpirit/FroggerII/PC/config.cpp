
#include <windows.h>
#include <windowsx.h>
#include <crtdbg.h>
#include <commctrl.h>
#include <cguid.h>
#include <ddraw.h>
#include <d3d.h>
#include <dsound.h>
#include <dinput.h>
#include <dplay.h>
#include <dplobby.h>
#include <stdio.h>
#include "network.h"
#include "netchat.h"
#include "netgame.h"
#include "config.h"

extern "C" {

#include <ultra64.h>
#include "block.h"
#include "directx.h"
#include "controll.h"
#include "types.h"
#include "define.h"
#include "font.h"
#include "actor.h"
#include "overlays.h"
#include "frogger.h"
#include "maths.h"
#include "babyfrogs.h"
#include "game.h"
#include "specfx.h"
#include "audio.h"
#include "frogmove.h"

extern unsigned long USE_MENUS;
extern long winMode;
extern long scaleMode;
extern unsigned long synchSpeed;
extern unsigned long pingOffset;
extern unsigned long synchRecovery;

CONFIG cfgOptList[] = 
{
	"*ping_offset"		,"%i"			,&pingOffset,
	"*synch_rate"		,"%i"			,&synchSpeed,
	"*synch_recovery"	,"%i"			,&synchRecovery,

	"" ,NULL,NULL
};

const char *cfg_file = "frogcfg.txt";

/*	--------------------------------------------------------------------------------
	Function		: GetArgs
	Purpose			: Process Cmd Line Args
	Parameters		: 
	Returns			: 
	Info			: 
*/

void GetArgs(char *arglist)
{
	char cmdMode;

	while (*arglist)
	{
		switch(*arglist++)
		{
			case '+': case '/':	case '-':
				cmdMode = 1;
				break;

			case 'W': case 'w':
				if (cmdMode) winMode = 1;
				break;

			case 'S': case 's':
				if (cmdMode) scaleMode = 1;
				break;

			case 'C': case 'c':
				if (cmdMode) swingCam = 0;
				break;

			case 'M': case 'm':
				if (cmdMode) USE_MENUS = 1;
				break;

			default:
				cmdMode = 0;
				break;
		}		
	}
}

/*	--------------------------------------------------------------------------------
	Function		: GetArgs
	Purpose			: Process Cmd Line Args
	Parameters		: 
	Returns			: 
	Info			: 
*/

void ProcessCfgLine(char *line)
{
	unsigned long lenCmd,ccmd;
	CONFIG *cur;

	// Skip leading spaces and comments
	while ((line) && (*line!='*') && (*line!=';')) line++;

	// Nothing Line, or comment
	if (!line) return;
	if (*line = ';') return;
	
	// Find length of command section
	lenCmd=0;
	while ((line[lenCmd]) && (line[lenCmd]!=' ') && (line[lenCmd]!='\t')) lenCmd++;

	// No Seperator
	if (!(line[lenCmd])) return;
	
	// Process command
	cur = cfgOptList;
	while (*cur->name)
	{
		if (strncmp(line,cur->name,lenCmd)==0)
		{
			line = &line[lenCmd];
			
			while ((line) && (*line!=' ') && (*line!='\t')) line++;
			
			sscanf(line,cur->type,cur->var);

			return;
		}
		cur++;
	}
}

}