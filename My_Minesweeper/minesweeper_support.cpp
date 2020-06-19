#include "minesweeper_support.h"

HINSTANCE hInst;
HWND hWnd;
HMENU hMenu;
HWND hInfo, hMapArea;
HWND hMine, hTime;

HBITMAP hbm_reset, hbm_click, hbm_fail, hbm_success;
HBITMAP hbm_infonums[BM_INFONUMS_COUNT];
HBITMAP hbm_unit, hbm_unitflag, hbm_unitmark;
HBITMAP hbm_mapnums[BM_MAPNUMS_COUNT];
HBITMAP hbm_unitmine, hbm_unitbomb, hbm_unitwrong, hbm_unitmarkdown;
HBITMAP &hbm_unitdown = hbm_mapnums[0];


int lparamtopos(LPARAM lparam) {
	POINTS p;
	p = MAKEPOINTS(lparam);
	return xytoindex(p.x / BM_MAPUNIT_SIZEM, p.y / BM_MAPUNIT_SIZEM);
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

void loadbitmaps() {
	hbm_reset = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RESETBUTTON));
	hbm_click = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CLICK));
	hbm_fail = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FAIL));
	hbm_success = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SUCCESS));
	for (int i = 0; i < BM_INFONUMS_COUNT; i++)
		hbm_infonums[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NUM0 + i));
	hbm_unit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_UNIT));
	hbm_unitflag = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_UNITFLAG));
	hbm_unitmark = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_UNITMARK));
	for (int i = 0; i < BM_MAPNUMS_COUNT; i++)
		hbm_mapnums[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_UNITNULL + i));
	hbm_unitmine = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_UNITMINE));
	hbm_unitbomb = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_UNITBOMB));
	hbm_unitwrong = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_UNITWRONG));
	hbm_unitmarkdown = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_UNITMARKDOWN));
}

void freebitmaps() {
	DeleteObject(hbm_reset);
	DeleteObject(hbm_click);
	DeleteObject(hbm_fail);
	DeleteObject(hbm_success);
	for (int i = 0; i < BM_INFONUMS_COUNT; i++)
		DeleteObject(hbm_infonums[i]);
	DeleteObject(hbm_unit);
	DeleteObject(hbm_unitflag);
	DeleteObject(hbm_unitmark);
	for (int i = 0; i < BM_MAPNUMS_COUNT; i++)
		DeleteObject(hbm_mapnums[i]);
	DeleteObject(hbm_unitmine);
	DeleteObject(hbm_unitbomb);
	DeleteObject(hbm_unitwrong);
	DeleteObject(hbm_unitmarkdown);
}

int infow() {
	switch (Game.mode) {
	case EASY:		return IE_WM;
	case NORMAL:	return IN_WM;
	case HARD:		return IH_WM;
	case CUSTOM:	return (Game.width * BM_MAPUNIT_SIZEM + DDLGEDGE);
	default:		break;
	}
	return IE_WM;
}

int resetbuttonx() {
	switch (Game.mode) {
	case EASY:		return RESETBE_LEFTM;
	case NORMAL:	return RESETBN_LEFTM;
	case HARD:		return RESETBH_LEFTM;
	case CUSTOM:	return ((Game.width * BM_MAPUNIT_SIZEM - RESETB_SIZE) / 2);
	default:		break;
	}
	return RESETBE_LEFTM;
}

int timedlgx() {
	switch (Game.mode) {
	case EASY:		return TIMEE_LEFTM;
	case NORMAL:	return TIMEN_LEFTM;
	case HARD:		return TIMEH_LEFTM;
	case CUSTOM:	return (Game.width * BM_MAPUNIT_SIZEM - TIME_ROFFSET);
	default:		break;
	}
	return TIMEE_LEFTM;
}

int mapareaw() {
	switch (Game.mode) {
	case EASY:		return MAE_WM;
	case NORMAL:	return MAN_WM;
	case HARD:		return MAH_WM;
	case CUSTOM:	return (Game.width * BM_MAPUNIT_SIZEM + DDLGEDGE);
	default:		break;
	}
	return MAE_WM;
}

