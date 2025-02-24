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
 * encapsulations.c
 *****************************************************************************
 * This file contains encapsulations of UI functions and IO operations.
 * This file contains bitmap management functions.
 * This file also contians some useful transform functions.
 *
 * NOTE: Most functions do NOT check arguments, use with care.
\*****************************************************************************/


#include "stdafx.h"
#include "encapsulations.h"


/* position related functions */

int ppos2index(PGameInfo pGame, int px, int py)
{
    return xy2index(pGame, px2x(px), py2y(py));
}

int index2px(PGameInfo pGame, int index)
{
    return x2px(index2x(pGame, index));
}

int index2py(PGameInfo pGame, int index)
{
    return y2py(index2y(pGame, index));
}

bool isLparamInRB(LPARAM lparam, BYTE n_map_width)
{
    POINTS p = MAKEPOINTS(lparam);
    p = (POINTS){ p.x - RB_LEFT(n_map_width), p.y - RB_TOP };
    return (p.x >= 0 && p.x < RB_SIZE && p.y >= 0 && p.y < RB_SIZE);
}

bool isLparamInMap(LPARAM lparam, BYTE n_map_width, BYTE n_map_height)
{
    POINTS p = MAKEPOINTS(lparam);
    p = (POINTS){ p.x - MAP_LEFT, p.y - MAP_TOP };
    return (p.x >= 0 && p.x < MAP_WIDTH(n_map_width) && p.y >= 0 && p.y < MAP_HEIGHT(n_map_height));
}

int lparam2index(PGameInfo pGame, LPARAM lparam)
{
    POINTS p = MAKEPOINTS(lparam);
    return ppos2index(pGame, p.x - MAP_LEFT, p.y - MAP_TOP);
}


/* bitmap management functions */

void loadBitmaps(PRBHBM pRBhbm, HINSTANCE hinst)
{
    pRBhbm->normal = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_RESET));
    pRBhbm->click = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_CLICK));
    pRBhbm->win = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_WIN));
    pRBhbm->loss = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_LOSS));
    pRBhbm->current = pRBhbm->normal;
}

void freeBitmaps(PRBHBM pRBhbm)
{
    DeleteObject(pRBhbm->normal);
    DeleteObject(pRBhbm->click);
    DeleteObject(pRBhbm->win);
    DeleteObject(pRBhbm->loss);
    DeleteObject(pRBhbm->current);
    memset(pRBhbm, 0, sizeof(RBHBM));
}

void setCurrBitmap(PRBHBM pRBhbm, HBITMAP hbm)
{
    pRBhbm->current = hbm;
}


/* UI functions */

void paintINums(HDC hdstdc, int left, int top, int num)
{
    HBITMAP hbmbuffer = CreateCompatibleBitmap(hdstdc, INUMS_WIDTH, INUMS_HEIGHT);
    HDC hdcbuffer = CreateCompatibleDC(hdstdc);

    SelectObject(hdcbuffer, hbmbuffer);
    drawDCINums(hdcbuffer, 0, 0, num, INUMSF_SHOWALL);
    BitBlt(hdstdc, left, top, INUMS_WIDTH, INUMS_HEIGHT, hdcbuffer, 0, 0, SRCCOPY);

    DeleteDC(hdcbuffer);
    DeleteObject(hbmbuffer);
}

void paintResetButton(HDC hdstdc, int left, int top, HBITMAP hbm, bool clicked)
{
    HBITMAP hbmbuffer = CreateCompatibleBitmap(hdstdc, RB_SIZE, RB_SIZE);
    HDC hdcbuffer = CreateCompatibleDC(hdstdc);

    SelectObject(hdcbuffer, hbmbuffer);
    drawDCResetButton(hdcbuffer, 0, 0, hbm, clicked);
    BitBlt(hdstdc, left, top, RB_SIZE, RB_SIZE, hdcbuffer, 0, 0, SRCCOPY);

    DeleteDC(hdcbuffer);
    DeleteObject(hbmbuffer);
}

