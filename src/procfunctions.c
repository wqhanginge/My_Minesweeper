/*****************************************************************************\
 *  My Minesweepper -- a classic minesweeper game
 *  Copyright (C) 2020-2022  Gee W.
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
 * procfunctions.c
 *****************************************************************************
 * this file contains Win32 Window Proc Functions
 * each function is aim to handle a particular Window Message or is a Child
 * Window's Procedure Function
 * 
 * NOTE:most functions have no arg check, use with care
\*****************************************************************************/


#include "procfunctions.h"

/* private global variables */

TCHAR conf_path[MAX_PATH];	//conf file path
bool last_dbclick;			//indicate if last mouse event was a double click
bool rb_capture;			//indicate if ResetButton get the capture


//change GameMode checked in Menu, do nothing if GameMode is illegal
void setMenuChecked(byte GameMode)
{
	MENUITEMINFO mii = { sizeof(MENUITEMINFO) };

	switch (GameMode) {
	case JUNIOR:
		mii.fMask = MIIM_STATE;
		mii.fState = MFS_CHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_JUNIOR, FALSE, &mii);
		mii.fState = MFS_UNCHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_MIDDLE, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_SENIOR, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_CUSTOM, FALSE, &mii);
		break;
	case MIDDLE:
		mii.fMask = MIIM_STATE;
		mii.fState = MFS_CHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_MIDDLE, FALSE, &mii);
		mii.fState = MFS_UNCHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_JUNIOR, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_SENIOR, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_CUSTOM, FALSE, &mii);
		break;
	case SENIOR:
		mii.fMask = MIIM_STATE;
		mii.fState = MFS_CHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_SENIOR, FALSE, &mii);
		mii.fState = MFS_UNCHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_JUNIOR, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_MIDDLE, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_CUSTOM, FALSE, &mii);
		break;
	case CUSTOM:
		mii.fMask = MIIM_STATE;
		mii.fState = MFS_CHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_CUSTOM, FALSE, &mii);
		mii.fState = MFS_UNCHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_JUNIOR, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_MIDDLE, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_SENIOR, FALSE, &mii);
		break;
	default:
		break;
	}
}

//check Question Mark in Menu if Mark is true
void setQMarkChecked(bool Mark)
{
	MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
	mii.fMask = MIIM_STATE;
	mii.fState = (Mark) ? MFS_CHECKED : MFS_UNCHECKED;
	SetMenuItemInfo(hMenu, ID_GAME_MARK, FALSE, &mii);
}



/* Proc Functions */

