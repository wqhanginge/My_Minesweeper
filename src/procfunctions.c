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
 * procfunctions.c
 *****************************************************************************
 * This file contains Win32 Window Proc Functions and Message Processing
 * Functions.
 * This file uses several global window handles defined in main.c and uses
 * several global variables for Game and UI.
 * 
 * NOTE: Most functions have NO arg check, use with care.
\*****************************************************************************/


#include "stdafx.h"
#include "procfunctions.h"


/* private global variables */

GameInfo Game;      //Game information
GameScore Score;    //Record information
RBHBM RBhbm;        //bitmap handles, point to bitmaps which will be drawn on ResetButton

bool last_dbclick;  //indicate if last mouse event was a double button click
bool rb_capture;    //indicate if ResetButton get the capture


void setMenuChecked(BYTE GameMode)
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
    }
}

void setQMarkChecked(bool Mark)
{
    MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
    mii.fMask = MIIM_STATE;
    mii.fState = (Mark) ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo(hMenu, ID_GAME_MARK, FALSE, &mii);
}

void updateRecordContent(HWND hrecord)
{
    HWND hstatic;
    TCHAR str[10];

    hstatic = GetDlgItem(hrecord, IDC_JTIME);
    StringCchPrintf(str, 10, TEXT("%d %s"), Score.junior_time, TEXT(TIMEUNIT_EN));
    SetWindowText(hstatic, str);

    hstatic = GetDlgItem(hrecord, IDC_MTIME);
    StringCchPrintf(str, 10, TEXT("%d %s"), Score.middle_time, TEXT(TIMEUNIT_EN));
    SetWindowText(hstatic, str);

    hstatic = GetDlgItem(hrecord, IDC_STIME);
    StringCchPrintf(str, 10, TEXT("%d %s"), Score.senior_time, TEXT(TIMEUNIT_EN));
    SetWindowText(hstatic, str);

    hstatic = GetDlgItem(hrecord, IDC_JNAME);
    SetWindowText(hstatic, Score.junior_name);

    hstatic = GetDlgItem(hrecord, IDC_MNAME);
    SetWindowText(hstatic, Score.middle_name);

    hstatic = GetDlgItem(hrecord, IDC_SNAME);
    SetWindowText(hstatic, Score.senior_name);
}

void updateCustomContent(HWND hcustom, LPARAM whm)
{
    HWND hedit;
    TCHAR str[4];

    hedit = GetDlgItem(hcustom, IDC_EDITWIDTH);
    StringCchPrintf(str, 4, TEXT("%u"), GETCHGWIDTH(whm));
    SetWindowText(hedit, str);

    hedit = GetDlgItem(hcustom, IDC_EDITHEIGHT);
    StringCchPrintf(str, 4, TEXT("%u"), GETCHGHEIGHT(whm));
    SetWindowText(hedit, str);

    hedit = GetDlgItem(hcustom, IDC_EDITMINES);
    StringCchPrintf(str, 4, TEXT("%u"), GETCHGMINES(whm));
    SetWindowText(hedit, str);
}

LPARAM getCustomContent(HWND hcustom)
{
    HWND hedit;
    DWORD width, height, mines;
    TCHAR str[8];

    hedit = GetDlgItem(hcustom, IDC_EDITWIDTH);
    GetWindowText(hedit, str, 8);
    width = (DWORD)_ttoi(str);

    hedit = GetDlgItem(hcustom, IDC_EDITHEIGHT);
    GetWindowText(hedit, str, 8);
    height = (DWORD)_ttoi(str);

    hedit = GetDlgItem(hcustom, IDC_EDITMINES);
    GetWindowText(hedit, str, 8);
    mines = (DWORD)_ttoi(str);

    return MAKECHGLPARAM(width, height, mines);
}


