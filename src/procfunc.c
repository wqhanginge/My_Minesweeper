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
 * procfunc.c
 *****************************************************************************
 * This file contains Win32 Window Procedure Functions and Message Processing
 * Functions.
 * This file contains several global variables for Game and UI.
 *
 * NOTE: Most functions do NOT check arguments, use with care.
\*****************************************************************************/


#include "stdafx.h"
#include "procfunc.h"


/* private global variables */

GameInfo Game;      //Game information
GameScore Score;    //Record information
RBHB Bitmap;        //bitmap handles for ResetButton
UIRT Runtime;       //UI runtime status


/* commonly used functions */

BOOL isRectVisible(HDC hdc, int left, int top, int width, int height)
{
    RECT rect = { left, top, left + width, top + height };
    return !IsRectEmpty(&rect) && RectVisible(hdc, &rect);
}

BOOL addInvalidRect(HWND hwnd, int left, int top, int width, int height)
{
    RECT rect = { left, top, left + width, top + height };
    return !IsRectEmpty(&rect) && InvalidateRect(hwnd, &rect, FALSE);
}

BOOL checkGameMode(HMENU hmenu, BYTE mode)
{
    UINT miid;
    switch (mode) {
    case GM_JUNIOR: miid = ID_GAME_JUNIOR; break;
    case GM_MIDDLE: miid = ID_GAME_MIDDLE; break;
    case GM_SENIOR: miid = ID_GAME_SENIOR; break;
    case GM_CUSTOM: miid = ID_GAME_CUSTOM; break;
    default: return FALSE;
    }
    return CheckMenuRadioItem(hmenu, ID_GAME_JUNIOR, ID_GAME_CUSTOM, miid, MF_BYCOMMAND);
}

BOOL checkQuestionMark(HMENU hmenu, bool qmark)
{
    MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
    mii.fMask = MIIM_STATE;
    mii.fState = (qmark) ? MFS_CHECKED : MFS_UNCHECKED;
    return SetMenuItemInfo(hmenu, ID_GAME_MARK, FALSE, &mii);
}

