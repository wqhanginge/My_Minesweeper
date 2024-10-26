/*****************************************************************************\
 *  My Minesweepper -- a classic minesweeper game
 *  Copyright (C) 2020-2024 Gee Wang
 *
 *  This file is part of My Minesweeper.
 *
 *  My Minesweeper is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  My Minesweeper is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
\*****************************************************************************/
/*****************************************************************************\
 * main.c
 *****************************************************************************
 * This file is the program entry and contains the main Window Procedure.
\*****************************************************************************/


#include "stdafx.h"
#include "procfunctions.h"


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
    case WMAPP_GAMERESET:
        return onGameReset(hwnd, wparam, lparam);
    case WMAPP_GAMEWIN:
        return onGameWin(hwnd, wparam, lparam);
    case WMAPP_GAMELOSS:
        return onGameLoss(hwnd, wparam, lparam);
    case WMAPP_GAMEMODECHANGE:
        return onGameModeChange(hwnd, wparam, lparam);
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
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wndc = { sizeof(WNDCLASSEX) };
    wndc.cbClsExtra = 0;
    wndc.cbWndExtra = 0;
    wndc.hInstance = hInstance;
    wndc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));
    wndc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));
    wndc.hbrBackground = NULL;
    wndc.lpfnWndProc = WndProc;
    wndc.lpszClassName = TEXT(WNDCLS_NAME);
    wndc.lpszMenuName = NULL;
    wndc.style = CS_VREDRAW | CS_HREDRAW;

    RegisterClassEx(&wndc);

    HWND hwnd = CreateWindow(
        wndc.lpszClassName,
        TEXT(WND_NAME),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MAINMENU)),
        hInstance,
        NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