INT_PTR CALLBACK AboutProc(HWND habout, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_INITDIALOG: {
        HWND htext = GetDlgItem(habout, IDC_ABOUTTEXT);
        TCHAR aboutinfo[MAX_PROPSTR];
        
        //show program description
        loadProperty(aboutinfo, MAX_PROPSTR);
        SetWindowText(htext, aboutinfo);
        return TRUE;
    }
    case WM_CLOSE: {
        EndDialog(habout, 0);
        return TRUE;
    }
    case WM_COMMAND: {
        switch (LOWORD(wparam)) {
        case IDLICENSE:
            //open license dialog
            DialogBox(hInst, MAKEINTRESOURCE(IDD_LICENSE), habout, LicenseProc);
            break;
        case IDOK:
            EndDialog(habout, 0);
            break;
        }
        return TRUE;
    }
    }
    return FALSE;
}

INT_PTR CALLBACK LicenseProc(HWND hlicense, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_INITDIALOG: {
        HWND hedit = GetDlgItem(hlicense, IDC_LICENSEBOX);
        LPTSTR licstr = malloc(sizeof(TCHAR) * MAX_LICSTR);
        if (licstr) {   //load license resource and show
            loadLicense(licstr, MAX_LICSTR);
            SetWindowText(hedit, licstr);
            free(licstr);
            return TRUE;
        }
        return FALSE;
    }
    case WM_CLOSE: {
        EndDialog(hlicense, 0);
        return TRUE;
    }
    case WM_COMMAND: {
        if (LOWORD(wparam) == IDOK) EndDialog(hlicense, 0);
        return TRUE;
    }
    }
    return FALSE;
}

INT_PTR CALLBACK RecordProc(HWND hrecord, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_INITDIALOG: {
        //init static control's content
        updateRecordContent(hrecord);
        return TRUE;
    }
    case WM_CLOSE: {
        EndDialog(hrecord, 0);
        return TRUE;
    }
    case WM_COMMAND: {
        switch (LOWORD(wparam)) {
        case IDRESET:
            //reset the record
            resetRecord(&Score);
            updateRecordContent(hrecord);
            break;
        case IDOK:
            EndDialog(hrecord, 0);
            break;
        }
        return TRUE;
    }
    }
    return FALSE;
}

INT_PTR CALLBACK GetNameProc(HWND hgetname, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_INITDIALOG: {
        //get edit control handle and init default presentation
        HWND hedit = GetDlgItem(hgetname, IDC_EDITNAME);
        SetWindowText(hedit, getpRecordName(&Score, Game.mode));
        SendMessage(hedit, EM_LIMITTEXT, SCORE_NAME_LEN - 1, 0);
        return TRUE;
    }
    case WM_DESTROY: {
        //get what in the edit control when exit dialog
        HWND hedit = GetDlgItem(hgetname, IDC_EDITNAME);
        GetWindowText(hedit, getpRecordName(&Score, Game.mode), SCORE_NAME_LEN);
        return TRUE;
    }
    case WM_COMMAND: {
        if (LOWORD(wparam) == IDOK) EndDialog(hgetname, 0);
        return TRUE;
    }
    }
    return FALSE;
}

INT_PTR CALLBACK CustomProc(HWND hcustom, UINT msg, WPARAM wparam, LPARAM lparam)
{
    static LPARAM whm;  //a copy of map settings

    switch (msg) {
    case WM_INITDIALOG: {
        //init edit control show
        whm = MAKECHGLPARAM(Game.width, Game.height, Game.mines);
        updateCustomContent(hcustom, whm);
        return TRUE;
    }
    case WM_CLOSE: {
        EndDialog(hcustom, 0);
        return TRUE;
    }
    case WM_DESTROY: {
        //set game mode when exit dialog
        PostMessage(hWnd, WMAPP_GAMEMODECHG, CUSTOM, whm);
        return TRUE;
    }
    case WM_COMMAND: {
        switch (LOWORD(wparam)) {
        case IDOK:
            //get what in edit control when click OK
            whm = getCustomContent(hcustom);
            EndDialog(hcustom, 0);
            break;
        case IDCANCEL:
            EndDialog(hcustom, 0);
            break;
        }
        return TRUE;
    }
    }
    return FALSE;
}


LRESULT onMenu(WPARAM wparam)
{
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
        //enable or disable Question Mark mode
        setMark(&Game, !Game.mark);
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
    }
    return 0;
}

