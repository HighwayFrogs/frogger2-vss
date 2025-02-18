/****************************************************************

	ISL DREAMCAST LIBRARY (c) 2000 Blitz Games Ltd.		
	islvideo.c	Video player
														tgaulton
*****************************************************************/

#include	<shinobi.h>
#include	<sg_mw.h>
#include	<string.h>

#include	"islvideo.h"
#include	"usr.h"
#include	"km2enum.h"
#include	"DCK_System.h"

static StrDataType *vidStream;

/*	Turn over flag(ON/OFF)	*/
#define NOT_FLAG(flag)		(((flag) == OFF) ? ON : OFF)

/*	Application structure											*/
typedef struct {
	Sint32	term_flag;				//	Application terminate flag
	Sint32	disp_flag;				//	Display information flag
	Sint32	pause_flag;				//	Pause flag
	Sint32	first_hpel_flag;		//	First halfpel flag
	MWPLY	ply;					//	Player
	Sint32	vol;					//	Volume
	MWS_PLY_CPRM_SFD cprm;		//	Create parameter
} AP_OBJ, *AP;

/*	Application Object												*/
AP_OBJ ap_obj = {
	OFF,						//	Execute App.
	ON,							//	Display Info.
	OFF,						//	not puase
	ON,
	NULL,
	0,
};

int quitAllVideo = FALSE;

/*	Callback function when an error occur in middleware	*/
void ap_mw_err_func(void *obj, char *errmsg)
{
	AP ap=(AP)obj;

//	for (;;);
}

/*	Initialize Middleware				*/
void ApInitMw(AP ap,
		Sint32 mod, Sint32 frm, Sint32 cnt, Sint32 ltncy, void *vtxbuf)
{
	MWS_PLY_INIT_SFD	iprm;

	ADXT_Init();

	memset(&iprm, 0, sizeof(iprm));	/*	initialize reserved member		*/
	iprm.mode		= mod;			/*	Resolution, NTSC, PAL, VGA		*/
	iprm.frame		= frm;			/*	RGB565, ARGB8888				*/
	iprm.count		= cnt;			/*	Game period by the V-Sync count */
	iprm.latency	= ltncy;		/*	Latency (2V or 3V)				*/
#ifdef NINJA1
	mwPlyInitSofdec(&iprm);
#else
	mwPlyInitSfdFx(&iprm);
	mwPlySetVertexBuffer(vtxbuf);
#endif
	mwPlyEntryErrFunc(ap_mw_err_func, ap);
}

/*	ミドルウェアの終了					*/
/*	Finalize Middleware					*/
void ApFinishMw(AP ap)
{
	mwPlyFinishSfdFx();

	ADXT_Finish();
}

/*	アプリケーション開始処理	*/
/*	Start application		*/
void ApStart(AP ap)
{
	MWS_PLY_CPRM_SFD	*cprm;

	/*	Create handle	*/
	cprm = &ap->cprm;
	memset(cprm, 0, sizeof(MWS_PLY_CPRM_SFD));
	cprm->compo_mode = MWD_PLY_COMPO_OPEQ;
	cprm->ftype			= MWD_PLY_FTYPE_SFD;
	cprm->dtype			= MWD_PLY_DTYPE_WND;
	cprm->max_bps = 900*1024*8;
	cprm->max_width = vidStream->width;
	cprm->max_height = vidStream->height;
	cprm->nfrm_pool_wk = 6;
	cprm->wksize	= mwPlyCalcWorkCprmSfd(cprm);
	cprm->work = syMalloc(cprm->wksize);
	ap->ply = mwPlyCreateSofdec(cprm);
	if ( ap->ply == NULL )
		for (;;) ;

	mwPlySetDispPos(ap->ply, 0,0);
	mwPlySetDispSize(ap->ply, 640,480);
	mwPlySetFastHalfpel(ap->ply, ap->first_hpel_flag);

	/*	Start to play middleware */
	mwPlyStartFname(ap->ply, vidStream->strName);
}

/*	Stop application		*/
void ApStop(AP ap)
{
	mwPlyStop(ap->ply);					/*	Stop to play middleware		*/
									
	mwPlyDestroy(ap->ply);				/*	Destroy handle				*/
								
	syFree(ap->cprm.work);
	ap->ply = NULL;
	ap->cprm.work = NULL;
}

/*	Display infomation	*/
void ap_disp_info(AP ap)
{
	Sint32	time, tunit, hh, mm, ss, ff;

	mwPlyGetTime(ap->ply, &time, &tunit);
	UsrSfcnt2time(tunit, time, &hh, &mm, &ss, &ff);
	UsrPrintf(3, 3, "Time:%02d:%02d:%02d.%02d", hh, mm, ss, ff);
}

/*	動画の再生					*/
/*	Playing the animation file	*/
Sint32 ApExec(AP ap)
{
	MWPLY			ply;
	MWE_PLY_STAT	stat;
	PDS_PERIPHERAL	*per;
	char			*fname;

	ply = ap->ply;
	fname = vidStream->strName;

	per = pdGetPeripheral(PDD_PORT_A0);
	if ( per->press & PDD_DGT_ST )
	{
		ap->term_flag = 1;
		quitAllVideo = TRUE;
//		ap->pause_flag = NOT_FLAG(ap->pause_flag);
//		mwPlyPause(ply, ap->pause_flag);
	}
	if ( per->press & PDD_DGT_TA )
		mwPlyStartFname(ply, fname);
	if ( per->press & PDD_DGT_TL)
		ap->disp_flag = NOT_FLAG(ap->disp_flag);

	/*	Checking status												*/
	stat = mwPlyGetStat(ply);
	if ( stat == MWE_PLY_STAT_PLAYEND )
		ap->term_flag = 1;
//		mwPlyStartFname(ply, fname);
	if ( stat == MWE_PLY_STAT_ERROR )
		for(;;);

	if ( ap->disp_flag == ON )
		ap_disp_info(ap);

	mwPlySetOutVol(ply, ap->vol);

	return TRUE;
}

/*	Callback function when an error occur in GD file system	*/
void ApGdErrFunc(void *obj, Sint32 errcode)
{
	AP ap=(AP)obj;

	if ( errcode == GDD_ERR_TRAYOPEND || errcode == GDD_ERR_UNITATTENT )
		ap->term_flag = ON;			/*	Terminate application		*/
									
}


short videoPlayStream(StrDataType *str, int palMode, short (*keyHandler)(void))
{
	AP ap=&ap_obj;
	Bool ret;

	vidStream = str;

	ap->term_flag = 0;

	//	Entry callback function of GD file system error
	gdFsEntryErrFuncAll((void *)ApGdErrFunc, ap);

	ApInitMw(ap, KM_DSPMODE_NTSCNI640x480/*displayMode*/, KM_DSPBPP_RGB888/*frameBufferFormat*/, 1, 2, &vertexBufferDesc/*vertexBuffer*/);

	ApStart(ap);
	while (1)
	{
		mwPlyExecTexSvr();			//	Load Texture To V-RAM			
		UsrBeginDraw();
		mwPlyExecDrawSvr();			//	Draw polygons 					
		ret = ApExec(ap);
		UsrEndDraw();
		if ( ret == FALSE || ap->term_flag == ON )
			break;
		UsrWaitVBlank();
	}
	ApStop(ap);
	//	Finalize application
	ApFinishMw(ap);			//	Finalize Middleware					
}

void videoSetAudioChannel(int channel)
{

}
