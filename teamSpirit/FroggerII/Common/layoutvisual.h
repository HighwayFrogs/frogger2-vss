/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.


	File		: layoutvisual.h
	Programmer	: Andrew Eder
	Date		: 01/03/98

----------------------------------------------------------------------------------------------- */

#ifndef LAYOUTVISUAL_H_INCLUDED
#define LAYOUTVISUAL_H_INCLUDED


// DEFINES THE LAYOUT OF WORLDS AND LEVELS (WITHIN A WORLD)

#define NO_MULTI_LEV		0
#define MULTI_PART			1

//////////////////////////////////////////////////////////////////////////////////////////////////
//************************************************************************************************
// PC VERSION ONLY - PC VERSION ONLY - PC VERSION ONLY - PC VERSION ONLY - PC VERSION ONLY
//************************************************************************************************
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef PC_VERSION

#define LOAD_ALL_LEVELBANKS	0xFFFF

#define WORLD_NO_FLAGS		0
#define	WORLD_OPEN			1
#define WORLD_CLOSED		0
#define LEVEL_OPEN			1
#define LEVEL_CLOSED		0

#define MAX_WORLDS			10
#define MAX_LEVELS			10

#define FONT16_DATACHUNK_OFFSET		40
#define FONT8_DATACHUNK_OFFSET		40 + 9600


//----- [ OBJECT AND TEXTURE BANKS ] -----

enum worldNames
{
	GARDEN,
	GREENHOUSE,
	CITY,
	SEWER,
	SPACE,
	ANCIENT,
	GAMEWORLD,
	CAVERNS,
	HALLOWEEN,
	SUPERRETRO,
};

enum
{
	SYSTEM_TEX_BANK,
	INGAMEGENERIC_TEX_BANK,

	GARDEN_TEX_BANK,
	ANCIENT_TEX_BANK,
	SPACE_TEX_BANK,
	CITY_TEX_BANK,
	SUBTERRANEAN_TEX_BANK,
	LABORATORY_TEX_BANK,
	TOYSHOP_TEX_BANK,
	HALLOWEEN_TEX_BANK,
	SUPERRETRO_TEX_BANK,

	TITLES_TEX_BANK,
	TITLESGENERIC_TEX_BANK,
	OLDEFROG_TEX_BANK,
	FRONT_TEX_BANK,
	FRONTEND_TEX_BANK,

	SNDVIEW_TEX_BANK,
};

enum
{
	SYSTEM_OBJ_BANK,

	GARDENMASTER_OBJ_BANK,
	GARDENLEV1_OBJ_BANK,
	GARDENLEV2_OBJ_BANK,
	GARDENLEV3_OBJ_BANK,
	GARDENBOSSA_OBJ_BANK,
	GARDENBOSSB_OBJ_BANK,
	GARDENBONUS_OBJ_BANK,
	GARDENMULTI_OBJ_BANK,

	ANCIENTMASTER_OBJ_BANK,
	ANCIENTLEV1_OBJ_BANK,
	ANCIENTLEV2_OBJ_BANK,
	ANCIENTLEV3_OBJ_BANK,
	ANCIENTBOSS_OBJ_BANK,
	ANCIENTBONUS_OBJ_BANK,
	ANCIENTMULTI_OBJ_BANK,

	SPACEMASTER_OBJ_BANK,
	SPACELEV1_OBJ_BANK,
	SPACELEV2_OBJ_BANK,
	SPACELEV3_OBJ_BANK,
	SPACEBOSS_OBJ_BANK,
	SPACEBONUS_OBJ_BANK,
	SPACEMULTI_OBJ_BANK,

	CITYMASTER_OBJ_BANK,
	CITYLEV1_OBJ_BANK,
	CITYLEV2_OBJ_BANK,
	CITYLEV3_OBJ_BANK,
	CITYBOSS_OBJ_BANK,
	CITYBONUS_OBJ_BANK,
	CITYMULTI_OBJ_BANK,

	SUBTERRANEANMASTER_OBJ_BANK,
	SUBTERRANEANLEV1_OBJ_BANK,
	SUBTERRANEANLEV2_OBJ_BANK,
	SUBTERRANEANLEV3_OBJ_BANK,
	SUBTERRANEANBOSS_OBJ_BANK,
	SUBTERRANEANBONUS_OBJ_BANK,
	SUBTERRANEANMULTI_OBJ_BANK,

