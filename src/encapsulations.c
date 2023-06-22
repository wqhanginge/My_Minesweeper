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

#include "encapsulations.h"


HBITMAP hbm_rb, hbm_click, hbm_fail, hbm_success;
HBITMAP hbm_current;


/* usefull functions */

int ppos2index(int px, int py)
{
	return xy2index(px2x(px), py2y(py));
}

int px2x(int px)
{
	return (px / MU_SIZE);
}

int py2y(int py)
{
	return (py / MU_SIZE);
}

int index2px(int index)
{
	return x2px(index2x(index));
}

int index2py(int index)
{
	return y2py(index2y(index));
}

int x2px(int x)
{
	return (x * MU_SIZE);
}

int y2py(int y)
{
	return (y * MU_SIZE);
}

bool lparamIsInRB(LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	p = (POINTS){ p.x - RB_LEFT,p.y - RB_TOP };
	return (p.x >= 0 && p.x < RB_SIZE&& p.y >= 0 && p.y < RB_SIZE);
}

bool lparamIsInMap(LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	p = (POINTS){ p.x - MAP_LEFT,p.y - MAP_TOP };
	return (p.x >= 0 && p.x < x2px(Game.width) && p.y >= 0 && p.y < y2py(Game.height));
}

int lparam2index(LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	return ppos2index(p.x - MAP_LEFT, p.y - MAP_TOP);
}



void loadBitmaps(HINSTANCE hinst)
{
	hbm_rb = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_RESET));
	hbm_click = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_CLICK));
	hbm_success = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_SUCCESS));
	hbm_fail = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_FAIL));
	hbm_current = hbm_rb;
}

void freeBitmaps()
{
	DeleteObject(hbm_rb);
	DeleteObject(hbm_click);
	DeleteObject(hbm_success);
	DeleteObject(hbm_fail);
}

void paintMapUnit(HDC hdestdc, int muleft, int mutop, int index)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, MU_SIZE, MU_SIZE);
	SelectObject(hdcbuffer, hbmbuffer);
	paintMapUnitNB(hdcbuffer, 0, 0, index);
	BitBlt(hdestdc, muleft, mutop, MU_SIZE, MU_SIZE, hdcbuffer, 0, 0, SRCCOPY);
	DeleteObject(hdcbuffer);
	DeleteObject(hbmbuffer);
}

void paintMapUnitNB(HDC hdestdc, int muleft, int mutop, int index)
{
	switch (GETMUSTATE(Game.map[index])) {
	case MUS_COVER:
		drawMUCoverBg(hdestdc, muleft, mutop);
		break;
	case MUS_FLAG:
		drawMUFlag(hdestdc, muleft, mutop);
		break;
	case MUS_MARK:
		drawMUMark(hdestdc, muleft, mutop, false);
		break;
	case MUS_UNCOV:
		if (MUISMINE(Game.map[index])) drawMUMine(hdestdc, muleft, mutop, false);
		else drawMUNum(hdestdc, muleft, mutop, GETMUMINES(Game.map[index]));
		break;
	case MUS_BOMB:
		drawMUMine(hdestdc, muleft, mutop, true);
		break;
	case MUS_WRONG:
		drawMUWrong(hdestdc, muleft, mutop);
		break;
	default:
		drawMUCoverBg(hdestdc, muleft, mutop);
		break;
	}
	REMMUUPDATE(Game.map[index]);
}

void paintMap(HDC hdestdc, int mapleft, int maptop, bool force)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, MAP_WIDTH, MAP_HEIGHT);
	SelectObject(hdcbuffer, hbmbuffer);
	BitBlt(hdcbuffer, 0, 0, MAP_WIDTH, MAP_HEIGHT, hdestdc, mapleft, maptop, SRCCOPY);
	paintMapNB(hdcbuffer, 0, 0, force);
	BitBlt(hdestdc, mapleft, maptop, MAP_WIDTH, MAP_HEIGHT, hdcbuffer, 0, 0, SRCCOPY);
	DeleteObject(hbmbuffer);
	DeleteObject(hdcbuffer);
}

void paintMapNB(HDC hdestdc, int mapleft, int maptop, bool force)
{
	for (word i = 0; i < Game.size; i++) {
		if (!force && !MUISUPDATE(Game.map[i])) continue;
		paintMapUnitNB(hdestdc, mapleft + index2px(i), maptop + index2py(i), i);
	}
}

void paintResetButton(HDC hdestdc, int rbleft, int rbtop, bool clicked)
{
	drawResetButton(hdestdc, rbleft, rbtop, hbm_current, clicked);
}

void setRBBitmap(HBITMAP hbm)
{
	hbm_current = hbm;
}


/* it needs to clear clicked state after mouse left prior position
 * first remove Update bit of clicked map_units,
 * then refresh the whole map,
 * then redraw clicked map_units,
 * finally set Update bit of clicked map_units
 */

void showClickedMapUnit(HDC hdestdc, int mapleft, int maptop, int index)
{
	if (index < 0 || index >= (int)Game.size) return;
	REMMUUPDATE(Game.map[index]);
	paintMap(hdestdc, mapleft, maptop, false);
	if (GETMUSTATE(Game.map[index]) == MUS_COVER)
		drawMUUncovBg(hdestdc, mapleft + index2px(index), maptop + index2py(index));
	else if (GETMUSTATE(Game.map[index]) == MUS_MARK)
		drawMUMark(hdestdc, mapleft + index2px(index), maptop + index2py(index), true);
	SETMUUPDATE(Game.map[index]);
}

