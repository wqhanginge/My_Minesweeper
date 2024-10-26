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
 * procfunctions.c
 *****************************************************************************
 * This file contains Win32 Window Procedure Functions and Message Processing
 * Functions.
 * This file contains several global variables for Game and UI.
 *
 * NOTE: Most functions do NOT check arguments, use with care.
\*****************************************************************************/


#include "stdafx.h"
#include "procfunctions.h"


/* private global variables */

GameInfo Game;      //Game information
GameScore Score;    //Record information
RBHBM RBhbm;        //bitmap handles for ResetButton

bool last_sclick;   //if last mouse event was a simultaneous button click
bool rb_capture;    //if ResetButton get the capture


/* commonly used functions */

void setMenuChecked(HMENU hmenu, BYTE mode)
{
    UINT miid;
    switch (mode) {
    case JUNIOR: miid = ID_GAME_JUNIOR; break;
    case MIDDLE: miid = ID_GAME_MIDDLE; break;
    case SENIOR: miid = ID_GAME_SENIOR; break;
    case CUSTOM: miid = ID_GAME_CUSTOM; break;
    default: return;
    }
    CheckMenuRadioItem(hmenu, ID_GAME_JUNIOR, ID_GAME_CUSTOM, miid, MF_BYCOMMAND);
}

void setQMarkChecked(HMENU hmenu, bool mark)
{
    MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
    mii.fMask = MIIM_STATE;
    mii.fState = (mark) ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo(hmenu, ID_GAME_MARK, FALSE, &mii);
}

void updateRecordContent(HWND hrecord)
{
    HWND hstatic;
    TCHAR str[10];

    hstatic = GetDlgItem(hrecord, IDC_JTIME);
    StringCchPrintf(str, 10, TEXT("%d %s"), Score.junior_time, TEXT(DLG_TIMEUNIT_EN));
    SetWindowText(hstatic, str);

    hstatic = GetDlgItem(hrecord, IDC_MTIME);
    StringCchPrintf(str, 10, TEXT("%d %s"), Score.middle_time, TEXT(DLG_TIMEUNIT_EN));
    SetWindowText(hstatic, str);

    hstatic = GetDlgItem(hrecord, IDC_STIME);
    StringCchPrintf(str, 10, TEXT("%d %s"), Score.senior_time, TEXT(DLG_TIMEUNIT_EN));
    SetWindowText(hstatic, str);

    hstatic = GetDlgItem(hrecord, IDC_JNAME);
    SetWindowText(hstatic, Score.junior_name);

    hstatic = GetDlgItem(hrecord, IDC_MNAME);
    SetWindowText(hstatic, Score.middle_name);

    hstatic = GetDlgItem(hrecord, IDC_SNAME);
    SetWindowText(hstatic, Score.senior_name);
}

void updateCustomContent(HWND hcustom, LPARAM mapinfo)
{
    HWND hedit;
    TCHAR str[4];

    hedit = GetDlgItem(hcustom, IDC_EDITWIDTH);
    StringCchPrintf(str, 4, TEXT("%u"), GETMAPINFOWIDTH(mapinfo));
    SetWindowText(hedit, str);

    hedit = GetDlgItem(hcustom, IDC_EDITHEIGHT);
    StringCchPrintf(str, 4, TEXT("%u"), GETMAPINFOHEIGHT(mapinfo));
    SetWindowText(hedit, str);

    hedit = GetDlgItem(hcustom, IDC_EDITMINES);
    StringCchPrintf(str, 4, TEXT("%u"), GETMAPINFOMINES(mapinfo));
    SetWindowText(hedit, str);
}

LPARAM getCustomContent(HWND hcustom)
{
    HWND hedit;
    WORD width, height, mines;
    TCHAR str[8];   //take care of the value length in Edit Box

    hedit = GetDlgItem(hcustom, IDC_EDITWIDTH);
    GetWindowText(hedit, str, 8);
    width = (WORD)min(max(_ttoi(str), 0), UCHAR_MAX);

    hedit = GetDlgItem(hcustom, IDC_EDITHEIGHT);
    GetWindowText(hedit, str, 8);
    height = (WORD)min(max(_ttoi(str), 0), UCHAR_MAX);

    hedit = GetDlgItem(hcustom, IDC_EDITMINES);
    GetWindowText(hedit, str, 8);
    mines = (WORD)min(max(_ttoi(str), 0), USHRT_MAX);

    //overflow occurs if the value is cast directly and then passed
    return MAKEMAPINFO(width, height, mines);
}


