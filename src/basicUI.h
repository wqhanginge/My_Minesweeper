/*****************************************************************************\
 *  My Minesweepper -- a classic minesweeper game
 *  Copyright (C) 2020-2023 Gee Wang
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
 * basicUI.h
 *****************************************************************************
 * This file contains the Game UI design and basic drawing functions.
 * This file also contains some transform functions.
 * Drawing functions use Win32 APIs and draw directly on Device Context(DC)
 * without creating a DC-buffer.
 * 
 * NOTE: Almost all functions have NO arg check process, use with care.
\*****************************************************************************/


#pragma once

#include "stdafx.h"


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
 * The Main Client Area is composed of Info Part and MapArea Part.
 * 
 * NOTE: These two Parts have edges of their own.
 * NOTE: There are edges around Main Client Area and between these two Parts.
 * 
 * 'Info_height' is static.
 * 'MapArea_height' and 'width' are flexible.
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
 * A:left_dist  B:inums_width   C:mid_dist  D:reset_button_size
 * E:mid_dist   F:inums_width   G:right_dist
 * 
 * The Info Part contains Mine Part, Time Part and ResetButton.
 * 
 * static: top_dist, bottom_dist, nums_height, left_dist, right_dist,
 *         nums_width, reset_button_size
 * flexible: mid_dist
 * 
 * NOTE: ResetButton is NOT a "BUTTON" Window.
 * NOTE: ResetButton is a Square, Mine Part and Time Part are Rectangles.
 * NOTE: 'reset_button_size' is different from 'inums_height' and 'inums_width',
 *       i.e. 'bottom_dist' can be varied, but 'top_dist' is constant.
 * NOTE: Mine Part and Time Part have edges of their own,
 *       but ResetButton have no edges.
 * 
 * Mine Part, Time Part and ResetButton have static size, and ResetButton is
 * always center aligned and Mine Part/Time Part is always close to the edge,
 * but Info Part has flexible width, so that it's necessary to recalculate
 * the position when GameMode is changed.
 * 
 * PS: Although we say that ResetButton has NO edge, we still regard it as a
 *     edge+inner structure because it also has a 3D outlook(button like),
 *     so as the MapUnit in MapArea Part.
 */

/*     Mine            Time
 * +---+---+---+   +---+---+---+
 * |   |   |   |   |   |   |   |
 * | N | N | N |   | N | N | N |
 * |   |   |   |   |   |   |   |
 * +---+---+---+   +---+---+---+
 * 
 * N: A one-digit number from 0 to 9.
 * 
 * The Mine Part and Time Part are both called Num Part, which means
 * they have same UI structure; and the Num Part is composed of
 * three same one-digit number, which is called InfoNum.
 * 
 * For convenience, we call the combination of InfoNum as INums; and relevantly,
 * we call the INums in Mine Part and Time Part as MNums and TNums, respectively.
 * 
 * NOTE: InfoNum has static size so that INums has static size.
 * NOTE: INums has no edges.
 */

/*                MapArea
 * +-----------------------------------+---
 * | U | U | U |      ... ...      | U |mapunit_size
 * |---+---+---+ ----------------- +---|---
 * | U |  ... ...                      |
 * |---+ ----------------------------- |
 * |  ... ...                          |
 * +-----------------------------------+
 * 
 * U: MapUnit
 * 
 * The MapArea Part is composed of MapUnits, and each MapUnit has a fixed size.
 * The MapArea Part contains MxN MapUnits in M rows and N columns, where M and
 * N are defined by current GameMode, which means that the size of MapArea Part
 * varies while the GameMode varies.
 * 
 * PS: MapUnit also has a 3D outlook(button like) but it has NO edges.
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
 * A Part usually have edges, and can have another Part inside its Content.
 * There are edges around Main Client Area and two main Parts.
 * 
 * NOTE: The child Part may not totally fill up the parent Part's Content.
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
 * 'px' and 'py' means positon in pixel on window.
 * 'x' and 'y' means position in unit on GameMap.
 * px = x * w, py = y * h, ppos = (px, py)
 * 
 * NOTE: 'px' and 'py' indicate the left-top of a MapUnit.
 */


#define MU_SIZE         24
#define AREA_EDGE       6
#define PART_EDGE       2
#define COLOR_DEFBG     RGB(240,240,240)
#define COLOR_DEFDARK   RGB(192,192,192)
#define COLOR_DEFLIGHT  RGB(255,255,255)
#define COLOR_DEFSEMIL  RGB(227,227,227)
#define COLOR_DEFSHADOW RGB(128,128,128)
#define COLOR_DEFSEMIS  RGB(160,160,160)

