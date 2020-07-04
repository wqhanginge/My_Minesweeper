#include "minesweeper_support.h"

INT_PTR CALLBACK RecordProc(HWND hrecord, UINT msg, WPARAM wparam, LPARAM lparam) {
	static HWND het, hnt, hht, hen, hnn, hhn;
	char timebuffer[TIMESTRBUF];

	switch (msg) {
	case WM_INITDIALOG:
		//get static handels
		het = FindWindowEx(hrecord, NULL, TEXT("STATIC"), NULL);
		hnt = FindWindowEx(hrecord, het, TEXT("STATIC"), NULL);
		hht = FindWindowEx(hrecord, hnt, TEXT("STATIC"), NULL);
		hen = FindWindowEx(hrecord, hht, TEXT("STATIC"), NULL);
		hnn = FindWindowEx(hrecord, hen, TEXT("STATIC"), NULL);
		hhn = FindWindowEx(hrecord, hnn, TEXT("STATIC"), NULL);

		//init static control show
		maketimestr(timebuffer, Score.easytime);
		SetWindowText(het, timebuffer);
		maketimestr(timebuffer, Score.normaltime);
		SetWindowText(hnt, timebuffer);
		maketimestr(timebuffer, Score.hardtime);
		SetWindowText(hht, timebuffer);
		SetWindowText(hen, Score.easyname);
		SetWindowText(hnn, Score.normalname);
		SetWindowText(hhn, Score.hardname);
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDRESET:
			//reset the record
			cleanscore();
			maketimestr(timebuffer, MAXTIME);
			SetWindowText(het, timebuffer);
			SetWindowText(hnt, timebuffer);
			SetWindowText(hht, timebuffer);
			SetWindowText(hen, DEFSCORENAME);
			SetWindowText(hnn, DEFSCORENAME);
			SetWindowText(hhn, DEFSCORENAME);
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
		//get edit control handle and init default show
		heditname = FindWindowEx(hgetname, NULL, TEXT("EDIT"), NULL);
		SetWindowText(heditname, bestname());
		SendMessage(heditname, EM_LIMITTEXT, SCOREEDITLEN - 1, 0);
		SendMessage(heditname, EM_SETSEL, 0, -1);
		SetFocus(heditname);
		break;
	case WM_DESTROY:
		//get what writen in the edit control when exit dialog
		GetWindowText(heditname, bestname(), SCOREEDITLEN);
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDC_OK:
			EndDialog(hgetname, 0);
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

INT_PTR CALLBACK CustomProc(HWND hcustom, UINT msg, WPARAM wparam, LPARAM lparam) {
	static HWND heditw, hedith, heditm;
	char str[MAPEDITLEN];
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
		int2str(str, width);
		SetWindowText(heditw, str);
		int2str(str, height);
		SetWindowText(hedith, str);
		int2str(str, mines);
		SetWindowText(heditm, str);
		break;
	case WM_DESTROY:
		//set game mode when exit dialog
		setgamemode(CUSTOM, (byte)width, (byte)height, (word)mines);
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDOK:
			//get what int edit control when click OK
			GetWindowText(heditw, str, MAPEDITLEN);
			str2int(str, width);
			GetWindowText(hedith, str, MAPEDITLEN);
			str2int(str, height);
			GetWindowText(heditm, str, MAPEDITLEN);
			str2int(str, mines);
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

INT_PTR CALLBACK TimeProc(HWND htime, UINT msg, WPARAM wparam, LPARAM lparam) {
	static HWND hnumh, hnumt, hnumu;
	int h, t, u;

	switch (msg) {
	case WM_INITDIALOG:
		//get picture control handle
		hnumh = FindWindowEx(htime, NULL, TEXT("STATIC"), NULL);
		hnumt = FindWindowEx(htime, hnumh, TEXT("STATIC"), NULL);
		hnumu = FindWindowEx(htime, hnumt, TEXT("STATIC"), NULL);

		//adjust picture control position
		MoveWindow(hnumh, 0, 0, BM_INFONUMS_WIDTH, BM_INFONUMS_HEIGHT, FALSE);
		MoveWindow(hnumt, BM_INFONUMS_WIDTH, 0, BM_INFONUMS_WIDTH, BM_INFONUMS_HEIGHT, FALSE);
		MoveWindow(hnumu, BM_INFONUMS_WIDTH * 2, 0, BM_INFONUMS_WIDTH, BM_INFONUMS_HEIGHT, FALSE);

		//init time show
		PostMessage(htime, WM_SETTIME, 0, 0);
		break;
	case WM_SETTIME:
		//show specific time
		if (lparam < 1000) {
			h = (int)lparam / 100;
			t = ((int)lparam / 10) % 10;
			u = (int)lparam % 10;
			PostMessage(hnumh, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_infonums[h]);
			PostMessage(hnumt, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_infonums[t]);
			PostMessage(hnumu, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_infonums[u]);
		}
		break;
	case WM_TIMESTART:
		//start timer
		SetTimer(htime, GAMETIMERID, GAMETIMERELAPSE, NULL);
		Game.time = 0;
		break;
	case WM_TIMESTOP:
		//stop timer
		KillTimer(htime, GAMETIMERID);
		break;
	case WM_TIMER:
		//proccess time message
		if (Game.time < 1000) {
			Game.time++;
			h = Game.time / 100;
			t = (Game.time / 10) % 10;
			u = Game.time % 10;
			PostMessage(hnumh, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_infonums[h]);
			PostMessage(hnumt, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_infonums[t]);
			PostMessage(hnumu, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_infonums[u]);
		}
		break;
	case WM_LBUTTONDOWN:
		if (Game.state == FAIL || Game.state == SUCCESS) break;
		PostMessage(hInfo, WM_CLICKDOWN, 0, 0);
		break;
	case WM_LBUTTONUP:
		if (Game.state == FAIL || Game.state == SUCCESS) break;
		PostMessage(hInfo, WM_CLICKUP, 0, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK MineProc(HWND hmine, UINT msg, WPARAM wparam, LPARAM lparam) {
	static HWND hnumh, hnumt, hnumu;
	int h, t, u;

	switch (msg) {
	case WM_INITDIALOG:
		//get picture control handle
		hnumh = FindWindowEx(hmine, NULL, TEXT("STATIC"), NULL);
		hnumt = FindWindowEx(hmine, hnumh, TEXT("STATIC"), NULL);
		hnumu = FindWindowEx(hmine, hnumt, TEXT("STATIC"), NULL);

		//adjust picture control position
		MoveWindow(hnumh, 0, 0, BM_INFONUMS_WIDTH, BM_INFONUMS_HEIGHT, FALSE);
		MoveWindow(hnumt, BM_INFONUMS_WIDTH, 0, BM_INFONUMS_WIDTH, BM_INFONUMS_HEIGHT, FALSE);
		MoveWindow(hnumu, BM_INFONUMS_WIDTH * 2, 0, BM_INFONUMS_WIDTH, BM_INFONUMS_HEIGHT, FALSE);

		//init mines show
		PostMessage(hmine, WM_SETMINE, 0, Game.mines);
		break;
	case WM_SETMINE:
		//shwo specific mines
		if ((int)lparam >= 0 && (int)lparam < 1000) {
			h = (int)lparam / 100;
			t = ((int)lparam / 10) % 10;
			u = (int)lparam % 10;
		}
		else if ((int)lparam > -100 && (int)lparam < 0) {
			h = 10;
			t = (-(int)lparam / 10) % 10;
			u = -(int)lparam % 10;
		}
		else
			h = t = u = 10;
		PostMessage(hnumh, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_infonums[h]);
		PostMessage(hnumt, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_infonums[t]);
		PostMessage(hnumu, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_infonums[u]);
		break;
	case WM_MINECHG:
		if (Game.minesleft >= 0 && Game.minesleft < 1000) {
			h = Game.minesleft / 100;
			t = (Game.minesleft / 10) % 10;
			u = Game.minesleft % 10;
		}
		else if (Game.minesleft > -100 && Game.minesleft < 0) {
			h = 10;
			t = (-Game.minesleft / 10) % 10;
			u = -Game.minesleft % 10;
		}
		else
			h = t = u = 10;
		PostMessage(hnumh, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_infonums[h]);
		PostMessage(hnumt, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_infonums[t]);
		PostMessage(hnumu, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_infonums[u]);
		break;
	case WM_LBUTTONDOWN:
		if (Game.state == FAIL || Game.state == SUCCESS) break;
		PostMessage(hInfo, WM_CLICKDOWN, 0, 0);
		break;
	case WM_LBUTTONUP:
		if (Game.state == FAIL || Game.state == SUCCESS) break;
		PostMessage(hInfo, WM_CLICKUP, 0, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK InfoProc(HWND hinfo, UINT msg, WPARAM wparam, LPARAM lparam) {
	//handles
	static HWND hbutton;

	switch (msg) {
	case WM_INITDIALOG:
		//adjust info dialog size
		MoveWindow(hinfo, INFO_LEFT, INFO_TOP, infow(), INFO_H, FALSE);

		//find controls and init
		hbutton = FindWindowEx(hinfo, NULL, TEXT("BUTTON"), NULL);
		MoveWindow(hbutton, resetbuttonx(), RESETB_TOP, RESETB_SIZE, RESETB_SIZE, FALSE);
		PostMessage(hbutton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_reset);

		//create child dialog
		hMine = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MINE), hinfo, MineProc);
		MoveWindow(hMine, MINE_LEFT, MINE_TOP, MINE_WIDTH, MINE_HEIGHT, FALSE);
		hTime = CreateDialog(hInst, MAKEINTRESOURCE(IDD_TIME), hinfo, TimeProc);
		MoveWindow(hTime, timedlgx(), TIME_TOP, TIME_WIDTH, TIME_HEIGHT, FALSE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDC_RESET:
			//reset game
			SendMessage(hWnd, WM_GAMERESET, 0, 0);
			break;
		default:
			break;
		}
		break;
	case WM_GAMERESET:
		PostMessage(hbutton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_reset);
		break;
	case WM_LBUTTONDOWN:
	case WM_CLICKDOWN:
		if (Game.state == FAIL || Game.state == SUCCESS) break;
		PostMessage(hbutton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_click);
		break;
	case WM_LBUTTONUP:
	case WM_CLICKUP:
		if (Game.state == FAIL || Game.state == SUCCESS) break;
		PostMessage(hbutton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_reset);
		break;
	case WM_GAMEFAIL:
		PostMessage(hbutton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_fail);
		break;
	case WM_GAMESUCCESS:
		PostMessage(hbutton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_success);
		break;
	case WM_GAMEMODECHG:
		//change window size
		MoveWindow(hinfo, INFO_LEFT, INFO_TOP, infow(), INFO_H, FALSE);
		MoveWindow(hbutton, resetbuttonx(), RESETB_TOP, RESETB_SIZE, RESETB_SIZE, FALSE);
		MoveWindow(hTime, timedlgx(), TIME_TOP, TIME_WIDTH, TIME_HEIGHT, FALSE);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK MapAreaProc(HWND hmaparea, UINT msg, WPARAM wparam, LPARAM lparam) {
	static HWND *hstatic;
	int pos;
	Neighbor neipos;

	switch (msg) {
	case WM_INITDIALOG:
		//init maparea static controls
		MoveWindow(hmaparea, MA_LEFT, MA_TOP, mapareaw(), mapareah(), FALSE);
		hstatic = createstatics(hmaparea);
		break;
	case WM_GAMERESET:
		//refresh maparea
		memset(Game.mapchange, 1, sizeof(bool)*Game.size);
		paintmap(hstatic);
		break;
	case WM_GAMEMODECHG:
		//create new static controls
		deletestatics(&hstatic, HIWORD(lparam)*LOWORD(lparam));
		MoveWindow(hmaparea, MA_LEFT, MA_TOP, mapareaw(), mapareah(), FALSE);
		hstatic = createstatics(hmaparea);
		break;
	case WM_GAMESTART:
		//click unit and open blanks when game start
		clickunit((int)lparam);
		openblanks((int)lparam);
		if (Game.mines == Game.size - Game.unitsuncov)
			SendMessage(hWnd, WM_GAMESUCCESS, 0, 0);
		else
			paintmap(hstatic);
		break;
	case WM_GAMEFAIL:
	case WM_GAMESUCCESS:
		//show all mines positions when game set
		uncovallmines();
		paintmap(hstatic);
		break;
	case WM_LBUTTONDOWN:
		//won't work after game finishing
		if (Game.state == FAIL || Game.state == SUCCESS) break;

		PostMessage(hInfo, WM_CLICKDOWN, 0, 0);
		pos = lparamtopos(lparam);
		if (wparam & MK_RBUTTON) {	//double buttons down
			getneighbors(neipos, pos);
			showclickdown(hstatic, neipos);
		}
		else {	//single button
			showclickdown(hstatic, pos);
		}
		break;
	case WM_LBUTTONUP:
		//won't work after game finishing
		if (Game.state == FAIL || Game.state == SUCCESS) break;

		PostMessage(hInfo, WM_CLICKUP, 0, 0);
		pos = lparamtopos(lparam);
		if (wparam & MK_RBUTTON) {	//double buttons
			int ret = digneighbors(pos);
			if (ret == -1) {
				SendMessage(hWnd, WM_GAMEFAIL, 0, 0);
			}
			else if (Game.mines == Game.size - Game.unitsuncov) {
				SendMessage(hWnd, WM_GAMESUCCESS, 0, 0);
			}
			else
				paintmap(hstatic);
		}
		else {	//single button
			if (Game.state == INITIAL) {	//first click
				SendMessage(hWnd, WM_GAMESTART, 0, pos);
			}
			else {	//not first click
				int ret = clickunit(pos);
				if (ret == 0) openblanks(pos);
				if (ret == -1) {
					SendMessage(hWnd, WM_GAMEFAIL, 0, 0);
				}
				else if (Game.mines == Game.size - Game.unitsuncov) {
					SendMessage(hWnd, WM_GAMESUCCESS, 0, 0);
				}
				else
					paintmap(hstatic);
			}
		}
		break;
	case WM_RBUTTONDOWN:
		//won't work after game finishing
		if (Game.state == FAIL || Game.state == SUCCESS) break;

		pos = lparamtopos(lparam);
		if (wparam & MK_LBUTTON) {	//double buttons
			getneighbors(neipos, pos);
			showclickdown(hstatic, neipos);
		}
		else {	//single button, flag a unit or mark a unit
			if (GETUNITSTATE(Game.map[pos]) == UNITCOVER) {
				Game.map[pos] = SETUNITSTATE(UNITFLAG, Game.map[pos]);
				PostMessage(hstatic[pos], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unitflag);
				Game.minesleft--;
				PostMessage(hMine, WM_MINECHG, 0, 0);
			}
			else if (GETUNITSTATE(Game.map[pos]) == UNITFLAG && Game.mark) {
				Game.map[pos] = SETUNITSTATE(UNITMARK, Game.map[pos]);
				PostMessage(hstatic[pos], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unitmark);
				Game.minesleft++;
				PostMessage(hMine, WM_MINECHG, 0, 0);
			}
			else if (GETUNITSTATE(Game.map[pos]) == UNITFLAG && !Game.mark)
			{
				Game.map[pos] = SETUNITSTATE(UNITCOVER, Game.map[pos]);
				PostMessage(hstatic[pos], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unit);
				Game.minesleft++;
				PostMessage(hMine, WM_MINECHG, 0, 0);
			}
			else if (GETUNITSTATE(Game.map[pos]) == UNITMARK) {
				Game.map[pos] = SETUNITSTATE(UNITCOVER, Game.map[pos]);
				PostMessage(hstatic[pos], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_unit);
			}
		}
		break;
	case WM_RBUTTONUP:
		//won't work after game finishing
		if (Game.state == FAIL || Game.state == SUCCESS) break;

		PostMessage(hInfo, WM_CLICKUP, 0, 0);
		pos = lparamtopos(lparam);
		if (wparam & MK_LBUTTON) {	//double buttons
			int ret = digneighbors(pos);
			if (ret == -1) {
				SendMessage(hWnd, WM_GAMEFAIL, 0, 0);
			}
			else if (Game.mines == Game.size - Game.unitsuncov) {
				SendMessage(hWnd, WM_GAMESUCCESS, 0, 0);
			}
			else
				paintmap(hstatic);
		}
		break;
	case WM_MOUSEMOVE:
		//won't work after game finishing
		if (Game.state == FAIL || Game.state == SUCCESS) break;

		pos = lparamtopos(lparam);
		if (wparam & MK_LBUTTON) {	//with mouse button down
			if (wparam & MK_RBUTTON) {	//double buttons
				Neighbor neipos;
				getneighbors(neipos, pos);
				showclickdown(hstatic, neipos);
			}
			//single button
			else
				showclickdown(hstatic, pos);
		}
		//without mouse button down
		else
			paintmap(hstatic);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

void MenuProc(WPARAM wparam) {
	MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
	byte oldmode, oldwidth, oldheight;

	switch (LOWORD(wparam)) {
	case ID_GAME_START:
		PostMessage(hWnd, WM_GAMERESET, 0, 0);
		break;
	case ID_GAME_EASY:
		//change game mode
		changechecked(EASY);
		oldmode = Game.mode;
		oldwidth = Game.width;
		oldheight = Game.height;
		setgamemode(EASY);
		PostMessage(hWnd, WM_GAMEMODECHG, oldmode, MAKELPARAM(oldwidth, oldheight));
		break;
	case ID_GAME_NORMAL:
		changechecked(NORMAL);
		oldmode = Game.mode;
		oldwidth = Game.width;
		oldheight = Game.height;
		setgamemode(NORMAL);
		PostMessage(hWnd, WM_GAMEMODECHG, oldmode, MAKELPARAM(oldwidth, oldheight));
		break;
	case ID_GAME_HARD:
		changechecked(HARD);
		oldmode = Game.mode;
		oldwidth = Game.width;
		oldheight = Game.height;
		setgamemode(HARD);
		PostMessage(hWnd, WM_GAMEMODECHG, oldmode, MAKELPARAM(oldwidth, oldheight));
		break;
	case ID_GAME_CUSTOM:
		changechecked(CUSTOM);
		oldmode = Game.mode;
		oldwidth = Game.width;
		oldheight = Game.height;
		DialogBox(hInst, MAKEINTRESOURCE(IDD_CUSTOM), hWnd, CustomProc);
		PostMessage(hWnd, WM_GAMEMODECHG, oldmode, MAKELPARAM(oldwidth, oldheight));
		break;
	case ID_GAME_MARK:
		//enable ro disable Mark
		Game.mark = !Game.mark;
		changemark(Game.mark);
		break;
	case ID_GAME_RECORD:
		//show records
		DialogBox(hInst, MAKEINTRESOURCE(IDD_RECORD), hWnd, RecordProc);
		break;
	case ID_GAME_EXIT:
		PostMessage(hWnd, WM_DESTROY, 0, 0);
		break;
	case ID_HELP:
		//show help infomation
		MessageBox(hWnd, TEXT("Regular Minesweeper, please don't doubt"), TEXT("Help"), MB_OK);
		break;
	default:
		break;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	static int edgew, edgeh;
	static int strl;
	static char *buffer;
	//use for changing window size
	RECT wndrect, cltrect;

	switch (msg) {
	case WM_CREATE:
		//load bitmaps
		loadbitmaps();

		//init game from file
		buffer = new char[MAXPATHBUF];
		strl = GetEnvironmentVariable(DEFFILEPATHEV, buffer, MAXPATHBUF);
		strcat_s(buffer, strl + strlen(DEFFILENAME) + 1, DEFFILENAME);
		initgame(buffer);

		//init menu info
		changechecked(Game.mode);
		changemark(Game.mark);

		//adjust window size
		GetWindowRect(hwnd, &wndrect);
		GetClientRect(hwnd, &cltrect);
		edgew = (wndrect.right - wndrect.left) - cltrect.right;
		edgeh = (wndrect.bottom - wndrect.top) - cltrect.bottom;
		cltrect.right = mapareaw();
		cltrect.bottom = mapareah() + INFO_H;
		wndrect.right = wndrect.left + cltrect.right + edgew;
		wndrect.bottom = wndrect.top + cltrect.bottom + edgeh;		
		MoveWindow(hwnd, wndrect.left, wndrect.top, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, FALSE);

		//create Info Dialog and MapArea Dialog
		hInfo = CreateDialog(hInst, MAKEINTRESOURCE(IDD_INFO), hwnd, InfoProc);
		hMapArea = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAPAREA), hwnd, MapAreaProc);
		break;
	case WM_DESTROY:
		//release bitmaps
		freebitmaps();
		//save game info
		savegame(buffer);
		delete buffer;

		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		//messages from menu
		MenuProc(wparam);
		break;
	case WM_GAMERESET:
		cleanmap();
		PostMessage(hInfo, WM_GAMERESET, 0, 0);
		PostMessage(hMapArea, WM_GAMERESET, 0, 0);
		PostMessage(hMine, WM_SETMINE, 0, Game.mines);
		PostMessage(hTime, WM_TIMESTOP, 0, 0);
		PostMessage(hTime, WM_SETTIME, 0, 0);
		break;
	case WM_GAMESTART:
		createmap((int)lparam);
		Game.state = PROGRESS;
		PostMessage(hInfo, WM_GAMESTART, 0, lparam);
		PostMessage(hMapArea, WM_GAMESTART, 0, lparam);
		PostMessage(hTime, WM_TIMESTART, 0, 0);
		break;
	case WM_GAMEFAIL:
		Game.state = FAIL;
		PostMessage(hInfo, WM_GAMEFAIL, 0, 0);
		PostMessage(hMapArea, WM_GAMEFAIL, 0, 0);
		PostMessage(hTime, WM_TIMESTOP, 0, 0);
		break;
	case WM_GAMESUCCESS:
		Game.state = SUCCESS;
		PostMessage(hInfo, WM_GAMESUCCESS, 0, 0);
		PostMessage(hMapArea, WM_GAMESUCCESS, 0, 0);
		PostMessage(hMine, WM_SETMINE, 0, 0);
		PostMessage(hTime, WM_TIMESTOP, 0, 0);
		//if break record
		if (Game.mode == EASY || Game.mode == NORMAL || Game.mode == HARD) {
			if (Game.time < besttime()) {
				updatetime();
				DialogBox(hInst, MAKEINTRESOURCE(IDD_GETNAME), hWnd, GetNameProc);
				DialogBox(hInst, MAKEINTRESOURCE(IDD_RECORD), hWnd, RecordProc);
			}
		}
		break;
	case WM_GAMEMODECHG:
		//change window size for new game map size
		GetWindowRect(hwnd, &wndrect);
		wndrect.right = mapareaw() + edgew + wndrect.left;
		wndrect.bottom = mapareah() + INFO_H + edgeh + wndrect.top;
		MoveWindow(hwnd, wndrect.left, wndrect.top, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, TRUE);
		PostMessage(hInfo, WM_GAMEMODECHG, wparam, lparam);
		PostMessage(hMapArea, WM_GAMEMODECHG, wparam, lparam);
		PostMessage(hwnd, WM_GAMERESET, 0, 0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wndc = { sizeof(WNDCLASSEX) };
	wndc.cbClsExtra = 0;
	wndc.cbWndExtra = 0;
	wndc.hInstance = hInstance;
	wndc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndc.lpfnWndProc = WndProc;
	wndc.lpszClassName = TEXT("My_Minesweeper");
	wndc.lpszMenuName = NULL;
	wndc.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassEx(&wndc);

	hInst = hInstance;
	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));
	hWnd = CreateWindowEx(0, wndc.lpszClassName, TEXT("My Minesweeper"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		hMenu,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);
	//UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, hWnd, 0, 0) > 0) {
		if (!IsDialogMessage(hInfo, &msg)
			&& !IsDialogMessage(hMapArea, &msg)
			&& !IsDialogMessage(hMine, &msg)
			&& !IsDialogMessage(hTime, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}