void paintDCMapUnit(HDC hdstdc, int left, int top, BYTE mapunit)
{
    switch (GETMUSTATE(mapunit)) {
    case MUS_COVER:
        drawDCMUCover(hdstdc, left, top);
        break;
    case MUS_FLAG:
        drawDCMUFlag(hdstdc, left, top);
        break;
    case MUS_MARK:
        drawDCMUMark(hdstdc, left, top, false);
        break;
    case MUS_BARE:
        if (ISMUMINE(mapunit)) drawDCMUMine(hdstdc, left, top, false);
        else drawDCMUNum(hdstdc, left, top, GETMUNUMBER(mapunit));
        break;
    case MUS_BOMB:
        drawDCMUMine(hdstdc, left, top, true);
        break;
    case MUS_FALSE:
        drawDCMUFalse(hdstdc, left, top);
        break;
    default:
        drawDCMUCover(hdstdc, left, top);
        break;
    }
}

void paintDCMap(HDC hdstdc, int left, int top, PGameInfo pGame, PRECT pxyrect)
{
    RECT irect = { 0, 0, pGame->width, pGame->height };
    if (pxyrect) IntersectRect(&irect, &irect, pxyrect);
    for (int x = irect.left; x < irect.right; x++) {
        for (int y = irect.top; y < irect.bottom; y++) {
            paintDCMapUnit(hdstdc, left + x2px(x), top + y2py(y), pGame->map[xy2index(pGame, x, y)]);
        }
    }
}

void paintMapUnit(HDC hdstdc, int left, int top, BYTE mapunit)
{
    HBITMAP hbmbuffer = CreateCompatibleBitmap(hdstdc, MUP_SIZE, MUP_SIZE);
    HDC hdcbuffer = CreateCompatibleDC(hdstdc);

    SelectObject(hdcbuffer, hbmbuffer);
    paintDCMapUnit(hdcbuffer, 0, 0, mapunit);
    BitBlt(hdstdc, left, top, MUP_SIZE, MUP_SIZE, hdcbuffer, 0, 0, SRCCOPY);

    DeleteDC(hdcbuffer);
    DeleteObject(hbmbuffer);
}

void paintMap(HDC hdstdc, int left, int top, PGameInfo pGame)
{
    HBITMAP hbmbuffer = CreateCompatibleBitmap(hdstdc, MAP_WIDTH(pGame->width), MAP_HEIGHT(pGame->height));
    HDC hdcbuffer = CreateCompatibleDC(hdstdc);

    SelectObject(hdcbuffer, hbmbuffer);
    paintDCMap(hdcbuffer, 0, 0, pGame, NULL);
    BitBlt(hdstdc, left, top, MAP_WIDTH(pGame->width), MAP_HEIGHT(pGame->height), hdcbuffer, 0, 0, SRCCOPY);

    DeleteDC(hdcbuffer);
    DeleteObject(hbmbuffer);
}

void showSelectedMapUnit(HDC hdstdc, int mapleft, int maptop, PGameInfo pGame, int index, int last_index, bool area)
{
    if (!isidxinmap(pGame, index)) return;

    HBITMAP hbmbuffer = CreateCompatibleBitmap(hdstdc, MAP_WIDTH(pGame->width), MAP_HEIGHT(pGame->height));
    HDC hdcbuffer = CreateCompatibleDC(hdstdc);

    //get current content
    SelectObject(hdcbuffer, hbmbuffer);
    BitBlt(hdcbuffer, 0, 0, MAP_WIDTH(pGame->width), MAP_HEIGHT(pGame->height), hdstdc, mapleft, maptop, SRCCOPY);

    //reverse last selected state
    if (isidxinmap(pGame, last_index)) {
        Neighbor idxes;
        getNeighbors(pGame, idxes, index2x(pGame, last_index), index2y(pGame, last_index));
        for (int i = 0; i < NEI_TOTAL; i++) {
            int idx = idxes[i];
            if (idx == INV_INDEX) continue;
            paintDCMapUnit(hdcbuffer, index2px(pGame, idx), index2py(pGame, idx), pGame->map[idx]);
        }
    }

    //draw new selected state
    Neighbor idxes;
    getNeighbors(pGame, idxes, index2x(pGame, index), index2y(pGame, index));
    int cnt = (area) ? NEI_TOTAL : 1;
    for (int i = 0; i < cnt; i++) {
        int idx = idxes[i];
        if (idx == INV_INDEX) continue;
        BYTE mapunit_state = GETMUSTATE(pGame->map[idx]);
        if (mapunit_state == MUS_COVER)
            drawDCMUBare(hdcbuffer, index2px(pGame, idx), index2py(pGame, idx), false);
        else if (mapunit_state == MUS_MARK)
            drawDCMUMark(hdcbuffer, index2px(pGame, idx), index2py(pGame, idx), true);
    }

    //paste back to window
    BitBlt(hdstdc, mapleft, maptop, MAP_WIDTH(pGame->width), MAP_HEIGHT(pGame->height), hdcbuffer, 0, 0, SRCCOPY);

    DeleteDC(hdcbuffer);
    DeleteObject(hbmbuffer);
}