#define MAPUNITS_WIDTH(nw)  ((nw) * MU_SIZE)
#define MAPUNITS_HEIGHT(nh) ((nh) * MU_SIZE)

//Info Part
#define INFO_WIDTH(nw)  (PART_EDGE + MAPUNITS_WIDTH(nw) + PART_EDGE)
#define INFO_HEIGHT     (PART_EDGE + 36 + PART_EDGE)
#define INFO_LEFT       AREA_EDGE
#define INFO_TOP        AREA_EDGE

#define COLOR_INFO      COLOR_DEFBG
#define COLOR_INFOL     COLOR_DEFLIGHT
#define COLOR_INFOS     COLOR_DEFSHADOW
//end Info Part

//Mine Part, Time Part and ResetButton
#define LEFT_DIST       (PART_EDGE + 6)
#define RIGHT_DIST      LEFT_DIST
#define INFONUM_WIDTH   15
#define INFONUM_HEIGHT  27
#define INUMS_WIDTH     (INFONUM_WIDTH * 3)
#define INUMS_HEIGHT    INFONUM_HEIGHT
#define INUMS_EDGE      1
#define NUM_WIDTH       (INUMS_EDGE + INUMS_WIDTH + INUMS_EDGE)
#define NUM_HEIGHT      (INUMS_EDGE + INUMS_HEIGHT + INUMS_EDGE)

#define COLOR_INFONUMBG RGB(0,0,0)
#define COLOR_INFONUMBT RGB(255,0,0)
#define COLOR_INFONUMDK RGB(128,0,0)
#define COLOR_NUM       COLOR_DEFBG
#define COLOR_NUML      COLOR_DEFLIGHT
#define COLOR_NUMS      COLOR_DEFSHADOW

#define MINE_WIDTH      NUM_WIDTH
#define MINE_HEIGHT     NUM_HEIGHT
#define MINE_LEFT       (INFO_LEFT + LEFT_DIST)
#define MINE_TOP        (INFO_TOP + PART_EDGE + 3)
#define MNUMS_WIDTH     INUMS_WIDTH
#define MNUMS_HEIGHT    INUMS_HEIGHT
#define MNUMS_LEFT      (MINE_LEFT + INUMS_EDGE)
#define MNUMS_TOP       (MINE_TOP + INUMS_EDGE)

#define COLOR_MINE      COLOR_NUM
#define COLOR_MINEL     COLOR_NUML
#define COLOR_MINES     COLOR_NUMS

#define TIME_WIDTH      NUM_WIDTH
#define TIME_HEIGHT     NUM_HEIGHT
#define TIME_LEFT(nw)   (INFO_LEFT + INFO_WIDTH(nw) - RIGHT_DIST - TIME_WIDTH)
#define TIME_TOP        MINE_TOP
#define TNUMS_WIDTH     INUMS_WIDTH
#define TNUMS_HEIGHT    INUMS_HEIGHT
#define TNUMS_LEFT(nw)  (TIME_LEFT(nw) + INUMS_EDGE)
#define TNUMS_TOP       (TIME_TOP + INUMS_EDGE)

#define COLOR_TIME      COLOR_NUM
#define COLOR_TIMEL     COLOR_NUML
#define COLOR_TIMES     COLOR_NUMS

#define RB_SIZE         32
#define RB_LEFT(nw)     (INFO_LEFT + (INFO_WIDTH(nw) - RB_SIZE) / 2)
#define RB_TOP          (INFO_TOP + PART_EDGE + 2)
#define BMP_SIZE        28
#define BMP_LEFT(nw)    (RB_LEFT(nw) + 2)
#define BMP_TOP         (RB_TOP + 2)

#define COLOR_RB        COLOR_DEFBG
#define COLOR_RBL       COLOR_DEFLIGHT
#define COLOR_RBS       COLOR_DEFSHADOW
#define COLOR_RBSL      COLOR_DEFSEMIL
#define COLOR_RBSS      COLOR_DEFSEMIS
//end Mine Part, Time Part and ResetButton

