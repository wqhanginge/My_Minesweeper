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
 * |           HeadArea          |HeadArea_height
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
 * The Main Client Area is composed of HeadArea SubArea and MapArea SubArea.
 * 
 * NOTE: These two SubAreas have edges of their own.
 * NOTE: There are frames around Main Client Area and between these two SubAreas.
 * 
 * 'HeadArea_height' is static.
 * 'MapArea_height' and 'width' are flexible.
 */

/*                  HeadArea
 * +----------------------------------------+---
 * |  +------+      +------+ --   +------+  |--- top_dist
 * |  | Mine |      |Reset | |    | Time |  |info_height
 * |  +------+      |      | |D   +------+  |---
 * |                +------+ --             |bottom_dist
 * +----------------------------------------+---
 * |--|------|------|------|------|------|--|
 *  A    B      C      D      E      F    G
 * A:left_dist  B:info_width    C:mid_dist  D:reset_button_size
 * E:mid_dist   F:info_width    G:right_dist
 * 
 * The HeadArea SubArea contains Mine SubArea, Time SubArea and ResetButton.
 * 
 * static: top_dist, bottom_dist, info_height, left_dist, right_dist,
 *         info_width, reset_button_size
 * flexible: mid_dist
 * 
 * NOTE: ResetButton is NOT a "BUTTON" Window.
 * NOTE: ResetButton is a Square, Mine SubArea and Time SubArea are Rectangles.
 * NOTE: 'reset_button_size' is different from 'info_height' and 'info_width',
 *       i.e. 'bottom_dist' can be varied, but 'top_dist' is constant.
 * NOTE: Mine SubArea and Time SubArea have edges of their own,
 *       but ResetButton does not have edges.
 * 
 * Mine SubArea, Time SubArea and ResetButton have static size, and ResetButton is
 * always center aligned while Mine/Time SubArea is always close to the edge,
 * but HeadArea SubArea has flexible width, so that it's necessary to recalculate
 * the position when GameMode is changed.
 * 
 * PS: Although we say that ResetButton has NO edge, we still regard it as a
 *     edge+inner structure because it also has a 3D outlook(button like),
 *     so as the MapUnit in MapArea SubArea.
 */

/*     Mine            Time
 * +---+---+---+   +---+---+---+
 * |   |   |   |   |   |   |   |
 * | N | N | N |   | N | N | N |
 * |   |   |   |   |   |   |   |
 * +---+---+---+   +---+---+---+
 *   a   b   c       a   b   c
 * 
 * N: A one-digit number from 0 to 9.
 * 
 * The Mine SubArea and Time SubArea are both called Info SubArea, which means
 * they have the same UI structure, further more, each Info SubArea contains
 * three digits, which is called InfoNum.
 * 
 * For convenience, we call the combination of three InfoNums as INums; and relevantly,
 * we call the INums in Mine SubArea and Time SubArea as MNums and TNums, respectively.
 * 
 * NOTE: InfoNum has static size so that INums has static size.
 * NOTE: Info SubArea is composed of INums and edges.
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
 * The MapArea SubArea contains the GameMap, which is called Map for simplicity.
 * The Map is composed of MxN MapUnits in M rows and N columns, where M and N are
 * decided by current GameMode, which means that the size of MapArea SubArea
 * varies while the GameMode varies.
 * 
 * NOTE: MapUnit has a fixed size.
 * NOTE: MapUnit also has a 3D outlook(button like) but it has NO edges.
 */

