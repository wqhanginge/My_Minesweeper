/*****************************************************************************\
 *  My Minesweepper -- a classic minesweeper game
 *  Copyright (C) 2020 Gee Wang
 *
 *  This file is part of My Minesweeper.
 *
 *  My Minesweeper is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  My Minesweeper is distributed in the hope that it will be useful,
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
 * This file contains encapsulations of UI functions and IO operations.
 * This file contains a struct and related functions for bitmap management.
 * This file also contians some useful transform functions.
 *
 * NOTE: Most functions do NOT check arguments, use with care.
\*****************************************************************************/


#pragma once

#include "stdafx.h"
#include "gamecore.h"
#include "basicUI.h"



/* IO defines */

#define CONF_FNAME      "MyMinesweeper.ini"
#define CONF_PATHENV    "LOCALAPPDATA"
#define MAX_APPPATH     480
#define MAX_CONFPATH    MAX_APPPATH

//keywords in config file
#define CKEY_INIT_ANAME      "Init"
#define CKEY_INIT_XPOS       "xpos"
#define CKEY_INIT_YPOS       "ypos"
#define CKEY_INIT_MODE       "mode"
#define CKEY_INIT_WIDTH      "width"
#define CKEY_INIT_HEIGHT     "height"
#define CKEY_INIT_MINES      "mines"
#define CKEY_INIT_MARK       "mark"
#define CKEY_SCORE_ANAME     "Record"
#define CKEY_SCORE_JTIME     "junior_time"
#define CKEY_SCORE_MTIME     "middle_time"
#define CKEY_SCORE_STIME     "senior_time"
#define CKEY_SCORE_JNAME     "junior_name"
#define CKEY_SCORE_MNAME     "middle_name"
#define CKEY_SCORE_SNAME     "senior_name"
//end keywords


/* property related defines */

//StringFileInfo block is 000004b0 (Language Neutral)
#define QUERYBLOCKSTR   "\\StringFileInfo\\000004b0"
#define PNQUERYSTR      QUERYBLOCKSTR "\\ProductName"
#define PVQUERYSTR      QUERYBLOCKSTR "\\ProductVersion"
#define LCQUERYSTR      QUERYBLOCKSTR "\\LegalCopyright"
#define MAX_PROPSTR     320
#define MAX_LICSTR      35840
#define LICRESTYPE      "TEXT"
#define LICNOTESTR      "\
This program comes with ABSOLUTELY NO WARRANTY.\n\
This is free software, and you are welcome to redistribute it under certain conditions.\n\n\
This program is licensed under the GNU GPL v3. For more details, check <License>."



//Structure for bitmap handles, these bitmaps are used to draw ResetButton.
typedef struct _ResetButtonHBitmaps {
    HBITMAP normal;     //show at default state
    HBITMAP click;      //show when click on window
    HBITMAP win;        //show when win the game
    HBITMAP loss;       //show when lose the game
    HBITMAP current;    //remember current using bitmap
} RBHBM, * PRBHBM;



/* position related functions */

//Transform between pixel position and GameMap index.
//Assume the top-left of the whole Map is position (0, 0).
int ppos2index(PGameInfo pGame, int px, int py);
int index2px(PGameInfo pGame, int index);
int index2py(PGameInfo pGame, int index);

//Check if a mouse position is inside the ResetButton region.
bool isLparamInRB(LPARAM lparam, BYTE n_map_width);

//Check if a mouse position is inside the Map region.
bool isLparamInMap(LPARAM laparm, BYTE n_map_width, BYTE n_map_height);

//Convert a mouse position to GameMap index.
//The input should be the LPARAM of WM_MOUSEMOVE message or something similarly
//that can be unpacked using MAKEPOINTS macro.
int lparam2index(PGameInfo pGame, LPARAM lparam);


/* bitmap management functions */

void loadBitmaps(PRBHBM pRBhbm, HINSTANCE hinst);
void freeBitmaps(PRBHBM pRBhbm);

//Change current ResetButton bitmap handle to specified bitmap.
//NOTE: This function ONLY changes the handle and does NOT update the window.
void setCurrBitmap(PRBHBM pRBhbm, HBITMAP hbm);


/* UI functions */

//Paint the specific number for INums with default color.
//Available range:[INUMS_MIN, INUMS_MAX], otherwise, it draws symbol '---'.
void paintINums(HDC hdstdc, int left, int top, int num);


//Paint ResetButton with specified bitmap.
void paintResetButton(HDC hdstdc, int left, int top, HBITMAP hbm, bool clicked);


//Paint a MapUnit depends on MapUnitState with default color directly on DC.
//Paint a covered MapUnit by default.
//w:MUP_SIZE, h:MUP_SIZE
//no DC buffer
void paintDCMapUnit(HDC hdstdc, int left, int top, BYTE mapunit);

//Paint GameMap directly on DC.
//Specify the draw rect in unit (not pixel), or NULL to represent the whole GameMap;
//the left and top are included while the right and bottom are excluded.
//w:MAP_WIDTH, h:MAP_HEIGHT
//no DC buffer
void paintDCMap(HDC hdstdc, int left, int top, PGameInfo pGame, PRECT pxyrect);

//Paint a MapUnit depends on its content with default color.
//Paint a covered MapUnit by default.
void paintMapUnit(HDC hdstdc, int left, int top, BYTE mapunit);

//Paint GameMap.
void paintMap(HDC hdstdc, int left, int top, PGameInfo pGame);


//Show selected state when a MapUnit or a group of MapUnits are clicked down.
//Do nothing if the index is out of GameMap range(INV_INDEX).
//NOTE: Use the left and top position of the whole Map rather than a MapUnit.
void showSelectedMapUnit(HDC hdstdc, int mapleft, int maptop, PGameInfo pGame, int index, int last_index, bool area);


/* config(save) file management */

//Make a full path to the config file.
//Make sure the buffer has enough space.
void getConfPath(LPTSTR path, DWORD size_ch);

//Load settings from a config file.
//Load and init GameInfo and ScoreInfo, or init with default settings if the load fails.
//Load last window position into POINT struct, which remains unchanged if the load fails.
void loadGame(LPCTSTR path, PGameInfo pGame, PGameScore pScore, PPOINT pwndpos);

//Save settings into a config file, including GameInfo, ScoreInfo and window position.
void saveGame(LPCTSTR path, PGameInfo pGame, PGameScore pScore, PPOINT pwndpos);


/* property related functions */

//Get program version information.
void loadProperty(LPTSTR prop, DWORD size_ch);

//Load license resource and convert its encoding.
void loadLicense(LPTSTR lic, DWORD size_ch);
