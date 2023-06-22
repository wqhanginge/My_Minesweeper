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
 * encapsulations.c
 *****************************************************************************
 * this file contains encapsulations of UI operations and IO functions
 * this file also contians some useful functions
 * 
 * this file contains several HBITMAP global variables which can be accessed
 * by external functions, they are used for drawing Reset Button,
 * DO NOT rewrite them directly, but use predefined functions(IMPORTANT)
 * 
 * NOTE:most functions do not have arg check process, use with care
\*****************************************************************************/


#include "encapsulations.h"


//bitmap handle, point to bitmaps which will be drawn on Reset Button
HBITMAP hbm_rb, hbm_click, hbm_fail, hbm_success;
//an argument to remember current using bitmap
HBITMAP hbm_current;


/* usefull functions */

//check if a mouse position is inside the ResetButton area
bool lparamIsInRB(LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	p = (POINTS){ p.x - RB_LEFT,p.y - RB_TOP };
	return (p.x >= 0 && p.x < RB_SIZE&& p.y >= 0 && p.y < RB_SIZE);
}

//check if a mouse position is inside the Map area
bool lparamIsInMap(LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	p = (POINTS){ p.x - MAP_LEFT,p.y - MAP_TOP };
	return (p.x >= 0 && p.x < x2px(Game.width) && p.y >= 0 && p.y < y2py(Game.height));
}

//change a mouse position to map index
int lparam2index(LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	return ppos2index(p.x - MAP_LEFT, p.y - MAP_TOP);
}



//manage bitmaps
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


//paint specific numbers on Num Part with default color
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


//paint ResetButton without changing its bitmap
void paintResetButton(HDC hdestdc, int left, int top, bool clicked)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, RB_SIZE, RB_SIZE);

	SelectObject(hdcbuffer, hbmbuffer);
	drawDCResetButton(hdcbuffer, 0, 0, hbm_current, clicked);
	BitBlt(hdestdc, left, top, RB_SIZE, RB_SIZE, hdcbuffer, 0, 0, SRCCOPY);

	DeleteDC(hdcbuffer);
	DeleteObject(hbmbuffer);
}

//change bitmap of ResetButton
void setRBBitmap(HBITMAP hbm)
{
	hbm_current = hbm;
}


//draw a mapunit depends on MapUnit data with default color
void paintMapUnit(HDC hdestdc, int muleft, int mutop, int index)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, MU_SIZE, MU_SIZE);

	SelectObject(hdcbuffer, hbmbuffer);
	drawDCMapUnit(hdcbuffer, 0, 0, Game.map[index]);
	REMMUUPDATE(Game.map[index]);
	BitBlt(hdestdc, muleft, mutop, MU_SIZE, MU_SIZE, hdcbuffer, 0, 0, SRCCOPY);

	DeleteDC(hdcbuffer);
	DeleteObject(hbmbuffer);
}

//paint GameMap, the mapleft-maptop is position 0
void paintMap(HDC hdestdc, int mapleft, int maptop, bool force)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, MAP_WIDTH, MAP_HEIGHT);

	SelectObject(hdcbuffer, hbmbuffer);
	//copy current UI content and draw new content on it
	BitBlt(hdcbuffer, 0, 0, MAP_WIDTH, MAP_HEIGHT, hdestdc, mapleft, maptop, SRCCOPY);
	for (word i = 0; i < Game.size; i++) {
		if (!force && !MUISUPDATE(Game.map[i])) continue;
		drawDCMapUnit(hdcbuffer, index2px(i), index2py(i), Game.map[i]);
		REMMUUPDATE(Game.map[i]);
	}
	BitBlt(hdestdc, mapleft, maptop, MAP_WIDTH, MAP_HEIGHT, hdcbuffer, 0, 0, SRCCOPY);

	DeleteDC(hdcbuffer);
	DeleteObject(hbmbuffer);
}


/* it needs to clear clicked state after mouse mapleft prior position
 * first remove Update bit of clicked map_units,
 * then refresh the whole map,
 * then redraw clicked map_units,
 * finally set Update bit of clicked map_units
 */

 //show clicked state when a MapUnit is clicked down
void showClickedMapUnit(HDC hdestdc, int mapleft, int maptop, int index)
{
	if (index < 0 || index >= (int)Game.size) return;

	REMMUUPDATE(Game.map[index]);
	paintMap(hdestdc, mapleft, maptop, false);
	if (GETMUSTATE(Game.map[index]) == MUS_COVER)
		drawDCMUUncov(hdestdc, mapleft + index2px(index), maptop + index2py(index));
	else if (GETMUSTATE(Game.map[index]) == MUS_MARK)
		drawDCMUMark(hdestdc, mapleft + index2px(index), maptop + index2py(index), true);
	SETMUUPDATE(Game.map[index]);
}

