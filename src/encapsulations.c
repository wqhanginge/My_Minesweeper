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
 * encapsulations.c
 *****************************************************************************
 * This file contains encapsulations of UI operations and IO functions.
 * This file contains a struct and relative functions for bitmap management.
 * This file also contians some useful transform functions.
 * 
 * NOTE: Most functions do NOT have arg check process, use with care.
\*****************************************************************************/


#include "encapsulations.h"


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

bool lparamIsInRB(LPARAM lparam, BYTE map_width)
{
	POINTS p = MAKEPOINTS(lparam);
	p = (POINTS){ p.x - RB_LEFT(map_width),p.y - RB_TOP};
	return (p.x >= 0 && p.x < RB_SIZE && p.y >= 0 && p.y < RB_SIZE);
}

bool lparamIsInMap(LPARAM lparam, BYTE map_width, BYTE map_height)
{
	POINTS p = MAKEPOINTS(lparam);
	p = (POINTS){ p.x - MAP_LEFT,p.y - MAP_TOP };
	return (p.x >= 0 && p.x < x2px(map_width) && p.y >= 0 && p.y < y2py(map_height));
}

int lparam2index(PGameInfo pGame, LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	return ppos2index(pGame, p.x - MAP_LEFT, p.y - MAP_TOP);
}


void loadBitmaps(PRBHBM pRBhbm, HINSTANCE hinst)
{
	pRBhbm->normal = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_RESET));
	pRBhbm->click = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_CLICK));
	pRBhbm->success = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_SUCCESS));
	pRBhbm->fail = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_FAIL));
	pRBhbm->current = pRBhbm->normal;
}

void freeBitmaps(PRBHBM pRBhbm)
{
	DeleteObject(pRBhbm->normal);
	DeleteObject(pRBhbm->click);
	DeleteObject(pRBhbm->success);
	DeleteObject(pRBhbm->fail);
	DeleteObject(pRBhbm->current);
	*pRBhbm = (RBHBM){0};
}

void setCurrBitmap(PRBHBM pRBhbm, HBITMAP hbm)
{
	pRBhbm->current = hbm;
}


void paintINums(HDC hdestdc, int left, int top, int num)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, INUMS_WIDTH, INUMS_HEIGHT);

	SelectObject(hdcbuffer, hbmbuffer);
	drawDCINums(hdcbuffer, 0, 0, num);
	BitBlt(hdestdc, left, top, INUMS_WIDTH, INUMS_HEIGHT, hdcbuffer, 0, 0, SRCCOPY);

	DeleteDC(hdcbuffer);
	DeleteObject(hbmbuffer);
}

void paintResetButton(HDC hdestdc, int left, int top, HBITMAP hbm, bool clicked)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, RB_SIZE, RB_SIZE);

	SelectObject(hdcbuffer, hbmbuffer);
	drawDCResetButton(hdcbuffer, 0, 0, hbm, clicked);
	BitBlt(hdestdc, left, top, RB_SIZE, RB_SIZE, hdcbuffer, 0, 0, SRCCOPY);

	DeleteDC(hdcbuffer);
	DeleteObject(hbmbuffer);
}

void paintDCMapUnit(HDC hdestdc, int left, int top, BYTE mapunit)
{
	switch (GETMUSTATE(mapunit)) {
	case MUS_COVER:
		drawDCMUCover(hdestdc, left, top);
		break;
	case MUS_FLAG:
		drawDCMUFlag(hdestdc, left, top);
		break;
	case MUS_MARK:
		drawDCMUMark(hdestdc, left, top, false);
		break;
	case MUS_UNCOV:
		if (MUISMINE(mapunit)) drawDCMUMine(hdestdc, left, top, false);
		else drawDCMUNum(hdestdc, left, top, GETMUMINES(mapunit));
		break;
	case MUS_BOMB:
		drawDCMUMine(hdestdc, left, top, true);
		break;
	case MUS_WRONG:
		drawDCMUWrong(hdestdc, left, top);
		break;
	default:
		drawDCMUCover(hdestdc, left, top);
		break;
	}
}

