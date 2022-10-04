/*****************************************************************************\
 *  My Minesweepper -- a classic minesweeper game
 *  Copyright (C) 2020-2022 Gee W.
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
 * this file contains encapsulations of UI operations and IO functions
 * this file also contians some useful functions
 * 
 * this file contains several HBITMAP global variables which can be accessed
 * by external functions, they are used for drawing ResetButton,
 * DO NOT rewrite them directly, but use predefined functions(IMPORTANT)
 * 
 * NOTE:most functions do not have arg check process, use with care
\*****************************************************************************/


#pragma once

#include "stdincludes.h"
#include "basicUI.h"



/* IO defines and game init defines */

#define DEF_WND_LEFT	128
#define DEF_WND_TOP		128
#define DEF_WND_WIDTH	750
#define DEF_WND_HEIGHT	700
#define DEF_CONFNAME	"MyMinesweeper.ini"
#define DEF_CONFPATHENV	"LOCALAPPDATA"
#define STRBUFFERLEN	5

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

/* property related defines */

//lang-codepage is 000004b0 (Language Neutral)
#define PNQUERYSTR		"\\StringFileInfo\\000004b0\\ProductName"
#define PVQUERYSTR		"\\StringFileInfo\\000004b0\\ProductVersion"
#define LCQUERYSTR		"\\StringFileInfo\\000004b0\\LegalCopyright"
#define COPYRIGHTSTR	\
"This program comes with ABSOLUTELY NO WARRANTY.\n\
This is free software, and you are welcome to redistribute it under certain conditions.\n\n\
Check <https://www.gnu.org/licenses/> and view GNU General Public License v3.0 for details."



//bitmap handles structure, these bitmaps are used to draw Reset Button
typedef struct ResetButtonHBitmaps {
	HBITMAP def;		//default bitmap
	HBITMAP click;		//show when click on map
	HBITMAP fail;		//show when game fail
	HBITMAP success;	//show when game success
	HBITMAP current;	//remember current using bitmap
} RBHBM;



//bitmap handles, point to bitmaps which will be drawn on Reset Button
//DO NOT write it directly, use loadBitmaps(), freeBitmaps() and setRBBitmap()
extern RBHBM RBhbm;


//check if a mouse position is inside the ResetButton area
bool lparamIsInRB(LPARAM lparam);

//check if a mouse position is inside the Map area
bool lparamIsInMap(LPARAM laparm);

//change a mouse position to map index
//the input should be the LPARAM of WM_MOUSEMOVE message or something similarly
//i.e. it can use MAKEPOINTS() macro to unpack
int lparam2index(LPARAM lparam);



//manage bitmaps
void loadBitmaps(HINSTANCE hinst);
void freeBitmaps();



/* UI functions */

//paint specific numbers on Num Part with default color
//if num is out of range, it draws '---'
//-100 < num < 1000
void paintINums(HDC hdestdc, int left, int top, int num);


//paint ResetButton without changing its bitmap
void paintResetButton(HDC hdestdc, int left, int top, bool clicked);

//change bitmap on ResetButton
//NOTE:you need to redraw ResetButton after calling this function
void setRBBitmap(HBITMAP hbm);


//draw a mapunit depends on MapUnit data with default color
//draw a covered mapunit by default
void paintMapUnit(HDC hdestdc, int muleft, int mutop, int index);

//paint GameMap, the left-top is position 0
//it redraws the whole MapArea content
void paintMap(HDC hdestdc, int mapleft, int maptop);


//show clicked state when a MapUnit is clicked down
//it will do nothing if the index is out of GameMap range
//NOTE: use the whole Map's left and top position instead of a MapUnit's
void showClickedMapUnit(HDC hdestdc, int mapleft, int maptop, int index);

//show clicked state when a group of MapUnits are clicked down
//it jumps MapUnit whose index is out of GameMap range
//NOTE: use the whole Map's left and top position instead of a MapUnit's
void showClickedMapUnits(HDC hdestdc, int mapleft, int maptop, Neighbor* pindexes);



/* save/config file management */

//load infomation from a config file
//return a POINT struct which contains left-top position where the window was
//use default setting to init Game if error
void initGame(TCHAR* Path, POINT* plastwndpos);

//save Game infomation into a config file
//save the current window position as well
void saveGame(TCHAR* Path, POINT* pwndpos);



/* get program version information */
void getProperty(TCHAR* property, size_t size_in_ch);