void showClickedMapUnits(HDC hdestdc, int mapleft, int maptop, Neighbor* pindexes)
{
	for (word i = 0; i < 9; i++) {
		if ((*pindexes)[i] >= 0 && (*pindexes)[i] < (int)Game.size)
			REMMUUPDATE(Game.map[(*pindexes)[i]]);
	}
	paintMap(hdestdc, mapleft, maptop, false);
	for (word i = 0; i < 9; i++) {
		if ((*pindexes)[i] < 0 || (*pindexes)[i] >= (int)Game.size) continue;
		if (GETMUSTATE(Game.map[(*pindexes)[i]]) == MUS_COVER)
			drawMUUncovBg(hdestdc, mapleft + index2px((*pindexes)[i]), maptop + index2py((*pindexes)[i]));
		else if (GETMUSTATE(Game.map[(*pindexes)[i]]) == MUS_MARK)
			drawMUMark(hdestdc, mapleft + index2px((*pindexes)[i]), maptop + index2py((*pindexes)[i]), true);
	}
	for (word i = 0; i < 9; i++) {
		if ((*pindexes)[i] >= 0 && (*pindexes)[i] < (int)Game.size)
			SETMUUPDATE(Game.map[(*pindexes)[i]]);
	}
}


void initGame(TCHAR* Path, POINT* plastwndpos)
{
	plastwndpos->x = GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_XPOS), DEF_WND_LEFT, Path);
	plastwndpos->y = GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_YPOS), DEF_WND_TOP, Path);
	RECT desktop_rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &desktop_rect, 0);
	if ((dword)(plastwndpos->x - desktop_rect.left) > (dword)(desktop_rect.right - desktop_rect.left)
		|| (dword)(plastwndpos->y - desktop_rect.top) > (dword)(desktop_rect.bottom - desktop_rect.top))
		*plastwndpos = (POINT){ DEF_WND_LEFT,DEF_WND_TOP };

	int mode, width, height, mines, mark;
	mode = (byte)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_MODE), CRUSH, Path);
	width = (byte)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_WIDTH), 0, Path);
	height = (byte)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_HEIGHT), 0, Path);
	mines = (word)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_MINES), 0, Path);
	mark = (bool)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_MARK), 0, Path);
	setGameMode(mode, width, height, mines);
	setMark(mark);

	Score.junior_time = (word)GetPrivateProfileInt(TEXT(SCORE_ANAME), TEXT(SCORE_JTIME), MAX_TIME, Path);
	GetPrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_JNAME), TEXT(DEF_SCORE_NAME_EN), Score.junior_name, SCORE_NAME_LEN, Path);
	Score.middle_time = (word)GetPrivateProfileInt(TEXT(SCORE_ANAME), TEXT(SCORE_MTIME), MAX_TIME, Path);
	GetPrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_MNAME), TEXT(DEF_SCORE_NAME_EN), Score.middle_name, SCORE_NAME_LEN, Path);
	Score.senior_time = (word)GetPrivateProfileInt(TEXT(SCORE_ANAME), TEXT(SCORE_STIME), MAX_TIME, Path);
	GetPrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_SNAME), TEXT(DEF_SCORE_NAME_EN), Score.senior_name, SCORE_NAME_LEN, Path);
}

void saveGame(TCHAR* Path, POINT* pwndpos)
{
	TCHAR str[CONTENT_STRLEN];
	_sntprintf_s(str, CONTENT_STRLEN, _TRUNCATE, TEXT("%d"), pwndpos->x);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_XPOS), str, Path);
	_sntprintf_s(str, CONTENT_STRLEN, _TRUNCATE, TEXT("%d"), pwndpos->y);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_YPOS), str, Path);

	_sntprintf_s(str, CONTENT_STRLEN, _TRUNCATE, TEXT("%d"), Game.mode);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_MODE), str, Path);
	_sntprintf_s(str, CONTENT_STRLEN, _TRUNCATE, TEXT("%d"), Game.width);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_WIDTH), str, Path);
	_sntprintf_s(str, CONTENT_STRLEN, _TRUNCATE, TEXT("%d"), Game.height);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_HEIGHT), str, Path);
	_sntprintf_s(str, CONTENT_STRLEN, _TRUNCATE, TEXT("%d"), Game.mines);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_MINES), str, Path);
	_sntprintf_s(str, CONTENT_STRLEN, _TRUNCATE, TEXT("%d"), Game.mark);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_MARK), str, Path);

	_sntprintf_s(str, CONTENT_STRLEN, _TRUNCATE, TEXT("%d"), Score.junior_time);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_JTIME), str, Path);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_JNAME), Score.junior_name, Path);
	_sntprintf_s(str, CONTENT_STRLEN, _TRUNCATE, TEXT("%d"), Score.middle_time);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_MTIME), str, Path);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_MNAME), Score.middle_name, Path);
	_sntprintf_s(str, CONTENT_STRLEN, _TRUNCATE, TEXT("%d"), Score.senior_time);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_STIME), str, Path);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_SNAME), Score.senior_name, Path);
}

void getProperty(TCHAR* property, size_t size_in_ch)
{
	memset(property, 0, sizeof(TCHAR) * size_in_ch);
	TCHAR szAppFullPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szAppFullPath, MAX_PATH);	//get program module name and full path

	//get current exe-file version infomation
	DWORD dwLen = GetFileVersionInfoSize(szAppFullPath, NULL);
	if (dwLen) {
		void* pszAppVersion = malloc(dwLen);
		memset(pszAppVersion, 0, dwLen);
		GetFileVersionInfo(szAppFullPath, 0, dwLen, pszAppVersion);

		UINT pnLen = 0, pvLen = 0, lcLen = 0;
		TCHAR* pProductName = nullptr, * pProductVersion = nullptr, * pLegalCopyright = nullptr;
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
				TEXT(RIGHTSTR)
			);
		}
		free(pszAppVersion);
	}
}
