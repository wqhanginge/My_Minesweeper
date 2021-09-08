#include "procfunctions.h"

TCHAR save_path[MAX_PATH];
bool last_dbclick;
bool rb_capture;

void setMenuChecked(byte GameMode)
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

void setQMarkChecked(bool Mark)
{
	MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
	mii.fMask = MIIM_STATE;
	mii.fState = (Mark) ? MFS_CHECKED : MFS_UNCHECKED;
	SetMenuItemInfo(hMenu, ID_GAME_MARK, FALSE, &mii);
}



/* Proc Functions */

INT_PTR CALLBACK AboutProc(HWND habout, UINT msg, WPARAM wparam, LPARAM lparam)
{
	HWND htext;
	TCHAR aboutinfo[ABOUT_INFO_LEN];

	switch (msg) {
	case WM_INITDIALOG:
		//get text handel
		htext = FindWindowEx(habout, NULL, TEXT("STATIC"), NULL);
		
		//show program description
		_tcscpy_s(aboutinfo, ABOUT_INFO_LEN, TEXT(ABOUT_TEXT));
		getVersion(&aboutinfo[_tcslen(aboutinfo)], ABOUT_INFO_LEN - _tcslen(aboutinfo));
		SetWindowText(htext, aboutinfo);
		break;
	case WM_CLOSE:
		EndDialog(habout, 0);
		break;
	case WM_COMMAND:
		if (LOWORD(wparam) == IDOK) EndDialog(habout, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK RecordProc(HWND hrecord, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static HWND hjt, hmt, hst, hjn, hmn, hsn;
	TCHAR strbuffer[TIME_STRLEN];

	switch (msg) {
	case WM_INITDIALOG:
		//get static handels
		hjt = FindWindowEx(hrecord, NULL, TEXT("STATIC"), NULL);
		hmt = FindWindowEx(hrecord, hjt, TEXT("STATIC"), NULL);
		hst = FindWindowEx(hrecord, hmt, TEXT("STATIC"), NULL);
		hjn = FindWindowEx(hrecord, hst, TEXT("STATIC"), NULL);
		hmn = FindWindowEx(hrecord, hjn, TEXT("STATIC"), NULL);
		hsn = FindWindowEx(hrecord, hmn, TEXT("STATIC"), NULL);

		//init static control show
		tcsprintf(strbuffer, TIME_STRLEN, TEXT("%d"), Score.junior_time);
		_tcsncat_s(strbuffer, TIME_STRLEN, TEXT(DEF_TIMEUNIT_EN), _TRUNCATE);
		SetWindowText(hjt, strbuffer);
		tcsprintf(strbuffer, TIME_STRLEN, TEXT("%d"), Score.middle_time);
		_tcsncat_s(strbuffer, TIME_STRLEN, TEXT(DEF_TIMEUNIT_EN), _TRUNCATE);
		SetWindowText(hmt, strbuffer);
		tcsprintf(strbuffer, TIME_STRLEN, TEXT("%d"), Score.senior_time);
		_tcsncat_s(strbuffer, TIME_STRLEN, TEXT(DEF_TIMEUNIT_EN), _TRUNCATE);
		SetWindowText(hst, strbuffer);
		SetWindowText(hjn, Score.junior_name);
		SetWindowText(hmn, Score.middle_name);
		SetWindowText(hsn, Score.senior_name);
		break;
	case WM_CLOSE:
		EndDialog(hrecord, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDRESET:
			//reset the record
			resetRecord();
			tcsprintf(strbuffer, TIME_STRLEN, TEXT("%d"), MAX_TIME);
			_tcsncat_s(strbuffer, TIME_STRLEN, TEXT(DEF_TIMEUNIT_EN), _TRUNCATE);
			SetWindowText(hjt, strbuffer);
			SetWindowText(hmt, strbuffer);
			SetWindowText(hst, strbuffer);
			SetWindowText(hjn, TEXT(DEF_SCORE_NAME_EN));
			SetWindowText(hmn, TEXT(DEF_SCORE_NAME_EN));
			SetWindowText(hsn, TEXT(DEF_SCORE_NAME_EN));
			break;
		case IDOK:
			EndDialog(hrecord, 0);
			break;
		default:
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK GetNameProc(HWND hgetname, UINT msg, WPARAM wparam, LPARAM lparam) {
	static HWND heditname;

	switch (msg) {
	case WM_INITDIALOG:
		//get edit control handle and init default presentation
		heditname = FindWindowEx(hgetname, NULL, TEXT("EDIT"), NULL);
		SetWindowText(heditname, getpRecordName(Game.mode));
		SendMessage(heditname, EM_LIMITTEXT, NAME_EDIT_LEN - 1, 0);
		SendMessage(heditname, EM_SETSEL, 0, -1);
		SetFocus(heditname);
		break;
	case WM_DESTROY:
		//get what writen in the edit control when exit dialog
		GetWindowText(heditname, getpRecordName(Game.mode), NAME_EDIT_LEN);
		break;
	case WM_COMMAND:
		if (LOWORD(wparam) == IDC_OK) EndDialog(hgetname, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK CustomProc(HWND hcustom, UINT msg, WPARAM wparam, LPARAM lparam) {
	static HWND heditw, hedith, heditm;
	TCHAR str[CUSTOM_EDIT_LEN];
	static dword width, height, mines;

	switch (msg) {
	case WM_INITDIALOG:
		//get edit control handle
		heditw = FindWindowEx(hcustom, NULL, TEXT("EDIT"), NULL);
		hedith = FindWindowEx(hcustom, heditw, TEXT("EDIT"), NULL);
		heditm = FindWindowEx(hcustom, hedith, TEXT("EDIT"), NULL);

		//copy to buffer
		width = Game.width;
		height = Game.height;
		mines = Game.mines;

		//init edit control show
		tcsprintf(str, CUSTOM_EDIT_LEN, TEXT("%u"), width);
		SetWindowText(heditw, str);
		tcsprintf(str, CUSTOM_EDIT_LEN, TEXT("%u"), height);
		SetWindowText(hedith, str);
		tcsprintf(str, CUSTOM_EDIT_LEN, TEXT("%u"), mines);
		SetWindowText(heditm, str);
		break;
	case WM_CLOSE:
		EndDialog(hcustom, 0);
		break;
	case WM_DESTROY:
		//set game mode when exit dialog
		PostMessage(hWnd, WM_GAMEMODECHG, CUSTOM, MAKECHGLPARAM(width, height, mines));
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDOK:
			//get what int edit control when click OK
			GetWindowText(heditw, str, CUSTOM_EDIT_LEN);
			tcsscanf_s(str, TEXT("%u"), &width);
			GetWindowText(hedith, str, CUSTOM_EDIT_LEN);
			tcsscanf_s(str, TEXT("%u"), &height);
			GetWindowText(heditm, str, CUSTOM_EDIT_LEN);
			tcsscanf_s(str, TEXT("%u"), &mines);
			EndDialog(hcustom, 0);
			break;
		case IDCANCEL:
			EndDialog(hcustom, 0);
			break;
		default:
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}


/* message response functions */

LRESULT onMenu(WPARAM wparam) {
	MENUITEMINFO mii = { sizeof(MENUITEMINFO) };

	switch (LOWORD(wparam)) {
	case ID_GAME_START:
		PostMessage(hWnd, WM_GAMERESET, 0, 0);
		break;
	case ID_GAME_JUNIOR:
		//change game mode
		setMenuChecked(JUNIOR);
		PostMessage(hWnd, WM_GAMEMODECHG, JUNIOR, 0);
		break;
	case ID_GAME_MIDDLE:
		setMenuChecked(MIDDLE);
		PostMessage(hWnd, WM_GAMEMODECHG, MIDDLE, 0);
		break;
	case ID_GAME_SENIOR:
		setMenuChecked(SENIOR);
		PostMessage(hWnd, WM_GAMEMODECHG, SENIOR, 0);
		break;
	case ID_GAME_CUSTOM:
		setMenuChecked(CUSTOM);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_CUSTOM), hWnd, CustomProc);
		break;
	case ID_GAME_MARK:
		//enable/disable Question Mark mode
		setMark(!Game.mark);
		setQMarkChecked(Game.mark);
		break;
	case ID_GAME_RECORD:
		//show records
		DialogBox(hInst, MAKEINTRESOURCE(IDD_RECORD), hWnd, RecordProc);
		break;
	case ID_GAME_EXIT:
		PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	case ID_ABOUT:
		//show about infomation
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, AboutProc);
		break;
	default:
		break;
	}
	return 0;
}

LRESULT onCreate(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	POINT wndpos;
	RECT wndrect, cltrect;
	int edgew, edgeh;

	//load bitmaps
	loadBitmaps(hInst);

	//init game from file
	GetEnvironmentVariable(TEXT(DEF_FILEPATH_EV), save_path, MAX_PATH);
	_tcscat_s(save_path, TEXT("\\"));
	_tcscat_s(save_path, TEXT(DEF_FILENAME));
	initGame(save_path, wndpos);
	srand((dword)time(nullptr));

	//init menu info
	setMenuChecked(Game.mode);
	setQMarkChecked(Game.mark);

	//adjust window size
	GetWindowRect(hwnd, &wndrect);
	GetClientRect(hwnd, &cltrect);
	edgew = (wndrect.right - wndrect.left) - cltrect.right;
	edgeh = (wndrect.bottom - wndrect.top) - cltrect.bottom;
	wndrect = { wndpos.x,wndpos.y,wndpos.x + CLIENT_WIDTH + edgew,wndpos.y + CLIENT_HEIGHT + edgeh };
	MoveWindow(hwnd, wndrect.left, wndrect.top, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, FALSE);

	//init double mouse button state
	last_dbclick = false;
	return 0;
}

LRESULT onDestroy(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	POINT wndpos;
	RECT wndrect;

	//release bitmaps
	freeBitmaps();
	//save game info
	GetWindowRect(hwnd, &wndrect);
	wndpos = { wndrect.left,wndrect.top };
	saveGame(save_path, wndpos);

	PostQuitMessage(0);
	return 0;
}

LRESULT onPaint(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	//redraw the whole client area
	PAINTSTRUCT ps;
	HDC hpaintdc = BeginPaint(hwnd, &ps);
	HDC hdcbuffer = CreateCompatibleDC(hpaintdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hpaintdc, CLIENT_WIDTH, CLIENT_HEIGHT);
	SelectObject(hdcbuffer, hbmbuffer);

	drawClientBg(hdcbuffer, 0, 0);
	drawInfoBg(hdcbuffer, INFO_LEFT, INFO_TOP);
	drawMapAreaBg(hdcbuffer, MAPAREA_LEFT, MAPAREA_TOP);
	drawMineBg(hdcbuffer, MINE_LEFT, MINE_TOP);
	drawTimeBg(hdcbuffer, TIME_LEFT, TIME_TOP);
	drawINNB(hdcbuffer, MN_LEFT, MN_TOP, Game.mine_remains);
	drawINNB(hdcbuffer, TN_LEFT, TN_TOP, Game.time);
	drawResetButtonNB(hdcbuffer, RB_LEFT, RB_TOP, hbm_current, false);
	paintMapNB(hdcbuffer, MAP_LEFT, MAP_TOP, true);

	BitBlt(hpaintdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, hdcbuffer, 0, 0, SRCCOPY);
	DeleteObject(hbmbuffer);
	DeleteObject(hdcbuffer);

	EndPaint(hwnd, &ps);
	return 0;
}

LRESULT onCommand(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	if (lparam == 0 && HIWORD(wparam) == 0)
		return onMenu(wparam);
	return 0;
}

LRESULT onGameReset(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	KillTimer(hwnd, GAME_TIMER_ID);
	resetGame();
	last_dbclick = false;
	setRBBitmap(hbm_rb);
	InvalidateRect(hwnd, NULL, FALSE);
	return 0;
}

LRESULT onGameStart(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	int ret = gameStart((int)lparam);
	paintMap(hdc, MAP_LEFT, MAP_TOP, false);
	drawIN(hdc, MN_LEFT, MN_TOP, Game.mine_remains);
	drawIN(hdc, TN_LEFT, TN_TOP, Game.time);
	switch (ret) {
	case 1:
		PostMessage(hwnd, WM_GAMESUCCESS, 0, 0);
		break;
	case -1:
		PostMessage(hwnd, WM_GAMEFAIL, 0, 0);
		break;
	case 0:
		SetTimer(hwnd, GAME_TIMER_ID, GAME_TIMER_ELAPSE, NULL);
		break;
	default:
		break;
	}
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onGameFail(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	KillTimer(hwnd, GAME_TIMER_ID);
	gameSet();
	setRBBitmap(hbm_fail);
	paintResetButton(hdc, RB_LEFT, RB_TOP, false);
	paintMap(hdc, MAP_LEFT, MAP_TOP, false);
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onGameSuccess(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	KillTimer(hwnd, GAME_TIMER_ID);
	int ret = gameSet();
	setRBBitmap(hbm_success);
	paintResetButton(hdc, RB_LEFT, RB_TOP, false);
	drawIN(hdc, MN_LEFT, MN_TOP, 0);
	paintMap(hdc, MAP_LEFT, MAP_TOP, false);

	//if break record
	if (ret) {
		setRecordTime(Game.mode, Game.time);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_GETNAME), hwnd, GetNameProc);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_RECORD), hwnd, RecordProc);
	}
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onGameModeChg(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	RECT wndrect, cltrect;

	KillTimer(hwnd, GAME_TIMER_ID);
	setGameMode((byte)wparam, GETCHGWIDTH(lparam), GETCHGHEIGHT(lparam), GETCHGMINES(lparam));
	last_dbclick = false;

	//change window size for new game map size, no need to reset
	GetWindowRect(hwnd, &wndrect);
	GetClientRect(hwnd, &cltrect);
	wndrect.right = CLIENT_WIDTH + ((wndrect.right - wndrect.left) - cltrect.right) + wndrect.left;
	wndrect.bottom = CLIENT_HEIGHT + ((wndrect.bottom - wndrect.top) - cltrect.bottom) + wndrect.top;

	//invalidate all client area for repaint
	MoveWindow(hwnd, wndrect.left, wndrect.top, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, TRUE);
	setRBBitmap(hbm_rb);
	InvalidateRect(hwnd, NULL, FALSE);
	return 0;
}

LRESULT onLButtonDwon(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	SetCapture(hwnd);
	if (lparamIsInRB(lparam)) {	//click on Reset Button
		rb_capture = true;	//set ResetButton capture
		paintResetButton(hdc, RB_LEFT, RB_TOP, true);
	}
	else if (Game.state < GAMESET){	//won't work after game finished
		setRBBitmap(hbm_click);
		paintResetButton(hdc, RB_LEFT, RB_TOP, false);

		//if in the map area
		if (lparamIsInMap(lparam)) {
			int index = lparam2index(lparam);
			Neighbor indexes;
			if (wparam & MK_RBUTTON) {	//double buttons down
				last_dbclick = true;
				getNeighbors(indexes, index);
				showClickedMapUnit(hdc, MAP_LEFT, MAP_TOP, indexes);
			}
			else {	//single button
				last_dbclick = false;
				showClickedMapUnit(hdc, MAP_LEFT, MAP_TOP, index);
			}
		}
	}
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onLButtonUp(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	ReleaseCapture();
	if (rb_capture) {	//if ResetButton got capture
		if (lparamIsInRB(lparam)) {	//click up
			paintResetButton(hdc, RB_LEFT, RB_TOP, false);
			PostMessage(hwnd, WM_GAMERESET, 0, 0);
		}
		rb_capture = false;	//release ResetButton capture
	}
	else if (Game.state < GAMESET) {	//won't work after game finished
		setRBBitmap(hbm_rb);
		paintResetButton(hdc, RB_LEFT, RB_TOP, false);

		//if in the map area
		if (lparamIsInMap(lparam)) {
			int index = lparam2index(lparam);
			if (wparam & MK_RBUTTON) {	//double buttons
				last_dbclick = true;
				int ret = digNeighbors(index);
				if (ret == -1) {
					PostMessage(hwnd, WM_GAMEFAIL, 0, 0);
				}
				else if (isGameSuccessful()) {
					PostMessage(hwnd, WM_GAMESUCCESS, 0, 0);
				}
				else
					paintMap(hdc, MAP_LEFT, MAP_TOP, false);
			}
			else if (!last_dbclick) {	//single button and last was not double button
				last_dbclick = false;
				if (Game.state == INIT && GETMUSTATE(Game.map[index]) != MUS_FLAG) {	//first click
					PostMessage(hwnd, WM_GAMESTART, 0, index);
				}
				else {	//not first click
					int ret = clickUnit(index);
					if (ret == 0) openBlanks(index);
					if (ret == -1) {
						PostMessage(hwnd, WM_GAMEFAIL, 0, 0);
					}
					else if (isGameSuccessful()) {
						PostMessage(hwnd, WM_GAMESUCCESS, 0, 0);
					}
					else
						paintMap(hdc, MAP_LEFT, MAP_TOP, false);
				}
			}
			else {	//single button and last was double button
				last_dbclick = false;
				paintMap(hdc, MAP_LEFT, MAP_TOP, false);
			}
		}
	}
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onRButtonDown(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	//won't work after game finished or the mouse not in the map area or ResetButton got capture
	if (!rb_capture && Game.state < GAMESET && lparamIsInMap(lparam)) {
		int index = lparam2index(lparam);
		Neighbor indexes;
		if (wparam & MK_LBUTTON) {	//double buttons
			last_dbclick = true;
			setRBBitmap(hbm_click);
			paintResetButton(hdc, RB_LEFT, RB_TOP, false);
			getNeighbors(indexes, index);
			showClickedMapUnit(hdc, MAP_LEFT, MAP_TOP, indexes);
		}
		else {	//single button, flag a unit or mark a unit
			last_dbclick = false;
			if (GETMUSTATE(Game.map[index]) == MUS_COVER) {
				SETMUSTATE(MUS_FLAG, Game.map[index]);
				paintMapUnit(hdc, MAP_LEFT + index2px(index), MAP_TOP + index2py(index), Game.map[index]);
				Game.mine_remains--;
				drawIN(hdc, MN_LEFT, MN_TOP, Game.mine_remains);
			}
			else if (GETMUSTATE(Game.map[index]) == MUS_FLAG && Game.mark) {
				SETMUSTATE(MUS_MARK, Game.map[index]);
				paintMapUnit(hdc, MAP_LEFT + index2px(index), MAP_TOP + index2py(index), Game.map[index]);
				Game.mine_remains++;
				drawIN(hdc, MN_LEFT, MN_TOP, Game.mine_remains);
			}
			else if (GETMUSTATE(Game.map[index]) == MUS_FLAG && !Game.mark)
			{
				SETMUSTATE(MUS_COVER, Game.map[index]);
				paintMapUnit(hdc, MAP_LEFT + index2px(index), MAP_TOP + index2py(index), Game.map[index]);
				Game.mine_remains++;
				drawIN(hdc, MN_LEFT, MN_TOP, Game.mine_remains);
			}
			else if (GETMUSTATE(Game.map[index]) == MUS_MARK) {
				SETMUSTATE(MUS_COVER, Game.map[index]);
				paintMapUnit(hdc, MAP_LEFT + index2px(index), MAP_TOP + index2py(index), Game.map[index]);
			}
		}
	}
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onRButtonUp(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	ReleaseCapture();
	//won't work after game finishing or ResetButton got capture
	if (!rb_capture && Game.state < GAMESET) {
		setRBBitmap(hbm_rb);
		paintResetButton(hdc, RB_LEFT, RB_TOP, false);

		//if in the map area
		if (lparamIsInMap(lparam)) {
			int index = lparam2index(lparam);
			if (wparam & MK_LBUTTON) {	//double buttons
				last_dbclick = true;
				int ret = digNeighbors(index);
				if (ret == -1) {
					PostMessage(hwnd, WM_GAMEFAIL, 0, 0);
				}
				else if (isGameSuccessful()) {
					PostMessage(hwnd, WM_GAMESUCCESS, 0, 0);
				}
				else
					paintMap(hdc, MAP_LEFT, MAP_TOP, false);
			}
			else	//single button
				last_dbclick = false;
		}
	}
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onMouseMove(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	if (rb_capture && (wparam & MK_LBUTTON))	//if ResetButton got capture
		paintResetButton(hdc, RB_LEFT, RB_TOP, lparamIsInRB(lparam));
	else if (Game.state < GAMESET) {	//won't work after game finishing
		if (!lparamIsInMap(lparam)) {	//if not in the map area
			paintMap(hdc, MAP_LEFT, MAP_TOP, false);
		}
		else {	//if in the map area
			int index = lparam2index(lparam);
			if (wparam & MK_LBUTTON) {	//with left button down
				if (wparam & MK_RBUTTON) {	//double buttons
					Neighbor indexes;
					getNeighbors(indexes, index);
					showClickedMapUnit(hdc, MAP_LEFT, MAP_TOP, indexes);
				}
				//single button
				else
					showClickedMapUnit(hdc, MAP_LEFT, MAP_TOP, index);
			}
			//without mouse button down
			else
				paintMap(hdc, MAP_LEFT, MAP_TOP, false);
		}
	}
	ReleaseDC(hwnd, hdc);
	return 0;
}

LRESULT onTimer(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	if (Game.time <= MAX_TIME) {
		HDC hdc = GetDC(hwnd);
		nextGameTime();
		drawIN(hdc, TN_LEFT, TN_TOP, Game.time);
		ReleaseDC(hwnd, hdc);
	}
	return 0;
}