//MapArea
#define MAPAREA_WIDTH(nw)   INFO_WIDTH(nw)
#define MAPAREA_HEIGHT(nh)  (PART_EDGE + MAPUNITS_HEIGHT(nh) + PART_EDGE)
#define MAPAREA_LEFT        AREA_EDGE
#define MAPAREA_TOP         (AREA_EDGE + INFO_HEIGHT + AREA_EDGE)

#define MAP_WIDTH(nw)   MAPUNITS_WIDTH(nw)
#define MAP_HEIGHT(nh)  MAPUNITS_HEIGHT(nh)
#define MAP_LEFT        (MAPAREA_LEFT + PART_EDGE)
#define MAP_TOP         (MAPAREA_TOP + PART_EDGE)

#define COLOR_MAPAREA   COLOR_DEFBG
#define COLOR_MAPAREAL  COLOR_DEFLIGHT
#define COLOR_MAPAREAS  COLOR_DEFSHADOW

#define COLOR_MUCOVER   COLOR_DEFDARK
#define COLOR_MUCOVERL  COLOR_DEFLIGHT
#define COLOR_MUCOVERS  COLOR_DEFSHADOW
#define COLOR_MUUNCOV   COLOR_DEFBG
#define COLOR_MUUNCOVE  COLOR_DEFSHADOW
#define COLOR_MUUNCOVB  RGB(237,28,36)
#define COLOR_MUMINE    RGB(0,0,0)
#define COLOR_MUMINEL   RGB(255,255,255)
#define COLOR_MUFLAGF   RGB(255,0,0)
#define COLOR_MUFLAGB   RGB(0,0,0)
#define COLOR_MUMARK    RGB(0,0,0)
#define COLOR_MUCROSS   RGB(255,0,0)
#define COLOR_MUNUM1    RGB(0,0,255)
#define COLOR_MUNUM2    RGB(0,128,0)
#define COLOR_MUNUM3    RGB(255,0,0)
#define COLOR_MUNUM4    RGB(0,0,128)
#define COLOR_MUNUM5    RGB(128,0,0)
#define COLOR_MUNUM6    RGB(0,128,128)
#define COLOR_MUNUM7    RGB(0,0,0)
#define COLOR_MUNUM8    RGB(128,128,128)
//end MapArea

//Client Area
#define CLIENT_WIDTH(nw)    (AREA_EDGE + INFO_WIDTH(nw) + AREA_EDGE)
#define CLIENT_HEIGHT(nh)   (AREA_EDGE + INFO_HEIGHT + AREA_EDGE + MAPAREA_HEIGHT(nh) + AREA_EDGE)
#define CLIENT_LEFT         0
#define CLIENT_TOP          0

#define COLOR_CLIENT    COLOR_DEFBG
#define COLOR_CLIENTL   COLOR_DEFLIGHT
#define COLOR_CLIENTS   COLOR_DEFSHADOW
//end Client Area



/* This array stores InfoNum backgroud, it is private.
/*
const bool InfoNumBG[INFONUM_WIDTH][INFONUM_HEIGHT];
*/



/* These function implementations are private,
 * they are invisible outside.
 */

/* These functions draw a concave or convex like area with different type of edges.
/**********************************************************************************
//Draw 1 pixel half edge 2D like background,
//draw left edge and top edge only.
static void drawhalfedgebg(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int width,
    _In_ int height,
    _In_ COLORREF inner,
    _In_ COLORREF edge
);
//Draw 1 pixel edge concave background,
//exchange 'light' and 'shadow' to draw a convex background.
static void drawfulledgebg(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int width,
    _In_ int height,
    _In_ COLORREF inner,
    _In_ COLORREF light,
    _In_ COLORREF shadow
);
//Draw 2 pixel edge concave background,
//exchange 'light' and 'shadow' to draw a convex background.
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
//Draw 2 pixel edge concave background with 2 layers of color,
//exchange colors to draw a convex background.
static void drawdualedgebg(
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
*/

/* These functions draw a seven-segment display like area,
 * which is used for InfoNum.
/*****************************************************************************
// a 7sd's look and defination
//    a
//  +---+
// f| g |b
//  +---+
// e|   |c
//  +---+
//    d

//Draw 7sd background.
static inline void draw7sdbg(_In_ HDC hdestdc, _In_ int left, _In_ int top);

//following draw each segment in 7sd
static void draw7sda(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdb(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdc(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdd(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sde(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdf(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdg(_In_ HDC h7sddc, _In_ int left, _In_ int top);
*/