	LABORATORYMASTER_OBJ_BANK,
	LABORATORYLEV1_OBJ_BANK,
	LABORATORYLEV2_OBJ_BANK,
	LABORATORYLEV3_OBJ_BANK,
	LABORATORYBOSSA_OBJ_BANK,
	LABORATORYBOSSB_OBJ_BANK,
	LABORATORYBOSSC_OBJ_BANK,
	LABORATORYBONUS_OBJ_BANK,
	LABORATORYMULTI_OBJ_BANK,

	TOYSHOPMASTER_OBJ_BANK,
	TOYSHOPLEV1_OBJ_BANK,
	TOYSHOPLEV2_OBJ_BANK,
	TOYSHOPLEV3_OBJ_BANK,
	TOYSHOPBOSS_OBJ_BANK,
	TOYSHOPBONUS_OBJ_BANK,
	TOYSHOPMULTI_OBJ_BANK,

	HALLOWEENMASTER_OBJ_BANK,
	HALLOWEENLEV1_OBJ_BANK,
	HALLOWEENLEV2_OBJ_BANK,
	HALLOWEENLEV3_OBJ_BANK,
	HALLOWEENBOSS_OBJ_BANK,
	HALLOWEENBONUS_OBJ_BANK,
	HALLOWEENMULTI_OBJ_BANK,

	RETROMASTER_OBJ_BANK,
	RETROLEV1_OBJ_BANK,
	RETROLEV2_OBJ_BANK,
	RETROLEV3_OBJ_BANK,
	RETROBOSS_OBJ_BANK,
	RETROBONUS_OBJ_BANK,
	RETROMULTI_OBJ_BANK,


	LANGUAGE_OBJ_BANK,
	FRONTEND_OBJ_BANK,

	FRONTEND1_OBJ_BANK,
	FRONTEND2_OBJ_BANK,
	FRONTEND3_OBJ_BANK,
	FRONTEND4_OBJ_BANK,
	FRONTEND5_OBJ_BANK,

	SUPERRETRO_OBJ_BANK5,
};


//----- [ WORLD ID's ] -----
enum
{
	WORLDID_GARDEN,
	WORLDID_ANCIENT,
	WORLDID_SPACE,
	WORLDID_CITY,
	WORLDID_SUBTERRANEAN,
	WORLDID_LABORATORY,
	WORLDID_TOYSHOP,
	WORLDID_HALLOWEEN,
	WORLDID_SUPERRETRO,
	WORLDID_LANGUAGE,
	WORLDID_FRONTEND,
};

//----- [ LEVEL ID's ] -----
enum
{
	LEVELID_GARDENLAWN,
	LEVELID_GARDENMAZE,
	LEVELID_GARDENVEGPATCH,
	LEVELID_GARDEN4,
	LEVELID_GARDEN5,
	LEVELID_GARDEN6,
};

enum
{
	LEVELID_ANCIENT1,
	LEVELID_ANCIENT2,
	LEVELID_ANCIENT3,
	LEVELID_ANCIENT4,
	LEVELID_ANCIENT5,
	LEVELID_ANCIENT6,
};

enum
{
	LEVELID_SPACE1,
	LEVELID_SPACE2,
	LEVELID_SPACE3,
	LEVELID_SPACE4,
	LEVELID_SPACE5,
	LEVELID_SPACE6,
};

enum
{
	LEVELID_CITYDOCKS,
	LEVELID_CITYWAREHOUSE,
	LEVELID_CITYSTREETS,
	LEVELID_CITYTOWER,
	LEVELID_CITYBONUS,
	LEVELID_CITYMULTI,
};


enum
{
	LEVELID_SUBTERRANEAN1,
	LEVELID_SUBTERRANEAN2,
	LEVELID_SUBTERRANEAN3,
	LEVELID_SUBTERRANEAN4,
	LEVELID_SUBTERRANEAN5,
	LEVELID_SUBTERRANEAN6,
};


enum
{
	LEVELID_LABORATORY1,
	LEVELID_LABORATORY2,
	LEVELID_LABORATORY3,
	LEVELID_LABORATORY4,
	LEVELID_LABORATORY5,
	LEVELID_LABORATORY6,
	LEVELID_LABORATORY7,
	LEVELID_LABORATORY8,
	LEVELID_LABORATORY9
};