/* config(save) file management */

void getConfPath(LPTSTR path, DWORD size_ch)
{
    GetEnvironmentVariable(TEXT(CONF_PATHENV), path, size_ch);
    StringCchCat(path, size_ch, TEXT("\\"));
    StringCchCat(path, size_ch, TEXT(CONF_FNAME));
}

void loadGame(LPCTSTR path, PGameInfo pGame, PGameScore pScore, PPOINT pwndpos)
{
    //try to load last window position
    POINT pt = {
        GetPrivateProfileInt(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_XPOS), LONG_MAX, path),
        GetPrivateProfileInt(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_YPOS), LONG_MAX, path)
    };
    HMONITOR hmon = MonitorFromPoint(pt, MONITOR_DEFAULTTONULL);
    if (hmon) {
        MONITORINFO moni = { sizeof(MONITORINFO) };
        GetMonitorInfo(hmon, &moni);
        if (PtInRect(&moni.rcWork, pt)) *pwndpos = pt;
    }

    //try to load Game information, set to CRUSH if config data error
    int mode, width, height, mines, mark;
    mode = (BYTE)GetPrivateProfileInt(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_MODE), CRUSH, path);
    width = (BYTE)GetPrivateProfileInt(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_WIDTH), 0, path);
    height = (BYTE)GetPrivateProfileInt(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_HEIGHT), 0, path);
    mines = (WORD)GetPrivateProfileInt(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_MINES), 0, path);
    mark = (bool)GetPrivateProfileInt(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_MARK), 0, path);
    setGameMode(pGame, mode, width, height, mines);
    setMark(pGame, mark);
    resetGame(pGame);

    //try to load Record information, set to default if config data error
    pScore->junior_time = (WORD)GetPrivateProfileInt(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_JTIME), MAX_TIME, path);
    pScore->middle_time = (WORD)GetPrivateProfileInt(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_MTIME), MAX_TIME, path);
    pScore->senior_time = (WORD)GetPrivateProfileInt(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_STIME), MAX_TIME, path);
    GetPrivateProfileString(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_JNAME), TEXT(DEF_SCORE_NAME_EN), pScore->junior_name, SCORE_NAME_LEN, path);
    GetPrivateProfileString(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_MNAME), TEXT(DEF_SCORE_NAME_EN), pScore->middle_name, SCORE_NAME_LEN, path);
    GetPrivateProfileString(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_SNAME), TEXT(DEF_SCORE_NAME_EN), pScore->senior_name, SCORE_NAME_LEN, path);
}

