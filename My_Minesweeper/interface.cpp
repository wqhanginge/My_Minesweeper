#include "interface.h"

bool lparamispos(LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	p = { p.x - MAPUNITSLEFT,p.y - MAPUNITSTOP };
	return (p.x >= 0 && p.x < xtorleft(Game.width) && p.y >= 0 && p.y < ytortop(Game.height));
}

int lparamtoindex(LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	return rpostoindex(p.x - MAPUNITSLEFT, p.y - MAPUNITSTOP);
}

dword str2dword(TCHAR *str, dword len, dword &x)
{
	x = 0;
	for (dword i = 0; i < len && str[i] != TEXT('\0'); i++) {
		if (str[i] < TEXT('0') || str[i] > TEXT('9')) return -1;
		x = x * 10 + str[i] - TEXT('0');
	}
	return 0;
}

int dword2str(TCHAR *str, dword len, dword x)
{
	int ret;
	if (len < 2) return 0;
	ret = (x >= 10) ? dword2str(str, len, x / 10) : 0;
	if ((dword)ret < len - 1) {
		str[ret] = x % 10 + TEXT('0');
		ret++;
	}
	str[ret] = TEXT('\0');
	return ret;
}

void paintmap(HDC hdestdc, int muleft, int mutop)
{
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

void paintmapNB(HDC hdestdc, int muleft, int mutop)
{
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
void showclickdown(HDC hdestdc, int muleft, int mutop, Neighbor &indexes)
{
	for (word i = 0; i < 9; i++)
		if (indexes[i] >= 0 && indexes[i] < (int)Game.size)
			Game.mapchange[indexes[i]] = false;
	paintmap(hdestdc, muleft, mutop);
	for (word i = 0; i < 9; i++) {
		if (indexes[i] < 0 || indexes[i] >= (int)Game.size) continue;
		if (GETMUSTATE(Game.map[indexes[i]]) == MUS_COVER)
			drawmuuncovbg(hdestdc, muleft + indextorleft(indexes[i]), mutop + indextortop(indexes[i]));
		else if (GETMUSTATE(Game.map[indexes[i]]) == MUS_MARK)
			drawmumark(hdestdc, muleft + indextorleft(indexes[i]), mutop + indextortop(indexes[i]), true);
	}
	for (word i = 0; i < 9; i++)
		if (indexes[i] >= 0 && indexes[i] < (int)Game.size)
			Game.mapchange[indexes[i]] = true;
}

void showclickdown(HDC hdestdc, int muleft, int mutop, int index)
{
	if (index < 0 || index >= (int)Game.size) return;
	Game.mapchange[index] = false;
	paintmap(hdestdc, muleft, mutop);
	if (GETMUSTATE(Game.map[index]) == MUS_COVER)
		drawmuuncovbg(hdestdc, muleft + indextorleft(index), mutop + indextortop(index));
	else if (GETMUSTATE(Game.map[index]) == MUS_MARK)
		drawmumark(hdestdc, muleft + indextorleft(index), mutop + indextortop(index), true);
	Game.mapchange[index] = true;
}

void changechecked(byte GameMode)
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

void changemark(bool Mark)
{
	MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
	mii.fMask = MIIM_STATE;
	mii.fState = (Mark) ? MFS_CHECKED : MFS_UNCHECKED;
	SetMenuItemInfo(hMenu, ID_GAME_MARK, FALSE, &mii);
}

int maketimestr(TCHAR * buffer, int size, dword time, const TCHAR * timeunit/*=DEFTIMEUNITEN*/)
{
	int ret;
	if (size < 2) return 0;
	//transform time value into c_string
	ret = dword2str(buffer, size, time);
	if (ret == size - 1) return ret;
	//append time unit c_string
	_tcsncat_s(buffer, size, timeunit, size - ret - 1);
	return (int)_tcslen(buffer);
}

void initgame(TCHAR * Path, POINT &lastwndpos)
{
	lastwndpos.x = GetPrivateProfileInt(TEXT(INITAPPNAME), TEXT("xpos"), DEFWNDLEFT, Path);
	lastwndpos.y = GetPrivateProfileInt(TEXT(INITAPPNAME), TEXT("ypos"), DEFWNDTOP, Path);
	Game.mode = (byte)GetPrivateProfileInt(TEXT(INITAPPNAME), TEXT("mode"), CRUSH, Path);
	Game.width = (byte)GetPrivateProfileInt(TEXT(INITAPPNAME), TEXT("width"), 0, Path);
	Game.height = (byte)GetPrivateProfileInt(TEXT(INITAPPNAME), TEXT("height"), 0, Path);
	Game.mines = (word)GetPrivateProfileInt(TEXT(INITAPPNAME), TEXT("mines"), 0, Path);
	Game.mark = (bool)GetPrivateProfileInt(TEXT(INITAPPNAME), TEXT("mark"), 0, Path);
	setgamemode(Game.mode, Game.width, Game.height, Game.mines);
	Score.juniortime = (word)GetPrivateProfileInt(TEXT(SCOREAPPNAME), TEXT("timej"), MAXTIME, Path);
	GetPrivateProfileString(TEXT(SCOREAPPNAME), TEXT("namej"), TEXT(DEFSCORENAMEEN), Score.juniorname, SCORENAMELEN, Path);
	Score.middletime = (word)GetPrivateProfileInt(TEXT(SCOREAPPNAME), TEXT("timem"), MAXTIME, Path);
	GetPrivateProfileString(TEXT(SCOREAPPNAME), TEXT("namem"), TEXT(DEFSCORENAMEEN), Score.middlename, SCORENAMELEN, Path);
	Score.seniortime = (word)GetPrivateProfileInt(TEXT(SCOREAPPNAME), TEXT("times"), MAXTIME, Path);
	GetPrivateProfileString(TEXT(SCOREAPPNAME), TEXT("names"), TEXT(DEFSCORENAMEEN), Score.seniorname, SCORENAMELEN, Path);
}

void savegame(TCHAR * Path, POINT &wndpos)
{
	TCHAR str[5];
	dword2str(str, wndpos.x);
	WritePrivateProfileString(TEXT(INITAPPNAME), TEXT("xpos"), str, Path);
	dword2str(str, wndpos.y);
	WritePrivateProfileString(TEXT(INITAPPNAME), TEXT("ypos"), str, Path);
	dword2str(str, Game.mode);
	WritePrivateProfileString(TEXT(INITAPPNAME), TEXT("mode"), str, Path);
	dword2str(str, Game.width);
	WritePrivateProfileString(TEXT(INITAPPNAME), TEXT("width"), str, Path);
	dword2str(str, Game.height);
	WritePrivateProfileString(TEXT(INITAPPNAME), TEXT("height"), str, Path);
	dword2str(str, Game.mines);
	WritePrivateProfileString(TEXT(INITAPPNAME), TEXT("mines"), str, Path);
	dword2str(str, Game.mark);
	WritePrivateProfileString(TEXT(INITAPPNAME), TEXT("mark"), str, Path);
	dword2str(str, Score.juniortime);
	WritePrivateProfileString(TEXT(SCOREAPPNAME), TEXT("timej"), str, Path);
	WritePrivateProfileString(TEXT(SCOREAPPNAME), TEXT("namej"), Score.juniorname, Path);
	dword2str(str, Score.middletime);
	WritePrivateProfileString(TEXT(SCOREAPPNAME), TEXT("timem"), str, Path);
	WritePrivateProfileString(TEXT(SCOREAPPNAME), TEXT("namem"), Score.middlename, Path);
	dword2str(str, Score.seniortime);
	WritePrivateProfileString(TEXT(SCOREAPPNAME), TEXT("times"), str, Path);
	WritePrivateProfileString(TEXT(SCOREAPPNAME), TEXT("names"), Score.seniorname, Path);
}