/* Window Porcedure Functions */

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
        case IDLICENSE: //open the License Dialog
            DialogBox((HINSTANCE)GetWindowLongPtr(habout, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_LICENSE), habout, LicenseProc);
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
        }
        return TRUE;
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
        //show records
        updateRecordContent(hrecord);
        return TRUE;
    }
    case WM_CLOSE: {
        EndDialog(hrecord, 0);
        return TRUE;
    }
    case WM_COMMAND: {
        switch (LOWORD(wparam)) {
        case IDRESET:   //reset records
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
        //get Edit Control handle and init default presentation
        HWND hedit = GetDlgItem(hgetname, IDC_EDITNAME);
        SetWindowText(hedit, getpRecordName(&Score, Game.mode));
        SendMessage(hedit, EM_LIMITTEXT, SCORE_NAME_LEN - 1, 0);
        return TRUE;
    }
    case WM_DESTROY: {
        //get what in the Edit Control when exit Dialog
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
    static LPARAM mapinfo;  //a copy of MapInfo

    switch (msg) {
    case WM_INITDIALOG: {
        //display current MapInfo
        mapinfo = lparam;
        updateCustomContent(hcustom, mapinfo);
        return TRUE;
    }
    case WM_CLOSE: {
        EndDialog(hcustom, mapinfo);
        return TRUE;
    }
    case WM_COMMAND: {
        switch (LOWORD(wparam)) {
        case IDOK:  //get contents and exit when click OK
            mapinfo = getCustomContent(hcustom);
        case IDCANCEL:  //ignore contents and exit when click Cancel
            EndDialog(hcustom, mapinfo);
            break;
        }
        return TRUE;
    }
    }
    return FALSE;
}


/* Message Processing Functions */

LRESULT onMenu(HWND hwnd, UINT miid)
{
    HINSTANCE hinst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
    HMENU hmenu = GetMenu(hwnd);
    LPARAM mapinfo = MAKEMAPINFO(Game.width, Game.height, Game.mines);

    switch (miid) {
    case ID_GAME_START: //start a new Game
        PostMessage(hwnd, WMAPP_GAMERESET, 0, 0);
        break;
    case ID_GAME_JUNIOR:    //change GameMode
        CheckMenuRadioItem(hmenu, ID_GAME_JUNIOR, ID_GAME_CUSTOM, ID_GAME_JUNIOR, MF_BYCOMMAND);
        PostMessage(hwnd, WMAPP_GAMEMODECHANGE, JUNIOR, 0);
        break;
    case ID_GAME_MIDDLE:
        CheckMenuRadioItem(hmenu, ID_GAME_JUNIOR, ID_GAME_CUSTOM, ID_GAME_MIDDLE, MF_BYCOMMAND);
        PostMessage(hwnd, WMAPP_GAMEMODECHANGE, MIDDLE, 0);
        break;
    case ID_GAME_SENIOR:
        CheckMenuRadioItem(hmenu, ID_GAME_JUNIOR, ID_GAME_CUSTOM, ID_GAME_SENIOR, MF_BYCOMMAND);
        PostMessage(hwnd, WMAPP_GAMEMODECHANGE, SENIOR, 0);
        break;
    case ID_GAME_CUSTOM:
        CheckMenuRadioItem(hmenu, ID_GAME_JUNIOR, ID_GAME_CUSTOM, ID_GAME_CUSTOM, MF_BYCOMMAND);
        mapinfo = DialogBoxParam(hinst, MAKEINTRESOURCE(IDD_CUSTOM), hwnd, CustomProc, mapinfo);
        PostMessage(hwnd, WMAPP_GAMEMODECHANGE, CUSTOM, mapinfo);
        break;
    case ID_GAME_MARK:  //enable or disable QuestionMark
        setMark(&Game, !Game.mark);
        setQMarkChecked(hmenu, Game.mark);
        break;
    case ID_GAME_RECORD:    //show records
        DialogBox(hinst, MAKEINTRESOURCE(IDD_RECORD), hwnd, RecordProc);
        break;
    case ID_GAME_EXIT:
        PostMessage(hwnd, WM_CLOSE, 0, 0);
        break;
    case ID_ABOUT:  //show program description
        DialogBox(hinst, MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutProc);
        break;
    }
    return 0;
}

LRESULT onCreate(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lparam;

    //load settings from file
    POINT wndpos = { lpcs->x, lpcs->y };
    TCHAR conf_path[MAX_CONFPATH];
    getConfPath(conf_path, MAX_CONFPATH);
    loadGame(conf_path, &Game, &Score, &wndpos);

    //set random seed
    FILETIME systime;
    GetSystemTimeAsFileTime(&systime);
    srand(((systime.dwHighDateTime << 16) | (systime.dwLowDateTime >> 16)));

    //load bitmaps
    loadBitmaps(&RBhbm, lpcs->hInstance);

    //init menu info
    setMenuChecked(lpcs->hMenu, Game.mode);
    setQMarkChecked(lpcs->hMenu, Game.mark);

    //adjust window size
    RECT wndrect = { 0, 0, CLIENT_WIDTH(Game.width), CLIENT_HEIGHT(Game.height) };
    AdjustWindowRectEx(&wndrect, lpcs->style, TRUE, lpcs->dwExStyle);
    MoveWindow(hwnd, wndpos.x, wndpos.y, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, FALSE);

    //init simultaneous button click state and capture state
    last_sclick = false;
    rb_capture = false;
    return 0;
}

LRESULT onDestroy(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    RECT wndrect;
    GetWindowRect(hwnd, &wndrect);
    POINT wndpos = { wndrect.left, wndrect.top };

    //save settings
    TCHAR conf_path[MAX_CONFPATH];
    getConfPath(conf_path, MAX_CONFPATH);
    saveGame(conf_path, &Game, &Score, &wndpos);

    //release bitmaps
    freeBitmaps(&RBhbm);

    PostQuitMessage(0);
    return 0;
}

LRESULT onPaint(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    //always redraw the whole Client Area
    PAINTSTRUCT ps;
    HDC hpaintdc = BeginPaint(hwnd, &ps);

    HBITMAP hbmbuffer = CreateCompatibleBitmap(hpaintdc, CLIENT_WIDTH(Game.width), CLIENT_HEIGHT(Game.height));
    HDC hdcbuffer = CreateCompatibleDC(hpaintdc);
    SelectObject(hdcbuffer, hbmbuffer);

    drawDCClientBg(hdcbuffer, CLIENT_LEFT, CLIENT_TOP, Game.width, Game.height);
    drawDCHeadAreaBg(hdcbuffer, HEADAREA_LEFT, HEADAREA_TOP, Game.width);
    drawDCMapAreaBg(hdcbuffer, MAPAREA_LEFT, MAPAREA_TOP, Game.width, Game.height);
    drawDCInfoBg(hdcbuffer, MINE_LEFT, MINE_TOP);
    drawDCInfoBg(hdcbuffer, TIME_LEFT(Game.width), TIME_TOP);
    drawDCINums(hdcbuffer, MNUMS_LEFT, MNUMS_TOP, Game.mine_remains, INUMSF_SHOWALL);
    drawDCINums(hdcbuffer, TNUMS_LEFT(Game.width), TNUMS_TOP, Game.time, INUMSF_SHOWALL);
    drawDCResetButton(hdcbuffer, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);
    paintDCMap(hdcbuffer, MAP_LEFT, MAP_TOP, &Game, NULL);

    BitBlt(hpaintdc, 0, 0, CLIENT_WIDTH(Game.width), CLIENT_HEIGHT(Game.height), hdcbuffer, 0, 0, SRCCOPY);
    DeleteDC(hdcbuffer);
    DeleteObject(hbmbuffer);

    EndPaint(hwnd, &ps);
    return 0;
}

LRESULT onCommand(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (HIWORD(wparam) == 0)    //menu message
        return onMenu(hwnd, LOWORD(wparam));
    return 0;
}

LRESULT onGameReset(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    KillTimer(hwnd, WND_TIMER_ID);
    resetGame(&Game);
    last_sclick = false;
    setCurrBitmap(&RBhbm, RBhbm.normal);
    InvalidateRect(hwnd, NULL, FALSE);
    return 0;
}

LRESULT onGameWin(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    HDC hdc = GetDC(hwnd);
    KillTimer(hwnd, WND_TIMER_ID);
    setCurrBitmap(&RBhbm, RBhbm.win);
    paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);
    paintINums(hdc, MNUMS_LEFT, MNUMS_TOP, 0);

    //if break record
    if (isNewRecord(&Game, &Score)) {
        HINSTANCE hinst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
        setRecordTime(&Score, Game.mode, Game.time);
        DialogBox(hinst, MAKEINTRESOURCE(IDD_GETNAME), hwnd, GetNameProc);
        DialogBox(hinst, MAKEINTRESOURCE(IDD_RECORD), hwnd, RecordProc);
    }
    ReleaseDC(hwnd, hdc);
    return 0;
}

