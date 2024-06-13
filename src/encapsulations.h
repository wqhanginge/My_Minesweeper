/*****************************************************************************\
 *  My Minesweepper -- a classic minesweeper game
 *  Copyright (C) 2020-2024 Gee Wang
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
 * This file contains encapsulations of UI operations and IO functions.
 * This file contains a struct and relative functions for bitmap management.
 * This file also contians some useful transform functions.
 * 
 * NOTE: Most functions do NOT have arg check process, use with care.
\*****************************************************************************/


#pragma once

#include "gamecore.h"
#include "basicUI.h"



/* IO defines and game init defines */

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

//lang-codepage is 000004b0 (Language Neutral)
#define PNQUERYSTR      "\\StringFileInfo\\000004b0\\ProductName"
#define PVQUERYSTR      "\\StringFileInfo\\000004b0\\ProductVersion"
#define LCQUERYSTR      "\\StringFileInfo\\000004b0\\LegalCopyright"
#define MAX_PROPSTR     320
#define MAX_LICSTR      35840
#define LICRESTYPE      "TEXT"
#define LICNOTESTR      \
"This program comes with ABSOLUTELY NO WARRANTY.\n\
This is free software, and you are welcome to redistribute it under certain conditions.\n\n\
This program is licensed under the GNU GPL v3. For more details, check <License>."



//Bitmap handles structure, these bitmaps are used to draw ResetButton.
typedef struct _ResetButtonHBitmaps {
    HBITMAP normal;     //default bitmap
    HBITMAP click;      //show when click on window
    HBITMAP fail;       //show when game fail
    HBITMAP success;    //show when game success
    HBITMAP current;    //remember current using bitmap
} RBHBM, * PRBHBM;



/* position relative functions */

//Transform between pixel position on window and GameMap index.
//Assume the whole Map's left-top as position 0.
int ppos2index(PGameInfo pGame, int px, int py);
int index2px(PGameInfo pGame, int index);
int index2py(PGameInfo pGame, int index);

//Check if a mouse position is inside the ResetButton region.
bool isLparamInRB(LPARAM lparam, BYTE n_map_width);

//Check if a mouse position is inside the Map region.
bool isLparamInMap(LPARAM laparm, BYTE n_map_width, BYTE n_map_height);

//Change a mouse position to GameMap index.
//The input should be the LPARAM of WM_MOUSEMOVE message or something similarly
//that can be unpacked using MAKEPOINTS macro.
int lparam2index(PGameInfo pGame, LPARAM lparam);


/* manage bitmaps */

void loadBitmaps(PRBHBM pRBhbm, HINSTANCE hinst);
void freeBitmaps(PRBHBM pRBhbm);

//Change current ResetButton bitmap handle to specified bitmap.
//NOTE: This function ONLY changes the handle and do NOT affect the ResetButton.
void setCurrBitmap(PRBHBM pRBhbm, HBITMAP hbm);


/* UI functions */

//Paint the specific number for INums with default color.
//Available range:[INUMS_MIN, INUMS_MAX], otherwise, it draws symbol '---'.
void paintINums(HDC hdestdc, int left, int top, int num);


//Paint ResetButton with specified bitmap.
void paintResetButton(HDC hdestdc, int left, int top, HBITMAP hbm, bool clicked);


//Paint a MapUnit depends on MapUnitState with default color directly on DC.
//Paint a covered MapUnit by default.
//w:MUP_SIZE, h:MUP_SIZE
//no DC-buffer
void paintDCMapUnit(HDC hdestdc, int left, int top, BYTE mapunit);

//Paint GameMap directly on DC.
//Specify the draw rect, or NULL that representing the whole GameMap;
//the left and top are included while the right and bottom are excluded.
//w:MAP_WIDTH, h:MAP_HEIGHT
//no DC-Buffer
void paintDCMap(HDC hdestdc, int left, int top, PGameInfo pGame, PRECT pxyrect);

//Paint a MapUnit depends on MapUnit data with default color.
//Paint a covered MapUnit by default.
void paintMapUnit(HDC hdestdc, int left, int top, BYTE mapunit);

//Paint GameMap.
void paintMap(HDC hdestdc, int left, int top, PGameInfo pGame);


//Show selected state when a MapUnit or a group of MapUnits are clicked down.
//It will do nothing if the index is out of GameMap range(INV_INDEX).
//NOTE: Use the whole Map's left and top position instead of a MapUnit's.
void showSelectedMapUnit(HDC hdestdc, int mapleft, int maptop, PGameInfo pGame, int index, int last_index, bool area);


/* config(save) file management */

//Make a full path to the config file.
//Make sure the buffer has enough space.
void getConfPath(LPTSTR Path, DWORD size_ch);

//Load infomation from a config file.
//Load and init GameInfo and ScoreInfo, or init by default settings if load fail.
//Load last window position to POINT struct, or do nothing if load fail.
void loadGame(LPCTSTR Path, PGameInfo pGame, PGameScore pScore, PPOINT pwndpos);

//Save infomation into a config file, including GameInfo, ScoreInfo and window position.
void saveGame(LPCTSTR Path, PGameInfo pGame, PGameScore pScore, PPOINT pwndpos);


/* get program version information */
void loadProperty(LPTSTR prop, DWORD size_ch);

/* load license resource and convert its encoding */
void loadLicense(LPTSTR lic, DWORD size_ch);