LRESULT onCreate(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    POINT wndpos;
    RECT wndrect, cltrect;
    FILETIME systime;
    TCHAR conf_path[MAX_CONFPATH];
    int edgew, edgeh;

    //load bitmaps
    loadBitmaps(&RBhbm, hInst);

    //init game from file
    getConfPath(conf_path, MAX_CONFPATH);
    initGame(conf_path, &Game, &Score, &wndpos);
    GetSystemTimeAsFileTime(&systime);
    srand(((systime.dwHighDateTime << 16) | (systime.dwLowDateTime >> 16)));

    //init menu info
    setMenuChecked(Game.mode);
    setQMarkChecked(Game.mark);

    //adjust window size
    GetWindowRect(hwnd, &wndrect);
    GetClientRect(hwnd, &cltrect);
    edgew = (wndrect.right - wndrect.left) - cltrect.right;
    edgeh = (wndrect.bottom - wndrect.top) - cltrect.bottom;
    wndrect = (RECT){ wndpos.x, wndpos.y, wndpos.x + CLIENT_WIDTH(Game.width) + edgew, wndpos.y + CLIENT_HEIGHT(Game.height) + edgeh };
    MoveWindow(hwnd, wndrect.left, wndrect.top, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, FALSE);

    //init double button click state and capture state
    last_dbclick = false;
    rb_capture = false;
    return 0;
}

LRESULT onDestroy(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    POINT wndpos;
    RECT wndrect;
    TCHAR conf_path[MAX_CONFPATH];

    //release bitmaps
    freeBitmaps(&RBhbm);
    //save game info
    GetWindowRect(hwnd, &wndrect);
    wndpos = (POINT){ wndrect.left, wndrect.top };
    getConfPath(conf_path, MAX_CONFPATH);
    saveGame(conf_path, &Game, &Score, &wndpos);

    PostQuitMessage(0);
    return 0;
}

LRESULT onPaint(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    //redraw the whole client area
    PAINTSTRUCT ps;
    HDC hpaintdc = BeginPaint(hwnd, &ps);

    HDC hdcbuffer = CreateCompatibleDC(hpaintdc);
    HBITMAP hbmbuffer = CreateCompatibleBitmap(hpaintdc, CLIENT_WIDTH(Game.width), CLIENT_HEIGHT(Game.height));
    SelectObject(hdcbuffer, hbmbuffer);

    drawDCClientBg(hdcbuffer, 0, 0, Game.width, Game.height);
    drawDCInfoBg(hdcbuffer, INFO_LEFT, INFO_TOP, Game.width);
    drawDCMapAreaBg(hdcbuffer, MAPAREA_LEFT, MAPAREA_TOP, Game.width, Game.height);
    drawDCNumBg(hdcbuffer, MINE_LEFT, MINE_TOP);
    drawDCNumBg(hdcbuffer, TIME_LEFT(Game.width), TIME_TOP);
    drawDCINums(hdcbuffer, MNUMS_LEFT, MNUMS_TOP, Game.mine_remains);
    drawDCINums(hdcbuffer, TNUMS_LEFT(Game.width), TNUMS_TOP, Game.time);
    drawDCResetButton(hdcbuffer, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);
    paintDCMap(hdcbuffer, MAP_LEFT, MAP_TOP, &Game);

    BitBlt(hpaintdc, 0, 0, CLIENT_WIDTH(Game.width), CLIENT_HEIGHT(Game.height), hdcbuffer, 0, 0, SRCCOPY);
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
    resetGame(&Game);
    last_dbclick = false;
    setCurrBitmap(&RBhbm, RBhbm.normal);
    InvalidateRect(hwnd, NULL, FALSE);
    return 0;
}

LRESULT onGameFail(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    HDC hdc = GetDC(hwnd);
    KillTimer(hwnd, GAME_TIMER_ID);
    setCurrBitmap(&RBhbm, RBhbm.fail);
    paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);
    ReleaseDC(hwnd, hdc);
    return 0;
}