/*       LEFT           RIGHT
 *         v              v
 * TOP---->+--------------+
 *         | +----------+ |
 * Edge----->|  Content | |
 *         | |          | |
 *         | +----------+ |
 * BOTTOM->+--------------+
 *             SubArea
 * 
 * A SubArea usually have edges, and can have another SubArea inside its Content.
 * A SubArea that is inside another SubArea is called child SubArea, relevantly,
 * the outer SubArea is called parent SubArea.
 * 
 * NOTE: The child SubArea may not totally fill up the parent SubArea's Content.
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


#define MUP_SIZE        24
#define MUP_NSIZE(n)    ((n) * MUP_SIZE)
#define FRAME_WIDTH     6
#define SUBAREA_EDGE    2
#define UICOL_CLTAREA   RGB(240,240,240)
#define UICOL_CONVEX    RGB(192,192,192)
#define UICOL_WHITE     RGB(255,255,255)
#define UICOL_GRAY      RGB(128,128,128)
#define UICOL_LIGHT     RGB(227,227,227)
#define UICOL_DARK      RGB(160,160,160)

//HeadArea SubArea
#define HEADAREA_WIDTH(nw)  (SUBAREA_EDGE + MUP_NSIZE(nw) + SUBAREA_EDGE)
#define HEADAREA_HEIGHT     (SUBAREA_EDGE + 36 + SUBAREA_EDGE)
#define HEADAREA_LEFT       FRAME_WIDTH
#define HEADAREA_TOP        FRAME_WIDTH

#define HACOL_SURFACE   UICOL_CLTAREA
#define HACOL_BRIGHT    UICOL_WHITE
#define HACOL_DARK      UICOL_GRAY
//end HeadArea SubArea

//Mine SubArea, Time SubArea and ResetButton
#define INFONUM_MIN     0
#define INFONUM_MAX     9
#define INUMS_MIN       (-99)
#define INUMS_MAX       999

#define INUMSF_SHOWALL  0x00
#define INUMSF_HIDEA    0x01
#define INUMSF_HIDEB    0x02
#define INUMSF_HIDEC    0x04
#define INUMSF_HIDEALL  0x07
#define INUMSF_SKIPA    0x08
#define INUMSF_SKIPB    0x10
#define INUMSF_SKIPC    0x20
#define INUMSF_SKIPALL  0x38

#define INFONUM_WIDTH   15
#define INFONUM_HEIGHT  27
#define INUMS_WIDTH     (INFONUM_WIDTH * 3)
#define INUMS_HEIGHT    INFONUM_HEIGHT

#define INFO_LDIST      (SUBAREA_EDGE + 6)
#define INFO_RDIST      INFO_LDIST
#define INFO_EDGE       1
#define INFO_WIDTH      (INFO_EDGE + INUMS_WIDTH + INFO_EDGE)
#define INFO_HEIGHT     (INFO_EDGE + INUMS_HEIGHT + INFO_EDGE)

#define INCOL_BLANK     RGB(0,0,0)
#define INCOL_SEGON     RGB(255,0,0)
#define INCOL_SEGOFF    RGB(128,0,0)
#define ICOL_SURFACE    UICOL_CLTAREA
#define ICOL_BRIGHT     UICOL_WHITE
#define ICOL_DARK       UICOL_GRAY

#define MINE_WIDTH      INFO_WIDTH
#define MINE_HEIGHT     INFO_HEIGHT
#define MINE_LEFT       (HEADAREA_LEFT + INFO_LDIST)
#define MINE_TOP        (HEADAREA_TOP + SUBAREA_EDGE + 3)
#define MNUMS_WIDTH     INUMS_WIDTH
#define MNUMS_HEIGHT    INUMS_HEIGHT
#define MNUMS_LEFT      (MINE_LEFT + INFO_EDGE)
#define MNUMS_TOP       (MINE_TOP + INFO_EDGE)

#define MCOL_SURFACE    ICOL_SURFACE
#define MCOL_BRIGHT     ICOL_BRIGHT
#define MCOL_DARK       ICOL_DARK

#define TIME_WIDTH      INFO_WIDTH
#define TIME_HEIGHT     INFO_HEIGHT
#define TIME_LEFT(nw)   (HEADAREA_LEFT + HEADAREA_WIDTH(nw) - INFO_RDIST - TIME_WIDTH)
#define TIME_TOP        MINE_TOP
#define TNUMS_WIDTH     INUMS_WIDTH
#define TNUMS_HEIGHT    INUMS_HEIGHT
#define TNUMS_LEFT(nw)  (TIME_LEFT(nw) + INFO_EDGE)
#define TNUMS_TOP       (TIME_TOP + INFO_EDGE)

#define TCOL_SURFACE    ICOL_SURFACE
#define TCOL_BRIGHT     ICOL_BRIGHT
#define TCOL_DARK       ICOL_DARK

#define RB_SIZE         32
#define RB_LEFT(nw)     (HEADAREA_LEFT + (HEADAREA_WIDTH(nw) - RB_SIZE) / 2)
#define RB_TOP          (HEADAREA_TOP + SUBAREA_EDGE + 2)
#define BMP_SIZE        28
#define BMP_LEFT(nw)    (RB_LEFT(nw) + 2)
#define BMP_TOP         (RB_TOP + 2)

#define RBCOL_SURFACE   UICOL_CLTAREA
#define RBCOL_BRIGHT1   UICOL_WHITE
#define RBCOL_DARK1     UICOL_GRAY
#define RBCOL_BRIGHT2   UICOL_LIGHT
#define RBCOL_DARK2     UICOL_DARK
//end Mine SubArea, Time SubArea and ResetButton

//MapArea SubArea
#define MAPAREA_WIDTH(nw)   HEADAREA_WIDTH(nw)
#define MAPAREA_HEIGHT(nh)  (SUBAREA_EDGE + MUP_NSIZE(nh) + SUBAREA_EDGE)
#define MAPAREA_LEFT        FRAME_WIDTH
#define MAPAREA_TOP         (FRAME_WIDTH + HEADAREA_HEIGHT + FRAME_WIDTH)

#define MAP_WIDTH(nw)   MUP_NSIZE(nw)
#define MAP_HEIGHT(nh)  MUP_NSIZE(nh)
#define MAP_LEFT        (MAPAREA_LEFT + SUBAREA_EDGE)
#define MAP_TOP         (MAPAREA_TOP + SUBAREA_EDGE)

#define MACOL_SURFACE   UICOL_CLTAREA
#define MACOL_BRIGHT    UICOL_WHITE
#define MACOL_DARK      UICOL_GRAY

#define MUCOL_COVERSF   UICOL_CONVEX
#define MUCOL_COVERBT   UICOL_WHITE
#define MUCOL_COVERDK   UICOL_GRAY
#define MUCOL_UNCOVSF   UICOL_CLTAREA
#define MUCOL_UNCOVEG   UICOL_GRAY
#define MUCOL_BOMBBG    RGB(237,28,36)
#define MUCOL_MINESF    RGB(0,0,0)
#define MUCOL_MINEHL    RGB(255,255,255)
#define MUCOL_FLAGFLAG  RGB(255,0,0)
#define MUCOL_FLAGBASE  RGB(0,0,0)
#define MUCOL_MARK      RGB(0,0,0)
#define MUCOL_CROSS     RGB(255,0,0)
#define MUCOL_NUMBER1   RGB(0,0,255)
#define MUCOL_NUMBER2   RGB(0,128,0)
#define MUCOL_NUMBER3   RGB(255,0,0)
#define MUCOL_NUMBER4   RGB(0,0,128)
#define MUCOL_NUMBER5   RGB(128,0,0)
#define MUCOL_NUMBER6   RGB(0,128,128)
#define MUCOL_NUMBER7   RGB(0,0,0)
#define MUCOL_NUMBER8   RGB(128,128,128)
//end MapArea SubArea

//Client Area
#define CLIENT_WIDTH(nw)    (FRAME_WIDTH + HEADAREA_WIDTH(nw) + FRAME_WIDTH)
#define CLIENT_HEIGHT(nh)   (FRAME_WIDTH + HEADAREA_HEIGHT + FRAME_WIDTH + MAPAREA_HEIGHT(nh) + FRAME_WIDTH)
#define CLIENT_LEFT         0
#define CLIENT_TOP          0

#define CLTCOL_SURFACE  UICOL_CLTAREA
//end Client Area



/* This array stores InfoNum backgroud, it is private.
/*
const bool InfoNumBG[INFONUM_WIDTH][INFONUM_HEIGHT];
*/



