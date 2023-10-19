/*****************************************************************************\
 *  My Minesweepper -- a classic minesweeper game
 *  Copyright (C) 2020-2023 Gee Wang
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
 * procfunctions.h
 *****************************************************************************
 * This file contains definations for Window and Message Queue.
 * This file contains Win32 Window Proc Functions and Message Processing
 * Functions.
 * 
 * NOTE: Most functions have NO arg check, use with care.
\*****************************************************************************/


#pragma once

#include "encapsulations.h"



/* Game's private window messages */

#define WMAPP_GAMERESET     (WM_APP + 0)        //send when game needs reset
#define WMAPP_GAMESUCCESS   (WM_APP + 1)        //send when game is succeed
#define WMAPP_GAMEFAIL      (WM_APP + 2)        //send when game is failed

//send when game mode is changed,
//use wparam as new GameMode,
//use lparam as a combination of new width, height and mines
//ignore lparam if new GameMode is a standard Mode(not CUSTOM)
//call MAKECHGLAPRAM to create a lparam
//call GETCHG*** family to unpack a lparam
#define WMAPP_GAMEMODECHG   (WM_APP + 3)

//create a lparam by width, height and mines
#define MAKECHGLPARAM(w, h, m)  ((LPARAM)((((DWORD)(w) & 0xFF) | (((DWORD)(h) & 0xFF) << 8)) | (((DWORD)(m) & 0xFFFF) << 16)))

//unpack infomation from a lparam
#define GETCHGWIDTH(l)          ((BYTE)((DWORD)(l) & 0xFF))
#define GETCHGHEIGHT(l)         ((BYTE)(((DWORD)(l) >> 8) & 0xFF))
#define GETCHGMINES(l)          ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))


/* Dialog defines */

//Record Dialog
#define TIMEUNIT_EN     "Sec"
//end Record Dialog


/* miscellaneous defines */

#define WNDCLS_NAME         "MYMINESWEEPER"
#define WND_NAME            "My Minesweeper"
#define GAME_TIMER_ID       1
#define GAME_TIMER_ELAPSE   1000



/* following arguments are private, they can not be seen external */
/*
GameInfo Game;      //Game information
GameScore Score;    //Record information
RBHBM RBhbm;        //bitmap handles, point to bitmaps which will be drawn on ResetButton

bool last_dbclick;  //indicate if last mouse event was a double button click
bool rb_capture;    //indicate if ResetButton get the capture
*/



/* sub functions for specific use */

//Change GameMode checked in Menu, do nothing if GameMode is illegal.
void setMenuChecked(HMENU hmenu, BYTE GameMode);

//Check or uncheck QuestionMark in Menu.
void setQMarkChecked(HMENU hmenu, bool Mark);

//update the information on Record Dialog with current score data
void updateRecordContent(HWND hrecord);

//update the contents in Edits of Custom Dialog with specified data
void updateCustomContent(HWND hcustom, LPARAM whm);

//get the contents in Edits of Custom Dialog
LPARAM getCustomContent(HWND hcustom);


/* Window Porc Functions */

//About Dialog, show program description on child modal window
INT_PTR CALLBACK AboutProc(HWND habout, UINT msg, WPARAM wparam, LPARAM lparam);

//License Dialog, show license content on child modal window
INT_PTR CALLBACK LicenseProc(HWND hlicense, UINT msg, WPARAM wparam, LPARAM lparam);

//Record Dialog, show record on child modal window
INT_PTR CALLBACK RecordProc(HWND hrecord, UINT msg, WPARAM wparam, LPARAM lparam);

//GetName Dialog, provide an edit box to get the player name after breaking Record
INT_PTR CALLBACK GetNameProc(HWND hgetname, UINT msg, WPARAM wparam, LPARAM lparam);

//Custom Dialog, get customized GameMap information and return it through the EndDialog function
//provide the dufault GameMap information through the DialogBoxParam function
INT_PTR CALLBACK CustomProc(HWND hcustom, UINT msg, WPARAM wparam, LPARAM lparam);


/* following functions are encapsulations of operations in WndProc */

//response menu message
LRESULT onMenu(HWND hwnd, UINT miid);

//WM_CREATE
LRESULT onCreate(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WM_DESTROY
LRESULT onDestroy(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WM_PAINT
LRESULT onPaint(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WM_COMMAND
LRESULT onCommand(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WMAPP_GAMERESET
LRESULT onGameReset(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WMAPP_GAMEFAIL
LRESULT onGameFail(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WMAPP_GAMESUCCESS
LRESULT onGameSuccess(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WMAPP_GAMEMODECHG
LRESULT onGameModeChg(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WM_LBUTTONDOWN
LRESULT onLButtonDwon(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WM_LBUTTONUP
LRESULT onLButtonUp(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WM_RBUTTONDOWN
LRESULT onRButtonDown(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WM_RBUTTONUP
LRESULT onRButtonUp(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WM_MOUSEMOVE
LRESULT onMouseMove(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WM_TIMER
LRESULT onTimer(HWND hwnd, WPARAM wparam, LPARAM lparam);
