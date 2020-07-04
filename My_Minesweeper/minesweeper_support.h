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

#define WM_GAMERESET	(WM_APP + 0)		//-
#define WM_GAMESUCCESS	(WM_APP + 1)		//-
#define WM_GAMEFAIL		(WM_APP + 2)		//-
#define WM_GAMESTART	(WM_APP + 3)		//use lparam as start position
#define WM_GAMEMODECHG	(WM_APP + 4)		//use wparam as old mode, use lparam as old width(loword) and height(hiword)
//#define WM_MENUMARK		(WM_APP + 5)		//use wparam as mark sign
#define WM_MINECHG		(WM_APP + 6)		//-
#define WM_SETMINE		(WM_APP + 7)		//use lparam as mines
#define WM_TIMESTART	(WM_APP + 8)		//-
#define WM_TIMESTOP		(WM_APP + 9)		//-
#define WM_SETTIME		(WM_APP + 10)		//use lparam as time
#define WM_CLICKDOWN	(WM_APP + 11)		//-
#define WM_CLICKUP		(WM_APP + 12)		//-

extern HINSTANCE hInst;
extern HWND hWnd;
extern HMENU hMenu;
extern HWND hInfo, hMapArea;
extern HWND hMine, hTime;

extern HBITMAP hbm_reset, hbm_click, hbm_fail, hbm_success;
extern HBITMAP hbm_infonums[BM_INFONUMS_COUNT];
extern HBITMAP hbm_unit, hbm_unitflag, hbm_unitmark;
extern HBITMAP hbm_mapnums[BM_MAPNUMS_COUNT];
extern HBITMAP hbm_unitmine, hbm_unitbomb, hbm_unitwrong, hbm_unitmarkdown;
extern HBITMAP &hbm_unitdown;


//change a mouse position to map index
int lparamtopos(LPARAM lparam);

//transform between positive integer and c_string
//return -1 if error
int str2int(char *str, dword size, dword &x);
template <dword size> int str2int(char(&str)[size], dword &x) { return str2int(str, size, x); }
//return length of transformed c_string
int int2str(char *str, dword size, dword x);
template <dword size> int int2str(char(&str)[size], dword x) { return int2str(str, size, x); }

//manage bitmaps
void loadbitmaps();
void freebitmaps();

//get flexiable dialog's position
int infow();
int resetbuttonx();
int timedlgx();
int mapareaw();
int mapareah();

//manage Static Control in MapArea
HWND *createstatics(HWND hparent);
void deletestatics(HWND **hstatic, word size);

void paintmap(HWND hstatic[]);
void showclickdown(HWND hstatic[], Neighbor &poses);
void showclickdown(HWND hstatic[], int pos);

//change checked position in Menu
void changechecked(byte GameMode);
void changemark(bool Mark);

//get best score info
dword besttime();
char *bestname();

//create a c_string to show time score
//return length of the new c_string
int maketimestr(char *buffer, int size, dword time, const char *timeunit = DEFTIMEUNIT);
template <dword size>
int maketimestr(char(&buffer)[size], dword time, const char *timeunit = DEFTIMEUNIT)
{ return maketimestr(buffer, size, time, timeunit); }