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
 * procfunctions.h
 *****************************************************************************
 * This file contains definations for Window and Message Queue.
 * This file contains Win32 Window Proc Functions and Message Processing
 * Functions.
 * 
 * NOTE: Most functions have NO arg check, use with care.
\*****************************************************************************/


#pragma once

#include "stdincludes.h"
#include "encapsulations.h"



/* Game's private window messages */

#define WMAPP_GAMERESET		(WM_APP + 0)		//send when game needs reset
#define WMAPP_GAMESUCCESS	(WM_APP + 1)		//send when game is succeed
#define WMAPP_GAMEFAIL		(WM_APP + 2)		//send when game is failed

/* send when game mode is changed,
 * use wparam as new GameMode,
 * use lparam as a combination of new width, height and mines
 * ignore lparam if new GameMode is a standard Mode(not CUSTOM)
 * call MAKECHGLAPRAM to create a lparam
 * call GETCHG*** family to unpack a lparam
 */
#define WMAPP_GAMEMODECHG	(WM_APP + 3)

//create a lparam by width, height and mines
#define MAKECHGLPARAM(w, h, m)	((LPARAM)((((DWORD)(w) & 0xFF) | (((DWORD)(h) & 0xFF) << 8)) | (((DWORD)(m) & 0xFFFF) << 16)))

//unpack infomation from a lparam
#define GETCHGWIDTH(l)			((BYTE)((DWORD)(l) & 0xFF))
#define GETCHGHEIGHT(l)			((BYTE)(((DWORD)(l) >> 8) & 0xFF))
#define GETCHGMINES(l)			((WORD)(((DWORD)(l) >> 16) & 0xFFFF))


/* Dialog defines */

//Custom Dialog
#define CUSTOM_EDIT_LEN		8
//end Custom Dialog

//GetName Dialog
#define NAME_EDIT_LEN		SCORE_NAME_LEN
//end GetName Dialog

//Record Dialog
#define TIME_STRBUFFERLEN	10
#define DEF_TIMEUNIT_EN		"Sec"
//end Record Dialog

//About Dialog
#define ABOUT_INFO_LEN		320
//end About Dialog


/* miscellaneous defines */

#define APP_NAME			"My Minesweeper"
#define WNDC_NAME			"My_Minesweeper"
#define GAME_TIMER_ID		1
#define GAME_TIMER_ELAPSE	1000



/* following arguments are private, they can not be seen external */
/*
GameInfo Game;		//Game information
GameScore Score;	//Record information
RBHBM RBhbm;		//bitmap handles, point to bitmaps which will be drawn on Reset Button

TCHAR conf_path[MAX_CONFPATH];	//config file path
bool last_dbclick;				//indicate if last mouse event was a double click
bool rb_capture;				//indicate if ResetButton get the capture
*/


/* Followings are global handles for program and window,
 * they are defined in main.c but need to be accessed in this file.
 */

extern HINSTANCE hInst;	//program instance handle
extern HWND hWnd;		//main window handle
extern HMENU hMenu;		//main menu handle



/* change checked position in Menu */

//Change GameMode checked in Menu, do nothing if GameMode is illegal.
void setMenuChecked(BYTE GameMode);

//Check or uncheck QuestionMark in Menu.
void setQMarkChecked(bool Mark);


/* Window Porc Functions */

//About Dialog, show program description on child modal window
INT_PTR CALLBACK AboutProc(HWND habout, UINT msg, WPARAM wparam, LPARAM lparam);

//Record Dialog, show record on child modal window
INT_PTR CALLBACK RecordProc(HWND hrecord, UINT msg, WPARAM wparam, LPARAM lparam);

//GetName Dialog, provide an edit box to get Record Name after breaking Record
INT_PTR CALLBACK GetNameProc(HWND hgetname, UINT msg, WPARAM wparam, LPARAM lparam);

//Custom Dialog, use to customize Game Map
INT_PTR CALLBACK CustomProc(HWND hcustom, UINT msg, WPARAM wparam, LPARAM lparam);


/* following functions are encapsulations of operations in WndProc */

//response menu message
LRESULT onMenu(WPARAM wparam);

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