enum
{
	LEVELID_TOYSHOP1,
	LEVELID_TOYSHOP2,
	LEVELID_TOYSHOP3,
	LEVELID_TOYSHOP4,
	LEVELID_TOYSHOP5,
	LEVELID_TOYSHOP6,
};

enum
{
	LEVELID_HALLOWEEN1,
	LEVELID_HALLOWEEN2,
	LEVELID_HALLOWEEN3,
	LEVELID_HALLOWEEN4,
	LEVELID_HALLOWEEN5,
	LEVELID_HALLOWEEN6,
};

enum
{
	LEVELID_SUPERRETRO1,
	LEVELID_SUPERRETRO2,
	LEVELID_SUPERRETRO3,
	LEVELID_SUPERRETRO4,
	LEVELID_SUPERRETRO5,
	LEVELID_SUPERRETRO6,
};

enum
{
	LEVELID_FRONTEND1,
	LEVELID_FRONTEND2,
	LEVELID_FRONTEND3,
	LEVELID_FRONTEND4,
	LEVELID_FRONTEND5,
};

enum
{
	LEVELID_LANGUAGE1,
};





//////////////////////////////////////////////////////////////////////////////////////////////////
//************************************************************************************************
// N64 VERSION ONLY - N64 VERSION ONLY - N64 VERSION ONLY - N64 VERSION ONLY - N64 VERSION ONLY
//************************************************************************************************
//////////////////////////////////////////////////////////////////////////////////////////////////
#else

#define LOAD_ALL_LEVELBANKS	0xFFFF

#define WORLD_NO_FLAGS		0
#define	WORLD_OPEN			( 1 << 0 )
#define WORLD_CLOSED		( 1 << 1 )
#define LEVEL_OPEN			( 1 << 2 )
#define LEVEL_CLOSED		( 1 << 3 )

#define MAX_WORLDS			10
#define MAX_LEVELS			10

//----- [ OBJECT AND TEXTURE BANKS ] -----

enum worldNames
{
	GARDEN,
	ANCIENTS,
	SPACE,
	CITY,
	SUBTERRANEAN,
	LABRATORY,
	TOYSHOP,
	HALLOWEEN,
	SUPERRETRO,
};

enum
{
	SYSTEM_TEX_BANK,		// holds the fonts
	
	TITLESGENERIC_TEX_BANK,
	INGAMEGENERIC_TEX_BANK,

	GENERIC_GARDEN_TEX_BANK,
	LEVEL1_GARDEN_TEX_BANK,
	LEVEL2_GARDEN_TEX_BANK,
	LEVEL3_GARDEN_TEX_BANK,
	BOSSA_GARDEN_TEX_BANK,
	BOSSB_GARDEN_TEX_BANK,
	BONUS_GARDEN_TEX_BANK,
	MULTI_GARDEN_TEX_BANK,

	GENERIC_ANCIENT_TEX_BANK,
	LEVEL1_ANCIENT_TEX_BANK,
	LEVEL2_ANCIENT_TEX_BANK,
	LEVEL3_ANCIENT_TEX_BANK,
	BOSS_ANCIENT_TEX_BANK,
	BOSSA_ANCIENT_TEX_BANK,
	BOSSB_ANCIENT_TEX_BANK,
	BOSSC_ANCIENT_TEX_BANK,
	BONUS_ANCIENT_TEX_BANK,
	MULTI_ANCIENT_TEX_BANK,

	GENERIC_SPACE_TEX_BANK,
	LEVEL1_SPACE_TEX_BANK,
	LEVEL2_SPACE_TEX_BANK,
	LEVEL3_SPACE_TEX_BANK,
	BOSSA_SPACE_TEX_BANK,
	BONUS_SPACE_TEX_BANK,
	MULTI_SPACE_TEX_BANK,

	GENERIC_CITY_TEX_BANK,
	LEVEL1_CITY_TEX_BANK,
	LEVEL2_CITY_TEX_BANK,
	LEVEL3_CITY_TEX_BANK,
	BOSSA_CITY_TEX_BANK,
	BONUS_CITY_TEX_BANK,
	MULTI_CITY_TEX_BANK,