void paintDCMap(HDC hdestdc, int left, int top, PGameInfo pGame)
{
	for (WORD i = 0; i < pGame->size; i++) {
		paintDCMapUnit(hdestdc, left + index2px(pGame, i), top + index2py(pGame, i), pGame->map[i]);
	}
}

void paintMapUnit(HDC hdestdc, int muleft, int mutop, BYTE mapunit)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, MU_SIZE, MU_SIZE);

	SelectObject(hdcbuffer, hbmbuffer);
	paintDCMapUnit(hdcbuffer, 0, 0, mapunit);
	BitBlt(hdestdc, muleft, mutop, MU_SIZE, MU_SIZE, hdcbuffer, 0, 0, SRCCOPY);

	DeleteDC(hdcbuffer);
	DeleteObject(hbmbuffer);
}

void paintMap(HDC hdestdc, int mapleft, int maptop, PGameInfo pGame)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, MAP_WIDTH(pGame->width), MAP_HEIGHT(pGame->height));

	SelectObject(hdcbuffer, hbmbuffer);
	paintDCMap(hdcbuffer, 0, 0, pGame);
	BitBlt(hdestdc, mapleft, maptop, MAP_WIDTH(pGame->width), MAP_HEIGHT(pGame->height), hdcbuffer, 0, 0, SRCCOPY);

	DeleteDC(hdcbuffer);
	DeleteObject(hbmbuffer);
}

void showClickedMapUnit(HDC hdestdc, int mapleft, int maptop, PGameInfo pGame, int index)
{
	if (!isidxinmap(pGame, index)) return;

	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, MAP_WIDTH(pGame->width), MAP_HEIGHT(pGame->height));

	SelectObject(hdcbuffer, hbmbuffer);
	paintDCMap(hdcbuffer, 0, 0, pGame);
	if (GETMUSTATE(pGame->map[index]) == MUS_COVER)
		drawDCMUUncov(hdcbuffer, index2px(pGame, index), index2py(pGame, index));
	else if (GETMUSTATE(pGame->map[index]) == MUS_MARK)
		drawDCMUMark(hdcbuffer, index2px(pGame, index), index2py(pGame, index), true);
	BitBlt(hdestdc, mapleft, maptop, MAP_WIDTH(pGame->width), MAP_HEIGHT(pGame->height), hdcbuffer, 0, 0, SRCCOPY);

	DeleteDC(hdcbuffer);
	DeleteObject(hbmbuffer);
}

void showClickedMapUnits(HDC hdestdc, int mapleft, int maptop, PGameInfo pGame, Neighbor indexes)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, MAP_WIDTH(pGame->width), MAP_HEIGHT(pGame->height));

	SelectObject(hdcbuffer, hbmbuffer);
	paintDCMap(hdcbuffer, 0, 0, pGame);
	for (WORD i = 0; i < 9; i++) {
		int index = indexes[i];
		if (!isidxinmap(pGame, index)) continue;
		if (GETMUSTATE(pGame->map[index]) == MUS_COVER)
			drawDCMUUncov(hdcbuffer, index2px(pGame, index), index2py(pGame, index));
		else if (GETMUSTATE(pGame->map[index]) == MUS_MARK)
			drawDCMUMark(hdcbuffer, index2px(pGame, index), index2py(pGame, index), true);
	}
	BitBlt(hdestdc, mapleft, maptop, MAP_WIDTH(pGame->width), MAP_HEIGHT(pGame->height), hdcbuffer, 0, 0, SRCCOPY);

	DeleteDC(hdcbuffer);
	DeleteObject(hbmbuffer);
}


