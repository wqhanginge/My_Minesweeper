/*****************************************************************************\
 *  My Minesweepper -- a classic minesweeper game
 *  Copyright (C) 2020-2022  Gee W.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
\*****************************************************************************/
/*****************************************************************************\
 * basicUI.h
 *****************************************************************************
 * this file contains the Game UI design and basic drawing functions
 * this file also contains some transform functions
 * drawing functions use Win32 APIs and will draw directly on DC without
 * creating a DC-buffer
 * 
 * NOTE:almost all functions have no arg check process, use with care
\*****************************************************************************/


#pragma once

#include "stdincludes.h"
#include "gamecore.h"


/*         Main Client Area
 * +-----------------------------+---
 * |                             | |
 * |            Info             |Info_height
 * |                             | |
 * +-----------------------------+---
 * |                             | |
 * |                             | |
 * |           MapArea           |MapArea_height
 * |                             | |
 * |                             | |
 * +-----------------------------+---
 * |------------width------------|
 *
 * the Main Client Area is composed of Info Part and MapArea Part
 * 
 * NOTE: these two Parts have edges of their own
 * NOTE: there are edges around Main Client Area and between these two Parts
 *
 * 'Info_height' is static
 * 'MapArea_height' and 'width' are flexible
 */

/*                    Info
 * +----------------------------------------+---
 * |  +------+      +------+ --   +------+  |--- top_dist
 * |  | Mine |      |Reset | |    | Time |  |inums_height
 * |  +------+      |      | |D   +------+  |---
 * |                +------+ --             |bottom_dist
 * +----------------------------------------+---
 * |--|------|------|------|------|------|--|
 *  A    B      C      D      E      F    G
 * A:left_dist	B:inums_width	C:mid_dist	D:reset_button_size
 * E:mid_dist	F:inums_width	G:right_dist
 *
 * the Info Part contains Mine Part, Time Part and Reset Button
 * 
 * static: top_dist, bottom_dist, nums_height, left_dist, right_dist,
 *         nums_width, reset_button_size
 * flexible: mid_dist
 * 
 * NOTE: Reset Button is NOT a "BUTTON" Window
 * NOTE: Reset Button is a Square, Mine Part and Time Part are Rectangles
 * NOTE: the 'reset_button_size' is different from 'inums_height' and 'inums_width',
 *       i.e. the 'bottom_dist' can be varied, but the 'top_dist' is static
 * NOTE: Mine Part and Time Part have edges of their own,
 *       but Reset Button have no edges
 *
 * Mine Part, Time Part and Reset Button have static size, and Reset Button is
 * always center aligned and Mine Part/Time Part is always close to the edge,
 * but Info Part has flexible width, so that it's necessary to recalculate
 * the position when Game Mode is changed
 * 
 * PS: although we say that Reset Button has NO edge, we still regard it as
 *     edge+inner structure because it also has a 3D outlook(button like),
 *     so as the MapUnit in MapArea Part
 */

/*     Mine            Time
 * +---+---+---+   +---+---+---+
 * |   |   |   |   |   |   |   |
 * | N | N | N |   | N | N | N |
 * |   |   |   |   |   |   |   |
 * +---+---+---+   +---+---+---+
 * 
 * N: a one-digit number from 0 to 9
 * 
 * the Mine Part and Time Part are both called Num Part, which means
 * they have same UI structure, and the Num Part is composed of
 * three same one-digit number, which is called InfoNum
 * 
 * for convenient, we call the combination of InfoNum as INums, and relevantly,
 * we call the INums in Mine Part and Time Part as MNums and TNums, respectively
 * 
 * NOTE: InfoNum has static size so that INums has static size
 * NOTE: the INums has no edges
 */

/*       LEFT           RIGHT
 *         v              v
 * TOP---->+--------------+
 *         | +----------+ |
 * Edge----->|  Content | |
 *         | |          | |
 *         | +----------+ |
 * BOTTOM->+--------------+
 *             Part/Area
 * 
 * a Part have edges, and can have another Part in its Content
 * there are edges between Main Client Area and two main Parts
 * 
 * NOTE: the child Part may not fully fill the parent Part's Content
 */

/*         px
 *       ----->
 *        0   1   2   3
 *      +---+---+---+---+ --
 * py| 0| 0 | 1 | 2 | 3 |  h
 *   |  +---+---+---+---+ --
 *   v 1| 4 |...index...|
 *      +---+---+---+---+
 *      |<w>|
 *              Map
 * 
 * 'px' and 'py' means positon in pixel on window
 * 'x' and 'y' means position in unit on Game Map
 * px = x * w, py = y * h, ppos = (px, py)
 * 
 * NOTE: 'px' and 'py' indicate the left-top of one MapUnit
 */