	GENERIC_SUBTERRANEAN_TEX_BANK,
	LEVEL1_SUBTERRANEAN_TEX_BANK,
	LEVEL2_SUBTERRANEAN_TEX_BANK,
	LEVEL3_SUBTERRANEAN_TEX_BANK,
	BOSSA_SUBTERRANEAN_TEX_BANK,
	BONUS_SUBTERRANEAN_TEX_BANK,
	MULTI_SUBTERRANEAN_TEX_BANK,

	GENERIC_LABORATORY_TEX_BANK,
	LEVEL1_LABORATORY_TEX_BANK,
	LEVEL2_LABORATORY_TEX_BANK,
	LEVEL3_LABORATORY_TEX_BANK,
	BOSSA_LABORATORY_TEX_BANK,
	BOSSB_LABORATORY_TEX_BANK,
	BOSSC_LABORATORY_TEX_BANK,
	BONUS_LABORATORY_TEX_BANK,
	MULTI_LABORATORY_TEX_BANK,

	GENERIC_TOYSHOP_TEX_BANK,
	LEVEL1_TOYSHOP_TEX_BANK,
	LEVEL2_TOYSHOP_TEX_BANK,
	LEVEL3_TOYSHOP_TEX_BANK,
	BOSSA_TOYSHOP_TEX_BANK,
	BONUS_TOYSHOP_TEX_BANK,
	MULTI_TOYSHOP_TEX_BANK,

	GENERIC_HALLOWEEN_TEX_BANK,
	LEVEL1_HALLOWEEN_TEX_BANK,
	LEVEL2_HALLOWEEN_TEX_BANK,
	LEVEL3_HALLOWEEN_TEX_BANK,
	BOSSA_HALLOWEEN_TEX_BANK,
	BONUS_HALLOWEEN_TEX_BANK,
	MULTI_HALLOWEEN_TEX_BANK,

	GENERIC_RETRO_TEX_BANK,
	LEVEL1_RETRO_TEX_BANK,
	LEVEL2_RETRO_TEX_BANK,
	LEVEL3_RETRO_TEX_BANK,
	BOSSA_RETRO_TEX_BANK,
	BONUS_RETRO_TEX_BANK,
	MULTI_RETRO_TEX_BANK,

	FRONTEND_TEX_BANK,
	FRONTEND1_TEX_BANK,
	FRONTEND2_TEX_BANK,
	FRONTEND3_TEX_BANK,
	FRONTEND4_TEX_BANK,
	FRONTEND5_TEX_BANK,

	OLDEFROG_TEX_BANK,
	SNDVIEW_TEX_BANK,
};

enum
{
	GENERIC_GARDEN_OBJ_BANK,
	LEVEL1_GARDEN_OBJ_BANK,
	LEVEL2_GARDEN_OBJ_BANK,
	LEVEL3_GARDEN_OBJ_BANK,
	BOSSA_GARDEN_OBJ_BANK,
	BOSSB_GARDEN_OBJ_BANK,
	BONUS_GARDEN_OBJ_BANK,
	MULTI_GARDEN_OBJ_BANK,
	
	GENERIC_ANCIENT_OBJ_BANK,
	LEVEL1_ANCIENT_OBJ_BANK,
	LEVEL2_ANCIENT_OBJ_BANK,
	LEVEL3_ANCIENT_OBJ_BANK,
	BOSS_ANCIENT_OBJ_BANK,
	BOSSA_ANCIENT_OBJ_BANK,
	BOSSB_ANCIENT_OBJ_BANK,
	BOSSC_ANCIENT_OBJ_BANK,
	BONUS_ANCIENT_OBJ_BANK,
	MULTI_ANCIENT_OBJ_BANK,
		
	GENERIC_SPACE_OBJ_BANK,
	LEVEL1_SPACE_OBJ_BANK,
	LEVEL2_SPACE_OBJ_BANK,
	LEVEL3_SPACE_OBJ_BANK,
	BOSSA_SPACE_OBJ_BANK,
	BONUS_SPACE_OBJ_BANK,
	MULTI_SPACE_OBJ_BANK,
		
	GENERIC_CITY_OBJ_BANK,
	LEVEL1_CITY_OBJ_BANK,
	LEVEL2_CITY_OBJ_BANK,
	LEVEL3_CITY_OBJ_BANK,
	BOSSA_CITY_OBJ_BANK,
	BONUS_CITY_OBJ_BANK,
	MULTI_CITY_OBJ_BANK,

