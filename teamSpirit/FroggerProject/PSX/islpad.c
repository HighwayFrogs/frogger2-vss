/************************************************************************************
	ISL PSX LIBRARY	(c) 1999 Interactive Studios Ltd.

	islpad.c			Controller pad routines

************************************************************************************/

#include <stddef.h>
#include <libetc.h>
#include <libpad.h>
#include <stdio.h>
#include "islpad.h"

enum {									// Supported pad types
	PADTYPE_NONE,
	PADTYPE_DIGITAL,
	PADTYPE_ANALOG,
	PADTYPE_DUALSHOCK,
};

typedef struct _PadPacket {
     short			digital;
     unsigned char	rightX;
     unsigned char	rightY;
     unsigned char	leftX;
     unsigned char	leftY;
} PadPacket;

typedef struct _PadDataType2 {
	unsigned char	buffer[2][34];				// HW packet buffers
	int				state[8];					// Port HW state
	unsigned short	digitalPrev[8];				// Previous d-pad reading
	int				newShock[8];
} PadDataType2;

static PadDataType2		padData2;
static unsigned char	MULTITAP_SUPPORT;


PadDataType				padData;

static unsigned char	HWalign[6]={0,1,0xFF,0xFF,0xFF,0xFF};
unsigned char			analog[8];
unsigned short			pad[8], debounce[8];
unsigned short			leftx[8], lefty[8];
unsigned short			rightx[8], righty[8];
unsigned short			bothdebounces, bothpads;



/**************************************************************************
	FUNCTION:	padInitialise()
	PURPOSE:	Initialise pads
	PARAMETERS:	multi-tap on/off
	RETURNS:	
**************************************************************************/

void padInitialise(unsigned char multiTap)
{
	int		loop;

	MULTITAP_SUPPORT = multiTap;

	if(MULTITAP_SUPPORT)
		PadInitMtap(padData2.buffer[0],padData2.buffer[1]);
	else
		PadInitDirect(padData2.buffer[0],padData2.buffer[1]);

	VSync(5);
	PadStartCom();
	padData.numPads[0] = 1;
	padData.numPads[1] = 1;
	VSync(5);
	for(loop=0; loop<8; loop++)
	{
		padData.digital[loop] = 0;
		padData2.digitalPrev[loop] = 0;
		padData.debounce[loop] = 0;
		padData.analogX[loop] = 128;
		padData.analogY[loop] = 128;
		padData.analogX2[loop] = 128;
		padData.analogY2[loop] = 128;
	}
}


static inline unsigned char padGetTypeFromPacket(unsigned char id)
{
	switch(id)
	{
	case 4:
		return PADTYPE_DIGITAL;
	case 7:
		return PADTYPE_ANALOG;
	}
	return PADTYPE_NONE;
}