/* These function implementations are private,
 * they are invisible outside.
 */

/* These functions draw a concave or convex like region with different type of edges.
/**********************************************************************************
//Draw a solid color background.
static void drawsolidcolorbg(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int width,
    _In_ int height,
    _In_ COLORREF surface
);
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
//exchange 'bright' and 'dark' to draw a convex background.
static void drawfulledgebg(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int width,
    _In_ int height,
    _In_ COLORREF inner,
    _In_ COLORREF bright,
    _In_ COLORREF dark
);
//Draw 2 pixel edge concave background,
//exchange 'bright' and 'dark' to draw a convex background.
static void drawthickedgebg(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int width,
    _In_ int height,
    _In_ COLORREF inner,
    _In_ COLORREF bright,
    _In_ COLORREF dark
);
//Draw 2 pixel edge concave background with 2 layers of color,
//exchange 'bright*' and 'dark*' to draw a convex background.
static void drawdualedgebg(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int width,
    _In_ int height,
    _In_ COLORREF inner,
    _In_ COLORREF bright1,
    _In_ COLORREF bright2,
    _In_ COLORREF dark1,
    _In_ COLORREF dark2
);
*/

/* These functions draw a seven-segment display like region,
 * which is used for InfoNum.
/*****************************************************************************
// a 7sd's appearance and defination
//    a
//  +---+
// f| g |b
//  +---+
// e|   |c
//  +---+
//    d

//Draw 7sd background.
static inline void draw7sdbg(_In_ HDC hdestdc, _In_ int left, _In_ int top);

//followings draw each segment in 7sd
static void draw7sda(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdb(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdc(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdd(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sde(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdf(_In_ HDC h7sddc, _In_ int left, _In_ int top);
static void draw7sdg(_In_ HDC h7sddc, _In_ int left, _In_ int top);
*/