	GENERIC_SUBTERRANEAN_OBJ_BANK,
	LEVEL1_SUBTERRANEAN_OBJ_BANK,
	LEVEL2_SUBTERRANEAN_OBJ_BANK,
	LEVEL3_SUBTERRANEAN_OBJ_BANK,
	BOSSA_SUBTERRANEAN_OBJ_BANK,
	BONUS_SUBTERRANEAN_OBJ_BANK,
	MULTI_SUBTERRANEAN_OBJ_BANK,

	GENERIC_LABORATORY_OBJ_BANK,
	LEVEL1_LABORATORY_OBJ_BANK,
	LEVEL2_LABORATORY_OBJ_BANK,
	LEVEL3_LABORATORY_OBJ_BANK,
	BOSSA_LABORATORY_OBJ_BANK,
	BONUS_LABORATORY_OBJ_BANK,
	MULTI_LABORATORY_OBJ_BANK,

	GENERIC_TOYSHOP_OBJ_BANK,
	LEVEL1_TOYSHOP_OBJ_BANK,
	LEVEL2_TOYSHOP_OBJ_BANK,
	LEVEL3_TOYSHOP_OBJ_BANK,
	BOSSA_TOYSHOP_OBJ_BANK,
	BONUS_TOYSHOP_OBJ_BANK,
	MULTI_TOYSHOP_OBJ_BANK,

	GENERIC_HALLOWEEN_OBJ_BANK,
	LEVEL1_HALLOWEEN_OBJ_BANK,
	LEVEL2_HALLOWEEN_OBJ_BANK,
	LEVEL3_HALLOWEEN_OBJ_BANK,
	BOSSA_HALLOWEEN_OBJ_BANK,
	BONUS_HALLOWEEN_OBJ_BANK,
	MULTI_HALLOWEEN_OBJ_BANK,

	GENERIC_RETRO_OBJ_BANK,
	LEVEL1_RETRO_OBJ_BANK,
	LEVEL2_RETRO_OBJ_BANK,
	LEVEL3_RETRO_OBJ_BANK,
	BOSSA_RETRO_OBJ_BANK,
	BONUS_RETRO_OBJ_BANK,
	MULTI_RETRO_OBJ_BANK,

	FRONTEND_OBJ_BANK,
	FRONTEND1_OBJ_BANK,
	FRONTEND2_OBJ_BANK,
	FRONTEND3_OBJ_BANK,
	FRONTEND4_OBJ_BANK,
	FRONTEND5_OBJ_BANK,
};


//----- [ WORLD ID's ] -----
enum
{
	WORLDID_GARDEN,
	WORLDID_ANCIENT,
	WORLDID_SPACE,
	WORLDID_CITY,
	WORLDID_SUBTERRANEAN,
	WORLDID_LABORATORY,
	WORLDID_TOYSHOP,
	WORLDID_HALLOWEEN,
	WORLDID_SUPERRETRO,
	WORLDID_FRONTEND,
};

//----- [ LEVEL ID's ] -----
enum
{
	LEVELID_GARDENLEV1,
	LEVELID_GARDENLEV2,
	LEVELID_GARDENLEV3,
	LEVELID_GARDENBOSSA,
	LEVELID_GARDENBOSSB,
	LEVELID_GARDENBONUS,
	LEVELID_GARDENMULTI,
};

enum
{
	LEVELID_ANCIENTLEV1,
	LEVELID_ANCIENTLEV2,
	LEVELID_ANCIENTLEV3,
	LEVELID_ANCIENTBOSSA,
	LEVELID_ANCIENTBOSSB,
	LEVELID_ANCIENTBOSSC,
	LEVELID_ANCIENTBOSSD,
	LEVELID_ANCIENTBONUS,
	LEVELID_ANCIENTMULTI,
};

enum
{
	LEVELID_SPACELEV1,
	LEVELID_SPACELEV2,
	LEVELID_SPACELEV3,
	LEVELID_SPACEBOSSA,
	LEVELID_SPACEBONUS,
	LEVELID_SPACEMULTI,
};

enum
{
	LEVELID_CITYLEV1,
	LEVELID_CITYLEV2,
	LEVELID_CITYLEV3,
	LEVELID_CITYBOSSA,
	LEVELID_CITYBONUS,
	LEVELID_CITYMULTI,
};