LRESULT onGameSuccess(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    HDC hdc = GetDC(hwnd);
    KillTimer(hwnd, GAME_TIMER_ID);
    setCurrBitmap(&RBhbm, RBhbm.success);
    paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);
    paintINums(hdc, MNUMS_LEFT, MNUMS_TOP, 0);

    //if break record
    if (isNewRecord(&Game, &Score)) {
        setRecordTime(&Score, Game.mode, Game.time);
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
    setGameMode(&Game, (BYTE)wparam, GETCHGWIDTH(lparam), GETCHGHEIGHT(lparam), GETCHGMINES(lparam));
    last_dbclick = false;

    //change window size for new game map size, no need to reset Game again
    //and invalidate the whole client area for repaint
    GetWindowRect(hwnd, &wndrect);
    GetClientRect(hwnd, &cltrect);
    wndrect.right = CLIENT_WIDTH(Game.width) + ((wndrect.right - wndrect.left) - cltrect.right) + wndrect.left;
    wndrect.bottom = CLIENT_HEIGHT(Game.height) + ((wndrect.bottom - wndrect.top) - cltrect.bottom) + wndrect.top;
    MoveWindow(hwnd, wndrect.left, wndrect.top, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, TRUE);

    setCurrBitmap(&RBhbm, RBhbm.normal);
    InvalidateRect(hwnd, NULL, FALSE);
    return 0;
}

LRESULT onLButtonDwon(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    HDC hdc = GetDC(hwnd);
    SetCapture(hwnd);
    if (lparamIsInRB(lparam, Game.width)) { //click on ResetButton
        rb_capture = true;  //set ResetButton capture
        paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, true);
    }
    else if (!ISGAMESET(Game.state)){   //won't work after game is finished
        setCurrBitmap(&RBhbm, RBhbm.click);
        paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);

        //if in the map area
        if (lparamIsInMap(lparam, Game.width, Game.height)) {
            int index = lparam2index(&Game, lparam);
            bool double_buttons = wparam & MK_RBUTTON;
            showSelectedMapUnit(hdc, MAP_LEFT, MAP_TOP, &Game, index, INV_INDEX, double_buttons);
        }
    }
    ReleaseDC(hwnd, hdc);
    return 0;
}

LRESULT onLButtonUp(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    HDC hdc = GetDC(hwnd);
    ReleaseCapture();
    if (rb_capture) {   //if ResetButton got capture
        if (lparamIsInRB(lparam, Game.width)) { //click up
            paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);
            PostMessage(hwnd, WMAPP_GAMERESET, 0, 0);
        }
        rb_capture = false; //release ResetButton capture
    }
    else if (!ISGAMESET(Game.state)) {  //won't work after game is finished
        setCurrBitmap(&RBhbm, RBhbm.normal);
        paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);

        //if in the map area
        if (lparamIsInMap(lparam, Game.width, Game.height)) {
            int index = lparam2index(&Game, lparam);
            if (wparam & MK_RBUTTON) {  //double buttons
                last_dbclick = true;
                int ret = clickAround(&Game, index);
                if (ret == RETVAL_GAMEFAIL) {
                    PostMessage(hwnd, WMAPP_GAMEFAIL, 0, 0);
                }
                else if (ret == RETVAL_GAMESUCCESS) {
                    PostMessage(hwnd, WMAPP_GAMESUCCESS, 0, 0);
                }
                paintMap(hdc, MAP_LEFT, MAP_TOP, &Game);
            }
            else if (!last_dbclick) {   //single button and last was not double button
                last_dbclick = false;
                if (isFirstClick(&Game, index)) {   //first click, should be done before calling leftClick()
                    SetTimer(hwnd, GAME_TIMER_ID, GAME_TIMER_ELAPSE, NULL);
                    paintINums(hdc, MNUMS_LEFT, MNUMS_TOP, Game.mines); //reset the MinePart
                }
                int ret = leftClick(&Game, index);  //click, then judge success
                if (ret == RETVAL_GAMEFAIL) {
                    PostMessage(hwnd, WMAPP_GAMEFAIL, 0, 0);
                }
                else if (ret == RETVAL_GAMESUCCESS) {
                    PostMessage(hwnd, WMAPP_GAMESUCCESS, 0, 0);
                }
                paintMap(hdc, MAP_LEFT, MAP_TOP, &Game);
            }
            else {  //single button and last was double button
                last_dbclick = false;
                paintMap(hdc, MAP_LEFT, MAP_TOP, &Game);
            }
        }
    }
    ReleaseDC(hwnd, hdc);
    return 0;
}

