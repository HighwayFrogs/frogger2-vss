/*

	This file is part of Frogger2, (c) 1999 Interactive Studios Ltd.


	File		: language.c
	Programmer	: James Healey
	Date		: 31/03/99

----------------------------------------------------------------------------------------------- */

#define F3DEX_GBI_2

#include "ultra64.h"
#include "language.h"

char *DIKStrings[256][LANG_NUMLANGS] = 
{
//english			//french			//german		//italian		//american

	"",				"",               	"",             "",             "",                   
	"Escape",		"Echap",			"Esc",			"Esc",			"Escape",		// 0
	"1",			"1",              	"1",            "1",            "1",                  
	"2",			"2",              	"2",            "2",            "2",                  
	"3",			"3",              	"3",            "3",            "3",                  
	"4",			"4",              	"4",            "4",            "4",                  
	"5",			"5",              	"5",            "5",            "5",                  
	"6",			"6",              	"6",            "6",            "6",                  
	"7",			"7",              	"7",            "7",            "7",                  
	"8",			"8",              	"8",            "8",            "8",                  
	"9",			"9",              	"9",            "9",            "9",                  
	"0",			"0",				"0",			"0",			"0",			// 10 DIK_UP
	"-",			")",              	"�",            "�",            "-",                  
	"=",			"=",              	"�",            "�",            "=",                  
	"Backspace",	"Espace arri�re",  	"R�cktaste",    "Indietro",	    "Backspace",          
	"Tab",			"Tabulation",      	"Tab",          "Tab",          "Tab",                
	"q",			"a",              	"q",            "q",            "q",                  
	"w",			"z",              	"w",            "w",            "w",                  
	"e",			"e",              	"e",            "e",            "e",                  
	"r",			"r",              	"r",            "r",            "r",                  
	"t",			"t",              	"t",            "t",            "t",                  
	"y",			"y",				"z",			"y",			"y",			// 20
	"u",			"u",              	"u",            "u",            "u",                  
	"i",			"i",              	"i",            "i",            "i",                  
	"o",			"o",              	"o",            "o",            "o",                  
	"p",			"p",              	"p",            "p",            "p",                  
	"[",			"^",              	"�",            "�",            "[",                  
	"]",			"$",              	"+",            "+",            "]",                  
	"Return",		"Entr�e",         	"Eingabe",      "Invio",	    "Enter",             
	"Left Control",	"Ctrl gauche",   	"Strg links",	"Ctrl sinistro","Left Control",       
	"a",			"q",              	"a",            "a",            "a",                  
	"s",			"s",				"s",			"s",			"s",			// 30
	"d",			"d",              	"d",            "d",            "d",                  
	"f",			"f",              	"f",            "f",            "f",                  
	"g",			"g",              	"g",            "g",            "g",                  
	"h",			"h",              	"h",            "h",            "h",                  
	"j",			"j",              	"j",            "j",            "j",                  
	"k",			"k",              	"k",            "k",            "k",                  
	"l",			"l",              	"l",            "l",            "l",                  
	";",			"m",              	"�",            "�",            ";",                  
	"'",			"�",              	"�",            "�",            "'",                  
	"`",			"�",	          	"^",	        "\\",	        "`",
	"Left Shift",	"Maj gauche",     	"Umschalt links",   "Maiusc sinistro",   "Left Shift",         
	"#",			"*",		      	"#",		    "�",		    "#",          
	"z",			"w",              	"y",            "z",            "z",                  
	"x",			"x",              	"x",            "x",            "x",                  
	"c",			"c",              	"c",            "c",            "c",                  
	"v",			"v",              	"v",            "v",            "v",                  
	"b",			"b",              	"b",            "b",            "b",                  
	"n",			"n",              	"n",            "n",            "n",                  
	"m",			",",              	"m",            "m",            "m",                  
	",",			";",				";",			",",			",",			// 50
	".",			":",              	":",            ".",            ".",                  
	"/",			"!",              	"-",            "-",            "/",                  
	"Right Shift",	"Maj droite",    	"Umschalt rechts","Maiusc destro","Right Shift",        
	"*",			"*",              	"*",            "*",            "*",                  
	"Left Alt",		"Alt",		       	"Alt",		    "Alt",     "Left Alt",           
	"Space",		"Barre d'espacement","Leertaste",   "Barra spaziatrice","Space",              
	"Caps Lock",	"Verr maj",      	"Feststell",    "Bloc Maiusc",  "Caps Lock",          
	"F1",			"F1",             	"F1",           "F1",           "F1",                 
	"F2",			"F2",             	"F2",           "F2",           "F2",                 
	"F3",			"F3",             	"F3",           "F3",           "F3",                 
	"F4",			"F4",             	"F4",           "F4",           "F4",                 
	"F5",			"F5",             	"F5",           "F5",           "F5",                 
	"F6",			"F6",             	"F6",           "F6",           "F6",                 
	"F7",			"F7",             	"F7",           "F7",           "F7",                 
	"F8",			"F8",             	"F8",           "F8",           "F8",                 
	"F9",			"F9",             	"F9",           "F9",           "F9",                 
	"F10",			"F10",            	"F10",          "F10",          "F10",                
	"NumLock",		"Verr Num",        	"Num-Taste",    "Bloc Num",     "NumLock",            
	"Scroll Lock",	"Arr�t d�fil",    	"Rollen",		"Bloc Scorr",	"Scroll Lock",        
	"Numpad 7",		"Pavnum 7",       	"Zifblk 7",     "Tast. num. 7",     "Numpad 7",           
	"Numpad 8",		"Pavnum 8",       	"Zifblk 8",     "Tast. num. 8",     "Numpad 8",           
	"Numpad 9",		"Pavnum 9",       	"Zifblk 9",     "Tast. num. 9",     "Numpad 9",           
	"-",			"-",              	"-",            "-",            "-",                  
	"Numpad 4",		"Pavnum 4",       	"Zifblk 4",     "Tast. num. 4",     "Numpad 4",           
	"Numpad 5",		"Pavnum 5",       	"Zifblk 5",     "Tast. num. 5",     "Numpad 5",           
	"Numpad 6",		"Pavnum 6",       	"Zifblk 6",     "Tast. num. 6",     "Numpad 6",           
	"+",			"+",              	"+",            "+",            "+",                  
	"Numpad 1",		"Pavnum 1",       	"Zifblk 1",     "Tast. num. 1",     "Numpad 1",           
	"Numpad 2",		"Pavnum 2",       	"Zifblk 2",     "Tast. num. 2",     "Numpad 2",           
	"Numpad 3",		"Pavnum 3",       	"Zifblk 3",     "Tast. num. 3",     "Numpad 3",           
	"Numpad 0",		"Pavnum 0",       	"Zifblk 0",     "Tast. num. 0",     "Numpad 0",           
	".",			".",              	".",            ".",            ".",                  
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"\\",			"<",               	"<",            "<",            "\\",                   
	"F11",			"F11",            	"F11",          "F11",          "F11",                
	"F12",			"F12",            	"F12",          "F12",          "F12",                
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"F13",			"F13",            	"F13",          "F13",          "F13",                
	"F14",			"F14",            	"F14",          "F14",          "F14",                
	"F15",			"F15",            	"F15",          "F15",          "F15",                
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"Kana",			"Kana",           	"Kana",         "Kana",         "Kana",               
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"Convert",		"Convert",        	"Convert",      "Convert",      "Convert",            
	"",				"",               	"",             "",             "",                   
	"No Convert",	"No Convert",     	"No Convert",   "No Convert",   "No Convert",         
	"",				"",               	"",             "",             "",                   
	"Yen",			"Yen",            	"Yen",          "Yen",          "Yen",                
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"=",			"=",              	"=",            "=",            "=",                  
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"Circumflex",	"Circumflex",     	"Circumflex",   "Circumflex",   "Circumflex",         
	"@",			"%",              	"�",            "�",            "@",                  
	":",			":",              	":",            ":",            ":",                  
	"Underline",	"Underline",      	"Underline",    "Underline",    "Underline",          
	"Kanji",		"Kanji",          	"Kanji",        "Kanji",        "Kanji",              
	"Stop",			"Stop",           	"Stop",         "Stop",         "Stop",               
	"AX",			"AX",             	"AX",           "AX",           "AX",                 
	"Unlabeled",	"Unlabeled",		"Unlabeled",	"Unlabeled",	"Unlabeled",	// 100
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"Numpad Enter",	"Entr",			   	"Enter",		"Invio",		"Numpad Enter",       
	"Right Control","Ctrl droit",	  	"Strg rechts",	"Ctrl destro",	"Right Control",      
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	",",			",",              	",",            ",",            ",",                  
	"",				"",               	"",             "",             "",                   
	"/",			"!",              	"-",            "-",            "/",                  
	"",				"",               	"",             "",             "",                   
	"SysRq",		"Syst",          	"S-Abf",        "R Sist",       "SysRq",              
	"Right Alt",	"Alt Gr",	      	"Alt Gr",	    "Alt Gr",    "Right Alt",          
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"Home",			"Origine",         	"Pos 1",        "Home",         "Home",               
	"Up",			"Touche haut",		"Nach-oben-Taste","Freccia su","Up",                 
	"Page Up",		"Pg. pr�c.",       	"Bild-auf",     "Pgsu",		    "Page Up",            
	"",				"",               	"",             "",             "",                   
	"Left",			"Touche gauche",	"Nach-links-Taste","Freccia sinistra","Left",               
	"",				"",               	"",             "",             "",                   
	"Right",		"Touche droite",	"Nach-rechts-Taste","Freccia destra","Right",              
	"",				"",               	"",             "",             "",                   
	"End",			"Fin",            	"Ende",         "Fine",         "End",                
	"Down",			"Touche bas",		"Nach-unten-Taste","Freccia gi�","Down",               
	"Page Down",	"Pg. suiv.",      	"Bild-ab",	    "Pggi�",	    "Page Down",          
	"Insert",		"Inser",         	"Einfg",		"Ins",	       "Insert",             
	"Delete",		"Suppr",         	"Entf",		    "Canc",		    "Delete",             
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"",				"",               	"",             "",             "",                   
	"Left Win",		"Symbole Windows G","Windowstaste links","Tasto Win sinistro","Left Win",           
	"Right Win",	"Symbole Windows D","Windowstaste rechts","Tasto Win destro","Right Win",          
	"Apps",			"Touche d'application","Anwendungstaste","Tasto menu a tendina","Apps",               
};