enum
{
	LEVELID_SUBTERRANEANLEV1,
	LEVELID_SUBTERRANEANLEV2,
	LEVELID_SUBTERRANEANLEV3,
	LEVELID_SUBTERRANEANBOSSA,
	LEVELID_SUBTERRANEANBONUS,
	LEVELID_SUBTERRANEANMULTI,
};


enum
{
	LEVELID_LABORATORYLEV1,
	LEVELID_LABORATORYLEV2,
	LEVELID_LABORATORYLEV3,
	LEVELID_LABORATORYBOSSA,
	LEVELID_LABORATORYBOSSB,
	LEVELID_LABORATORYBOSSC,
	LEVELID_LABORATORYBONUS,
	LEVELID_LABORATORYMULTI,
};

enum
{
	LEVELID_TOYSHOPLEV1,
	LEVELID_TOYSHOPLEV2,
	LEVELID_TOYSHOPLEV3,
	LEVELID_TOYSHOPBOSSA,
	LEVELID_TOYSHOPBONUS,
	LEVELID_TOYSHOPMULTI,
};

enum
{
	LEVELID_HALLOWEENLEV1,
	LEVELID_HALLOWEENLEV2,
	LEVELID_HALLOWEENLEV3,
	LEVELID_HALLOWEENBOSSA,
	LEVELID_HALLOWEENBONUS,
	LEVELID_HALLOWEENMULTI,
};

enum
{
	LEVELID_SUPERRETROLEV1,
	LEVELID_SUPERRETROLEV2,
	LEVELID_SUPERRETROLEV3,
	LEVELID_SUPERRETROBOSSA,
	LEVELID_SUPERRETROBONUS,
	LEVELID_SUPERRETROMULTI,
};

enum
{
	LEVELID_FRONTEND1,
	LEVELID_FRONTEND2,
	LEVELID_FRONTEND3,
	LEVELID_FRONTEND4,
	LEVELID_FRONTEND5,
};

enum
{
	GARDENLEV1_ENT,
	GARDENLEV2_ENT,
	GARDENLEV3_ENT,
	GARDENBOSSA_ENT,
	GARDENBOSSB_ENT,
	GARDENBONUS_ENT,
	GARDENMULTI_ENT,
};

enum
{
	ANCIENTLEV1_ENT,
	ANCIENTLEV2_ENT,
	ANCIENTLEV3_ENT,
	ANCIENTBOSSA_ENT,
	ANCIENTBOSSB_ENT,
	ANCIENTBOSSC_ENT,
	ANCIENTBOSSD_ENT,
	ANCIENTBONUS_ENT,
	ANCIENTMULTI_ENT,
};

enum
{
	SPACELEV1_ENT,
	SPACELEV2_ENT,
	SPACELEV3_ENT,
	SPACEBOSSA_ENT,
	SPACEBONUS_ENT,
	SPACEMULTI_ENT,
};

enum
{
	CITYLEV1_ENT,
	CITYLEV2_ENT,
	CITYLEV3_ENT,
	CITYBOSSA_ENT,
	CITYBONUS_ENT,
	CITYMULTI_ENT,
};

enum
{
	LABORATORYLEV1_ENT,
	LABORATORYLEV2_ENT,
	LABORATORYLEV3_ENT,
	LABORATORYBOSSA_ENT,
	LABORATORYBONUS_ENT,
	LABORATORYMULTI_ENT,
};

enum
{
	SRETROLEV1_ENT,
	SRETROLEV2_ENT,
	SRETROLEV3_ENT,
	SRETROBOSSA_ENT,
	SRETROBONUS_ENT,
	SRETROMULTI_ENT,
};

enum
{
	FRONTEND1_ENT,
	FRONTEND2_ENT,
	FRONTEND3_ENT,
	FRONTEND4_ENT,
	FRONTEND5_ENT,
};

#endif


//////////////////////////////////////////////////////////////////////////////////////////////////
//************************************************************************************************
// BOTH VERSIONS (PC AND N64)
//************************************************************************************************
//////////////////////////////////////////////////////////////////////////////////////////////////

enum
{
	SYSTEM_COL,

