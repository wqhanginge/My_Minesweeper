/*****************************************************************************\
 *  My Minesweepper -- a classic minesweeper game
 *  Copyright (C) 2020-2023 Gee W.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
\*****************************************************************************/
/*****************************************************************************\
 * encapsulations.h
 *****************************************************************************
 * This file contains encapsulations of UI operations and IO functions.
 * This file contains a struct and relative functions for bitmap management.
 * This file also contians some useful transform functions.
 * 
 * NOTE: Most functions do NOT have arg check process, use with care.
\*****************************************************************************/


#pragma once

#include "stdincludes.h"
#include "gamecore.h"
#include "basicUI.h"



/* IO defines and game init defines */

#define DEF_WND_LEFT	128
#define DEF_WND_TOP		128
#define DEF_WND_WIDTH	750
#define DEF_WND_HEIGHT	700
#define DEF_CONFNAME	"MyMinesweeper.ini"
#define DEF_CONFPATHENV	"LOCALAPPDATA"
#define CONFBUFFLEN		5
#define MAX_APPPATH		480
#define MAX_CONFPATH	MAX_APPPATH

//key words in config file
#define INIT_ANAME		"Init"
#define INIT_XPOS		"xpos"
#define INIT_YPOS		"ypos"
#define INIT_MODE		"mode"
#define INIT_WIDTH		"width"
#define INIT_HEIGHT		"height"
#define INIT_MINES		"mines"
#define INIT_MARK		"mark"
#define SCORE_ANAME		"Record"
#define SCORE_JTIME		"junior_time"
#define SCORE_MTIME		"middle_time"
#define SCORE_STIME		"senior_time"
#define SCORE_JNAME		"junior_name"
#define SCORE_MNAME		"middle_name"
#define SCORE_SNAME		"senior_name"
//end key words

/* property related defines */

//lang-codepage is 000004b0 (Language Neutral)
#define PNQUERYSTR	"\\StringFileInfo\\000004b0\\ProductName"
#define PVQUERYSTR	"\\StringFileInfo\\000004b0\\ProductVersion"
#define LCQUERYSTR	"\\StringFileInfo\\000004b0\\LegalCopyright"
#define LICENSEURL	"https://www.gnu.org/licenses/gpl-3.0.html"
#define LICENSESTR	\
"This program comes with ABSOLUTELY NO WARRANTY.\n\
This is free software, and you are welcome to redistribute it under certain conditions.\n\n\
Click \"License\" to consult the GNU General Public License version 3 (or any later) for details."



//Bitmap handles structure, these bitmaps are used to draw ResetButton.
typedef struct ResetButtonHBitmaps {
	HBITMAP normal;		//default bitmap
	HBITMAP click;		//show when click on map
	HBITMAP fail;		//show when game fail
	HBITMAP success;	//show when game success
	HBITMAP current;	//remember current using bitmap
} RBHBM, * PRBHBM;



/* position relative functions */

//Transform between pixel position on MapArea and GameMap index.
//Assume the whole Map's left-top as position 0.
int ppos2index(PGameInfo pGame, int px, int py);
int index2px(PGameInfo pGame, int index);
int index2py(PGameInfo pGame, int index);

//Check if a mouse position is inside the ResetButton area.
bool lparamIsInRB(LPARAM lparam, BYTE map_width);

//Check if a mouse position is inside the GameMap area.
bool lparamIsInMap(LPARAM laparm, BYTE map_width, BYTE map_height);

//Change a mouse position to GameMap index.
//The input should be the LPARAM of WM_MOUSEMOVE message or something similarly
//that can be unpacked using MAKEPOINTS() macro.
int lparam2index(PGameInfo pGame, LPARAM lparam);


/* manage bitmaps */

void loadBitmaps(PRBHBM pRBhbm, HINSTANCE hinst);
void freeBitmaps(PRBHBM pRBhbm);

//Change current ResetButton bitmap pointer to specified bitmap.
//NOTE: This function ONLY changes the pointer and do NOT affect the ResetButton.
void setCurrBitmap(PRBHBM pRBhbm, HBITMAP hbm);


/* UI functions */

//Paint specific numbers on Num Part with default color.
//If num is out of range, it draws '---'.
//(-100 < num < 1000)
void paintINums(HDC hdestdc, int left, int top, int num);


//Paint ResetButton with specified bitmap.
void paintResetButton(HDC hdestdc, int left, int top, HBITMAP hbm, bool clicked);


//Draw a MapUnit depends on MapUnitState with default color.
//Draw a covered MapUnit by default.
//w:MU_SIZE, h:MU_SIZE
//no DC-buffer
void paintDCMapUnit(HDC hdestdc, int left, int top, BYTE mapunit);

//Draw GameMap directly on DC.
//w:MAP_WIDTH, h:MAP_HEIGHT
//no DC-Buffer
void paintDCMap(HDC hdestdc, int left, int top, PGameInfo pGame);

//Draw a MapUnit depends on MapUnit data with default color.
//Draw a covered MapUnit by default.
void paintMapUnit(HDC hdestdc, int muleft, int mutop, BYTE mapunit);

//Paint GameMap, the left-top is position 0.
//This function redraws the whole MapArea content.
void paintMap(HDC hdestdc, int mapleft, int maptop, PGameInfo pGame);


//Show clicked state when a MapUnit is clicked down.
//It will do nothing if the index is out of GameMap range.
//NOTE: Use the whole Map's left and top position instead of a MapUnit's.
void showClickedMapUnit(HDC hdestdc, int mapleft, int maptop, PGameInfo pGame, int index);

//Show clicked state when a group of MapUnits are clicked down.
//It jumps MapUnit whose index is out of GameMap range.
//NOTE: Use the whole Map's left and top position instead of a MapUnit's.
void showClickedMapUnits(HDC hdestdc, int mapleft, int maptop, PGameInfo pGame, Neighbor indexes);


/* save(config) file management */

//Load infomation from a config file, including GameInfo, ScoreInfo and a POINT struct
//which contains left-top position where the window was last time.
//Use default settings to init if error.
void initGame(LPCTSTR Path, PGameInfo pGame, PGameScore pScore, PPOINT plastwndpos);

//Save infomation into a config file, including GameInfo, ScoreInfo and window position.
void saveGame(LPCTSTR Path, PGameInfo pGame, PGameScore pScore, PPOINT pwndpos);


/* get program version information */
void getProperty(LPTSTR property, size_t size_in_ch);