void initGame(LPCTSTR Path, PGameInfo pGame, PGameScore pScore, PPOINT plastwndpos)
{
	//load last window position, use default position if config data error or out of desktop
	plastwndpos->x = GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_XPOS), DEF_WND_LEFT, Path);
	plastwndpos->y = GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_YPOS), DEF_WND_TOP, Path);
	RECT desktop_rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &desktop_rect, 0);
	if ((DWORD)(plastwndpos->x - desktop_rect.left) > (DWORD)(desktop_rect.right - desktop_rect.left)
		|| (DWORD)(plastwndpos->y - desktop_rect.top) > (DWORD)(desktop_rect.bottom - desktop_rect.top))
		*plastwndpos = (POINT){ DEF_WND_LEFT,DEF_WND_TOP };

	//load Game information, use JUNIOR if config data error
	int mode, width, height, mines, mark;
	mode = (BYTE)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_MODE), CRUSH, Path);
	width = (BYTE)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_WIDTH), 0, Path);
	height = (BYTE)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_HEIGHT), 0, Path);
	mines = (WORD)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_MINES), 0, Path);
	mark = (bool)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_MARK), 0, Path);
	setGameMode(pGame, mode, width, height, mines);
	setMark(pGame, mark);

	//load Record information, use default if config data error
	pScore->junior_time = (WORD)GetPrivateProfileInt(TEXT(SCORE_ANAME), TEXT(SCORE_JTIME), MAX_TIME, Path);
	pScore->middle_time = (WORD)GetPrivateProfileInt(TEXT(SCORE_ANAME), TEXT(SCORE_MTIME), MAX_TIME, Path);
	pScore->senior_time = (WORD)GetPrivateProfileInt(TEXT(SCORE_ANAME), TEXT(SCORE_STIME), MAX_TIME, Path);
	GetPrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_JNAME), TEXT(DEF_SCORE_NAME_EN), pScore->junior_name, SCORE_NAME_LEN, Path);
	GetPrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_MNAME), TEXT(DEF_SCORE_NAME_EN), pScore->middle_name, SCORE_NAME_LEN, Path);
	GetPrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_SNAME), TEXT(DEF_SCORE_NAME_EN), pScore->senior_name, SCORE_NAME_LEN, Path);
}

void saveGame(LPCTSTR Path, PGameInfo pGame, PGameScore pScore, PPOINT pwndpos)
{
	TCHAR str[STRBUFFERLEN];

	//save window position
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), pwndpos->x);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_XPOS), str, Path);
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), pwndpos->y);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_YPOS), str, Path);

	//save Game information
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), pGame->mode);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_MODE), str, Path);
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), pGame->width);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_WIDTH), str, Path);
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), pGame->height);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_HEIGHT), str, Path);
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), pGame->mines);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_MINES), str, Path);
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), pGame->mark);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_MARK), str, Path);

	//save Record information
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), pScore->junior_time);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_JTIME), str, Path);
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), pScore->middle_time);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_MTIME), str, Path);
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), pScore->senior_time);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_STIME), str, Path);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_JNAME), pScore->junior_name, Path);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_MNAME), pScore->middle_name, Path);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_SNAME), pScore->senior_name, Path);
}

void getProperty(LPTSTR property, size_t size_in_ch)
{
	memset(property, 0, sizeof(TCHAR) * size_in_ch);
	TCHAR szAppFullPath[MAX_APPPATH] = { 0 };
	GetModuleFileName(NULL, szAppFullPath, MAX_APPPATH);	//get program module name and full path

	//get current exe-file version information length
	DWORD dwLen = GetFileVersionInfoSize(szAppFullPath, NULL);
	if (dwLen) {
		void* pszAppVersion = malloc(dwLen);
		memset(pszAppVersion, 0, dwLen);
		GetFileVersionInfo(szAppFullPath, 0, dwLen, pszAppVersion);	//get version content

		UINT pnLen = 0, pvLen = 0, lcLen = 0;
		LPTSTR pProductName = NULL, pProductVersion = NULL, pLegalCopyright = NULL;
		//get specific version information
		VerQueryValue(pszAppVersion, TEXT(PNQUERYSTR), (LPVOID*)&pProductName, &pnLen);
		VerQueryValue(pszAppVersion, TEXT(PVQUERYSTR), (LPVOID*)&pProductVersion, &pvLen);
		VerQueryValue(pszAppVersion, TEXT(LCQUERYSTR), (LPVOID*)&pLegalCopyright, &lcLen);
		if (pnLen && pvLen && lcLen)
		{
			//combine property description
			_sntprintf_s(property, size_in_ch, _TRUNCATE, TEXT("%s  %s\n%s\n\n%s"),
				pProductName,
				pProductVersion,
				pLegalCopyright,
				TEXT(COPYRIGHTSTR)
			);
		}
		free(pszAppVersion);
	}
}