	GARDENMASTER_COL,
	GARDENMASTERA_COL,
	GARDENMASTERB_COL,
	GARDENLEV1_COL,
	GARDENLEV2_COL,
	GARDENLEV3_COL,
	GARDENBONUS_COL,
	GARDENMULTI_COL,

	ANCIENTMASTER_COL,
	ANCIENTMASTERA_COL,
	ANCIENTMASTERB_COL,
	ANCIENTMASTERC_COL,
	ANCIENTMASTERD_COL,
	ANCIENTLEV1_COL,
	ANCIENTLEV2_COL,
	ANCIENTLEV3_COL,
	ANCIENTBONUS_COL,
	ANCIENTMULTI_COL,

	SPACEMASTER_COL,
	SPACEMASTERA_COL,
	SPACELEV1_COL,
	SPACELEV2_COL,
	SPACELEV3_COL,
	SPACEBONUS_COL,
	SPACEMULTI_COL,

	CITYMASTER_COL,
	CITYMASTERA_COL,
	CITYLEV1_COL,
	CITYLEV2_COL,
	CITYLEV3_COL,
	CITYBONUS_COL,
	CITYMULTI_COL,

	SUBTERRANEANMASTER_COL,
	SUBTERRANEANMASTERA_COL,
	SUBTERRANEANLEV1_COL,
	SUBTERRANEANLEV2_COL,
	SUBTERRANEANLEV3_COL,
	SUBTERRANEANBONUS_COL,
	SUBTERRANEANMULTI_COL,

	LABORATORYMASTER_COL,
	LABORATORYMASTERA_COL,
	LABORATORYMASTERB_COL,
	LABORATORYMASTERC_COL,
	LABORATORYLEV1_COL,
	LABORATORYLEV2_COL,
	LABORATORYLEV3_COL,
	LABORATORYBONUS_COL,
	LABORATORYMULTI_COL,

	TOYSHOPMASTER_COL,
	TOYSHOPMASTERA_COL,
	TOYSHOPLEV1_COL,
	TOYSHOPLEV2_COL,
	TOYSHOPLEV3_COL,
	TOYSHOPBONUS_COL,
	TOYSHOPMULTI_COL,

	HALLOWEENMASTER_COL,
	HALLOWEENMASTERA_COL,
	HALLOWEENLEV1_COL,
	HALLOWEENLEV2_COL,
	HALLOWEENLEV3_COL,
	HALLOWEENBONUS_COL,
	HALLOWEENMULTI_COL,

	RETROMASTER_COL,
	RETROMASTERA_COL,
	RETROLEV1_COL,
	RETROLEV2_COL,
	RETROLEV3_COL,
	RETROBONUS_COL,
	RETROMULTI_COL,

	FRONTEND1_COL,
	FRONTEND2_COL,
	FRONTEND3_COL,
	FRONTEND4_COL,
	FRONTEND5_COL,
};


//----- [ DATA STRUCTURES ] -----

typedef struct TAGLEVEL_VISUAL_DATA
{
	unsigned char		description[32];
	unsigned long		levelID;

	long				objectBankToUse;
	long				textureBankToUse;
	long				collBank;

	unsigned long		levelOpen;
	unsigned long		worldOpened;
	unsigned long		multiPartLevel;

	char				fogR,fogG,fogB;
	unsigned long		fogNearDist;
	unsigned long		fogFarDist;
	unsigned long		farDist;

} LEVEL_VISUAL_DATA;


typedef struct TAGWORLD_VISUAL_DATA
{
	unsigned char		description[32];
	unsigned long		worldID;

	unsigned long		masterObjectBank;
	unsigned long		masterTextureBank;

	unsigned long		worldOpen;
	unsigned long		numLevels;
	LEVEL_VISUAL_DATA	levelVisualData[MAX_LEVELS];

} WORLD_VISUAL_DATA;



// ***** ALL VERSIONS ****************************************************************************

extern unsigned long worldNum;
extern unsigned long levelNum;

extern WORLD_VISUAL_DATA	worldVisualData[MAX_WORLDS];

extern UBYTE darkenedLevel;
extern short lightIntensity;


//----- [ FUNCTION PROTOTYPES ] -----

void LoadVisualBanksForWorld(unsigned long worldID,unsigned long levelID);

void FreeAllTextureBanks();
void FreeAllObjectBanks();

void InitLevel(unsigned long worldID,unsigned long levelID);
void FreeAllLists();

#endif
