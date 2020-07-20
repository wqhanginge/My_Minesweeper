#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "resource.h"
#include "minesweeper_defines.h"
#include "minesweeper_game.h"

extern HINSTANCE hInst;
extern HWND hWnd;
extern HMENU hMenu;

extern HBITMAP hbm_resetb, hbm_click, hbm_fail, hbm_success;

/* this array stores infonum backgroud, it is private */
/*
const bool InfoNumBG[INFONUM_WIDTH][INFONUM_HEIGHT];
*/


//manage bitmaps
void loadbitmaps();
void freebitmaps();

//get flexiable dialog's position
int mapunitswidth();
int mapunitsheight();
int infowidth();
int timeleft();
int infotimenumsleft();
int resetbleft();
int mapareawidth();
int mapareaheight();
int clientwidth();
int clientheight();

/* encapslate functions in MACRO type */
#define MAPUNITSWIDTH	mapunitswidth()
#define MAPUNITSHEIGHT	mapunitsheight()
#define INFOWIDTH		infowidth()
#define TIMELEFT		timeleft()
#define ITNSLEFT		infotimenumsleft()
#define RESETBLEFT		resetbleft()
#define MAPAREAWIDTH	mapareawidth()
#define MAPAREAHEIGHT	mapareaheight()
#define CLIENTWIDTH		clientwidth()
#define CLIENTHEIGHT	clientheight()


//transform relative position on MapArea into GameMap index
//use the first MapUnit left-top as position zero
//take care of the input, they must be offset of MAPUNITSLEFT and MAPUNITSTOP
int rpostoindex(int rleft, int rtop);
int rlefttox(int rleft);
int rtoptoy(int rtop);

//transform a GameMap index into relative MapArea position
//use the first MapUnit left-top as position zero
//take care of the output, they are offset of MAPUNITSLEFT or MAPUNITSTOP
int indextorleft(int index);
int indextortop(int index);
int xtorleft(int x);
int ytortop(int y);


/* following functions draw specific bitmaps at appointed position */

//w:CLIENTWIDTH, h:CLIENTHEIGHT
void drawclientbg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF def = COLOR_CLIENT
);

//w:INFOWIDTH, h:INFOHEIGHT
void drawinfobg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF inner = COLOR_INFO,
	_In_ COLORREF light = COLOR_INFOL,
	_In_ COLORREF shadow = COLOR_INFOS
);

//w:MAPAREAWIDTH, h:MAPAREAHEIGHT
void drawmapareabg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF inner = COLOR_MAPAREA,
	_In_ COLORREF light = COLOR_MAPAREAL,
	_In_ COLORREF shadow = COLOR_MAPAREAS
);

/* this function implementation is private
 * it is invisible outside
 */
/*
static void drawthickedgebg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int width,
	_In_ int height,
	_In_ COLORREF inner,
	_In_ COLORREF light,
	_In_ COLORREF shadow
);
*/

//w:MINEWIDTH, h:MINEHEIGHT
void drawminebg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF inner = COLOR_MINE,
	_In_ COLORREF light = COLOR_MINEL,
	_In_ COLORREF shadow = COLOR_MINES
);

//w:TIMEWIDTH, h:TIMEHEIGHT
void drawtimebg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF inner = COLOR_TIME,
	_In_ COLORREF light = COLOR_TIMEL,
	_In_ COLORREF shadow = COLOR_TIMES
);

/* this function implementation is private
 * it is invisible outside
 */
/*
static void drawthinedgebg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int width,
	_In_ int height,
	_In_ COLORREF inner,
	_In_ COLORREF light,
	_In_ COLORREF shadow
);
*/

//if num is out of range, it draws '---'
//w:INSWIDTH, h:INSHEIGHT
void drawinfonums(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int num,
	_In_ COLORREF bg = COLOR_INFONUMSBG,
	_In_ COLORREF dark = COLOR_INFONUMSDK,
	_In_ COLORREF bright = COLOR_INFONUMSBT
);
//if num is out of range, it draws '---'
//this function will draw on DC directly without creating a DC-buffer
//w:INSWIDTH, h:INSHEIGHT
void drawinfonumsNB(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int num,
	_In_ COLORREF bg = COLOR_INFONUMSBG,
	_In_ COLORREF dark = COLOR_INFONUMSDK,
	_In_ COLORREF bright = COLOR_INFONUMSBT
);

//if num EQ -1, it draws '-', if num GE 10, it draws nothing
//w:INFONUMS_WIDTH, h:INFONUMS_HEIGHT
void drawinfonum(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int num,
	_In_ COLORREF bg = COLOR_INFONUMSBG,
	_In_ COLORREF dark = COLOR_INFONUMSDK,
	_In_ COLORREF bright = COLOR_INFONUMSBT
);

