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
 * procfunctions.h
 *****************************************************************************
 * This file contains definations for Window and Message Queue.
 * This file contains Win32 Window Procedure Functions and Message Processing
 * Functions.
 *
 * NOTE: Most functions do NOT check arguments, use with care.
\*****************************************************************************/


#pragma once

#include "stdafx.h"
#include "encapsulations.h"



/* private window messages for Game */

#define WMAPP_GAMERESET         (WM_APP + 0)    /* wParam: not used, lPapam: not used */
#define WMAPP_GAMEWIN           (WM_APP + 1)    /* wParam: not used, lPapam: not used */
#define WMAPP_GAMELOSS          (WM_APP + 2)    /* wParam: not used, lPapam: not used */
#define WMAPP_GAMEMODECHANGE    (WM_APP + 3)    /* wParam: GameMode, lPapam: GameMapInfo (optional if ISSTDMOD(GameMode)) */

//GameMapInfo (or MapInfo) is a combination of the width, height and mines of GameMap.
//Use the following macros to pack or unpack GameMapInfo to or from a lparam.

#define MAKEMAPINFO(w, h, m)    ((LPARAM)((BYTE)(w) | ((WORD)((BYTE)(h)) << 8) | ((DWORD)((WORD)(m)) << 16)))
#define GETMAPINFOWIDTH(l)      ((BYTE)(l))
#define GETMAPINFOHEIGHT(l)     ((BYTE)((WORD)(l) >> 8))
#define GETMAPINFOMINES(l)      ((WORD)((DWORD)(l) >> 16))


/* miscellaneous defines */

#define WNDCLS_NAME         "MYMINESWEEPER"
#define WND_NAME            "My Minesweeper"
#define WND_TIMER_ID        1
#define WND_TIMER_ELAPSE    1000
#define DLG_TIMEUNIT_EN     "Sec"



/* private global variables
/*****************************************************************************
GameInfo Game;      //Game information
GameScore Score;    //Record information
RBHBM RBhbm;        //bitmap handles for ResetButton

bool last_sclick;   //if last mouse event was a simultaneous button click
bool rb_capture;    //if ResetButton get the capture
**/



/* commonly used functions */

//Change checked state of GameMode items in Menu, do nothing if GameMode is invalid.
void setMenuChecked(HMENU hmenu, BYTE mode);

//Check or uncheck QuestionMark item in Menu.
void setQMarkChecked(HMENU hmenu, bool mark);

//Update the information on Record Dialog with current Score data.
void updateRecordContent(HWND hrecord);

//Update the contents of Edits in Custom Dialog with specified MapInfo.
void updateCustomContent(HWND hcustom, LPARAM mapinfo);

//Get the contents of Edits in Custom Dialog.
LPARAM getCustomContent(HWND hcustom);


/* Window Porcedure Functions */

//About Dialog, show program description.
INT_PTR CALLBACK AboutProc(HWND habout, UINT msg, WPARAM wparam, LPARAM lparam);

//License Dialog, show license content.
INT_PTR CALLBACK LicenseProc(HWND hlicense, UINT msg, WPARAM wparam, LPARAM lparam);

//Record Dialog, show records.
INT_PTR CALLBACK RecordProc(HWND hrecord, UINT msg, WPARAM wparam, LPARAM lparam);

//GetName Dialog, provide an Edit Box to acquire the player name after breaking a record.
INT_PTR CALLBACK GetNameProc(HWND hgetname, UINT msg, WPARAM wparam, LPARAM lparam);

//Custom Dialog, show and acquire the MapInfo, then return the new customized MapInfo.
//Pass the MapInfo through the DialogBoxParam() function.
INT_PTR CALLBACK CustomProc(HWND hcustom, UINT msg, WPARAM wparam, LPARAM lparam);


/* Message Processing Functions */

//Response menu message.
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

//WMAPP_GAMEWIN
LRESULT onGameWin(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WMAPP_GAMELOSS
LRESULT onGameLoss(HWND hwnd, WPARAM wparam, LPARAM lparam);

//WMAPP_GAMEMODECHANGE
LRESULT onGameModeChange(HWND hwnd, WPARAM wparam, LPARAM lparam);

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