/* These functions draw different content inside a MapUnit.
/***********************************************************************************
//Draw a mine icon.
static void drawmuitemmine(_In_ HDC hdestdc, _In_ int left, _In_ int top);

//Draw a question mark icon.
static void drawmuitemmark(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ bool clicked);

//Draw a flag icon.
static void drawmuitemflag(_In_ HDC hdestdc, _In_ int left, _In_ int top);

//Draw a cross icon.
static void drawmuitemcross(_In_ HDC hdestdc, _In_ int left, _In_ int top);

//Following draw a number inside MapUnit.
static void drawmuitemnum1(_In_ HDC hdestdc, _In_ int left, _In_ int top);
static void drawmuitemnum2(_In_ HDC hdestdc, _In_ int left, _In_ int top);
static void drawmuitemnum3(_In_ HDC hdestdc, _In_ int left, _In_ int top);
static void drawmuitemnum4(_In_ HDC hdestdc, _In_ int left, _In_ int top);
static void drawmuitemnum5(_In_ HDC hdestdc, _In_ int left, _In_ int top);
static void drawmuitemnum6(_In_ HDC hdestdc, _In_ int left, _In_ int top);
static void drawmuitemnum7(_In_ HDC hdestdc, _In_ int left, _In_ int top);
static void drawmuitemnum8(_In_ HDC hdestdc, _In_ int left, _In_ int top);
*/

/* logical xor
static inline bool _xor(const bool A, const bool B);
*/


/* following functions are public */

//Transform between pixel position on MapArea and GameMap xy-type.
//Assume the whole Map's left-top as position 0.
//The pixel position must be offset of MAP_LEFT and MAP_TOP.
int px2x(int px);
int py2y(int py);
int x2px(int x);
int y2py(int y);


/* These functions draw backgroung with edge for each Part
 * with predefined color, check relative macros.
 */

//w:CLIENT_WIDTH, h:CLIENT_HEIGHT
void drawDCClientBg(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int n_map_width,
    _In_ int n_map_height
);

//w:INFO_WIDTH, h:INFO_HEIGHT
void drawDCInfoBg(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int n_map_width
);

//w:MAPAREA_WIDTH, h:MAPAREA_HEIGHT
void drawDCMapAreaBg(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int n_map_width,
    _In_ int n_map_height
);

//w:NUM_WIDTH, h:NUM_HEIGHT
void drawDCNumBg(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top
);


/* these functions draw specfic numbers inside Num Part
 * with predefined color and size, check relative macros.
 */

//Draw all three InfoNum.
//If 'num' is out of range, it draws '---'.
//(-100 < num < 1000)
//w:INUMS_WIDTH, h:INUMS_HEIGHT
void drawDCINums(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int num
);

//Draw a single InfoNum directly on DC.
//If num EQ -1, it draws '-';
//if num is within [0, 9], it draws num;
//otherwise, it draws nothing.
//w:INFONUM_WIDTH, h:INFONUM_HEIGHT
void drawDCInfoNum(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int num
);


/* These functions draw a ResetButton inside Info Part
 * with predefined color and size, check relative macros.
 */

//Draw ResetButton background directly on DC.
//w:RB_SIZE, h:RB_SIZE
void drawDCResetButtonBg(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ bool clicked
);

//Draw a bitmap on ResetButton directly.
//it will do nothing if the hbm is NULL
//w:BMP_SIZE, h:BMP_SIZE
void drawDCResetButtonBmp(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ HBITMAP hbm,
    _In_ bool clicked
);

//Draw a ResetButton with specified bitmap.
//Use NULL if no bitmap to be drawn.
//w:RB_SIZE, h:RB_SIZE
void drawDCResetButton(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ HBITMAP hbm,
    _In_ bool clicked
);


/* These functions draw a MapUnit on MapArea Part
 * with predefined color and size, check relative macros.
 */

//w:MU_SIZE, h:MU_SIZE
void drawDCMUCover(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top
);

//w:MU_SIZE, h:MU_SIZE
void drawDCMUUncov(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top
);

//w:MU_SIZE, h:MU_SIZE
void drawDCMUFlag(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top
);

//w:MU_SIZE, h:MU_SIZE
void drawDCMUMark(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ bool clicked
);

//w:MU_SIZE, h:MU_SIZE
void drawDCMUMine(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ bool bomb
);

//w:MU_SIZE, h:MU_SIZE
void drawDCMUWrong(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top
);

//w:MU_SIZE, h:MU_SIZE
void drawDCMUNum(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int num
);