LRESULT onRButtonDown(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    HDC hdc = GetDC(hwnd);
    //won't work after game is finished or the mouse is not in the Map area or ResetButton got capture
    if (!rb_capture && !ISGAMESET(Game.state) && lparamIsInMap(lparam, Game.width, Game.height)) {
        int index = lparam2index(&Game, lparam);
        if (wparam & MK_LBUTTON) {  //double buttons
            setCurrBitmap(&RBhbm, RBhbm.click);
            paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);
            showSelectedMapUnit(hdc, MAP_LEFT, MAP_TOP, &Game, index, INV_INDEX, true);
        }
        else {  //single button, flag a unit or mark a unit
            int ret = rightClick(&Game, index);
            if (ret == RETVAL_NOERROR) {    //if mapunit state is changed
                paintMapUnit(hdc, MAP_LEFT + index2px(&Game, index), MAP_TOP + index2py(&Game, index), Game.map[index]);
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
    //won't work after game is finished or ResetButton got capture
    if (!rb_capture && !ISGAMESET(Game.state)) {
        setCurrBitmap(&RBhbm, RBhbm.normal);
        paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);

        //if in the Map area
        if (lparamIsInMap(lparam, Game.width, Game.height)) {
            int index = lparam2index(&Game, lparam);
            if (wparam & MK_LBUTTON) {  //double buttons
                last_dbclick = true;
                int ret = clickAround(&Game, index);
                if (ret == RETVAL_GAMEFAIL) {
                    PostMessage(hwnd, WMAPP_GAMEFAIL, 0, 0);
                }
                else if (ret == RETVAL_GAMESUCCESS) {
                    PostMessage(hwnd, WMAPP_GAMESUCCESS, 0, 0);
                }
                paintMap(hdc, MAP_LEFT, MAP_TOP, &Game);
            }
            else {  //single button
                last_dbclick = false;
            }
        }
    }
    ReleaseDC(hwnd, hdc);
    return 0;
}

LRESULT onMouseMove(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    HDC hdc = GetDC(hwnd);
    if (rb_capture && (wparam & MK_LBUTTON)) {  //if ResetButton got capture
        static bool last_in_rb = true;
        bool in_rb = lparamIsInRB(lparam, Game.width);
        if (last_in_rb != in_rb) {
            paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, in_rb);
            last_in_rb = in_rb;
        }
    }
    else if (!ISGAMESET(Game.state)) {  //won't work after game is finished
        static int last_index = INV_INDEX;
        if (!lparamIsInMap(lparam, Game.width, Game.height)) {  //if not in the Map area
            if (isidxinmap(&Game, last_index)) {    //update once
                paintMap(hdc, MAP_LEFT, MAP_TOP, &Game);
                last_index = INV_INDEX;
            }
        }
        else {  //if in the map area
            int index = lparam2index(&Game, lparam);
            if (last_index != index) {
                if (wparam & MK_LBUTTON) {  //with left button down
                    bool double_buttons = wparam & MK_RBUTTON;
                    showSelectedMapUnit(hdc, MAP_LEFT, MAP_TOP, &Game, index, last_index, double_buttons);
                }
                last_index = index;
            }
        }
    }
    ReleaseDC(hwnd, hdc);
    return 0;
}

LRESULT onTimer(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (Game.time <= MAX_TIME) {
        HDC hdc = GetDC(hwnd);
        stepGameTime(&Game);
        paintINums(hdc, TNUMS_LEFT(Game.width), TNUMS_TOP, Game.time);
        ReleaseDC(hwnd, hdc);
    }
    return 0;
}
