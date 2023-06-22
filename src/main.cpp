#include "stdincludes.h"
#include "procfunctions.h"


HINSTANCE hInst;
HWND hWnd;
HMENU hMenu;


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_CREATE:
		return onCreate(hwnd, wparam, lparam);
	case WM_DESTROY:
		return onDestroy(hwnd, wparam, lparam);
	case WM_PAINT:
		return onPaint(hwnd, wparam, lparam);
	case WM_COMMAND:
		return onCommand(hwnd, wparam, lparam);
	case WM_GAMERESET:
		return onGameReset(hwnd, wparam, lparam);
	case WM_GAMESTART:
		return onGameStart(hwnd, wparam, lparam);
	case WM_GAMEFAIL:
		return onGameFail(hwnd, wparam, lparam);
	case WM_GAMESUCCESS:
		return onGameSuccess(hwnd, wparam, lparam);
	case WM_GAMEMODECHG:
		return onGameModeChg(hwnd, wparam, lparam);
	case WM_LBUTTONDOWN:
		return onLButtonDwon(hwnd, wparam, lparam);
	case WM_LBUTTONUP:
		return onLButtonUp(hwnd, wparam, lparam);
	case WM_RBUTTONDOWN:
		return onRButtonDown(hwnd, wparam, lparam);
	case WM_RBUTTONUP:
		return onRButtonUp(hwnd, wparam, lparam);
	case WM_MOUSEMOVE:
		return onMouseMove(hwnd, wparam, lparam);
	case WM_TIMER:
		return onTimer(hwnd, wparam, lparam);
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wndc = { sizeof(WNDCLASSEX) };
	wndc.cbClsExtra = 0;
	wndc.cbWndExtra = 0;
	wndc.hInstance = hInstance;
	wndc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndc.lpfnWndProc = WndProc;
	wndc.lpszClassName = TEXT(WNDC_NAME);
	wndc.lpszMenuName = NULL;
	wndc.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassEx(&wndc);

	hInst = hInstance;
	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));
	hWnd = CreateWindowEx(0, wndc.lpszClassName, TEXT(APP_NAME),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
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