/* these functions are private
 * they are invisible outside
 */
/*
static inline void draw7sdbg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF bg = COLOR_INFONUMSBG,
	_In_ COLORREF dark = COLOR_INFONUMSDK
);
static void draw7sda(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdb(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdc(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdd(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sde(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdf(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdg(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static bool _xor(const bool A, const bool B);
*/

 //w:MAPUNITZISE, h:MAPUNITSIZE
void drawmucoverbg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF inner = COLOR_MUCOVER,
	_In_ COLORREF light = COLOR_MUCOVERL,
	_In_ COLORREF shadow = COLOR_MUCOVERS
);

//w:MAPUNITZISE, h:MAPUNITSIZE
void drawmuuncovbg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF inner = COLOR_MUUNCOV,
	_In_ COLORREF edge = COLOR_MUUNCOVE
);

//w:MAPUNITSIZE, h:MAPUNITHEIGHT
void drawmuflag(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF flag = COLOR_MUFLAGF,
	_In_ COLORREF base = COLOR_MUFLAGB,
	_In_ COLORREF bginner = COLOR_MUCOVER,
	_In_ COLORREF bglight = COLOR_MUCOVERL,
	_In_ COLORREF bgshadow = COLOR_MUCOVERS
);

//w:MAPUNITSIZE, h:MAPUNITHEIGHT
void drawmumark(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ bool down,
	_In_ COLORREF mark = COLOR_MUMARK,
	_In_ COLORREF bginner = COLOR_MUCOVER,
	_In_ COLORREF bglight = COLOR_MUCOVERL,
	_In_ COLORREF bgshadow = COLOR_MUCOVERS,
	_In_ COLORREF bgdowninner = COLOR_MUUNCOV,
	_In_ COLORREF bgdownedge = COLOR_MUUNCOVE
);

//w:MAPUNITSIZE, h:MAPUNITHEIGHT
void drawmumine(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ bool bomb,
	_In_ COLORREF mine = COLOR_MUMINE,
	_In_ COLORREF light = COLOR_MUMINEL,
	_In_ COLORREF bginner = COLOR_MUUNCOV,
	_In_ COLORREF bgbombinner = COLOR_MUUNCOVB,
	_In_ COLORREF bgedge = COLOR_MUUNCOVE
);

//w:MAPUNITSIZE, h:MAPUNITHEIGHT
void drawmuwrong(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF mine = COLOR_MUMINE,
	_In_ COLORREF light = COLOR_MUMINEL,
	_In_ COLORREF cross = COLOR_MUCROSS,
	_In_ COLORREF bginner = COLOR_MUUNCOV,
	_In_ COLORREF bgedge = COLOR_MUUNCOVE
);

//w:MAPUNITSIZE, h:MAPUNITHEIGHT
void drawmunum(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int num,
	_In_ COLORREF number = COLOR_MUNUMDEF,
	_In_ COLORREF bginner = COLOR_MUUNCOV,
	_In_ COLORREF bgedge = COLOR_MUUNCOVE
);

//draw a mapunit depends on MapUnitState with default color
//w:MAPUNITSIZE, h:MAPUNITSIZE
void drawmapunit(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int index
);
void drawmapunit(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int x,
	_In_ int y
);
//draw a mapunit depends on MapUnitState with default color
//this function will draw on DC directly without creating a DC-buffer
//w:MAPUNITSIZE, h:MAPUNITSIZE
void drawmapunitNB(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int index
);
void drawmapunitNB(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int x,
	_In_ int y
);

/* these functions are private, they are invisible outside */
/*
static void drawmuitemmine(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF mine = COLOR_MUMINE,
	_In_ COLORREF light = COLOR_MUMINEL
);
static void drawmuitemmark(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ bool moved,
	_In_ COLORREF mark = COLOR_MUMARK
);
static void drawmuitemflag(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF flag = COLOR_MUFLAGF,
	_In_ COLORREF base = COLOR_MUFLAGB
);
static void drawmuitemcross(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF cross = COLOR_MUCROSS
);
static void drawmuitemnum1(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF num = COLOR_MUNUM1
);
static void drawmuitemnum2(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF num = COLOR_MUNUM2
);
static void drawmuitemnum3(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF num = COLOR_MUNUM3
);
static void drawmuitemnum4(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF num = COLOR_MUNUM4
);
static void drawmuitemnum5(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF num = COLOR_MUNUM5
);
static void drawmuitemnum6(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF num = COLOR_MUNUM6
);
static void drawmuitemnum7(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF num = COLOR_MUNUM7
);
static void drawmuitemnum8(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF num = COLOR_MUNUM8
);*/