void updateRecordContent(HWND hrecord)
{
    HWND hstatic;
    TCHAR str[10];

    hstatic = GetDlgItem(hrecord, IDC_JTIME);
    StringCchPrintf(str, 10, TEXT("%d %s"), Score.junior_time, TEXT(DLG_TIMEUNIT));
    SetWindowText(hstatic, str);

    hstatic = GetDlgItem(hrecord, IDC_MTIME);
    StringCchPrintf(str, 10, TEXT("%d %s"), Score.middle_time, TEXT(DLG_TIMEUNIT));
    SetWindowText(hstatic, str);

    hstatic = GetDlgItem(hrecord, IDC_STIME);
    StringCchPrintf(str, 10, TEXT("%d %s"), Score.senior_time, TEXT(DLG_TIMEUNIT));
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
        SetWindowText(hedit, getRecordName(&Score, Game.mode));
        SendMessage(hedit, EM_LIMITTEXT, SCORE_NAME_LEN - 1, 0);
        return TRUE;
    }
    case WM_DESTROY: {
        //get what in the Edit Control when exit Dialog
        HWND hedit = GetDlgItem(hgetname, IDC_EDITNAME);
        GetWindowText(hedit, getRecordName(&Score, Game.mode), SCORE_NAME_LEN);
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
    switch (msg) {
    case WM_INITDIALOG: {
        //display current MapInfo
        updateCustomContent(hcustom, lparam);
        return TRUE;
    }
    case WM_CLOSE: {
        EndDialog(hcustom, 0);
        return TRUE;
    }
    case WM_COMMAND: {
        switch (LOWORD(wparam)) {
        case IDOK:  //get contents and exit when click OK
            EndDialog(hcustom, getCustomContent(hcustom));
            break;
        case IDCANCEL:  //ignore contents and exit when click Cancel
            EndDialog(hcustom, 0);
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
        PostMessage(hwnd, WMAPP_GAMEMODECHANGE, GM_JUNIOR, 0);
        break;
    case ID_GAME_MIDDLE:
        CheckMenuRadioItem(hmenu, ID_GAME_JUNIOR, ID_GAME_CUSTOM, ID_GAME_MIDDLE, MF_BYCOMMAND);
        PostMessage(hwnd, WMAPP_GAMEMODECHANGE, GM_MIDDLE, 0);
        break;
    case ID_GAME_SENIOR:
        CheckMenuRadioItem(hmenu, ID_GAME_JUNIOR, ID_GAME_CUSTOM, ID_GAME_SENIOR, MF_BYCOMMAND);
        PostMessage(hwnd, WMAPP_GAMEMODECHANGE, GM_SENIOR, 0);
        break;
    case ID_GAME_CUSTOM:
        CheckMenuRadioItem(hmenu, ID_GAME_JUNIOR, ID_GAME_CUSTOM, ID_GAME_CUSTOM, MF_BYCOMMAND);
        mapinfo = DialogBoxParam(hinst, MAKEINTRESOURCE(IDD_CUSTOM), hwnd, CustomProc, mapinfo);
        mapinfo = (mapinfo) ? mapinfo : MAKEMAPINFO(Game.width, Game.height, Game.mines);
        PostMessage(hwnd, WMAPP_GAMEMODECHANGE, GM_CUSTOM, mapinfo);
        break;
    case ID_GAME_MARK:  //toggle QuestionMark 
        Runtime.qmark = !Runtime.qmark;
        checkQuestionMark(hmenu, Runtime.qmark);
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

    //set random seed
    FILETIME systime;
    GetSystemTimeAsFileTime(&systime);
    srand(((systime.dwHighDateTime << 16) | (systime.dwLowDateTime >> 16)));

    //load settings from file
    BOOL qmark;
    POINT wndpos = { lpcs->x, lpcs->y };
    TCHAR conf_path[MAX_CONFPATH];
    getConfPath(conf_path, MAX_CONFPATH);
    loadGame(conf_path, &Game, &Score, &wndpos, &qmark);

    //load bitmaps and init runtime status
    loadBitmaps(&Bitmap, lpcs->hInstance);
    Runtime = (UIRT){ qmark, false, false, false, INV_INDEX, Bitmap.normal };

    //init menu info
    checkGameMode(lpcs->hMenu, Game.mode);
    checkQuestionMark(lpcs->hMenu, Runtime.qmark);

    //adjust window size
    RECT wndrect = { 0, 0, CLIENT_WIDTH(Game.width), CLIENT_HEIGHT(Game.height) };
    AdjustWindowRectEx(&wndrect, lpcs->style, TRUE, lpcs->dwExStyle);
    MoveWindow(hwnd, wndpos.x, wndpos.y, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, FALSE);
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
    saveGame(conf_path, &Game, &Score, wndpos, Runtime.qmark);

    //release bitmaps
    freeBitmaps(&Bitmap);

    PostQuitMessage(0);
    return 0;
}

LRESULT onPaint(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    PAINTSTRUCT ps;
    HDC hpdc = BeginPaint(hwnd, &ps);

    HBITMAP hbmpbuf = CreateCompatibleBitmap(hpdc, CLIENT_WIDTH(Game.width), CLIENT_HEIGHT(Game.height));
    HDC hdcbuf = CreateCompatibleDC(hpdc);
    SelectObject(hdcbuf, hbmpbuf);

    drawClient(hdcbuf, CLIENT_LEFT, CLIENT_TOP, Game.width, Game.height);
    drawHeadArea(hdcbuf, HEADAREA_LEFT, HEADAREA_TOP, Game.width);
    drawMapArea(hdcbuf, MAPAREA_LEFT, MAPAREA_TOP, Game.width, Game.height);
    drawInfo(hdcbuf, MINE_LEFT, MINE_TOP);
    drawInfo(hdcbuf, TIME_LEFT(Game.width), TIME_TOP);

    if (isRectVisible(hpdc, MNUMS_LEFT, MNUMS_TOP, MNUMS_WIDTH, MNUMS_HEIGHT))
        drawINums(hdcbuf, MNUMS_LEFT, MNUMS_TOP, (int)Game.mines - Game.flags, INUMSF_SHOWALL);
    if (isRectVisible(hpdc, TNUMS_LEFT(Game.width), TNUMS_TOP, TNUMS_WIDTH, TNUMS_HEIGHT))
        drawINums(hdcbuf, TNUMS_LEFT(Game.width), TNUMS_TOP, Game.time, INUMSF_SHOWALL);
    if (isRectVisible(hpdc, RB_LEFT(Game.width), RB_TOP, RB_SIZE, RB_SIZE))
        drawResetButton(hdcbuf, RB_LEFT(Game.width), RB_TOP, Runtime.rb_curr, Runtime.rb_click);
    if (isRectVisible(hpdc, MAP_LEFT, MAP_TOP, MAP_WIDTH(Game.width), MAP_HEIGHT(Game.height))) {
        RECT rect = { MAP_LEFT, MAP_TOP, MUP_SIZE, MUP_SIZE };
        for (int idx = 0; idx < Game.width * Game.height; idx++) {
            OffsetRect(&rect, index2px(&Game, idx), index2py(&Game, idx));
            if (RectVisible(hpdc, &rect)) paintMapUnit(hdcbuf, rect.left, rect.top, Game.map[idx]);
        }
    }

    BitBlt(hpdc, 0, 0, CLIENT_WIDTH(Game.width), CLIENT_HEIGHT(Game.height), hdcbuf, 0, 0, SRCCOPY);
    DeleteDC(hdcbuf);
    DeleteObject(hbmpbuf);

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
    Runtime.mb_simul = false;
    Runtime.rb_curr = Bitmap.normal;
    InvalidateRect(hwnd, NULL, FALSE);
    return 0;
}

LRESULT onGameOver(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    KillTimer(hwnd, WND_TIMER_ID);
    Runtime.rb_curr = (wparam) ? Bitmap.win : Bitmap.loss;

    addInvalidRect(hwnd, MNUMS_LEFT, MNUMS_TOP, MNUMS_WIDTH, MNUMS_HEIGHT);
    addInvalidRect(hwnd, RB_LEFT(Game.width), RB_TOP, RB_SIZE, RB_SIZE);

    //if break record
    if (wparam && isNewRecord(&Game, &Score)) {
        HINSTANCE hinst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
        setRecordTime(&Score, Game.mode, Game.time);
        UpdateWindow(hwnd); //refresh window before updating record
        DialogBox(hinst, MAKEINTRESOURCE(IDD_GETNAME), hwnd, GetNameProc);
        DialogBox(hinst, MAKEINTRESOURCE(IDD_RECORD), hwnd, RecordProc);
    }
    return 0;
}

LRESULT onGameModeChange(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    KillTimer(hwnd, WND_TIMER_ID);
    setGameMode(&Game, (BYTE)wparam, GETMAPINFOWIDTH(lparam), GETMAPINFOHEIGHT(lparam), GETMAPINFOMINES(lparam));
    resetGame(&Game);
    Runtime.mb_simul = false;
    Runtime.rb_curr = Bitmap.normal;

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
    SetCapture(hwnd);
    if (isLparamInRB(lparam, Game.width)) { //click on ResetButton
        Runtime.rb_capture = true;  //set ResetButton capture
        Runtime.rb_click = true;    //set ResetButton clicked
        addInvalidRect(hwnd, RB_LEFT(Game.width), RB_TOP, RB_SIZE, RB_SIZE);
    }
    else if (ISGAMEACTIVE(Game.state)) {    //click on other part of window
        Runtime.rb_click = false;
        Runtime.rb_curr = Bitmap.click;
        addInvalidRect(hwnd, RB_LEFT(Game.width), RB_TOP, RB_SIZE, RB_SIZE);

        //if in Map
        if (isLparamInMap(lparam, Game.width, Game.height)) {
            int index = lparam2index(&Game, lparam);
            Runtime.mu_select = index;
            selectMapUnits(&Game, index, INV_INDEX, wparam & MK_RBUTTON);

            RECT rect;
            clipMapUnits(&rect, &Game, index);
            OffsetRect(&rect, MAP_LEFT, MAP_TOP);
            InvalidateRect(hwnd, &rect, FALSE);
        }
    }
    return 0;
}

LRESULT onLButtonUp(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    ReleaseCapture();
    if (Runtime.rb_capture) {   //if ResetButton got capture
        Runtime.rb_capture = false; //release ResetButton capture
        if (isLparamInRB(lparam, Game.width)) { //activate ResetButton
            Runtime.rb_click = false;
            PostMessage(hwnd, WMAPP_GAMERESET, 0, 0);
        }
    }
    else if (ISGAMEACTIVE(Game.state)) {    //if not GameOver
        Runtime.rb_click = false;
        Runtime.rb_curr = Bitmap.normal;
        addInvalidRect(hwnd, RB_LEFT(Game.width), RB_TOP, RB_SIZE, RB_SIZE);

        //if in Map
        if (isLparamInMap(lparam, Game.width, Game.height)) {
            int index = lparam2index(&Game, lparam);
            if (wparam & MK_RBUTTON) {  //with right button down
                Runtime.mb_simul = true;
                int ret = simulClick(&Game, index);
                switch (ret) {
                case RETVAL_GAMEWIN: PostMessage(hwnd, WMAPP_GAMEOVER, TRUE, 0); break;
                case RETVAL_GAMELOSS: PostMessage(hwnd, WMAPP_GAMEOVER, FALSE, 0); break;
                }
            }
            else if (!Runtime.mb_simul) {   //single button and last was not simultaneous button click
                Runtime.mb_simul = false;
                int ret = leftClick(&Game, index);  //click, then test result
                switch (ret) {
                case RETVAL_GAMESTART:  //first click
                    SetTimer(hwnd, WND_TIMER_ID, WND_TIMER_ELAPSE, NULL);
                    addInvalidRect(hwnd, MNUMS_LEFT, MNUMS_TOP, MNUMS_WIDTH, MNUMS_HEIGHT); //reset the Mine Region
                    break;
                case RETVAL_GAMEWIN: PostMessage(hwnd, WMAPP_GAMEOVER, TRUE, 0); break;
                case RETVAL_GAMELOSS: PostMessage(hwnd, WMAPP_GAMEOVER, FALSE, 0); break;
                }
            }
            else {  //single button and last was simultaneous button click
                Runtime.mb_simul = false;
            }
            addInvalidRect(hwnd, MAP_LEFT, MAP_TOP, MAP_WIDTH(Game.width), MAP_HEIGHT(Game.height));
        }
    }
    return 0;
}

LRESULT onRButtonDown(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (!Runtime.rb_capture && ISGAMEACTIVE(Game.state) && isLparamInMap(lparam, Game.width, Game.height)) {
        int index = lparam2index(&Game, lparam);
        if (wparam & MK_LBUTTON) {  //with left button down
            Runtime.rb_click = false;
            Runtime.rb_curr = Bitmap.click;
            addInvalidRect(hwnd, RB_LEFT(Game.width), RB_TOP, RB_SIZE, RB_SIZE);
            Runtime.mu_select = index;
            selectMapUnits(&Game, index, INV_INDEX, true);
        }
        else {  //single button, flag a MapUnit or mark a MapUnit
            int ret = rightClick(&Game, index, Runtime.qmark);
            if (ret == RETVAL_NOERROR) {    //if the state of MapUnit is changed
                addInvalidRect(hwnd, MAP_LEFT + index2px(&Game, index), MAP_TOP + index2py(&Game, index), MUP_SIZE, MUP_SIZE);
                addInvalidRect(hwnd, MNUMS_LEFT, MNUMS_TOP, MNUMS_WIDTH, MNUMS_HEIGHT);
            }
        }
    }
    return 0;
}

LRESULT onRButtonUp(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (!Runtime.rb_capture && ISGAMEACTIVE(Game.state)) {
        Runtime.rb_click = false;
        Runtime.rb_curr = Bitmap.normal;
        addInvalidRect(hwnd, RB_LEFT(Game.width), RB_TOP, RB_SIZE, RB_SIZE);

        //if in Map
        if (isLparamInMap(lparam, Game.width, Game.height)) {
            int index = lparam2index(&Game, lparam);
            if (wparam & MK_LBUTTON) {  //with left button down
                Runtime.mb_simul = true;
                int ret = simulClick(&Game, index);
                switch (ret) {
                case RETVAL_GAMEWIN: PostMessage(hwnd, WMAPP_GAMEOVER, TRUE, 0); break;
                case RETVAL_GAMELOSS: PostMessage(hwnd, WMAPP_GAMEOVER, FALSE, 0); break;
                }
                addInvalidRect(hwnd, MAP_LEFT, MAP_TOP, MAP_WIDTH(Game.width), MAP_HEIGHT(Game.height));
            }
            else {  //single button
                Runtime.mb_simul = false;
            }
        }
    }
    return 0;
}

LRESULT onMouseMove(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (Runtime.rb_capture && (wparam & MK_LBUTTON)) {  //if ResetButton got capture
        bool in_rb = isLparamInRB(lparam, Game.width);
        if (in_rb != Runtime.rb_click) {
            Runtime.rb_click = in_rb;
            addInvalidRect(hwnd, RB_LEFT(Game.width), RB_TOP, RB_SIZE, RB_SIZE);
        }
    }
    else if (ISGAMEACTIVE(Game.state)) {    //if not GameOver
        if (!isLparamInMap(lparam, Game.width, Game.height)) {  //if not in Map
            if (isidxinmap(&Game, Runtime.mu_select)) { //remove any selected state
                paintMap(hdc, MAP_LEFT, MAP_TOP, &Game);
                last_index = INV_INDEX;
            }
        }
        else {  //if in Map
            int index = lparam2index(&Game, lparam);
            if (last_index != index) {  //set new selected state
                if (wparam & MK_LBUTTON) {  //with left button down
                    bool rbutton = wparam & MK_RBUTTON;
                    selectMapUnits(hdc, MAP_LEFT, MAP_TOP, &Game, index, last_index, rbutton);
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
