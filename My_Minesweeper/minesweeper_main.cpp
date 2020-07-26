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
		dword2str(str, width);
		SetWindowText(heditw, str);
		dword2str(str, height);
		SetWindowText(hedith, str);
		dword2str(str, mines);
		SetWindowText(heditm, str);
		break;
	case WM_DESTROY:
		//set game mode when exit dialog
		PostMessage(hWnd, WM_GAMEMODECHG, CUSTOM, MAKECHGLPARAM(width, height, mines));
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDOK:
			//get what int edit control when click OK
			GetWindowText(heditw, str, MAPEDITLEN);
			str2dword(str, width);
			GetWindowText(hedith, str, MAPEDITLEN);
			str2dword(str, height);
			GetWindowText(heditm, str, MAPEDITLEN);
			str2dword(str, mines);
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

void MenuProc(WPARAM wparam) {
	MENUITEMINFO mii = { sizeof(MENUITEMINFO) };

	switch (LOWORD(wparam)) {
	case ID_GAME_START:
		PostMessage(hWnd, WM_GAMERESET, 0, 0);
		break;
	case ID_GAME_EASY:
		//change game mode
		changechecked(EASY);
		PostMessage(hWnd, WM_GAMEMODECHG, EASY, 0);
		break;
	case ID_GAME_NORMAL:
		changechecked(NORMAL);
		PostMessage(hWnd, WM_GAMEMODECHG, NORMAL, 0);
		break;
	case ID_GAME_HARD:
		changechecked(HARD);
		PostMessage(hWnd, WM_GAMEMODECHG, HARD, 0);
		break;
	case ID_GAME_CUSTOM:
		changechecked(CUSTOM);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_CUSTOM), hWnd, CustomProc);
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
	static HWND hresetb;
	//use for paint
	static HDC hdc;
	HDC hpaintdc, hdcbuffer;
	PAINTSTRUCT ps;
	HBITMAP hbmbuffer;
	//use for file access
	static char buffer[MAX_PATH];
	//use for changing window size
	static int edgew, edgeh;
	RECT wndrect, cltrect;
	//use for GameMap updating
	int index;
	//remember if last was double mouse button down
	static bool lastdoublemb;

	switch (msg) {
	case WM_CREATE:
		//load bitmaps
		loadbitmaps();
		//get paint device contex
		hdc = GetDC(hwnd);

		//init game from file
		GetEnvironmentVariable(DEFFILEPATHEV, buffer, MAXPATHBUF);
		strcat_s(buffer, DEFFILENAME);
		initgame(buffer);

		//init menu info
		changechecked(Game.mode);
		changemark(Game.mark);

		//create reset button
		hresetb = CreateWindowEx(0, TEXT("BUTTON"), TEXT("ResetB"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			RESETBLEFT, RESETBTOP,
			RESETBSIZE, RESETBSIZE,
			hwnd,
			(HMENU)ID_RESETB,
			hInst,
			NULL);
		PostMessage(hresetb, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_resetb);

		//adjust window size
		GetWindowRect(hwnd, &wndrect);
		GetClientRect(hwnd, &cltrect);
		edgew = (wndrect.right - wndrect.left) - cltrect.right;
		edgeh = (wndrect.bottom - wndrect.top) - cltrect.bottom;
		wndrect.right = wndrect.left + CLIENTWIDTH + edgew;
		wndrect.bottom = wndrect.top + CLIENTHEIGHT + edgeh;
		MoveWindow(hwnd, wndrect.left, wndrect.top, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, FALSE);

		//init double mouse button state
		lastdoublemb = false;
		break;
	case WM_DESTROY:
		//release bitmaps
		freebitmaps();
		//release device contex
		ReleaseDC(hwnd, hdc);
		//save game info
		savegame(buffer);

		PostQuitMessage(0);
		break;
	case WM_PAINT:
		//redraw the whole client area
		hpaintdc = BeginPaint(hwnd, &ps);
		hdcbuffer = CreateCompatibleDC(hpaintdc);
		hbmbuffer = CreateCompatibleBitmap(hpaintdc, CLIENTWIDTH, CLIENTHEIGHT);
		SelectObject(hdcbuffer, hbmbuffer);

		drawclientbg(hdcbuffer, 0, 0);
		drawinfobg(hdcbuffer, INFOLEFT, INFOTOP);
		drawmapareabg(hdcbuffer, MAPAREALEFT, MAPAREATOP);
		drawminebg(hdcbuffer, MINELEFT, MINETOP);
		drawtimebg(hdcbuffer, TIMELEFT, TIMETOP);
		drawinfonumsNB(hdcbuffer, IMNSLEFT, IMNSTOP, Game.minesleft);
		drawinfonumsNB(hdcbuffer, ITNSLEFT, ITNSTOP, Game.time);
		memset(Game.mapchange, 1, sizeof(bool)*Game.size);
		paintmapNB(hdcbuffer, MAPUNITSLEFT, MAPUNITSTOP);

		BitBlt(hpaintdc, 0, 0, CLIENTWIDTH, CLIENTHEIGHT, hdcbuffer, 0, 0, SRCCOPY);
		DeleteObject(hbmbuffer);
		DeleteObject(hdcbuffer);

		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		if (lparam == 0 && HIWORD(wparam) == 0) MenuProc(wparam);
		else if (LOWORD(wparam) == ID_RESETB) {
			if (HIWORD(wparam) == BN_CLICKED) {
				PostMessage(hwnd, WM_GAMERESET, 0, 0);
				SetFocus(hwnd);
			}
		}
		break;
	case WM_GAMERESET:
		KillTimer(hwnd, GAMETIMERID);
		cleanmap();
		lastdoublemb = false;
		PostMessage(hresetb, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_resetb);
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_GAMESTART:
		createmap((int)lparam);
		Game.state = PROGRESS;
		Game.time = 0;
		SetTimer(hwnd, GAMETIMERID, GAMETIMERELAPSE, NULL);
		clickunit((int)lparam);
		openblanks((int)lparam);
		if (Game.mines == Game.size - Game.unitsuncov)
			PostMessage(hwnd, WM_GAMESUCCESS, 0, 0);
		else
			paintmap(hdc, MAPUNITSLEFT, MAPUNITSTOP);
		break;
	case WM_GAMEFAIL:
		KillTimer(hwnd, GAMETIMERID);
		Game.state = FAIL;
		PostMessage(hresetb, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_fail);
		//show all mines positions when game set
		uncovallmines();
		paintmap(hdc, MAPUNITSLEFT, MAPUNITSTOP);
		break;
	case WM_GAMESUCCESS:
		KillTimer(hwnd, GAMETIMERID);
		Game.state = SUCCESS;
		PostMessage(hresetb, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_success);
		//show all mines positions when game set
		Game.minesleft = 0;
		drawinfonums(hdc, IMNSLEFT, IMNSTOP, 0);
		uncovallmines();
		paintmap(hdc, MAPUNITSLEFT, MAPUNITSTOP);
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
		KillTimer(hwnd, GAMETIMERID);
		setgamemode((byte)wparam, GETCHGWIDTH(lparam), GETCHGHEIGHT(lparam), GETCHGMINES(lparam));
		lastdoublemb = false;
		//change window size for new game map size, no need to reset
		GetWindowRect(hwnd, &wndrect);
		wndrect.right = CLIENTWIDTH + edgew + wndrect.left;
		wndrect.bottom = CLIENTHEIGHT + edgeh + wndrect.top;
		//invalidate all client area for repaint
		InvalidateRect(hwnd, NULL, FALSE);
		MoveWindow(hwnd, wndrect.left, wndrect.top, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, TRUE);
		MoveWindow(hresetb, RESETBLEFT, RESETBTOP, RESETBSIZE, RESETBSIZE, TRUE);
		PostMessage(hresetb, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_resetb);
		break;
	case WM_LBUTTONDOWN:
		//won't work after game finishing
		if (Game.state == FAIL || Game.state == SUCCESS) break;

		PostMessage(hresetb, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_click);

		//break when not in the map area
		if (!lparamispos(lparam)) break;
		index = lparamtoindex(lparam);
		if (wparam & MK_RBUTTON) {	//double buttons down
			lastdoublemb = true;
			Neighbor indexes;
			getneighbors(indexes, index);
			showclickdown(hdc, MAPUNITSLEFT, MAPUNITSTOP, indexes);
		}
		else {	//single button
			lastdoublemb = false;
			showclickdown(hdc, MAPUNITSLEFT, MAPUNITSTOP, index);
		}
		break;
	case WM_LBUTTONUP:
		//won't work after game finishing
		if (Game.state == FAIL || Game.state == SUCCESS) break;

		PostMessage(hresetb, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_resetb);

		//break when not in the map area
		if (!lparamispos(lparam)) break;
		index = lparamtoindex(lparam);
		if (wparam & MK_RBUTTON) {	//double buttons
			lastdoublemb = true;
			int ret = digneighbors(index);
			if (ret == -1) {
				PostMessage(hwnd, WM_GAMEFAIL, 0, 0);
			}
			else if (Game.mines == Game.size - Game.unitsuncov) {
				PostMessage(hwnd, WM_GAMESUCCESS, 0, 0);
			}
			else
				paintmap(hdc, MAPUNITSLEFT, MAPUNITSTOP);
		}
		else if (!lastdoublemb) {	//single button and last was not double button
			lastdoublemb = false;
			if (Game.state == INITIAL && GETUNITSTATE(Game.map[index]) != UNITFLAG) {	//first click
				PostMessage(hwnd, WM_GAMESTART, 0, index);
			}
			else {	//not first click
				int ret = clickunit(index);
				if (ret == 0) openblanks(index);
				if (ret == -1) {
					PostMessage(hwnd, WM_GAMEFAIL, 0, 0);
				}
				else if (Game.mines == Game.size - Game.unitsuncov) {
					PostMessage(hwnd, WM_GAMESUCCESS, 0, 0);
				}
				else
					paintmap(hdc, MAPUNITSLEFT, MAPUNITSTOP);
			}
		}
		else {	//single button and last was double button
			lastdoublemb = false;
			paintmap(hdc, MAPUNITSLEFT, MAPUNITSTOP);
		}
		break;
	case WM_RBUTTONDOWN:
		//won't work after game finishing
		if (Game.state == FAIL || Game.state == SUCCESS) break;

		//break when not in the map area
		if (!lparamispos(lparam)) break;
		index = lparamtoindex(lparam);
		if (wparam & MK_LBUTTON) {	//double buttons
			lastdoublemb = true;
			PostMessage(hresetb, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_click);
			Neighbor indexes;
			getneighbors(indexes, index);
			showclickdown(hdc, MAPUNITSLEFT, MAPUNITSTOP, indexes);
		}
		else {	//single button, flag a unit or mark a unit
			lastdoublemb = false;
			if (GETUNITSTATE(Game.map[index]) == UNITCOVER) {
				Game.map[index] = SETUNITSTATE(UNITFLAG, Game.map[index]);
				drawmapunit(hdc, MAPUNITSLEFT + indextorleft(index), MAPUNITSTOP + indextortop(index), index);
				Game.minesleft--;
				drawinfonums(hdc, IMNSLEFT, IMNSTOP, Game.minesleft);
			}
			else if (GETUNITSTATE(Game.map[index]) == UNITFLAG && Game.mark) {
				Game.map[index] = SETUNITSTATE(UNITMARK, Game.map[index]);
				drawmapunit(hdc, MAPUNITSLEFT + indextorleft(index), MAPUNITSTOP + indextortop(index), index);
				Game.minesleft++;
				drawinfonums(hdc, IMNSLEFT, IMNSTOP, Game.minesleft);
			}
			else if (GETUNITSTATE(Game.map[index]) == UNITFLAG && !Game.mark)
			{
				Game.map[index] = SETUNITSTATE(UNITCOVER, Game.map[index]);
				drawmapunit(hdc, MAPUNITSLEFT + indextorleft(index), MAPUNITSTOP + indextortop(index), index);
				Game.minesleft++;
				drawinfonums(hdc, IMNSLEFT, IMNSTOP, Game.minesleft);
			}
			else if (GETUNITSTATE(Game.map[index]) == UNITMARK) {
				Game.map[index] = SETUNITSTATE(UNITCOVER, Game.map[index]);
				drawmapunit(hdc, MAPUNITSLEFT + indextorleft(index), MAPUNITSTOP + indextortop(index), index);
			}
		}
		break;
	case WM_RBUTTONUP:
		//won't work after game finishing
		if (Game.state == FAIL || Game.state == SUCCESS) break;

		PostMessage(hresetb, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm_resetb);

		//break when not in the map area
		if (!lparamispos(lparam)) break;
		index = lparamtoindex(lparam);
		if (wparam & MK_LBUTTON) {	//double buttons
			lastdoublemb = true;
			int ret = digneighbors(index);
			if (ret == -1) {
				PostMessage(hwnd, WM_GAMEFAIL, 0, 0);
			}
			else if (Game.mines == Game.size - Game.unitsuncov) {
				PostMessage(hwnd, WM_GAMESUCCESS, 0, 0);
			}
			else
				paintmap(hdc, MAPUNITSLEFT, MAPUNITSTOP);
		}
		else	//single button
			lastdoublemb = false;
		break;
	case WM_MOUSEMOVE:
		//won't work after game finishing
		if (Game.state == FAIL || Game.state == SUCCESS) break;

		//break when not in the map area
		if (!lparamispos(lparam)) {
			paintmap(hdc, MAPUNITSLEFT, MAPUNITSTOP);
			break;
		}
		index = lparamtoindex(lparam);
		if (wparam & MK_LBUTTON) {	//with left button down
			if (wparam & MK_RBUTTON) {	//double buttons
				Neighbor indexes;
				getneighbors(indexes, index);
				showclickdown(hdc, MAPUNITSLEFT, MAPUNITSTOP, indexes);
			}
			//single button
			else
				showclickdown(hdc, MAPUNITSLEFT, MAPUNITSTOP, index);
		}
		//without mouse button down
		else
			paintmap(hdc, MAPUNITSLEFT, MAPUNITSTOP);
		break;
	case WM_TIMER:
		if (Game.time <= MAXTIME) {
			Game.time++;
			drawinfonums(hdc, ITNSLEFT, ITNSTOP, Game.time);
		}
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
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, hWnd, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}