#define MU_SIZE			24
#define AREA_EDGE		6
#define PART_EDGE		2
#define COLOR_DEFBG		RGB(240,240,240)
#define COLOR_DEFDARK	RGB(192,192,192)
#define COLOR_DEFLIGHT	RGB(255,255,255)
#define COLOR_DEFSEMIL	RGB(227,227,227)
#define COLOR_DEFSHADOW	RGB(128,128,128)
#define COLOR_DEFSEMIS	RGB(160,160,160)

/*following two macros use variables from gamecore.h, make sure it is included*/
#define MAPUNITS_WIDTH	(Game.width * MU_SIZE)
#define MAPUNITS_HEIGHT	(Game.height * MU_SIZE)

//Info Part
#define INFO_WIDTH		(PART_EDGE + MAPUNITS_WIDTH + PART_EDGE)
#define INFO_HEIGHT		(PART_EDGE + 36 + PART_EDGE)
#define INFO_LEFT		AREA_EDGE
#define INFO_TOP		AREA_EDGE

#define COLOR_INFO		COLOR_DEFBG
#define COLOR_INFOL		COLOR_DEFLIGHT
#define COLOR_INFOS		COLOR_DEFSHADOW
//end Info Part

//Mine Part, Time Part and Reset Button
#define LEFT_DIST		(PART_EDGE + 6)
#define RIGHT_DIST		LEFT_DIST
#define INFONUM_WIDTH	15
#define INFONUM_HEIGHT	27
#define INUMS_WIDTH		(INFONUM_WIDTH * 3)
#define INUMS_HEIGHT	INFONUM_HEIGHT
#define INUMS_EDGE		1
#define NUM_WIDTH		(INUMS_EDGE + INUMS_WIDTH + INUMS_EDGE)
#define NUM_HEIGHT		(INUMS_EDGE + INUMS_HEIGHT + INUMS_EDGE)

#define COLOR_INFONUMBG	RGB(0,0,0)
#define COLOR_INFONUMBT	RGB(255,0,0)
#define COLOR_INFONUMDK	RGB(128,0,0)
#define COLOR_NUM		COLOR_DEFBG
#define COLOR_NUML		COLOR_DEFLIGHT
#define COLOR_NUMS		COLOR_DEFSHADOW

#define MINE_WIDTH		NUM_WIDTH
#define MINE_HEIGHT		NUM_HEIGHT
#define MINE_LEFT		(INFO_LEFT + LEFT_DIST)
#define MINE_TOP		(INFO_TOP + PART_EDGE + 3)
#define MNUMS_WIDTH		INUMS_WIDTH
#define MNUMS_HEIGHT	INUMS_HEIGHT
#define MNUMS_LEFT		(MINE_LEFT + INUMS_EDGE)
#define MNUMS_TOP		(MINE_TOP + INUMS_EDGE)

#define COLOR_MINE		COLOR_NUM
#define COLOR_MINEL		COLOR_NUML
#define COLOR_MINES		COLOR_NUMS

#define TIME_WIDTH		NUM_WIDTH
#define TIME_HEIGHT		NUM_HEIGHT
#define TIME_LEFT		(INFO_LEFT + INFO_WIDTH - RIGHT_DIST - TIME_WIDTH)
#define TIME_TOP		MINE_TOP
#define TNUMS_WIDTH		INUMS_WIDTH
#define TNUMS_HEIGHT	INUMS_HEIGHT
#define TNUMS_LEFT		(TIME_LEFT + INUMS_EDGE)
#define TNUMS_TOP		(TIME_TOP + INUMS_EDGE)

#define COLOR_TIME		COLOR_NUM
#define COLOR_TIMEL		COLOR_NUML
#define COLOR_TIMES		COLOR_NUMS

#define RB_SIZE			32
#define RB_LEFT			(INFO_LEFT + (INFO_WIDTH - RB_SIZE) / 2)
#define RB_TOP			(INFO_TOP + PART_EDGE + 2)
#define BMP_SIZE		28
#define BMP_LEFT		(RB_LEFT + 2)
#define BMP_TOP			(RB_TOP + 2)

#define COLOR_RB		COLOR_DEFBG
#define COLOR_RBL		COLOR_DEFLIGHT
#define COLOR_RBS		COLOR_DEFSHADOW
#define COLOR_RBSL		COLOR_DEFSEMIL
#define COLOR_RBSS		COLOR_DEFSEMIS
//end Mine Part, Time Part and Reset Button

//MapArea
#define MAPAREA_WIDTH	INFO_WIDTH
#define MAPAREA_HEIGHT	(PART_EDGE + MAPUNITS_HEIGHT + PART_EDGE)
#define MAPAREA_LEFT	AREA_EDGE
#define MAPAREA_TOP		(AREA_EDGE + INFO_HEIGHT + AREA_EDGE)

