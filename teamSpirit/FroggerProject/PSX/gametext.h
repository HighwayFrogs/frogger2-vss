/************************************************************************************
	ACTION MAN PS	(c) 1998-9 ISL

	gametext.h:		Game text strings

************************************************************************************/

#ifndef _GAMETEXT_H_
#define _GAMETEXT_H_


enum {
/*	LANG_UK,
	LANG_FRENCH,
	LANG_GERMAN,
	LANG_SPANISH,
	LANG_ITALIAN,
	LANG_DUTCH,
	LANG_SWEDISH,
*/
	LANG_UK,		// (United Kingdom)
	LANG_F,			// (France)
	LANG_D,			// (Germany)
	LANG_E,			// (Spain)
	LANG_IT,		// (Italy)
	LANG_NL,		// (Netherlands)
	LANG_SW,		// (Swedish)
	LANG_FIN,		// (Finland)
	LANG_N,			// (Norway)
	LANG_DK,		// (Denmark)
	LANG_US,		// (United States)

	LANG_NUMLANGS
};

#if PALMODE==1
	#define LANG_DEFAULT LANG_UK
#else
	#define LANG_DEFAULT LANG_US
#endif


enum {
	TEXTSTR_LANGNAME,
	TEXTSTR_YES,
	TEXTSTR_NO,
	TEXTSTR_TEST,
	TEXTSTR_COMMENT_MAINMENU,
	TEXTSTR_STARTGAME,
	TEXTSTR_LANG,
	TEXTSTR_OPTIONS,
	TEXTSTR_LEVELSELECT,
	TEXTSTR_PREVIEWTEXT,
	TEXTSTR_PREVIEWMUSIC,
	TEXTSTR_CONTINUE,
	TEXTSTR_COMMENT_LOADINGSCREENS,
	TEXTSTR_ENEMIESCAPTURED,
	TEXTSTR_POWERPOINTSCOLLECTED,
	TEXTSTR_COMMENT_APARTMENT,
	TEXTSTR_APART_CLOSEMENU,
	TEXTSTR_APART_VIDEOPLAYER,
	TEXTSTR_APART_SOUNDOPTIONS,
	TEXTSTR_APART_GAMEOPTIONS,
	TEXTSTR_APART_STORAGECHEST,
	TEXTSTR_APART_WARDROBE,
	TEXTSTR_APART_BOOKCASE,
	TEXTSTR_APART_WEAPONCABINET,
	TEXTSTR_APART_CLOSEWARDROBE,
	TEXTSTR_APART_CLOSECHEST,
	TEXTSTR_APART_STARTGAME,
	TEXTSTR_APART_CITYSTREETS,
	TEXTSTR_APART_DESERT,
	TEXTSTR_APART_CITYSKIES,
	TEXTSTR_APART_DOCKS,
	TEXTSTR_APART_ICEFLOW,
	TEXTSTR_APART_ICEBERG,
	TEXTSTR_APART_LOAD,
	TEXTSTR_APART_MUSICVOLUME,
	TEXTSTR_APART_EFFECTSVOLUME,
	TEXTSTR_APART_SPEECHVOLUME,
	TEXTSTR_APART_SOUNDMODE,
	TEXTSTR_APART_MUSICTEST,
	TEXTSTR_APART_EFFECTSTEST,
	TEXTSTR_APART_ADJUSTVOLUME,
	TEXTSTR_APART_MONO,
	TEXTSTR_APART_STEREO,
	TEXTSTR_APART_TRACK,
	TEXTSTR_APART_PLAYTRACK,
	TEXTSTR_APART_SELECTTRACK,
	TEXTSTR_APART_PLAYSOUND,
	TEXTSTR_APART_SELECTSOUND,
	TEXTSTR_APART_VIBRATION,
	TEXTSTR_APART_VIBRATIONON,
	TEXTSTR_APART_VIBRATIONOFF,
	TEXTSTR_APART_DIFFICULTY,
	TEXTSTR_APART_EASY,
	TEXTSTR_APART_NORMAL,
	TEXTSTR_APART_HARD,
	TEXTSTR_APART_SCREENADJUST,
	TEXTSTR_APART_ARROWSADJUST,
	TEXTSTR_APART_USECOMPUTER,
	TEXTSTR_APART_RUNPROGRAM,
	TEXTSTR_APART_QUITPROGRAM,
	TEXTSTR_APART_CHANGELETTER,
	TEXTSTR_APART_ADVANCE,
	TEXTSTR_APART_DELETE,
	TEXTSTR_APART_AVAILABLEPROGRAMS,
	TEXTSTR_APART_SELECTVIDEO,
	TEXTSTR_APART_CHANNELBLOCKED,
	TEXTSTR_APART_GAMEINTRO,
	TEXTSTR_APART_C1INTRO,
	TEXTSTR_APART_DSINTRO,
	TEXTSTR_APART_DSHELI,
	TEXTSTR_APART_DSEND,
	TEXTSTR_APART_C2INTRO,
	TEXTSTR_APART_DKINTRO,
	TEXTSTR_APART_DKCAPTUR,
	TEXTSTR_APART_DKJAIL,
	TEXTSTR_APART_DKEND,
	TEXTSTR_APART_ICFINTRO,
	TEXTSTR_APART_ICINTRO,
	TEXTSTR_APART_ICPENT,
	TEXTSTR_APART_ICEND,
	TEXTSTR_APART_CREDITS,
	TEXTSTR_APART_SELECTWEAPON,
	TEXTSTR_APART_SELECTOUTFIT,
	TEXTSTR_APART_POWERPOINTCOLLECTION,
	TEXTSTR_APART_POWERPOINTS,
	TEXTSTR_APART_NEWITEM,
	TEXTSTR_APART_SELECTITEM,
	TEXTSTR_APART_VIEWITEM,
	TEXTSTR_APART_SILVERSPEEDER,
	TEXTSTR_APART_TURBO4X4,
	TEXTSTR_APART_SUPERBIKE,
	TEXTSTR_APART_TOXICA,
	TEXTSTR_APART_PARAGLIDER,
	TEXTSTR_APART_DESERTGUARD,
	TEXTSTR_APART_DESERTPATROL,
	TEXTSTR_APART_OSPREY,
	TEXTSTR_APART_GARDENERROBOT,
	TEXTSTR_APART_ROBOTSPIDER,
	TEXTSTR_APART_GANGRENE,
	TEXTSTR_APART_MYSTERYROBOT,
	TEXTSTR_APART_HELIGUN,
	TEXTSTR_APART_GYROCOPTER,
	TEXTSTR_APART_JETPACK,
	TEXTSTR_APART_NIGHTGUARD,
	TEXTSTR_APART_SENTRYGUN,
	TEXTSTR_APART_ROBOTDOG,
	TEXTSTR_APART_NIGHTPATROL,
	TEXTSTR_APART_SKULLBOT,
	TEXTSTR_APART_LOADER,
	TEXTSTR_APART_MECH,
	TEXTSTR_APART_DRX,
	TEXTSTR_APART_MAXX,
	TEXTSTR_APART_JET,
	TEXTSTR_APART_JETSKI,
	TEXTSTR_APART_HOVERCRAFT,
	TEXTSTR_APART_DSV,
	TEXTSTR_ROBOT_SHARK,
	TEXTSTR_APART_ICEPATROL,
	TEXTSTR_APART_INSECT1,
	TEXTSTR_APART_INSECT2,
	TEXTSTR_APART_INSECT3,
	TEXTSTR_APART_GANGRENE2,
	TEXTSTR_APART_DRX2,
	TEXTSTR_APART_DRXMECH,
	TEXTSTR_APART_AMDESERT,
	TEXTSTR_APART_AMDOCKS,
	TEXTSTR_APART_AMICEBERG,
	TEXTSTR_APART_AMCOMMANDER,
	TEXTSTR_APART_AMSTEALTH,
	TEXTSTR_APART_AMNINJA,
	TEXTSTR_APART_AMTHUG,
	TEXTSTR_APART_AMURBAN,
	TEXTSTR_APART_AMPARATROOPER,
	TEXTSTR_APART_AMFIRE,
	TEXTSTR_APART_MUSICAPART,
	TEXTSTR_APART_MUSICVEHICLE,
	TEXTSTR_APART_MUSICDESERT,
	TEXTSTR_APART_MUSICDOCKS,
	TEXTSTR_APART_MUSICICEBRG,
	TEXTSTR_APART_MUSICBOSS,
	TEXTSTR_APART_MUSICDRX,
	TEXTSTR_APART_HANDGUN,
	TEXTSTR_APART_HANDGUNDESC,
	TEXTSTR_APART_SUPERHANDGUN,
	TEXTSTR_APART_SUPERHANDGUNDESC,
	TEXTSTR_APART_MACHINEGUN,
	TEXTSTR_APART_MACHINEGUNDESC,
	TEXTSTR_APART_TASER,
	TEXTSTR_APART_TASREDESC,
	TEXTSTR_APART_SONIC,
	TEXTSTR_APART_SONICDESC,
	TEXTSTR_APART_ENERGY,
	TEXTSTR_APART_ENERGYDESC,
	TEXTSTR_APART_SNIPER,
	TEXTSTR_APART_SNIPER2,
	TEXTSTR_APART_LONGBOW,
	TEXTSTR_APART_LONGBOWDESC,
	TEXTSTR_APART_CROSSBOW,
	TEXTSTR_APART_CROSSBOWDESC,
	TEXTSTR_APART_DISRUPTERS,
	TEXTSTR_APART_DISRUPTERSDESC,
	TEXTSTR_APART_EXPLOSIVES,
	TEXTSTR_APART_EXPLOSIVESDESC,
	TEXTSTR_APART_ICEGUN,
	TEXTSTR_APART_ICEGUNDESC,
	TEXTSTR_APART_RAILGUN,
	TEXTSTR_APART_RAILGUNDESC,
	TEXTSTR_APART_ELECTRONICSKIT,
	TEXTSTR_APART_ELECTRONICSKITDESC,
	TEXTSTR_APART_REMOTETRIGGER,
	TEXTSTR_APART_REMOTETRIGGER2,
	TEXTSTR_APART_TORCH,
	TEXTSTR_APART_TORCHDESC,
	TEXTSTR_COMMENT_MENUHEADINGS,
	TEXTSTR_MISSION,
	TEXTSTR_CURRENTOBJECTIVE,
	TEXTSTR_MISSIONUPDATE,
	TEXTSTR_WARNING,
	TEXTSTR_DEMOMODE,
	TEXTSTR_COMMENT_GAMESAVE,
	TEXTSTR_GAMESAVE_INSERT,
	TEXTSTR_GAMESAVE_INSERT2,
	TEXTSTR_GAMESAVE_HELPSAVE,
	TEXTSTR_GAMESAVE_HELPLOAD,
	TEXTSTR_GAMESAVE_NOCARD,
	TEXTSTR_GAMESAVE_NOSPACE,
	TEXTSTR_GAMESAVE_FORMAT,
	TEXTSTR_GAMESAVE_HELPCANCEL,
	TEXTSTR_GAMESAVE_HELPFORMAT,
	TEXTSTR_GAMESAVE_WARNING,
	TEXTSTR_GAMESAVE_SAVEFAIL,
	TEXTSTR_GAMESAVE_LOADFAIL,
	TEXTSTR_GAMESAVE_SAVED,
	TEXTSTR_GAMESAVE_LOADED,
	TEXTSTR_GAMESAVE_CANCEL,
	TEXTSTR_GAMESAVE_NOTFOUND,
	TEXTSTR_GAMESAVE_FORMATFAIL,
	TEXTSTR_GAMESAVE_UNFORMATTED,
	TEXTSTR_GAMESAVE_MEMCARDFULL,
	TEXTSTR_COMMENT_HELP,
	TEXTSTR_HELP_NOTHING,
	TEXTSTR_HELP_SELECTOPTION,
	TEXTSTR_HELP_CONTINUEMISSION,
	TEXTSTR_HELP_OPTIONS,
	TEXTSTR_HELP_SHOWMISSION,
	TEXTSTR_HELP_READYWEAPON,
	TEXTSTR_HELP_FIREWEAPON,
	TEXTSTR_HELP_GRABLEDGE,
	TEXTSTR_HELP_ACTIVATE,
	TEXTSTR_HELP_PUSHPULL,
	TEXTSTR_HELP_CLIMBUP,
	TEXTSTR_HELP_USE,
	TEXTSTR_HELP_NEED,
	TEXTSTR_HELP_INVENTORY,
	TEXTSTR_HELP_MOVE,
	TEXTSTR_HELP_ZOOM,
	TEXTSTR_HELP_FIRE,
	TEXTSTR_HELP_EXITGAME,
	TEXTSTR_HELP_EXITGAME2,
	TEXTSTR_HELP_HOLDWEAPON,
	TEXTSTR_COMMENT_ITEMSNWEAPONS,
	TEXTSTR_NOITEMS,
	TEXTSTR_HANDGUN,
	TEXTSTR_MACHINEGUN,
	TEXTSTR_ROCKETLAUNCHER,
	TEXTSTR_GRENADELAUNCHER,
	TEXTSTR_LONGBOW,
	TEXTSTR_SNIPERRIFLE,
	TEXTSTR_CROSSBOW,
	TEXTSTR_GRENADE,
	TEXTSTR_SONICGUN,
	TEXTSTR_TAZERGUN,
	TEXTSTR_GUNKGUN,
	TEXTSTR_ENERGYGUN,
	TEXTSTR_RAILGUN,
	TEXTSTR_ICEGUN,
	TEXTSTR_BOLTGUN,
	TEXTSTR_HANDGUN2,
	TEXTSTR_TORCH,
	TEXTSTR_POWERPOINT,
	TEXTSTR_KEY_HAB,
	TEXTSTR_KEY_POWER2,
	TEXTSTR_MOBILEPHONE,
	TEXTSTR_CIRCUITRY,
	TEXTSTR_CONNECTOR,
	TEXTSTR_SIGNALBOOSTER,
	TEXTSTR_POWERCELLS,
	TEXTSTR_AICHIP,
	TEXTSTR_KEY_DUMP,
	TEXTSTR_KEY_MAPROOM,
	TEXTSTR_COMLINKBOOST,
	TEXTSTR_KEY_CLOAK,
	TEXTSTR_KEY_DEFENCE,
	TEXTSTR_MANHOLE_KEY,
	TEXTSTR_ELECTRONICSKIT,
	TEXTSTR_CAMERA,
	TEXTSTR_GRAPPLEHOOK,
	TEXTSTR_C4PACK,
	TEXTSTR_MOTORWINCH,
	TEXTSTR_DOCKCODE1,
	TEXTSTR_DOCKCODE2,
	TEXTSTR_DOCKCODE3,
	TEXTSTR_DOCKCODE4,
	TEXTSTR_TURNKEY,
	TEXTSTR_FLARES,
	TEXTSTR_MACHINEGUN_AMMO,
	TEXTSTR_ROCKET_AMMO,
	TEXTSTR_GRENADE_AMMO,
	TEXTSTR_LONGBOW_AMMO,
	TEXTSTR_SNIPER_AMMO,
	TEXTSTR_CROSSBOW_AMMO,
	TEXTSTR_SONIC_AMMO,
	TEXTSTR_TAZER_AMMO,
	TEXTSTR_HEALTH,
	TEXTSTR_METALDETECTOR,
	TEXTSTR_CODETABLET1,
	TEXTSTR_CODETABLET2,
	TEXTSTR_CODETABLET3,
	TEXTSTR_CODETABLET4,
	TEXTSTR_CODEKEY1,
	TEXTSTR_CODEKEY2,
	TEXTSTR_CODEKEY3,
	TEXTSTR_CODEKEY4,
	TEXTSTR_PASSBADGE,
	TEXTSTR_GREENLENS,
	TEXTSTR_REDLENS,
	TEXTSTR_BLUELENS,
	TEXTSTR_GASMASK,
	TEXTSTR_ARMKEY,
	TEXTSTR_PASSKEY,
	TEXTSTR_DRILLER,
	TEXTSTR_PLANTSAMPLER,
	TEXTSTR_PLANTSAMPLE,
	TEXTSTR_DISRUPTER,
	TEXTSTR_REMOTETRIGGER,
	TEXTSTR_VIAL,
	TEXTSTR_DOCKCODE1234,
	TEXTSTR_DESCODE5678,
	TEXTSTR_ICECODE1234,
	TEXTSTR_DESCODE1234,
	TEXTSTR_SKULLCHIP,
	TEXTSTR_PACK,
	TEXTSTR_REQUIREDITEM,
	TEXTSTR_COMMENT_DESERTMAPAREAS,
	TEXTSTR_SETTLEMENT,
	TEXTSTR_MAINENTRANCE,
	TEXTSTR_PLANTLAB,
	TEXTSTR_PLANTGREENHOUSE,
	TEXTSTR_WATERPROCESSINGPLANT,
	TEXTSTR_MOTHERPLANT,
	TEXTSTR_TOXICREFINERY,
	TEXTSTR_TOXINSTORAGE,
	TEXTSTR_HELICOPTERPAD,
	TEXTSTR_LOADINGBAY,
	TEXTSTR_COMMENT_DOCKSMAPAREAS,
	TEXTSTR_JETTY,
	TEXTSTR_TURBINE,
	TEXTSTR_TURBINEWAREHOUSE,
	TEXTSTR_HARBOUR,
	TEXTSTR_SKULLBOTWAREHOUSE,
	TEXTSTR_FACTORYENTRANCE,
	TEXTSTR_ROOFTOPS,
	TEXTSTR_FACTORYREFINERY,
	TEXTSTR_FACTORYASSEMBLY,
	TEXTSTR_HOLD,
	TEXTSTR_STORAGEDECK,
	TEXTSTR_SILO,
	TEXTSTR_BRIDGE,
	TEXTSTR_BRIDGEDECK,
	TEXTSTR_HOLDA,
	TEXTSTR_STERN,
	TEXTSTR_COMMENT_ICEBERGMAPAREAS,
	TEXTSTR_SUBMARINEBAY,
	TEXTSTR_WARE1,
	TEXTSTR_POWER1,
	TEXTSTR_POWER2,
	TEXTSTR_DUMP,
	TEXTSTR_SKULLAB,
	TEXTSTR_DORM1,
	TEXTSTR_DORM2,
	TEXTSTR_GANG,
	TEXTSTR_MAPROOM,
	TEXTSTR_SAT,
	TEXTSTR_DEFENCE,
	TEXTSTR_ANTI,
	TEXTSTR_PENT,
	TEXTSTR_COMMENT_VEHICLEITEMS,
	TEXTSTR_OBJECTIVE,
	TEXTSTR_TARGET,
	TEXTSTR_HOMINGDEVICE,
	TEXTSTR_HOMINGDEVICEPIECE,
	TEXTSTR_SLIMER,
	TEXTSTR_SLICKER,
	TEXTSTR_SLAMMER,
	TEXTSTR_RAMMER,
	TEXTSTR_ROADSTER,
	TEXTSTR_TOXICA,
	TEXTSTR_TOXICACAR,
	TEXTSTR_TOXICVAN,
	TEXTSTR_TOXICHELICOPTER,
	TEXTSTR_TECHNICIAN,
	TEXTSTR_CRATE,
	TEXTSTR_SPEEDSTER,
	TEXTSTR_SUBCRUISER,
	TEXTSTR_TURRET,
	TEXTSTR_MINE,
	TEXTSTR_HYDROFOIL,
	TEXTSTR_MANOWAR,
	TEXTSTR_AQUAPLANE,
	TEXTSTR_SPEEDDRONE,
	TEXTSTR_HOVERSLICKER,
	TEXTSTR_COMMENT_VEHICLE_CAPTIONS,
	TEXTSTR_SELECT_VEHICLE,
	TEXTSTR_HOVERCRAFT,
	TEXTSTR_JETSKI,
	TEXTSTR_DSV,
	TEXTSTR_HELICOPTER,
	TEXTSTR_JETPACK,
	TEXTSTR_GYROCOPTER,
	TEXTSTR_SILVER_CAR,
	TEXTSTR_MOTORBIKE,
	TEXTSTR_JEEP,
	TEXTSTR_HOMING,
	TEXTSTR_DEVICE,
	TEXTSTR_PIECE,
	TEXTSTR_PIECES,
	TEXTSTR_POWER,
	TEXTSTR_POINT,
	TEXTSTR_POINTS,
	TEXTSTR_REPAIR,
	TEXTSTR_REMAINING,
	TEXTSTR_COMMENT_INTRO,
	TEXTSTR_INTROTEXT,
	TEXTSTR_INTROTEXT2,
	TEXTSTR_COMMENT_APARTMENTMISSIONS,
	TEXTSTR_MISSION_CITY1A,
	TEXTSTR_MISSION_CITY1B,
	TEXTSTR_MISSION_DESERTA,
	TEXTSTR_MISSION_DESERTB,
	TEXTSTR_MISSION_CITY2A,
	TEXTSTR_MISSION_CITY2B,
	TEXTSTR_MISSION_DOCKSA,
	TEXTSTR_MISSION_DOCKSB,
	TEXTSTR_MISSION_ICEFLOWA,
	TEXTSTR_MISSION_ICEFLOWB,
	TEXTSTR_MISSION_ICEBRGA,
	TEXTSTR_MISSION_ICEBRGB,
	TEXTSTR_COMMENT_CITY1DIALOGUE,
	TEXTSTR_DIALOGUE_CITY1A,
	TEXTSTR_DIALOGUE_CITY1B,
	TEXTSTR_DIALOGUE_CITY1C,
	TEXTSTR_DIALOGUE_CITY1D,
	TEXTSTR_DIALOGUE_CITY1E,
	TEXTSTR_DIALOGUE_CITY1F,
	TEXTSTR_DIALOGUE_CITY1G,
	TEXTSTR_DIALOGUE_CITY1H,
	TEXTSTR_DIALOGUE_CITY1I,
	TEXTSTR_DIALOGUE_CITY1J,
	TEXTSTR_COMMENT_CITY1MISSIONS,
	TEXTSTR_MISSION_CITY1,
	TEXTSTR_OBJECT_KILLGANGS,
	TEXTSTR_OBJECT_CITY1_GETPP,
	TEXTSTR_OBJECT_GET4X4,
	TEXTSTR_OBJECT_TOXICA,
	TEXTSTR_COMMENT_CITY1BOSS,
	TEXTSTR_NAME_TOXICA,
	TEXTSTR_DESC_TOXICA,
	TEXTSTR_WEAK_TOXICA,
	TEXTSTR_COMMENT_DESERTDIALOGUE,
	TEXTSTR_DIALOGUE_INDIAN,
	TEXTSTR_DIALOGUE_MAIN,
	TEXTSTR_DIALOGUE_MAIN2,
	TEXTSTR_DIALOGUE_GENETIC,
	TEXTSTR_DIALOGUE_GENETIC2,
	TEXTSTR_DIALOGUE_GENETIC3,
	TEXTSTR_DIALOGUE_GENETIC4,
	TEXTSTR_DIALOGUE_TOXIC,
	TEXTSTR_DIALOGUE_TOXIC2,
	TEXTSTR_DIALOGUE_MOTHER,
	TEXTSTR_DIALOGUE_REFINERY,
	TEXTSTR_DIALOGUE_REFINERY2,
	TEXTSTR_DIALOGUE_REFINERY3,
	TEXTSTR_DIALOGUE_REFINERY4,
	TEXTSTR_DIALOGUE_HELI,
	TEXTSTR_DIALOGUE_HELI2,
	TEXTSTR_DIALOGUE_CS_1,
	TEXTSTR_DIALOGUE_CS_2,
	TEXTSTR_COMMENT_DESERTMISSIONS,
	TEXTSTR_OBJECT_INVESTIGATE,
	TEXTSTR_MISSION_INVESTIGATE,
	TEXTSTR_OBJECT_METALDETECT,
	TEXTSTR_OBJECT_MONOLITHS,
	TEXTSTR_OBJECT_SNIPER1,
	TEXTSTR_OBJECT_CRANE,
	TEXTSTR_OBJECT_PLANTSAMPLE,
	TEXTSTR_OBJECT_ANALYSEPLANT,
	TEXTSTR_MISSION_DESTROYPLANTS,
	TEXTSTR_OBJECT_TURNOFFWATER,
	TEXTSTR_OBJECT_TURNOFFWATER2,
	TEXTSTR_OBJECT_LOWERBRIDGE,
	TEXTSTR_OBJECT_OPENCANOPY,
	TEXTSTR_OBJECT_DESTROYMOTHER,
	TEXTSTR_OBJECT_PLANTPURPOSE,
	TEXTSTR_MISSION_NEUTRALISETOXIN,
	TEXTSTR_OBJECT_PRODUCEANTITOXIN,
	TEXTSTR_OBJECT_USEANTITOXIN,
	TEXTSTR_OBJECT_USEDISRUPTERS,
	TEXTSTR_OBJECT_USEDISRUPTERS2,
	TEXTSTR_OBJECT_USEDRILL,
	TEXTSTR_COMMENT_DESERTHELP,
	TEXTSTR_INTRO_CODEKEYS,
	TEXTSTR_INTRO_SNIPER,
	TEXTSTR_INTRO_JUMP,
	TEXTSTR_INTRO_JUMP2,
	TEXTSTR_INTRO_PLANTSAMPLER,
	TEXTSTR_INTRO_ELECTRONICSKIT,
	TEXTSTR_INTRO_SWITCHBRIDGE,
	TEXTSTR_INTRO_SWITCHCURRENT,
	TEXTSTR_INTRO_VALVES,
	TEXTSTR_INTRO_MAPSCREEN,
	TEXTSTR_COMMENT_DESERTWARNINGS,
	TEXTSTR_WARNING_MAIN,
	TEXTSTR_WARNING_TOXIC,
	TEXTSTR_WARNING_MOTHER,
	TEXTSTR_WARNING_STORAGE,
	TEXTSTR_WARNING_EXPLOSIVE,
	TEXTSTR_COMMENT_CITY2DIALOGUE,
	TEXTSTR_DIALOGUE_CITY2A,
	TEXTSTR_DIALOGUE_CITY2B,
	TEXTSTR_DIALOGUE_CITY2C,
	TEXTSTR_DIALOGUE_CITY2D,
	TEXTSTR_DIALOGUE_CITY2E,
	TEXTSTR_DIALOGUE_CITY2F,
	TEXTSTR_DIALOGUE_CITY2G,
	TEXTSTR_DIALOGUE_CITY2H,
	TEXTSTR_DIALOGUE_CITY2I,
	TEXTSTR_DIALOGUE_CITY2J,
	TEXTSTR_DIALOGUE_CITY2K,
	TEXTSTR_DIALOGUE_CITY2L,
	TEXTSTR_COMMENT_CITY2MISSIONS,
	TEXTSTR_MISSION_CITY2,
	TEXTSTR_OBJECT_KILL_VANS,
	TEXTSTR_OBJECT_CITY2GETPP,
	TEXTSTR_OBJECT_GETGYRO,
	TEXTSTR_OBJECT_SPEEDSTER,
	TEXTSTR_COMMENT_CITY2BOSS,
	TEXTSTR_NAME_SPEEDSTER,
	TEXTSTR_DESC_SPEEDSTER,
	TEXTSTR_WEAK_SPEEDSTER,
	TEXTSTR_COMMENT_DOCKSDIALOGUE,
	TEXTSTR_DIALOGUE_JETTY,
	TEXTSTR_DIALOGUE_JETTY2,
	TEXTSTR_DIALOGUE_JETTY3,
	TEXTSTR_DIALOGUE_TURBINE,
	TEXTSTR_DIALOGUE_TURBINE2,
	TEXTSTR_DIALOGUE_SKULLWAR,
	TEXTSTR_DIALOGUE_SKULLENT,
	TEXTSTR_DIALOGUE_SKULLENT2,
	TEXTSTR_DIALOGUE_SKULLENT3,
	TEXTSTR_DIALOGUE_ROOFTOPS,
	TEXTSTR_DIALOGUE_SKULLENT4,
	TEXTSTR_DIALOGUE_FACTREF,
	TEXTSTR_DIALOGUE_FACTASM,
	TEXTSTR_DIALOGUE_CS_3,
	TEXTSTR_DIALOGUE_CS_4,
	TEXTSTR_DIALOGUE_CS_5,
	TEXTSTR_DIALOGUE_HOLD,
	TEXTSTR_DIALOGUE_SILO,
	TEXTSTR_DIALOGUE_SILO2,
	TEXTSTR_DIALOGUE_BRIDGE,
	TEXTSTR_DIALOGUE_BRIDGE2,
	TEXTSTR_DIALOGUE_BRIDGE3,
	TEXTSTR_DIALOGUE_BRIDGE4,
	TEXTSTR_DIALOGUE_BRIDGE5,
	TEXTSTR_DIALOGUE_BRIDGE6,
	TEXTSTR_COMMENT_DOCKSMISSIONS,
	TEXTSTR_MISSION_INVESTIGATEX,
	TEXTSTR_OBJECT_JETTY,
	TEXTSTR_OBJECT_SEWERKEY,
	TEXTSTR_OBJECT_EKDOOR,
	TEXTSTR_OBJECT_SPYCAMERA,
	TEXTSTR_MISSION_SKULLBOTINVESTIGATE,
	TEXTSTR_OBJECT_INVESTIGATE2,
	TEXTSTR_OBJECT_HARBOURSNIPER,
	TEXTSTR_OBJECT_SKULLBOTSOURCE,
	TEXTSTR_OBJECT_JUMPCRATES,
	TEXTSTR_MISSION_SKULLBOTNEUTRALISE,
	TEXTSTR_OBJECT_GRAPPLE,
	TEXTSTR_OBJECT_HOMINGSIGNAL,
	TEXTSTR_OBJECT_COLLECTC4,
	TEXTSTR_OBJECT_HIDEINBOX,
	TEXTSTR_OBJECT_PLACEEXPLOSIVES,
	TEXTSTR_OBJECT_ESCAPEEXPLOSIVES,
	TEXTSTR_MISSION_PREVENTMISSILELAUNCH,
	TEXTSTR_OBJECT_RECOVEREQUIPMENT,
	TEXTSTR_OBJECT_DEACTIVATEWARHEADS,
	TEXTSTR_OBJECT_CLAMPMISSILES,
	TEXTSTR_OBJECT_LAUNCH,
	TEXTSTR_OBJECT_LAUNCH2,
	TEXTSTR_OBJECT_ESCAPESHIP,
	TEXTSTR_COMMENT_DOCKSHELP,
	TEXTSTR_INTRO_VALVE,
	TEXTSTR_COMMENT_DOCKSWARNINGS,
	TEXTSTR_WARNING_JETTY,
	TEXTSTR_WARNING_HARBOUR,
	TEXTSTR_WARNING_SKULLWAR,
	TEXTSTR_WARNING_ROOFTOPS,
	TEXTSTR_COMMENT_ICEFLOWDIALOGUE,
	TEXTSTR_DIALOGUE_ICEFLOWA,
	TEXTSTR_DIALOGUE_ICEFLOWB,
	TEXTSTR_DIALOGUE_ICEFLOWC,
	TEXTSTR_DIALOGUE_ICEFLOWD,
	TEXTSTR_DIALOGUE_ICEFLOWE,
	TEXTSTR_DIALOGUE_ICEFLOWF,
	TEXTSTR_DIALOGUE_ICEFLOWG,
	TEXTSTR_DIALOGUE_ICEFLOWH,
	TEXTSTR_DIALOGUE_ICEFLOWI,
	TEXTSTR_COMMENT_ICEFLOWMISSIONS,
	TEXTSTR_MISSION_ICEFLOW,
	TEXTSTR_OBJECT_GETPP,
	TEXTSTR_OBJECT_GETDSV,
	TEXTSTR_OBJECT_GETHD,
	TEXTSTR_OBJECT_GOOVER,
	TEXTSTR_OBJECT_FINALPIECE,
	TEXTSTR_OBJECT_TECHNICIAN,
	TEXTSTR_COMMENT_ICEFLOWBOSS,
	TEXTSTR_NAME_TECHNICIAN,
	TEXTSTR_DESC_TECHNICIAN,
	TEXTSTR_WEAK_TECHNICIAN,
	TEXTSTR_COMMENT_ICEBERGDIALOGUE,
	TEXTSTR_DIALOGUE_SUBBAY,
	TEXTSTR_DIALOGUE_SUBBAY2,
	TEXTSTR_DIALOGUE_POWER2,
	TEXTSTR_DIALOGUE_POWER22,
	TEXTSTR_DIALOGUE_POWER23,
	TEXTSTR_DIALOGUE_POWER24,
	TEXTSTR_DIALOGUE_POWER25,
	TEXTSTR_DIALOGUE_POWER26,
	TEXTSTR_DIALOGUE_CS_6,
	TEXTSTR_DIALOGUE_CS_6B,
	TEXTSTR_DIALOGUE_CS_7,
	TEXTSTR_DIALOGUE_DUMP,
	TEXTSTR_DIALOGUE_DUMP2,
	TEXTSTR_DIALOGUE_MAPROOM,
	TEXTSTR_DIALOGUE_MAPROOM2,
	TEXTSTR_DIALOGUE_MAPROOM3,
	TEXTSTR_DIALOGUE_DEFENCE,
	TEXTSTR_DIALOGUE_DEFENCE2,
	TEXTSTR_DIALOGUE_DEFENCE3,
	TEXTSTR_DIALOGUE_SAT,
	TEXTSTR_DIALOGUE_SAT2,
	TEXTSTR_DIALOGUE_SAT3,
	TEXTSTR_DIALOGUE_ANTE,
	TEXTSTR_DIALOGUE_PENT,
	TEXTSTR_DIALOGUE_PENT2,
	TEXTSTR_DIALOGUE_PENT3,
	TEXTSTR_DIALOGUE_END,
	TEXTSTR_DIALOGUE_END2,
	TEXTSTR_DIALOGUE_END3,
	TEXTSTR_DIALOGUE_END4,
	TEXTSTR_COMMENT_ICEBERGMISSIONS,
	TEXTSTR_MISSION_DESTROYPOWER,
	TEXTSTR_OBJECT_SUBBAY,
	TEXTSTR_OBJECT_WAREHOUSE,
	TEXTSTR_OBJECT_PASSKEY,
	TEXTSTR_OBJECT_PASSKEY2,
	TEXTSTR_OBJECT_POWERFORCEFIELD,
	TEXTSTR_OBJECT_POWERDISRUPTER,
	TEXTSTR_MISSION_CREATEVIRUS,
	TEXTSTR_OBJECT_FINDAICHIP,
	TEXTSTR_OBJECT_USEINFRARED,
	TEXTSTR_OBJECT_COLLECTAICHIP,
	TEXTSTR_OBJECT_USEINFRARED2,
	TEXTSTR_OBJECT_ACCESSCOMPUTER,
	TEXTSTR_OBJECT_DEACTIVATEDEFENCE,
	TEXTSTR_OBJECT_CREATEVIRUS,
	TEXTSTR_MISSION_USEVIRUS,
	TEXTSTR_OBJECT_ACCESSSAT,
	TEXTSTR_OBJECT_ACCESSSAT2,
	TEXTSTR_OBJECT_REALIGNSAT,
	TEXTSTR_MISSION_NEUTRALISEX,
	TEXTSTR_OBJECT_ACCESSPENTHOUSE,
	TEXTSTR_OBJECT_ANTE,
	TEXTSTR_COMMENT_ICEBERGHELP,
	TEXTSTR_INTRO_LASERFENCE,
	TEXTSTR_INTRO_DRX,
	TEXTSTR_COMMENT_ICEBERGWARNINGS,
	TEXTSTR_WARNING_SKULLAB,
	TEXTSTR_COMMENT_DESERTENEMIES,
	TEXTSTR_NMENAME_DESERTGUARD,
	TEXTSTR_NMEDESC_DESERTGUARD,
	TEXTSTR_NMENAME_GARDENER,
	TEXTSTR_NMEDESC_GARDENER,
	TEXTSTR_NMENAME_PLANT,
	TEXTSTR_NMEDESC_PLANT,
	TEXTSTR_NMENAME_TECHG,
	TEXTSTR_NMEDESC_TECHG,
	TEXTSTR_NMENAME_SKULLBOT1,
	TEXTSTR_NMEDESC_SKULLBOT1,
	TEXTSTR_NMENAME_MOTHER,
	TEXTSTR_NMEDESC_MOTHER,
	TEXTSTR_NMEWEAK_MOTHER,
	TEXTSTR_NMENAME_SPIDER,
	TEXTSTR_NMEDESC_SPIDER,
	TEXTSTR_NMEWEAK_SPIDER,
	TEXTSTR_COMMENT_DOCKSENEMIES,
	TEXTSTR_NMENAME_NIGHTGUARD,
	TEXTSTR_NMEDESC_NIGHTGUARD,
	TEXTSTR_NMENAME_DOG,
	TEXTSTR_NMEDESC_DOG,
	TEXTSTR_NMENAME_CRATE,
	TEXTSTR_NMEDESC_CRATE,
	TEXTSTR_NMENAME_CAMERA,
	TEXTSTR_NMEDESC_CAMERA,
	TEXTSTR_NMENAME_GUNCAMERA,
	TEXTSTR_NMEDESC_GUNCAMERA,
	TEXTSTR_NMENAME_SENTRY,
	TEXTSTR_NMEDESC_SENTRY,
	TEXTSTR_NMENAME_TECHX,
	TEXTSTR_NMEDESC_TECHX,
	TEXTSTR_NMENAME_ROBOT,
	TEXTSTR_NMEDESC_ROBOT,
	TEXTSTR_NMENAME_SKULLBOT2,
	TEXTSTR_NMEDESC_SKULLBOT2,
	TEXTSTR_NMENAME_MECH,
	TEXTSTR_NMEDESC_MECH,
	TEXTSTR_NMEWEAK_MECH,
	TEXTSTR_NMENAME_JET,
	TEXTSTR_NMEDESC_JET,
	TEXTSTR_NMEWEAK_JET,
	TEXTSTR_COMMENT_ICEBERGENEMIES,
	TEXTSTR_NMENAME_ICEGUARD,
	TEXTSTR_NMEDESC_ICEGUARD,
	TEXTSTR_NMENAME_SKULLBOT3,
	TEXTSTR_NMEDESC_SKULLBOT3,
	TEXTSTR_NMENAME_LASER,
	TEXTSTR_NMEDESC_LASER,
	TEXTSTR_NMENAME_INSECT,
	TEXTSTR_NMEDESC_INSECT,
	TEXTSTR_NMEWEAK_INSECT,
	TEXTSTR_NMENAME_X,
	TEXTSTR_NMEDESC_X,
	TEXTSTR_NMEWEAK_X,

	TEXTSTR_MAX
};


#define TEXTSTR(n) (gameTextStr[n])


extern void	*gameTextStr[TEXTSTR_MAX];


/**************************************************************************
	FUNCTION:	gameTextInit
	PURPOSE:	Read in the text from gametext file
	PARAMETERS:	Current language
	RETURNS:	
**************************************************************************/

//void gameTextInit(int currLang);


/**************************************************************************
	FUNCTION:	gameTextInitFromFile
	PURPOSE:	Read in the text from a gametext file
	PARAMETERS:	Filename, num strings, current language
	RETURNS:	
**************************************************************************/

void gameTextInitFromFile(char *fName, int numStrings, int currLang);


/**************************************************************************
	FUNCTION:	gameTextDestroy
	PURPOSE:	Free up text
	PARAMETERS:	
	RETURNS:	
**************************************************************************/

void gameTextDestroy();


/* These functions can go once the text has been checked *****************/

void gameTextScrInitialise();
void gameTextScrDestroy();
void gameTextScrFrame();

#endif