/* These functions draw different contents inside a MapUnit.
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

//Transform between pixel position on window and GameMap xy-type.
//Assume the whole Map's left-top as position 0.
//The pixel position must be offset of MAP_LEFT and MAP_TOP.
int px2x(int px);
int py2y(int py);
int x2px(int x);
int y2py(int y);


/* These functions draw backgroung with edge for each SubArea
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

//w:HEADAREA_WIDTH, h:HEADAREA_HEIGHT
void drawDCHeadAreaBg(
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

//w:INFO_WIDTH, h:INFO_HEIGHT
void drawDCInfoBg(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top
);


/* These functions draw specfic numbers inside Info SubArea
 * with predefined color and size, check relative macros.
 */

//Draw the InfoNum background directly on DC,
//which means that all segments are off.
//w:INFONUM_WIDTH, h:INFONUM_HEIGHT
void drawDCInfoNumBg(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top
);

//Draw a number on InfoNum background directly.
//If 'num' is within [INFONUM_MIN, INFONUM_MAX], it draws the number;
//otherwise, it draws a symbol '-'.
//w:INFONUM_WIDTH, h:INFONUM_HEIGHT
void drawDCInfoNumNumber(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int num
);

//Draw a single InfoNum directly on DC.
//If 'num' is within [INFONUM_MIN, INFONUM_MAX], it draws the number;
//otherwise, it draws a symbol '-'.
//All segments will be off when 'hide' is true.
//w:INFONUM_WIDTH, h:INFONUM_HEIGHT
void drawDCInfoNum(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int num,
    _In_ bool hide
);

//Draw INums directly on DC.
//Available range:[INUMS_MIN, INUMS_MAX],
//otherwise, it draws symbol '---'.
//The drawing behavior is controlled by 'flag',
//which can be a combination of INUMSF* flags.
//INUMSF_HIDE* controls whether a InfoNum turns off all segments,
//INUMSF_SKIP* skips drawing a specific InfoNum in INums.
//w:INUMS_WIDTH, h:INUMS_HEIGHT
void drawDCINums(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int num,
    _In_ BYTE flag
);


/* These functions draw a ResetButton inside HeadArea SubArea
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
//It will do nothing if the 'hbm' is NULL
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


/* These functions draw a MapUnit on MapArea SubArea
 * with predefined color and size, check relative macros.
 */

//w:MUP_SIZE, h:MUP_SIZE
void drawDCMUCover(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top
);

//w:MUP_SIZE, h:MUP_SIZE
void drawDCMUUncov(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ bool bomb
);

//w:MUP_SIZE, h:MUP_SIZE
void drawDCMUFlag(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top
);

//w:MUP_SIZE, h:MUP_SIZE
void drawDCMUMark(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ bool clicked
);

//w:MUP_SIZE, h:MUP_SIZE
void drawDCMUMine(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ bool bomb
);

//w:MUP_SIZE, h:MUP_SIZE
void drawDCMUWrong(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top
);

//w:MUP_SIZE, h:MUP_SIZE
void drawDCMUNum(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int num
);
