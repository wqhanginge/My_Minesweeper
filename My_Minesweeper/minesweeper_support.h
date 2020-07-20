#pragma once

/*
 * this file is the main head file including all needed external functons
 * functions defined in this file are supplements for main.cpp
 */

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "resource.h"
#include "minesweeper_defines.h"
#include "minesweeper_game.h"
#include "minesweeper_ui.h"

#define WM_GAMERESET	(WM_APP + 0)		//-
#define WM_GAMESUCCESS	(WM_APP + 1)		//-
#define WM_GAMEFAIL		(WM_APP + 2)		//-
#define WM_GAMESTART	(WM_APP + 3)		//use lparam as start position
#define WM_GAMEMODECHG	(WM_APP + 4)		//use wparam as new mode, use lparam as new width, height and mines

#define MAKECHGLPARAM(w, h, m)	((LPARAM)((((dword)(w) & 0xFF) | (((dword)(h) & 0xFF) << 8)) | (((dword)(m) & 0xFFFF) << 16)))
#define GETCHGWIDTH(l)			((byte)((dword)(l) & 0xFF))
#define GETCHGHEIGHT(l)			((byte)(((dword)(l) >> 8) & 0xFF))
#define GETCHGMINES(l)			((word)(((dword)(l) >> 16) & 0xFFFF))

#define ID_RESETB	5000

extern HINSTANCE hInst;
extern HWND hWnd;
extern HMENU hMenu;

extern HBITMAP hbm_resetb, hbm_click, hbm_fail, hbm_success;


//if a mouse position is inside the MapUnits area
bool lparamispos(_In_ LPARAM laparm);
//change a mouse position to map index
int lparamtoindex(_In_ LPARAM lparam);

//transform between positive integer and c_string
//return -1 if error
int str2int(_In_ char *str, _In_ dword size, _Out_ dword &x);
template <dword size> inline int str2int(_In_ char(&str)[size], _Out_ dword &x)
{ return str2int(str, size, x); }

//return length of transformed c_string
int int2str(_Out_ char *str, _In_ dword size, _In_ dword x);
template <dword size> inline int int2str(_Out_ char(&str)[size], _In_ dword x)
{ return int2str(str, size, x); }

/* manage MapUnit in MapArea */
//paint GameMap, use 'left' and 'top' as position zero of the whole map
void paintmap(
	_In_ HDC hdestdc,
	_In_ int muleft,
	_In_ int mutop
);
//paint GameMap directly, use 'left' and 'top' as position zero of the whole map
void paintmapNB(
	_In_ HDC hdestdc,
	_In_ int muleft,
	_In_ int mutop
);
//paint specific MapUnit directly, use 'left' and 'top' as position zero of the whole map
void showclickdown(
	_In_ HDC hdestdc,
	_In_ int muleft,
	_In_ int mutop,
	_In_ Neighbor &indexes
);
//paint specific MapUnit directly, use 'left' and 'top' as position zero of the whole map
void showclickdown(
	_In_ HDC hdestdc,
	_In_ int muleft,
	_In_ int mutop,
	_In_ int index
);

//change checked position in Menu
void changechecked(_In_ byte GameMode);
void changemark(_In_ bool Mark);

//get best score info
dword besttime();
char *bestname();

//create a c_string to show time score
//return length of the new c_string
int maketimestr(_Out_ char *buffer, _In_ int size, _In_ dword time, _In_ const char *timeunit = DEFTIMEUNIT);
template <dword size>
int maketimestr(_Out_ char(&buffer)[size], _In_ dword time, _In_ const char *timeunit = DEFTIMEUNIT)
{ return maketimestr(buffer, size, time, timeunit); }