#define MAP_WIDTH		MAPUNITS_WIDTH
#define MAP_HEIGHT		MAPUNITS_HEIGHT
#define MAP_LEFT		(MAPAREA_LEFT + PART_EDGE)
#define MAP_TOP			(MAPAREA_TOP + PART_EDGE)

#define COLOR_MAPAREA	COLOR_DEFBG
#define COLOR_MAPAREAL	COLOR_DEFLIGHT
#define COLOR_MAPAREAS	COLOR_DEFSHADOW

#define COLOR_MUCOVER	COLOR_DEFDARK
#define COLOR_MUCOVERL	COLOR_DEFLIGHT
#define COLOR_MUCOVERS	COLOR_DEFSHADOW
#define COLOR_MUUNCOV	COLOR_DEFBG
#define COLOR_MUUNCOVE	COLOR_DEFSHADOW
#define COLOR_MUUNCOVB	RGB(237,28,36)
#define COLOR_MUMINE	RGB(0,0,0)
#define COLOR_MUMINEL	RGB(255,255,255)
#define COLOR_MUFLAGF	RGB(255,0,0)
#define COLOR_MUFLAGB	RGB(0,0,0)
#define COLOR_MUMARK	RGB(0,0,0)
#define COLOR_MUCROSS	RGB(255,0,0)
#define COLOR_MUNUM1	RGB(0,0,255)
#define COLOR_MUNUM2	RGB(0,128,0)
#define COLOR_MUNUM3	RGB(255,0,0)
#define COLOR_MUNUM4	RGB(0,0,128)
#define COLOR_MUNUM5	RGB(128,0,0)
#define COLOR_MUNUM6	RGB(0,128,128)
#define COLOR_MUNUM7	RGB(0,0,0)
#define COLOR_MUNUM8	RGB(128,128,128)
//end MapArea

//Client Area
#define CLIENT_WIDTH	(AREA_EDGE + INFO_WIDTH + AREA_EDGE)
#define CLIENT_HEIGHT	(AREA_EDGE + INFO_HEIGHT + AREA_EDGE + MAPAREA_HEIGHT + AREA_EDGE)
#define CLIENT_LEFT		0
#define CLIENT_TOP		0

#define COLOR_CLIENT	COLOR_DEFBG
#define COLOR_CLIENTL	COLOR_DEFLIGHT
#define COLOR_CLIENTS	COLOR_DEFSHADOW
//end Client Area



/* this array stores InfoNum backgroud, it is private
/*
const bool InfoNumBG[INFONUM_WIDTH][INFONUM_HEIGHT];
*/



/* these function implementations are private
 * they are invisible outside
 */

/* these functions draw a concave or convex like area with different type of edges
/**********************************************************************************
//draw 2 pixel edge concave background,
//exchange 'light' and 'shadow' to draw a convex background
//no DC-Buffer
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
//draw 2 pixel edge concave background with 2 layers of color,
//exchange colors to draw a convex background
//no DC-Buffer
static void drawdoubleedgebg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int width,
	_In_ int height,
	_In_ COLORREF inner,
	_In_ COLORREF light,
	_In_ COLORREF semilight,
	_In_ COLORREF shadow,
	_In_ COLORREF semishadow
);
//draw 1 pixel edge concave background,
//exchange 'light' and 'shadow' to draw a convex background
//no DC-Buffer
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
//draw 1 pixel half edge 2D like background,
//draw left edge and top edge
//no DC-Buffer
void drawhalfedgebg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int width,
	_In_ int height,
	_In_ COLORREF inner,
	_In_ COLORREF edge
);
*/

/* these functions draw a seven-segment display like area,
 * which is used for InfoNum
/*****************************************************************************
//no DC-Buffer
//draw 7sd background
static inline void draw7sdbg(_In_ HDC hdestdc, _In_ int left, _In_ int top);

//following draw each segment in 7sd
//    a
//  +---+
// f| g |b
//  +---+
// e|   |c
//  +---+
//    d
static void draw7sda(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdb(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdc(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdd(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sde(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdf(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdg(_In_ HDC h7sddc, _In_ int left, _In_ int top);
*/

/* these functions draw different content inside a MapUnit
/***********************************************************************************
//no DC-Buffer
//draw a mine icon
static void drawmuitemmine(_In_ HDC hdestdc, _In_ int left, _In_ int top);

//draw a question mark icon
static void drawmuitemmark(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ bool clicked);

//draw a flag icon
static void drawmuitemflag(_In_ HDC hdestdc, _In_ int left, _In_ int top);

//draw a cross icon
static void drawmuitemcross(_In_ HDC hdestdc, _In_ int left, _In_ int top);

//following draw a number inside MapUnit
static void drawmuitemnum1(_In_ HDC hdestdc, _In_ int left, _In_ int top);
static void drawmuitemnum2(_In_ HDC hdestdc, _In_ int left, _In_ int top);
static void drawmuitemnum3(_In_ HDC hdestdc, _In_ int left, _In_ int top);
static void drawmuitemnum4(_In_ HDC hdestdc, _In_ int left, _In_ int top);
static void drawmuitemnum5(_In_ HDC hdestdc, _In_ int left, _In_ int top);
static void drawmuitemnum6(_In_ HDC hdestdc, _In_ int left, _In_ int top);
static void drawmuitemnum7(_In_ HDC hdestdc, _In_ int left, _In_ int top);
static void drawmuitemnum8(_In_ HDC hdestdc, _In_ int left, _In_ int top);
*/


