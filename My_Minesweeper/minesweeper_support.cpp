#include "minesweeper_support.h"

HINSTANCE hInst;
HWND hWnd;
HMENU hMenu;

HBITMAP hbm_resetb, hbm_click, hbm_fail, hbm_success;


bool lparamispos(LPARAM lparam) {
	POINTS p = MAKEPOINTS(lparam);
	p = { p.x - MAPUNITSLEFT,p.y - MAPUNITSTOP };
	return (p.x >= 0 && p.x < xtorleft(Game.width) && p.y >= 0 && p.y < ytortop(Game.height));
}

int lparamtoindex(LPARAM lparam) {
	POINTS p = MAKEPOINTS(lparam);
	return rpostoindex(p.x - MAPUNITSLEFT, p.y - MAPUNITSTOP);
}

int str2int(char *str, dword len, dword &x) {
	x = 0;
	for (dword i = 0; i < len && str[i] != '\0'; i++) {
		if (str[i] < '0' || str[i] > '9') return -1;
		x = x * 10 + str[i] - '0';
	}
	return 0;
}

int int2str(char *str, dword len, dword x) {
	int ret;
	if (len < 2) return 0;
	ret = (x >= 10) ? int2str(str, len, x / 10) : 0;
	if ((dword)ret < len - 1) {
		str[ret] = x % 10 + '0';
		ret++;
	}
	str[ret] = '\0';
	return ret;
}

void paintmap(HDC hdestdc, int muleft, int mutop) {
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, MAPUNITSIZE, MAPUNITSIZE);
	SelectObject(hdcbuffer, hbmbuffer);

	for (word i = 0; i < Game.size; i++) {
		if (!Game.mapchange[i]) continue;
		drawmapunitNB(hdcbuffer, 0, 0, i);
		BitBlt(hdestdc, muleft + indextorleft(i), mutop + indextortop(i), MAPUNITSIZE, MAPUNITSIZE, hdcbuffer, 0, 0, SRCCOPY);
	}
	//remove "need update sign"
	memset(Game.mapchange, 0, sizeof(bool)*Game.size);

	DeleteObject(hbmbuffer);
	DeleteObject(hdcbuffer);
}

void paintmapNB(HDC hdestdc, int muleft, int mutop) {
	for (word i = 0; i < Game.size; i++) {
		if (!Game.mapchange[i]) continue;
		drawmapunitNB(hdestdc, muleft + indextorleft(i), mutop + indextortop(i), i);
	}
	//remove "need update sign"
	memset(Game.mapchange, 0, sizeof(bool)*Game.size);
}

/*first unmark clicked units,
 *then refresh whole map,
 *then change clicked units bitmap,
 *finally mark clicked units
 */
void showclickdown(HDC hdestdc, int muleft, int mutop, Neighbor &indexes) {
	for (word i = 0; i < 9; i++)
		if (indexes[i] >= 0 && indexes[i] < (int)Game.size)
			Game.mapchange[indexes[i]] = false;
	paintmap(hdestdc, muleft, mutop);
	for (word i = 0; i < 9; i++) {
		if (indexes[i] < 0 || indexes[i] >= (int)Game.size) continue;
		if (GETUNITSTATE(Game.map[indexes[i]]) == UNITCOVER)
			drawmuuncovbg(hdestdc, muleft + indextorleft(indexes[i]), mutop + indextortop(indexes[i]));
		else if (GETUNITSTATE(Game.map[indexes[i]]) == UNITMARK)
			drawmumark(hdestdc, muleft + indextorleft(indexes[i]), mutop + indextortop(indexes[i]), true);
	}
	for (word i = 0; i < 9; i++)
		if (indexes[i] >= 0 && indexes[i] < (int)Game.size)
			Game.mapchange[indexes[i]] = true;
}

void showclickdown(HDC hdestdc, int muleft, int mutop, int index) {
	if (index < 0 || index >= (int)Game.size) return;
	Game.mapchange[index] = false;
	paintmap(hdestdc, muleft, mutop);
	if (GETUNITSTATE(Game.map[index]) == UNITCOVER)
		drawmuuncovbg(hdestdc, muleft + indextorleft(index), mutop + indextortop(index));
	else if (GETUNITSTATE(Game.map[index]) == UNITMARK)
		drawmumark(hdestdc, muleft + indextorleft(index), mutop + indextortop(index), true);
	Game.mapchange[index] = true;
}

void changechecked(byte GameMode) {
	MENUITEMINFO mii = { sizeof(MENUITEMINFO) };

	switch (GameMode) {
	case EASY:
		mii.fMask = MIIM_STATE;
		mii.fState = MFS_CHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_EASY, FALSE, &mii);
		mii.fState = MFS_UNCHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_NORMAL, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_HARD, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_CUSTOM, FALSE, &mii);
		break;
	case NORMAL:
		mii.fMask = MIIM_STATE;
		mii.fState = MFS_CHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_NORMAL, FALSE, &mii);
		mii.fState = MFS_UNCHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_EASY, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_HARD, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_CUSTOM, FALSE, &mii);
		break;
	case HARD:
		mii.fMask = MIIM_STATE;
		mii.fState = MFS_CHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_HARD, FALSE, &mii);
		mii.fState = MFS_UNCHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_EASY, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_NORMAL, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_CUSTOM, FALSE, &mii);
		break;
	case CUSTOM:
		mii.fMask = MIIM_STATE;
		mii.fState = MFS_CHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_CUSTOM, FALSE, &mii);
		mii.fState = MFS_UNCHECKED;
		SetMenuItemInfo(hMenu, ID_GAME_EASY, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_NORMAL, FALSE, &mii);
		SetMenuItemInfo(hMenu, ID_GAME_HARD, FALSE, &mii);
		break;
	default:
		break;
	}
}

void changemark(bool Mark) {
	MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
	mii.fMask = MIIM_STATE;
	mii.fState = (Mark) ? MFS_CHECKED : MFS_UNCHECKED;
	SetMenuItemInfo(hMenu, ID_GAME_MARK, FALSE, &mii);
}

dword besttime() {
	switch (Game.mode) {
	case EASY:		return Score.easytime;
	case NORMAL:	return Score.normaltime;
	case HARD:		return Score.hardtime;
	default:		break;
	}
	return 0;
}

char *bestname() {
	switch (Game.mode) {
	case EASY:		return Score.easyname;
	case NORMAL:	return Score.normalname;
	case HARD:		return Score.hardname;
	default:		break;
	}
	return nullptr;
}

int maketimestr(char * buffer, int size, dword time, const char * timeunit/*=DEFTIMEUNIT*/) {
	int ret;
	if (size < 2) return 0;
	//transform time value into c_string
	ret = int2str(buffer, size, time);
	if (ret == size - 1) return ret;
	//append time unit c_string
	strncat_s(buffer, size, timeunit, size - ret - 1);
	return (int)strlen(buffer);
}