//About Dialog, show program description on child modal window
INT_PTR CALLBACK AboutProc(HWND habout, UINT msg, WPARAM wparam, LPARAM lparam)
{
	HWND htext;
	TCHAR aboutinfo[ABOUT_INFO_LEN];

	switch (msg) {
	case WM_INITDIALOG:
		//get text handel
		htext = FindWindowEx(habout, NULL, TEXT("STATIC"), NULL);
		
		//show program description
		getProperty(aboutinfo, ABOUT_INFO_LEN);
		SetWindowText(htext, aboutinfo);
		break;
	case WM_CLOSE:
		EndDialog(habout, 0);
		break;
	case WM_COMMAND:
		if (LOWORD(wparam) == IDOK) EndDialog(habout, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

//Record Dialog, show record on child modal window
INT_PTR CALLBACK RecordProc(HWND hrecord, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static HWND hjt, hmt, hst, hjn, hmn, hsn;
	TCHAR strbuffer[TIME_STRBUFFERLEN];

	switch (msg) {
	case WM_INITDIALOG:
		//get static handels
		hjt = FindWindowEx(hrecord, NULL, TEXT("STATIC"), NULL);
		hmt = FindWindowEx(hrecord, hjt, TEXT("STATIC"), NULL);
		hst = FindWindowEx(hrecord, hmt, TEXT("STATIC"), NULL);
		hjn = FindWindowEx(hrecord, hst, TEXT("STATIC"), NULL);
		hmn = FindWindowEx(hrecord, hjn, TEXT("STATIC"), NULL);
		hsn = FindWindowEx(hrecord, hmn, TEXT("STATIC"), NULL);

		//init static control's content
		_sntprintf_s(strbuffer, TIME_STRBUFFERLEN, _TRUNCATE, TEXT("%d %s"), Score.junior_time, TEXT(DEF_TIMEUNIT_EN));
		SetWindowText(hjt, strbuffer);
		_sntprintf_s(strbuffer, TIME_STRBUFFERLEN, _TRUNCATE, TEXT("%d %s"), Score.middle_time, TEXT(DEF_TIMEUNIT_EN));
		SetWindowText(hmt, strbuffer);
		_sntprintf_s(strbuffer, TIME_STRBUFFERLEN, _TRUNCATE, TEXT("%d %s"), Score.senior_time, TEXT(DEF_TIMEUNIT_EN));
		SetWindowText(hst, strbuffer);
		SetWindowText(hjn, Score.junior_name);
		SetWindowText(hmn, Score.middle_name);
		SetWindowText(hsn, Score.senior_name);
		break;
	case WM_CLOSE:
		EndDialog(hrecord, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDRESET:
			//reset the record
			resetRecord();
			_sntprintf_s(strbuffer, TIME_STRBUFFERLEN, _TRUNCATE, TEXT("%d %s"), MAX_TIME, TEXT(DEF_TIMEUNIT_EN));
			SetWindowText(hjt, strbuffer);
			SetWindowText(hmt, strbuffer);
			SetWindowText(hst, strbuffer);
			SetWindowText(hjn, TEXT(DEF_SCORE_NAME_EN));
			SetWindowText(hmn, TEXT(DEF_SCORE_NAME_EN));
			SetWindowText(hsn, TEXT(DEF_SCORE_NAME_EN));
			break;
		case IDOK:
			EndDialog(hrecord, 0);
			break;
		default:
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

//GetName Dialog, provide an edit box to get Record Name after breaking Record
INT_PTR CALLBACK GetNameProc(HWND hgetname, UINT msg, WPARAM wparam, LPARAM lparam) {
	static HWND heditname;

	switch (msg) {
	case WM_INITDIALOG:
		//get edit control handle and init default presentation
		heditname = FindWindowEx(hgetname, NULL, TEXT("EDIT"), NULL);
		SetWindowText(heditname, getpRecordName(Game.mode));
		SendMessage(heditname, EM_LIMITTEXT, NAME_EDIT_LEN - 1, 0);
		SendMessage(heditname, EM_SETSEL, 0, -1);
		SetFocus(heditname);
		break;
	case WM_DESTROY:
		//get what writen in the edit control when exit dialog
		GetWindowText(heditname, getpRecordName(Game.mode), NAME_EDIT_LEN);
		break;
	case WM_COMMAND:
		if (LOWORD(wparam) == IDC_OK) EndDialog(hgetname, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

//Custom Dialog, use to customize Game Map
INT_PTR CALLBACK CustomProc(HWND hcustom, UINT msg, WPARAM wparam, LPARAM lparam) {
	static HWND heditw, hedith, heditm;
	TCHAR str[CUSTOM_EDIT_LEN];
	static dword width, height, mines;

	switch (msg) {
	case WM_INITDIALOG:
		//get edit control handle
		heditw = FindWindowEx(hcustom, NULL, TEXT("EDIT"), NULL);
		hedith = FindWindowEx(hcustom, heditw, TEXT("EDIT"), NULL);
		heditm = FindWindowEx(hcustom, hedith, TEXT("EDIT"), NULL);

		//copy to buffer
		width = Game.width;
		height = Game.height;
		mines = Game.mines;

		//init edit control show
		_sntprintf_s(str, CUSTOM_EDIT_LEN, _TRUNCATE, TEXT("%u"), width);
		SetWindowText(heditw, str);
		_sntprintf_s(str, CUSTOM_EDIT_LEN, _TRUNCATE, TEXT("%u"), height);
		SetWindowText(hedith, str);
		_sntprintf_s(str, CUSTOM_EDIT_LEN, _TRUNCATE, TEXT("%u"), mines);
		SetWindowText(heditm, str);
		break;
	case WM_CLOSE:
		EndDialog(hcustom, 0);
		break;
	case WM_DESTROY:
		//set game mode when exit dialog
		PostMessage(hWnd, WMAPP_GAMEMODECHG, CUSTOM, MAKECHGLPARAM(width, height, mines));
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDOK:
			//get what in edit control when click OK
			GetWindowText(heditw, str, CUSTOM_EDIT_LEN);
			width = (dword)_ttoi(str);
			GetWindowText(hedith, str, CUSTOM_EDIT_LEN);
			height = (dword)_ttoi(str);
			GetWindowText(heditm, str, CUSTOM_EDIT_LEN);
			mines = (dword)_ttoi(str);
			EndDialog(hcustom, 0);
			break;
		case IDCANCEL:
			EndDialog(hcustom, 0);
			break;
		default:
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}


/* message response functions */

LRESULT onMenu(WPARAM wparam) {
	MENUITEMINFO mii = { sizeof(MENUITEMINFO) };

	switch (LOWORD(wparam)) {
	case ID_GAME_START:
		PostMessage(hWnd, WMAPP_GAMERESET, 0, 0);
		break;
	case ID_GAME_JUNIOR:
		//change game mode
		setMenuChecked(JUNIOR);
		PostMessage(hWnd, WMAPP_GAMEMODECHG, JUNIOR, 0);
		break;
	case ID_GAME_MIDDLE:
		setMenuChecked(MIDDLE);
		PostMessage(hWnd, WMAPP_GAMEMODECHG, MIDDLE, 0);
		break;
	case ID_GAME_SENIOR:
		setMenuChecked(SENIOR);
		PostMessage(hWnd, WMAPP_GAMEMODECHG, SENIOR, 0);
		break;
	case ID_GAME_CUSTOM:
		setMenuChecked(CUSTOM);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_CUSTOM), hWnd, CustomProc);
		break;
	case ID_GAME_MARK:
		//enable/disable Question Mark mode
		setMark(!Game.mark);
		setQMarkChecked(Game.mark);
		break;
	case ID_GAME_RECORD:
		//show records
		DialogBox(hInst, MAKEINTRESOURCE(IDD_RECORD), hWnd, RecordProc);
		break;
	case ID_GAME_EXIT:
		PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	case ID_ABOUT:
		//show about infomation
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, AboutProc);
		break;
	default:
		break;
	}
	return 0;
}

LRESULT onCreate(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	POINT wndpos;
	RECT wndrect, cltrect;
	int edgew, edgeh;

	//load bitmaps
	loadBitmaps(hInst);

	//init game from file
	GetEnvironmentVariable(TEXT(DEF_CONFPATHENV), conf_path, MAX_PATH);
	_tcscat_s(conf_path, MAX_PATH, TEXT("\\"));
	_tcscat_s(conf_path, MAX_PATH, TEXT(DEF_CONFNAME));
	initGame(conf_path, &wndpos);
	srand((dword)time(nullptr));

	//init menu info
	setMenuChecked(Game.mode);
	setQMarkChecked(Game.mark);

	//adjust window size
	GetWindowRect(hwnd, &wndrect);
	GetClientRect(hwnd, &cltrect);
	edgew = (wndrect.right - wndrect.left) - cltrect.right;
	edgeh = (wndrect.bottom - wndrect.top) - cltrect.bottom;
	wndrect = (RECT){ wndpos.x,wndpos.y,wndpos.x + CLIENT_WIDTH + edgew,wndpos.y + CLIENT_HEIGHT + edgeh };
	MoveWindow(hwnd, wndrect.left, wndrect.top, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, FALSE);

	//init double click state and capture state
	last_dbclick = false;
	rb_capture = false;
	return 0;
}

LRESULT onDestroy(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	POINT wndpos;
	RECT wndrect;

	//release bitmaps
	freeBitmaps();
	//save game info
	GetWindowRect(hwnd, &wndrect);
	wndpos = (POINT){ wndrect.left,wndrect.top };
	saveGame(conf_path, &wndpos);

	PostQuitMessage(0);
	return 0;
}

LRESULT onPaint(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	//redraw the whole client area
	PAINTSTRUCT ps;
	HDC hpaintdc = BeginPaint(hwnd, &ps);

	HDC hdcbuffer = CreateCompatibleDC(hpaintdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hpaintdc, CLIENT_WIDTH, CLIENT_HEIGHT);
	SelectObject(hdcbuffer, hbmbuffer);

	drawDCClientBg(hdcbuffer, 0, 0);
	drawDCInfoBg(hdcbuffer, INFO_LEFT, INFO_TOP);
	drawDCMapAreaBg(hdcbuffer, MAPAREA_LEFT, MAPAREA_TOP);
	drawDCNumBg(hdcbuffer, MINE_LEFT, MINE_TOP);
	drawDCNumBg(hdcbuffer, TIME_LEFT, TIME_TOP);
	drawDCINums(hdcbuffer, MNUMS_LEFT, MNUMS_TOP, Game.mine_remains);
	drawDCINums(hdcbuffer, TNUMS_LEFT, TNUMS_TOP, Game.time);
	drawDCResetButton(hdcbuffer, RB_LEFT, RB_TOP, hbm_current, false);
	drawDCMap(hdcbuffer, MAP_LEFT, MAP_TOP, Game.map, Game.size);

	BitBlt(hpaintdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, hdcbuffer, 0, 0, SRCCOPY);
	DeleteDC(hdcbuffer);
	DeleteObject(hbmbuffer);

	EndPaint(hwnd, &ps);
	return 0;
}

LRESULT onCommand(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	if (lparam == 0 && HIWORD(wparam) == 0)
		return onMenu(wparam);
	return 0;
}

LRESULT onGameReset(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	KillTimer(hwnd, GAME_TIMER_ID);
	resetGame();
	last_dbclick = false;
	setRBBitmap(hbm_rb);
	InvalidateRect(hwnd, NULL, FALSE);
	return 0;
}

LRESULT onGameStart(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	int ret = startGame(index2x((int)lparam), index2y((int)lparam));
	paintMap(hdc, MAP_LEFT, MAP_TOP, true);
	paintINums(hdc, MNUMS_LEFT, MNUMS_TOP, Game.mine_remains);
	paintINums(hdc, TNUMS_LEFT, TNUMS_TOP, Game.time);
	switch (ret) {
	case 1:
		PostMessage(hwnd, WMAPP_GAMESUCCESS, 0, 0);
		break;
	case -1:
		PostMessage(hwnd, WMAPP_GAMEFAIL, 0, 0);
		break;
	case 0:
		SetTimer(hwnd, GAME_TIMER_ID, GAME_TIMER_ELAPSE, NULL);
		break;
	default:
		break;
	}
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onGameFail(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	KillTimer(hwnd, GAME_TIMER_ID);
	finishGame();
	setRBBitmap(hbm_fail);
	paintResetButton(hdc, RB_LEFT, RB_TOP, false);
	paintMap(hdc, MAP_LEFT, MAP_TOP, false);
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onGameSuccess(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	KillTimer(hwnd, GAME_TIMER_ID);
	int ret = finishGame();
	setRBBitmap(hbm_success);
	paintResetButton(hdc, RB_LEFT, RB_TOP, false);
	paintINums(hdc, MNUMS_LEFT, MNUMS_TOP, 0);
	paintMap(hdc, MAP_LEFT, MAP_TOP, false);

	//if break record
	if (ret) {
		setRecordTime(Game.mode, Game.time);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_GETNAME), hwnd, GetNameProc);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_RECORD), hwnd, RecordProc);
	}
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onGameModeChg(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	RECT wndrect, cltrect;

	KillTimer(hwnd, GAME_TIMER_ID);
	setGameMode((byte)wparam, GETCHGWIDTH(lparam), GETCHGHEIGHT(lparam), GETCHGMINES(lparam));
	last_dbclick = false;

	//change window size for new game map size, no need to reset Game again
	//and invalidate all client area for repaint
	GetWindowRect(hwnd, &wndrect);
	GetClientRect(hwnd, &cltrect);
	wndrect.right = CLIENT_WIDTH + ((wndrect.right - wndrect.left) - cltrect.right) + wndrect.left;
	wndrect.bottom = CLIENT_HEIGHT + ((wndrect.bottom - wndrect.top) - cltrect.bottom) + wndrect.top;
	MoveWindow(hwnd, wndrect.left, wndrect.top, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, TRUE);

	setRBBitmap(hbm_rb);
	InvalidateRect(hwnd, NULL, FALSE);
	return 0;
}

LRESULT onLButtonDwon(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	SetCapture(hwnd);
	if (lparamIsInRB(lparam)) {	//click on Reset Button
		rb_capture = true;	//set ResetButton capture
		paintResetButton(hdc, RB_LEFT, RB_TOP, true);
	}
	else if (!ISGAMESET(Game.state)){	//won't work after game finished
		setRBBitmap(hbm_click);
		paintResetButton(hdc, RB_LEFT, RB_TOP, false);

		//if in the map area
		if (lparamIsInMap(lparam)) {
			int index = lparam2index(lparam);
			if (wparam & MK_RBUTTON) {	//double buttons down
				last_dbclick = true;
				Neighbor indexes;
				getNeighbors(&indexes, index2x(index), index2y(index));
				showClickedMapUnits(hdc, MAP_LEFT, MAP_TOP, &indexes);
			}
			else {	//single button
				last_dbclick = false;
				showClickedMapUnit(hdc, MAP_LEFT, MAP_TOP, index);
			}
		}
	}
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onLButtonUp(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	ReleaseCapture();
	if (rb_capture) {	//if ResetButton got capture
		if (lparamIsInRB(lparam)) {	//click up
			paintResetButton(hdc, RB_LEFT, RB_TOP, false);
			PostMessage(hwnd, WMAPP_GAMERESET, 0, 0);
		}
		rb_capture = false;	//release ResetButton capture
	}
	else if (!ISGAMESET(Game.state)) {	//won't work after game finished
		setRBBitmap(hbm_rb);
		paintResetButton(hdc, RB_LEFT, RB_TOP, false);

		//if in the map area
		if (lparamIsInMap(lparam)) {
			int index = lparam2index(lparam);
			if (wparam & MK_RBUTTON) {	//double buttons
				last_dbclick = true;
				int ret = clickAround(index);
				if (ret == -1) {
					PostMessage(hwnd, WMAPP_GAMEFAIL, 0, 0);
				}
				else if (isGameSuccessful()) {
					PostMessage(hwnd, WMAPP_GAMESUCCESS, 0, 0);
				}
				else
					paintMap(hdc, MAP_LEFT, MAP_TOP, false);
			}
			else if (!last_dbclick) {	//single button and last was not double button
				last_dbclick = false;
				if (Game.state == INIT && GETMUSTATE(Game.map[index]) != MUS_FLAG) {	//first click
					PostMessage(hwnd, WMAPP_GAMESTART, 0, index);
				}
				else {	//not first click
					int ret = leftClick(index);	//first click, then judge success
					if (ret == -1) {
						PostMessage(hwnd, WMAPP_GAMEFAIL, 0, 0);
					}
					else if (isGameSuccessful()) {
						PostMessage(hwnd, WMAPP_GAMESUCCESS, 0, 0);
					}
					else
						paintMap(hdc, MAP_LEFT, MAP_TOP, false);
				}
			}
			else {	//single button and last was double button
				last_dbclick = false;
				paintMap(hdc, MAP_LEFT, MAP_TOP, false);
			}
		}
	}
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onRButtonDown(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	//won't work after game finished or the mouse is not in the map area or ResetButton got capture
	if (!rb_capture && !ISGAMESET(Game.state) && lparamIsInMap(lparam)) {
		int index = lparam2index(lparam);
		if (wparam & MK_LBUTTON) {	//double buttons
			last_dbclick = true;
			setRBBitmap(hbm_click);
			paintResetButton(hdc, RB_LEFT, RB_TOP, false);
			Neighbor indexes;
			getNeighbors(&indexes, index2x(index), index2y(index));
			showClickedMapUnits(hdc, MAP_LEFT, MAP_TOP, &indexes);
		}
		else {	//single button, flag a unit or mark a unit
			last_dbclick = false;
			int ret = rightClick(index);
			if (ret == 0) {	//if mapunit state is changed
				paintMapUnit(hdc, MAP_LEFT + index2px(index), MAP_TOP + index2py(index), index);
				paintINums(hdc, MNUMS_LEFT, MNUMS_TOP, Game.mine_remains);
			}
		}
	}
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onRButtonUp(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	ReleaseCapture();
	//won't work after game finishing or ResetButton got capture
	if (!rb_capture && !ISGAMESET(Game.state)) {
		setRBBitmap(hbm_rb);
		paintResetButton(hdc, RB_LEFT, RB_TOP, false);

		//if in the map area
		if (lparamIsInMap(lparam)) {
			int index = lparam2index(lparam);
			if (wparam & MK_LBUTTON) {	//double buttons
				last_dbclick = true;
				int ret = clickAround(index);
				if (ret == -1) {
					PostMessage(hwnd, WMAPP_GAMEFAIL, 0, 0);
				}
				else if (isGameSuccessful()) {
					PostMessage(hwnd, WMAPP_GAMESUCCESS, 0, 0);
				}
				else
					paintMap(hdc, MAP_LEFT, MAP_TOP, false);
			}
			else	//single button
				last_dbclick = false;
		}
	}
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onMouseMove(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	if (rb_capture && (wparam & MK_LBUTTON)) {	//if ResetButton got capture
		paintResetButton(hdc, RB_LEFT, RB_TOP, lparamIsInRB(lparam));
	}
	else if (!ISGAMESET(Game.state)) {	//won't work after game finishing
		if (!lparamIsInMap(lparam)) {	//if not in the map area
			paintMap(hdc, MAP_LEFT, MAP_TOP, false);
		}
		else {	//if in the map area
			int index = lparam2index(lparam);
			if (wparam & MK_LBUTTON) {	//with left button down
				if (wparam & MK_RBUTTON) {	//double buttons
					Neighbor indexes;
					getNeighbors(&indexes, index2x(index), index2y(index));
					showClickedMapUnits(hdc, MAP_LEFT, MAP_TOP, &indexes);
				}
				//single button
				else
					showClickedMapUnit(hdc, MAP_LEFT, MAP_TOP, index);
			}
			//without mouse button down
			else
				paintMap(hdc, MAP_LEFT, MAP_TOP, false);
		}
	}
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onTimer(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	if (Game.time <= MAX_TIME) {
		HDC hdc = GetDC(hwnd);
		stepGameTime();
		paintINums(hdc, TNUMS_LEFT, TNUMS_TOP, Game.time);
		ReleaseDC(hwnd, hdc);
	}
	return 0;
}