void saveGame(LPCTSTR path, PGameInfo pGame, PGameScore pScore, PPOINT pwndpos)
{
    //convert number into string
    const int size_ch = 8;  //buff size for each number
    const int cnt = 10;     //numbers in total
    TCHAR str[10][8] = { 0 };
    LPTSTR lstr = (LPTSTR)str;
    LPCTSTR fmt = TEXT("%-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d");

    //the order of arguments is important
    StringCchPrintf(lstr, size_ch * cnt, fmt,
        pwndpos->x, pwndpos->y,
        pGame->mode, pGame->width, pGame->height, pGame->mines, pGame->mark,
        pScore->junior_time, pScore->middle_time, pScore->senior_time
    );
    for (int i = 0; i < size_ch * cnt; i++)
        lstr[i] *= (lstr[i] != TEXT(' '));

    //save window position
    WritePrivateProfileString(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_XPOS), str[0], path);
    WritePrivateProfileString(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_YPOS), str[1], path);

    //save Game information
    WritePrivateProfileString(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_MODE), str[2], path);
    WritePrivateProfileString(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_WIDTH), str[3], path);
    WritePrivateProfileString(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_HEIGHT), str[4], path);
    WritePrivateProfileString(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_MINES), str[5], path);
    WritePrivateProfileString(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_MARK), str[6], path);

    //save Record information
    WritePrivateProfileString(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_JTIME), str[7], path);
    WritePrivateProfileString(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_MTIME), str[8], path);
    WritePrivateProfileString(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_STIME), str[9], path);
    WritePrivateProfileString(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_JNAME), pScore->junior_name, path);
    WritePrivateProfileString(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_MNAME), pScore->middle_name, path);
    WritePrivateProfileString(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_SNAME), pScore->senior_name, path);
}


/* property related functions */

void loadProperty(LPTSTR prop, DWORD size_ch)
{
    memset(prop, 0, sizeof(TCHAR) * size_ch);

    TCHAR szAppFullPath[MAX_APPPATH] = { 0 };
    GetModuleFileName(NULL, szAppFullPath, MAX_APPPATH);    //get program module name and full path

    //get current exe-file version information length
    DWORD dwLen = GetFileVersionInfoSize(szAppFullPath, NULL);
    if (dwLen) {
        LPVOID pszAppVersion = malloc(dwLen);
        if (pszAppVersion) {
            memset(pszAppVersion, 0, dwLen);
            GetFileVersionInfo(szAppFullPath, 0, dwLen, pszAppVersion); //get version content

            UINT pnLen = 0, pvLen = 0, lcLen = 0;
            LPTSTR pProductName = NULL, pProductVersion = NULL, pLegalCopyright = NULL;
            //get specific version information, substrings do NOT need to free
            VerQueryValue(pszAppVersion, TEXT(PNQUERYSTR), (LPVOID*)&pProductName, &pnLen);
            VerQueryValue(pszAppVersion, TEXT(PVQUERYSTR), (LPVOID*)&pProductVersion, &pvLen);
            VerQueryValue(pszAppVersion, TEXT(LCQUERYSTR), (LPVOID*)&pLegalCopyright, &lcLen);
            if (pnLen && pvLen && lcLen) {
                //combine property description
                StringCchPrintf(prop, size_ch, TEXT("%s  %s\n%s\n\n%s"),
                    pProductName,
                    pProductVersion,
                    pLegalCopyright,
                    TEXT(LICNOTESTR)
                );
            }
            free(pszAppVersion);
        }
    }
}

void loadLicense(LPTSTR lic, DWORD size_ch)
{
    memset(lic, 0, sizeof(TCHAR) * size_ch);

    HRSRC hResID = FindResource(NULL, MAKEINTRESOURCE(IDR_LICENSE), TEXT(LICRESTYPE));
    if (hResID) {
        HGLOBAL hRes = LoadResource(NULL, hResID);
        DWORD dwResLen = SizeofResource(NULL, hResID);
        if (hRes) {
            LPVOID pRes = LockResource(hRes);
#ifdef UNICODE
            //convert license string to utf-16 encoding if use unicode
            MultiByteToWideChar(CP_UTF8, 0, pRes, dwResLen, lic, size_ch);
#else
            //convert license string to ANSI encoding if use multibyte
            LPWSTR wclic = malloc(sizeof(WCHAR) * MAX_LICSTR);
            if (wclic) {
                DWORD dwCCh = MultiByteToWideChar(CP_UTF8, 0, pRes, dwResLen, wclic, MAX_LICSTR);
                WideCharToMultiByte(CP_ACP, 0, wclic, dwCCh, lic, size_ch, NULL, NULL);
                free(wclic);
            }
#endif
        }
    }
}
