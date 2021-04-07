#pragma once

/*
 * this file defines functions to encapslate several optinos about ui and game
 * functions defined in this file are supplements for main.cpp
 */

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "defines.h"
#include "gamecore.h"
#include "userinterface.h"
#include "resource.h"
#include <tchar.h>
#include <strsafe.h>
#pragma comment(lib, "Version.lib")

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
int str2dword(
	_In_ TCHAR *str,
	_In_ dword size,
	_Out_ dword &x
);
template <dword size> inline int str2dword(
	_In_ TCHAR(&str)[size],
	_Out_ dword &x
)
{ return str2dword(str, size, x); }

//return length of transformed c_string
int dword2str(
	_Out_ TCHAR *str,
	_In_ dword size,
	_In_ dword x
);
template <dword size> inline int dword2str(
	_Out_ TCHAR(&str)[size],
	_In_ dword x
)
{ return dword2str(str, size, x); }

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

//create a c_string to show time score
//return length of the new c_string
int maketimestr(
	_Out_ TCHAR *buffer,
	_In_ int size,
	_In_ dword time,
	_In_ const TCHAR *timeunit = TEXT(DEFTIMEUNITEN)
);
template <dword size>
inline int maketimestr(
	_Out_ TCHAR(&buffer)[size],
	_In_ dword time,
	_In_ const TCHAR *timeunit = TEXT(DEFTIMEUNITEN)
)
{ return maketimestr(buffer, size, time, timeunit); }


//save file management
void initgame(_In_ TCHAR *Path, _Out_ POINT &lastwndpos);
void savegame(_In_ TCHAR *Path, _In_ POINT &wndpos);


//get file version information
void getversion(_Out_ TCHAR* version, _In_ int size_in_ch);