LRESULT onGameLoss(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    HDC hdc = GetDC(hwnd);
    KillTimer(hwnd, WND_TIMER_ID);
    setCurrBitmap(&RBhbm, RBhbm.loss);
    paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);
    ReleaseDC(hwnd, hdc);
    return 0;
}

LRESULT onGameModeChange(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    KillTimer(hwnd, WND_TIMER_ID);
    setGameMode(&Game, (BYTE)wparam, GETMAPINFOWIDTH(lparam), GETMAPINFOHEIGHT(lparam), GETMAPINFOMINES(lparam));
    resetGame(&Game);
    setCurrBitmap(&RBhbm, RBhbm.normal);
    last_sclick = false;

    //change window size for new GameMap size
    RECT wndrect, cltrect;
    GetWindowRect(hwnd, &wndrect);
    GetClientRect(hwnd, &cltrect);

    int wndw = CLIENT_WIDTH(Game.width) + ((wndrect.right - wndrect.left) - cltrect.right);
    int wndh = CLIENT_HEIGHT(Game.height) + ((wndrect.bottom - wndrect.top) - cltrect.bottom);
    MoveWindow(hwnd, wndrect.left, wndrect.top, wndw, wndh, TRUE);  //TRUE for 'bRepaint' to repaint NonClient Area on demand
    InvalidateRect(hwnd, NULL, FALSE);  //always repaint Client Area
    return 0;
}

