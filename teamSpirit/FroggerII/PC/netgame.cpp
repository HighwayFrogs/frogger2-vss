
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

#include "network.h"
#include "netchat.h"
#include "netgame.h"

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

}

DPID netPlayers[MAX_MULTIPLAYERS];


/*	--------------------------------------------------------------------------------
	Function		: HandleUpdateMessage
	Purpose			: interpret a game update from the network
	Parameters		: 
	Returns			: 
	Info			: 
*/
void HandleUpdateMessage( LPDPLAYINFO lpDPInfo,LPMSG_UPDATEGAME lpMsg,DWORD dwMsgSize,DPID idFrom,DPID idTo )
{
	long i;
	char tmp[256];

	for( i=1; i<MAX_MULTIPLAYERS; i++ )
		if( netPlayers[i] == idFrom )
		{
			controllerdata[i].button = lpMsg->data;

			// Start of level or unseen emergency measure only
			if( lpMsg->tileNum != (((DWORD)currTile[i] - (DWORD)firstTile) / sizeof(GAMETILE)) )
			{
				SetVector(&frog[i]->actor->pos, &firstTile[lpMsg->tileNum].centre);
				currTile[i] = &firstTile[lpMsg->tileNum];
			}

			break;
		}
}


/*	--------------------------------------------------------------------------------
	Function		: SendUpdateMessage
	Purpose			: sends an update message
	Parameters		: Player number
	Returns			: 
	Info			: 
*/
HRESULT SendUpdateMessage( )
{
	LPMSG_UPDATEGAME lpUpdateMessage = NULL;
	HRESULT	hRes;

	char tmp[256];

	if( !controllerdata[0].button )
		return DPERR_ABORTED;

	lpUpdateMessage = (LPMSG_UPDATEGAME)GlobalAllocPtr(GHND,sizeof(MSG_UPDATEGAME));
	if(lpUpdateMessage == NULL)
	{
		hRes = DPERR_OUTOFMEMORY;
		goto FAILURE;
	}

	// build message	
	lpUpdateMessage->dwType = APPMSG_UPDATEGAME;
	lpUpdateMessage->data = controllerdata[0].button;
	lpUpdateMessage->tileNum = ((DWORD)currTile[0] - (DWORD)firstTile) / sizeof(GAMETILE);

	// send this data to all other players
	hRes = DPInfo.lpDP4A->Send(DPInfo.dpidPlayer,DPID_ALLPLAYERS,DPSEND_GUARANTEED,lpUpdateMessage,sizeof(MSG_UPDATEGAME));
	if(FAILED(hRes))
		goto FAILURE;

FAILURE:
	if(lpUpdateMessage)
		GlobalFreePtr(lpUpdateMessage);

	return hRes;
}


/*	--------------------------------------------------------------------------------
	Function		: InitMPGameData
	Purpose			: Clear player mappings and stuff
	Parameters		: 
	Returns			: 
	Info			: 
*/
void InitMPGameData( )
{
	long i;

	netPlayers[0] = DPInfo.dpidPlayer;

	for( i=1; i<MAX_MULTIPLAYERS; i++ )
		netPlayers[i] = -1;

	DPInfo.lpDP4A->EnumPlayers( NULL, EnumPlayersCallback, NULL, DPENUMPLAYERS_REMOTE );
}


/*	--------------------------------------------------------------------------------
	Function		: RefreshMPFrogs
	Purpose			: Activate frogs for connected players
	Parameters		: 
	Returns			: 
	Info			: 
*/
void RefreshMPFrogs( )
{
	long i, count = 0;

	for( i=1; i < MAX_MULTIPLAYERS; i++ )
		if( netPlayers[i] == -1 )
		{
			MPRemoveFrog( i );
			count--;
		}
		else
		{
			MPAddFrog( i );
			count++;
		}

	if( count > 1 )
		NUM_FROGS = count;
}


/*	--------------------------------------------------------------------------------
	Function		: MPAddFrog
	Purpose			: Make a new frog when a player joins during a game
	Parameters		: 
	Returns			: Success?
	Info			: 
*/
int MPAddFrog( int i )
{
	VECTOR telePos;
	long j;

	if( i>=MAX_MULTIPLAYERS || i<1 || gameState.mode != GAME_MODE ) // Not found
		return 0;

	if( frog[i]->actor->xluOverride == 100 ) // Already enabled
		return 1;

	// Health sprites need init
	for( j=0; j<3; j++ )
		sprHeart[(i*3)+j]->draw = 1;

	frog[i]->actor->xluOverride = 100; // Make it visible

	SetVector(&telePos,&frog[i]->actor->pos);
	CreateAndAddFXRipple(RIPPLE_TYPE_TELEPORT,&telePos,&upVec,30,0,0,15);
	telePos.v[Y] += 20;
	CreateAndAddFXRipple(RIPPLE_TYPE_TELEPORT,&telePos,&upVec,25,0,0,20);
	telePos.v[Y] += 40;
	CreateAndAddFXRipple(RIPPLE_TYPE_TELEPORT,&telePos,&upVec,20,0,0,25);
	telePos.v[Y] += 60;
	CreateAndAddFXRipple(RIPPLE_TYPE_TELEPORT,&telePos,&upVec,15,0,0,30);
	PlaySample(88,NULL,255,128);

	return 1;
}


/*	--------------------------------------------------------------------------------
	Function		: MPRemoveFrog
	Purpose			: Make the frog inneffective when someone leaves
	Parameters		: 
	Returns			: Succuss?
	Info			: 
*/
int MPRemoveFrog( int i )
{
	long j;
	VECTOR telePos;

	if( i>=MAX_MULTIPLAYERS || i<1 || gameState.mode != GAME_MODE ) // Not found
		return 0;

	if( frog[i]->actor->xluOverride == 0 ) // Already disabled
		return 1;

	frog[i]->action.lives = 0;
	player[i].lives = 0;

	// Disable health sprites
	for( j=0; j<3; j++ )
		sprHeart[(i*3)+j]->draw = 0;

	frog[i]->actor->xluOverride = 0; // Make it invisible

	SetVector(&telePos,&frog[i]->actor->pos);
	CreateAndAddFXRipple(RIPPLE_TYPE_TELEPORT,&telePos,&upVec,30,0,0,15);
	telePos.v[Y] += 20;
	CreateAndAddFXRipple(RIPPLE_TYPE_TELEPORT,&telePos,&upVec,25,0,0,20);
	telePos.v[Y] += 40;
	CreateAndAddFXRipple(RIPPLE_TYPE_TELEPORT,&telePos,&upVec,20,0,0,25);
	telePos.v[Y] += 60;
	CreateAndAddFXRipple(RIPPLE_TYPE_TELEPORT,&telePos,&upVec,15,0,0,30);
	PlaySample(88,NULL,255,128);

	return 1;
}
