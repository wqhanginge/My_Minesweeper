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
 * interface.c
 *****************************************************************************
 * This file contains encapsulations of UI functions and IO operations.
 * This file also contians some useful transform functions.
 *
 * NOTE: Most functions do NOT check arguments, use with care.
\*****************************************************************************/


#include "stdafx.h"
#include "interface.h"


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

bool isLparamInRB(LPARAM lparam, BYTE n_mapw)
{
    POINTS p = MAKEPOINTS(lparam);
    p = (POINTS){ p.x - RB_LEFT(n_mapw), p.y - RB_TOP };
    return (p.x >= 0 && p.x < RB_SIZE && p.y >= 0 && p.y < RB_SIZE);
}

bool isLparamInMap(LPARAM lparam, BYTE n_mapw, BYTE n_maph)
{
    POINTS p = MAKEPOINTS(lparam);
    p = (POINTS){ p.x - MAP_LEFT, p.y - MAP_TOP };
    return (p.x >= 0 && p.x < MAP_WIDTH(n_mapw) && p.y >= 0 && p.y < MAP_HEIGHT(n_maph));
}

int lparam2index(PGameInfo pGame, LPARAM lparam)
{
    POINTS p = MAKEPOINTS(lparam);
    return ppos2index(pGame, p.x - MAP_LEFT, p.y - MAP_TOP);
}


/* structure operation functions */

void loadBitmaps(PRBHB pBitmap, HINSTANCE hinst)
{
    pBitmap->normal = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_RESET));
    pBitmap->click = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_CLICK));
    pBitmap->win = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_WIN));
    pBitmap->loss = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_LOSS));
}

void freeBitmaps(PRBHB pBitmap)
{
    DeleteObject(pBitmap->normal);
    DeleteObject(pBitmap->click);
    DeleteObject(pBitmap->win);
    DeleteObject(pBitmap->loss);
    memset(pBitmap, 0, sizeof(RBHB));
}

void initRuntime(PUIRT pRuntime, HBITMAP rb_curr)
{
    *pRuntime = (UIRT){ FALSE, rb_curr, false, false, false, false, INV_INDEX };
}


/* UI functions */

void paintMapUnit(HDC hdc, int left, int top, BYTE mapunit, bool clicked)
{
    switch (GETMUSTATE(mapunit)) {
    case MUS_COVER:
        if (clicked) drawMapUnitBare(hdc, left, top);
        else drawMapUnitCover(hdc, left, top);
        break;
    case MUS_BARE:
        if (ISMUMINE(mapunit)) drawMapUnitMine(hdc, left, top, false);
        else drawMapUnitNumber(hdc, left, top, GETMUNUMBER(mapunit));
        break;
    case MUS_FLAG:
        drawMapUnitFlag(hdc, left, top);
        break;
    case MUS_BOMB:
        drawMapUnitMine(hdc, left, top, true);
        break;
    case MUS_MARK:
        drawMapUnitMark(hdc, left, top, clicked);
        break;
    case MUS_FALSE:
        drawMapUnitFalse(hdc, left, top);
        break;
    default:
        drawMapUnitCover(hdc, left, top);
        break;
    }
}

void paintMap(HDC hdc, int left, int top, PGameInfo pGame)
{
    for (int idx = 0; idx < pGame->width * pGame->height; idx++) {
        paintMapUnit(hdc, left + index2px(pGame, idx), top + index2py(pGame, idx), pGame->map[idx], false);
    }
}


/* config(save) file management */

void getConfigPath(LPTSTR path, DWORD size_ch)
{
    GetEnvironmentVariable(TEXT(CONF_PATHENV), path, size_ch);
    StringCchCat(path, size_ch, TEXT("\\"));
    StringCchCat(path, size_ch, TEXT(CONF_FNAME));
}

void loadConfig(LPCTSTR path, PGameInfo pGame, PGameScore pScore, PPOINT pwndpos, PBOOL pmark)
{
    //try to load last window position and QuestionMark setting
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
    *pmark = (BOOL)GetPrivateProfileInt(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_MARK), FALSE, path);

    //try to load Game information, set to Crush if config data error
    int mode, width, height, mines;
    mode = (BYTE)GetPrivateProfileInt(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_MODE), GM_CRUSH, path);
    width = (BYTE)GetPrivateProfileInt(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_WIDTH), 0, path);
    height = (BYTE)GetPrivateProfileInt(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_HEIGHT), 0, path);
    mines = (WORD)GetPrivateProfileInt(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_MINES), 0, path);
    setGameMode(pGame, mode, width, height, mines);
    resetGame(pGame);

    //try to load Record information, set to default if config data error
    pScore->junior_time = (WORD)GetPrivateProfileInt(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_JTIME), MAX_TIME, path);
    pScore->middle_time = (WORD)GetPrivateProfileInt(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_MTIME), MAX_TIME, path);
    pScore->senior_time = (WORD)GetPrivateProfileInt(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_STIME), MAX_TIME, path);
    GetPrivateProfileString(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_JNAME), TEXT(SCORE_NAME_DEFT), pScore->junior_name, SCORE_NAME_LEN, path);
    GetPrivateProfileString(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_MNAME), TEXT(SCORE_NAME_DEFT), pScore->middle_name, SCORE_NAME_LEN, path);
    GetPrivateProfileString(TEXT(CKEY_SCORE_ANAME), TEXT(CKEY_SCORE_SNAME), TEXT(SCORE_NAME_DEFT), pScore->senior_name, SCORE_NAME_LEN, path);
}

void saveConfig(LPCTSTR path, PGameInfo pGame, PGameScore pScore, POINT wndpos, BOOL mark)
{
    //convert number into string
    const int size_ch = 8;  //buff size for each number
    const int cnt = 10;     //numbers in total
    TCHAR str[10][8] = { 0 };
    LPTSTR lstr = (LPTSTR)str;
    LPCTSTR fmt = TEXT("%-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d");

    //the order of arguments matters
    StringCchPrintf(lstr, size_ch * cnt, fmt,
        wndpos.x, wndpos.y, mark,
        pGame->mode, pGame->width, pGame->height, pGame->mines,
        pScore->junior_time, pScore->middle_time, pScore->senior_time
    );
    for (int i = 0; i < size_ch * cnt; i++) lstr[i] *= (lstr[i] != TEXT(' '));

    //save window position and QuestionMark setting
    WritePrivateProfileString(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_XPOS), str[0], path);
    WritePrivateProfileString(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_YPOS), str[1], path);
    WritePrivateProfileString(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_MARK), str[2], path);

    //save Game information
    WritePrivateProfileString(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_MODE), str[3], path);
    WritePrivateProfileString(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_WIDTH), str[4], path);
    WritePrivateProfileString(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_HEIGHT), str[5], path);
    WritePrivateProfileString(TEXT(CKEY_INIT_ANAME), TEXT(CKEY_INIT_MINES), str[6], path);

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

    //get current executable version information length
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