static void padHandlePort(int port)
{
	int			padLp, HWport, padNo, state, currPad;
	PadPacket	*packet;
	unsigned char		id;

	if (*padData2.buffer[port])
	{
		for(padLp=0; padLp<4; padLp++)
		{
			padNo = padLp|(port<<2);
			padData.present[padNo] = PADTYPE_NONE;
			padData2.state[padNo] = PadStateDiscon;
			padData.digital[padNo] = 0;
		}
		return;
	}

	if(MULTITAP_SUPPORT)
	{
		if (PadChkMtap(port<<4)==1)					// SORT THIS OUT - COULD BE GREEDY
		{
			padData.numPads[port] = 4;
		}
		else
		{
			padData.numPads[port] = 1;
		}
	}
	else
	{
		padData.numPads[port] = 1;
	}


	for(padLp=0; padLp<padData.numPads[port]; padLp++)
	{
		padNo = padLp|(port<<2);
		HWport = padLp|(port<<4);
		state = PadGetState(HWport);
		switch(state)										// Handle (dis)connection states
		{
		case PadStateDiscon:
#ifdef _DEBUG
			printf("  Controller disconnected\n");
#endif
  			padData.present[padNo] = PADTYPE_NONE;
			padData2.state[padNo] = PadStateDiscon;
			continue;
		case PadStateFindPad:
#ifdef _DEBUG
			printf("  Find controller connection (checking)\n");
#endif
			padData2.state[padNo] = PadStateFindPad;
			continue;
		case PadStateFindCTP1:
			if (padData2.state[padNo]!=PadStateStable)
			{
#ifdef _DEBUG
				printf("  Check for controller connection with controllers other than DUAL SHOCK (Complete the acquisition of controller information)\n");
#endif
				padData2.state[padNo] = PadStateStable;
				currPad = PadInfoMode(HWport, InfoModeCurID, 0);
				switch(currPad)
				{
				case 4:
#ifdef _DEBUG
					printf("%d: Standard Pad Connected\n", padNo);
#endif
					padData.present[padNo] = PADTYPE_DIGITAL;
					padData.digital[padNo] = 0;
					break;
				case 7:
#ifdef _DEBUG
					printf("%d: Old Analog Pad (Red Mode) Connected\n", padNo);
#endif
					padData.present[padNo] = PADTYPE_ANALOG;
					padData.digital[padNo] = 0;
					break;
				}
				continue;
			}
			break;
		case PadStateReqInfo:
#ifdef _DEBUG
			printf("  Actuator information being retrieved (data being retrieved)\n");
#endif
			continue;
		case PadStateStable:
			if (padData2.state[padNo]!=PadStateStable)
			{
#ifdef _DEBUG
				printf("  Retrieval of actuator information completed, or library-controller communication completed\n");
#endif
				padData2.state[padNo] = PadStateStable;
				currPad = PadInfoMode(HWport, InfoModeCurExID, 0);
				if(currPad)
				{
#ifdef _DEBUG
					printf("%d: Found DUAL SHOCK controller\n", padNo);
#endif
					padData.present[padNo] = PADTYPE_DUALSHOCK;
					padData.digital[padNo] = 0;
					padData2.newShock[padNo] = 10;
					PadSetMainMode(HWport,1,3);
				}
				else
				{
#ifdef _DEBUG
					printf("  Found unsupported extended controller\n");
#endif
					padData.present[padNo] = PADTYPE_NONE;
				}
				continue;
			}
			break;
		}
		if (padData.numPads[port]==4)
		{
			packet = (PadPacket *)&(padData2.buffer[port][(padLp*8)+2+2]);
			id = padData2.buffer[port][(padLp*8)+1+2]>>4;
		}
		else
		{
			packet = (PadPacket *)&(padData2.buffer[port][(padLp*8)+2+0]);
			id = padData2.buffer[port][(padLp*8)+1+0]>>4;
		}
		currPad = padData.present[padNo];
		if (currPad!=PADTYPE_DUALSHOCK)				// Dodgy old analog controller morphs into other ones
		{
			padData.present[padNo] = padGetTypeFromPacket(id);
			if (currPad!=padData.present[padNo])
			{
				switch(padData.present[padNo])
				{
				case PADTYPE_DIGITAL:
#ifdef _DEBUG
					printf("%d: Morphing controller became digital\n", padNo);
#endif
					break;
				case PADTYPE_ANALOG:
#ifdef _DEBUG
					printf("%d: Morphing controller became red analog\n", padNo);
#endif
					break;
				case PADTYPE_NONE:
#ifdef _DEBUG
					printf("%d: Morphing controller became unsupported\n", padNo);
#endif
					break;
				}
			}
		}
		if (padData.present[padNo]!=PADTYPE_NONE)
		{
			padData.digital[padNo] = ~packet->digital;
			padData.analogX[padNo] = packet->leftX;
			padData.analogY[padNo] = packet->leftY;
			padData.analogX2[padNo] = packet->rightX;
			padData.analogY2[padNo] = packet->rightY;
		}
		else
			padData.digital[padNo] = 0;
	}	
}


/**************************************************************************
	FUNCTION:	padHandler()
	PURPOSE:	Handle pad reading/connection etc.
	PARAMETERS:	
	RETURNS:	
**************************************************************************/

void padHandler()
{
	int		HWport, loop;
	unsigned short	temp;

	padHandlePort(0);
	padHandlePort(1);

	for(loop=0; loop<8; loop++)
	{
		padData.analog[loop] = (padData.present[loop]==PADTYPE_ANALOG)||(padData.present[loop]==PADTYPE_DUALSHOCK);

		temp = (padData.digital[loop] ^ padData2.digitalPrev[loop]);
		padData.debounce[loop] = (temp & padData.digital[loop]);
		padData2.digitalPrev[loop] = padData.digital[loop];

		if (padData.present[loop]==PADTYPE_DUALSHOCK)
		{
			if (padData2.newShock[loop])
			{
				if (padData2.newShock[loop]==1)
				{
					padData.motor[loop][0] = 0;
					padData.motor[loop][1] = 0;
					HWport = ((loop & 3)|((loop>>2)<<4));
					PadSetAct(HWport, padData.motor[loop], 2);
					PadSetActAlign(HWport,HWalign);
				}
				padData2.newShock[loop]--;
			}
			else
			{
				if ((padData.shock[loop]>>16)!=padData.motor[loop][1])
				{
					if (padData.shockDiv[loop]==0)
						padData.shockDiv[loop] = 1;
					padData.currShock[loop] += (padData.shock[loop]-padData.currShock[loop])/padData.shockDiv[loop];
					padData.motor[loop][1] = (padData.currShock[loop]>>16);
				}
				if (padData.buzzTime[loop]>0)
				{
					padData.buzzTime[loop]--;
					padData.motor[loop][0] = 1;
				}
				else
					padData.motor[loop][0] = 0;
			}
		}
		else
		{
			padData.motor[loop][0] = 0;
			padData.motor[loop][1] = 0;
		}
	}
}