//show clicked state when a group of MapUnits are clicked down
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
			drawDCMUUncov(hdestdc, mapleft + index2px((*pindexes)[i]), maptop + index2py((*pindexes)[i]));
		else if (GETMUSTATE(Game.map[(*pindexes)[i]]) == MUS_MARK)
			drawDCMUMark(hdestdc, mapleft + index2px((*pindexes)[i]), maptop + index2py((*pindexes)[i]), true);
	}
	for (word i = 0; i < 9; i++) {
		if ((*pindexes)[i] >= 0 && (*pindexes)[i] < (int)Game.size)
			SETMUUPDATE(Game.map[(*pindexes)[i]]);
	}
}


//load information from a conf file
void initGame(TCHAR* Path, POINT* plastwndpos)
{
	//load last window position, use default position if conf data error or out of desktop
	plastwndpos->x = GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_XPOS), DEF_WND_LEFT, Path);
	plastwndpos->y = GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_YPOS), DEF_WND_TOP, Path);
	RECT desktop_rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &desktop_rect, 0);
	if ((dword)(plastwndpos->x - desktop_rect.left) > (dword)(desktop_rect.right - desktop_rect.left)
		|| (dword)(plastwndpos->y - desktop_rect.top) > (dword)(desktop_rect.bottom - desktop_rect.top))
		*plastwndpos = (POINT){ DEF_WND_LEFT,DEF_WND_TOP };

	//load Game information, use JUNIOR if conf data error
	int mode, width, height, mines, mark;
	mode = (byte)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_MODE), CRUSH, Path);
	width = (byte)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_WIDTH), 0, Path);
	height = (byte)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_HEIGHT), 0, Path);
	mines = (word)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_MINES), 0, Path);
	mark = (bool)GetPrivateProfileInt(TEXT(INIT_ANAME), TEXT(INIT_MARK), 0, Path);
	setGameMode(mode, width, height, mines);
	setMark(mark);

	//load Record information, use default if conf data error
	Score.junior_time = (word)GetPrivateProfileInt(TEXT(SCORE_ANAME), TEXT(SCORE_JTIME), MAX_TIME, Path);
	GetPrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_JNAME), TEXT(DEF_SCORE_NAME_EN), Score.junior_name, SCORE_NAME_LEN, Path);
	Score.middle_time = (word)GetPrivateProfileInt(TEXT(SCORE_ANAME), TEXT(SCORE_MTIME), MAX_TIME, Path);
	GetPrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_MNAME), TEXT(DEF_SCORE_NAME_EN), Score.middle_name, SCORE_NAME_LEN, Path);
	Score.senior_time = (word)GetPrivateProfileInt(TEXT(SCORE_ANAME), TEXT(SCORE_STIME), MAX_TIME, Path);
	GetPrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_SNAME), TEXT(DEF_SCORE_NAME_EN), Score.senior_name, SCORE_NAME_LEN, Path);
}

//save Game information into a conf file
void saveGame(TCHAR* Path, POINT* pwndpos)
{
	TCHAR str[STRBUFFERLEN];

	//save window position
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), pwndpos->x);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_XPOS), str, Path);
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), pwndpos->y);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_YPOS), str, Path);

	//save Game information
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), Game.mode);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_MODE), str, Path);
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), Game.width);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_WIDTH), str, Path);
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), Game.height);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_HEIGHT), str, Path);
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), Game.mines);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_MINES), str, Path);
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), Game.mark);
	WritePrivateProfileString(TEXT(INIT_ANAME), TEXT(INIT_MARK), str, Path);

	//save Record information
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), Score.junior_time);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_JTIME), str, Path);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_JNAME), Score.junior_name, Path);
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), Score.middle_time);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_MTIME), str, Path);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_MNAME), Score.middle_name, Path);
	_sntprintf_s(str, STRBUFFERLEN, _TRUNCATE, TEXT("%d"), Score.senior_time);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_STIME), str, Path);
	WritePrivateProfileString(TEXT(SCORE_ANAME), TEXT(SCORE_SNAME), Score.senior_name, Path);
}

//get program version information
void getProperty(TCHAR* property, size_t size_in_ch)
{
	memset(property, 0, sizeof(TCHAR) * size_in_ch);
	TCHAR szAppFullPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szAppFullPath, MAX_PATH);	//get program module name and full path

	//get current exe-file version information length
	DWORD dwLen = GetFileVersionInfoSize(szAppFullPath, NULL);
	if (dwLen) {
		void* pszAppVersion = malloc(dwLen);
		memset(pszAppVersion, 0, dwLen);
		GetFileVersionInfo(szAppFullPath, 0, dwLen, pszAppVersion);	//get version content

		UINT pnLen = 0, pvLen = 0, lcLen = 0;
		TCHAR* pProductName = nullptr, * pProductVersion = nullptr, * pLegalCopyright = nullptr;
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