int mapareah() {
	switch (Game.mode) {
	case EASY:		return MAE_HM;
	case NORMAL:	return MAN_HM;
	case HARD:		return MAH_HM;
	case CUSTOM:	return (Game.height * BM_MAPUNIT_SIZEM + DDLGEDGE);
	default:		break;
	}
	return MAE_HM;
}

HWND *createstatics(HWND hparent) {
	HWND *hstatic = new HWND[Game.size];
	for (word i = 0; i < Game.size; i++) {
		hstatic[i] = CreateWindowEx(0, TEXT("STATIC"), NULL,
			WS_CHILD | WS_VISIBLE | SS_BITMAP,
			(i % Game.width)*BM_MAPUNIT_SIZEM, (i / Game.width)*BM_MAPUNIT_SIZEM,
			BM_MAPUNIT_SIZEM, BM_MAPUNIT_SIZEM,
			hparent,
			NULL,
			hInst,
			NULL);
		SendMessage(hstatic[i], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unit);
	}
	return hstatic;
}

void deletestatics(HWND **hstatic, word size) {
	for (word i = 0; i < size; i++) DestroyWindow((*hstatic)[i]);
	delete[] *hstatic;
	*hstatic = nullptr;
}

void paintmap(HWND hstatic[]) {
	for (word i = 0; i < Game.size; i++) {
		if (!Game.mapchange[i]) continue;
		switch (GETUNITSTATE(Game.map[i])) {
		case UNITCOVER:
			SendMessage(hstatic[i], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unit);
			break;
		case UNITFLAG:
			SendMessage(hstatic[i], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unitflag);
			break;
		case UNITMARK:
			SendMessage(hstatic[i], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unitmark);
			break;
		case UNITUNCOV:
			if (UNITISMINE(Game.map[i]))
				SendMessage(hstatic[i], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unitmine);
			else
				SendMessage(hstatic[i], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_mapnums[GETUNITMINES(Game.map[i])]);
			break;
		case UNITBOMB:
			SendMessage(hstatic[i], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unitbomb);
			break;
		case UNITWRONG:
			SendMessage(hstatic[i], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unitwrong);
			break;
		default:
			break;
		}
	}
	//remove "need update sign"
	memset(Game.mapchange, 0, sizeof(bool)*Game.size);
}

/*first unmark clicked units,
 *then refresh whole map,
 *then change clicked units bitmap,
 *finally mark clicked units
 */
void showclickdown(HWND hstatic[], Neighbor &poses) {
	for (word i = 0; i < 9; i++)
		if (poses[i] >= 0 && poses[i] < (int)Game.size)
			Game.mapchange[poses[i]] = false;
	paintmap(hstatic);
	for (word i = 0; i < 9; i++) {
		if (poses[i] < 0 || poses[i] >= (int)Game.size) continue;
		if (GETUNITSTATE(Game.map[poses[i]]) == UNITCOVER)
			PostMessage(hstatic[poses[i]], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unitdown);
		else if (GETUNITSTATE(Game.map[poses[i]]) == UNITMARK)
			PostMessage(hstatic[poses[i]], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unitmarkdown);
	}
	for (word i = 0; i < 9; i++)
		if (poses[i] >= 0 && poses[i] < (int)Game.size)
			Game.mapchange[poses[i]] = true;
}

void showclickdown(HWND hstatic[], int pos) {
	if (pos < 0 || pos >= (int)Game.size) return;
	Game.mapchange[pos] = false;
	paintmap(hstatic);
	if (GETUNITSTATE(Game.map[pos]) == UNITCOVER)
		PostMessage(hstatic[pos], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unitdown);
	else if (GETUNITSTATE(Game.map[pos]) == UNITMARK)
		PostMessage(hstatic[pos], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unitmarkdown);
	Game.mapchange[pos] = true;
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

void changemark(bool Mark){
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