/* following functions are public */

//transform pixel position on MapArea into GameMap index
//use the whole Map's left-top as position 0
//take care of the input, they must be offset of MAP_LEFT and MAP_TOP
int ppos2index(int px, int py);
int px2x(int px);
int py2y(int py);

//transform a GameMap index into MapArea pixels
//use the whole Map's left-top as position 0
//take care of the output, they are offset of MAP_LEFT and MAP_TOP respectively
int index2px(int index);
int index2py(int index);
int x2px(int x);
int y2py(int y);

//logical xor
inline bool _xor(const bool A, const bool B);


/* these functions draw backgroung with edge for each Part
 * color and size are predefined, check relative macros
 * a DC handle and position should be provided by calling function
 */
//w:CLIENT_WIDTH, h:CLIENT_HEIGHT
//no DC-Buffer
void drawDCClientBg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
);

//w:INFO_WIDTH, h:INFO_HEIGHT
//no DC-Buffer
void drawDCInfoBg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
);

//w:MAPAREA_WIDTH, h:MAPAREA_HEIGHT
//no DC-Buffer
void drawDCMapAreaBg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
);

//w:NUM_WIDTH, h:NUM_HEIGHT
//no DC-Buffer
void drawDCNumBg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
);


/* these functions draw specfic numbers inside Num Part
 * color and size are predefined, check relative macros
 * size is relative to external resource file, be careful on changing it
 */

//draw all three InfoNum
//if num is out of range, it draws '---'
//-100 < num < 1000
//w:INUMS_WIDTH, h:INUMS_HEIGHT
//no DC-Buffer
void drawDCINums(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int num
);

//draw a single InfoNum directly on DC
//if num EQ -1, it draws '-', if num GE 10, it draws nothing
//w:INFONUM_WIDTH, h:INFONUM_HEIGHT
//no DC-Buffer
void drawDCInfoNum(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int num
);


/* these functions draw a Reset Button inside Info Part
 * color and size are predefined, check relative macros
 */

//draw Reset Button background directly on DC
//w:RB_SIZE, h:RB_SIZE
//no DC-Buffer
void drawDCResetButtonBg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ bool clicked
);

//draw bitmap on Reset Button directly
//it will do nothing if the hbm is NULL
//w:BMP_SIZE, h:BMP_SIZE
//no DC-Buffer
void drawDCResetButtonBmp(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ HBITMAP hbm,
	_In_ bool clicked
);

//draw a Reset Button
//use NULL for hbm if no bitmap
//w:RB_SIZE, h:RB_SIZE
//no DC-buffer
void drawDCResetButton(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ HBITMAP hbm,
	_In_ bool clicked
);


/* these functions draw a MapUnit on MapArea Part
 * color and size are predefined, check relative macros
 */
//w:MU_SIZE, h:MU_SIZE
//no DC-buffer
void drawDCMUCover(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
);

//w:MU_SIZE, h:MU_SIZE
//no DC-buffer
void drawDCMUUncov(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
);

//w:MU_SIZE, h:MU_SIZE
//no DC-buffer
void drawDCMUFlag(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
);

//w:MU_SIZE, h:MU_SIZE
//no DC-buffer
void drawDCMUMark(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ bool clicked
);

//w:MU_SIZE, h:MU_SIZE
//no DC-buffer
void drawDCMUMine(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ bool bomb
);

//w:MU_SIZE, h:MU_SIZE
//no DC-buffer
void drawDCMUWrong(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
);

//w:MU_SIZE, h:MU_SIZE
//no DC-buffer
void drawDCMUNum(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int num
);

//draw a mapunit depends on MapUnitState with default color
//draw a covered mapunit by default
//do NOT affect the Update bit
//w:MU_SIZE, h:MU_SIZE
//no DC-buffer
void drawDCMapUnit(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ byte mapunit
);


//draw Game Map directly on DC
//update map_units with Update bit and clear Update bit
//update all given map_units
//do NOT affect the Update bit
//w:MAP_WIDTH, h:MAP_HEIGHT
//no DC-Buffer
void drawDCMap(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ byte* gamemap,
	_In_ word gamesize
);