LRESULT onLButtonDwon(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    HDC hdc = GetDC(hwnd);
    SetCapture(hwnd);
    if (isLparamInRB(lparam, Game.width)) { //click on ResetButton
        rb_capture = true;  //set ResetButton capture
        paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, true);
    }
    else if (ISGAMEACTIVE(Game.state)) {    //click on other part of window
        setCurrBitmap(&RBhbm, RBhbm.click);
        paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);

        //if in Map
        if (isLparamInMap(lparam, Game.width, Game.height)) {
            int index = lparam2index(&Game, lparam);
            bool rbutton = wparam & MK_RBUTTON;
            showSelectedMapUnit(hdc, MAP_LEFT, MAP_TOP, &Game, index, INV_INDEX, rbutton);
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
        rb_capture = false; //release ResetButton capture
        if (isLparamInRB(lparam, Game.width)) { //activate ResetButton
            paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);
            PostMessage(hwnd, WMAPP_GAMERESET, 0, 0);
        }
    }
    else if (ISGAMEACTIVE(Game.state)) {    //if not GameOver
        setCurrBitmap(&RBhbm, RBhbm.normal);
        paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);

        //if in Map
        if (isLparamInMap(lparam, Game.width, Game.height)) {
            int index = lparam2index(&Game, lparam);
            if (wparam & MK_RBUTTON) {  //with right button down
                last_sclick = true;
                int ret = simulClick(&Game, index);
                if (ret == RETVAL_GAMELOSS) {
                    PostMessage(hwnd, WMAPP_GAMELOSS, 0, 0);
                }
                else if (ret == RETVAL_GAMEWIN) {
                    PostMessage(hwnd, WMAPP_GAMEWIN, 0, 0);
                }
                paintMap(hdc, MAP_LEFT, MAP_TOP, &Game);
            }
            else if (!last_sclick) {    //single button and last was not simultaneous button click
                last_sclick = false;
                if (isFirstClick(&Game, index)) {   //first click, should be done before calling leftClick()
                    SetTimer(hwnd, WND_TIMER_ID, WND_TIMER_ELAPSE, NULL);
                    paintINums(hdc, MNUMS_LEFT, MNUMS_TOP, Game.mines); //reset the Mine SubArea
                }
                int ret = leftClick(&Game, index);  //click, then test result
                if (ret == RETVAL_GAMELOSS) {
                    PostMessage(hwnd, WMAPP_GAMELOSS, 0, 0);
                }
                else if (ret == RETVAL_GAMEWIN) {
                    PostMessage(hwnd, WMAPP_GAMEWIN, 0, 0);
                }
                paintMap(hdc, MAP_LEFT, MAP_TOP, &Game);
            }
            else {  //single button and last was simultaneous button click
                last_sclick = false;
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
    if (!rb_capture && ISGAMEACTIVE(Game.state) && isLparamInMap(lparam, Game.width, Game.height)) {
        int index = lparam2index(&Game, lparam);
        if (wparam & MK_LBUTTON) {  //with left button down
            setCurrBitmap(&RBhbm, RBhbm.click);
            paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);
            showSelectedMapUnit(hdc, MAP_LEFT, MAP_TOP, &Game, index, INV_INDEX, true);
        }
        else {  //single button, flag a MapUnit or mark a MapUnit
            int ret = rightClick(&Game, index);
            if (ret == RETVAL_NOERROR) {    //if the state of MapUnit is changed
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
    if (!rb_capture && ISGAMEACTIVE(Game.state)) {
        setCurrBitmap(&RBhbm, RBhbm.normal);
        paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, false);

        //if in Map
        if (isLparamInMap(lparam, Game.width, Game.height)) {
            int index = lparam2index(&Game, lparam);
            if (wparam & MK_LBUTTON) {  //with left button down
                last_sclick = true;
                int ret = simulClick(&Game, index);
                if (ret == RETVAL_GAMELOSS) {
                    PostMessage(hwnd, WMAPP_GAMELOSS, 0, 0);
                }
                else if (ret == RETVAL_GAMEWIN) {
                    PostMessage(hwnd, WMAPP_GAMEWIN, 0, 0);
                }
                paintMap(hdc, MAP_LEFT, MAP_TOP, &Game);
            }
            else {  //single button
                last_sclick = false;
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
        bool in_rb = isLparamInRB(lparam, Game.width);
        if (last_in_rb != in_rb) {
            paintResetButton(hdc, RB_LEFT(Game.width), RB_TOP, RBhbm.current, in_rb);
            last_in_rb = in_rb;
        }
    }
    else if (ISGAMEACTIVE(Game.state)) {    //if not GameOver
        static int last_index = INV_INDEX;
        if (!isLparamInMap(lparam, Game.width, Game.height)) {  //if not in Map
            if (isidxinmap(&Game, last_index)) {    //remove any selected state
                paintMap(hdc, MAP_LEFT, MAP_TOP, &Game);
                last_index = INV_INDEX;
            }
        }
        else {  //if in Map
            int index = lparam2index(&Game, lparam);
            if (last_index != index) {  //set new selected state
                if (wparam & MK_LBUTTON) {  //with left button down
                    bool rbutton = wparam & MK_RBUTTON;
                    showSelectedMapUnit(hdc, MAP_LEFT, MAP_TOP, &Game, index, last_index, rbutton);
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
