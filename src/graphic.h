/*****************************************************************************\
 *  My Minesweepper -- a classic minesweeper game
 *  Copyright (C) 2020 Gee Wang
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
 * graphic.h
 *****************************************************************************
 * This file contains the Game UI design and basic drawing functions.
 * Drawing functions use Win32 GDI and draw directly on Device Context (DC)
 * without creating a DC buffer.
 *
 * NOTE: Almost all functions do NOT check arguments, use with care.
\*****************************************************************************/


#pragma once

#include "stdafx.h"



/*       LEFT           RIGHT
 *         v              v            LEFT      RIGHT
 * TOP---->+--------------+              v         v
 *         | +----------+ |      TOP---->+---------+
 * Edge----->|  Content | |              |         |
 *         | |          | |              | Content |
 *         | +----------+ |              |         |
 * BOTTOM->+--------------+      BOTTOM->+---------+
 *              Region                      Object
 *
 * Region and Object are the basic UI Components.
 * Region may contain Objects and other Regions inside its Content, and usually
 * have edges. Object is atomic and usually do not have edges.
 * Region that is inside another Region is called child Region, relevantly,
 * the outer Region is called parent Region.
 *
 * NOTE: The child Region may not fill up the Content of parent Region entirely.
 */

/*           Client Area
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
 * The Client Area is composed of HeadArea Region and MapArea Region.
 *
 * NOTE: These two Regions have edges of their own.
 * NOTE: There are frames around and between these two Regions, which means
 *       that they do not fill up the Client Area entirely.
 *
 * 'HeadArea_height' is constant.
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
 * The HeadArea Region contains Mine Region, Time Region and ResetButton.
 *
 * constant: top_dist, bottom_dist, info_height, left_dist, right_dist,
 *           info_width, reset_button_size
 * flexible: mid_dist
 *
 * NOTE: ResetButton is not a "BUTTON" Control.
 * NOTE: ResetButton is a square, Mine Region and Time Region are rectangles.
 * NOTE: ResetButton does not use 'top_dist' and 'bottom_dist', they are only
 *       for Mine Region and Time Region.
 * NOTE: ResetButton does not have edges, but Mine Region and Time Region have
 *       edges of their own.
 *
 * Mine Region, Time Region and ResetButton have constant size, and ResetButton
 * is always center aligned while Mine/Time Region is always close to the edge,
 * but HeadArea Region has flexible width, so it is necessary to recalculate
 * the position when GameMode changes.
 *
 * PS: Although we say that ResetButton does not have edges, we still regard it
 *     as an edge+inner structure since it also has a 3D outlook (button like).
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
 * The Mine Region and Time Region are both called Info Region, which means
 * they have the same UI structure. Further more, each Info Region contains
 * three same Objects, which is called InfoNum.
 *
 * For convenience, we call the combination of three InfoNum Objects as INums;
 * and relevantly, we call the INums in Mine Region and Time Region as MNums
 * and TNums, respectively.
 *
 * NOTE: InfoNum has constant size so that INums has constant size.
 * NOTE: Info Region is composed of INums and edges.
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
 * The MapArea Region contains the GameMap, which is called Map for simplicity.
 * The Map is composed of MxN MapUnits in M rows and N columns, where M and N
 * are decided by current GameMode, which means that the size of MapArea Region
 * varies while GameMode varies.
 *
 * NOTE: MapUnit has a fixed size.
 * NOTE: MapUnit also has a 3D outlook (button like) with no edges.
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
 * 'px' and 'py' are positons in pixel on window.
 * 'x' and 'y' are positions in unit on GameMap.
 * px = x * w, py = y * h, ppos = (px, py)
 *
 * NOTE: (px, py) indicates the position of the top-left pixel of a MapUnit.
 */


#define AREA_FRAME      6
#define REGION_FEDGE    1
#define REGION_TEDGE    2
#define UICOL_CLTAREA   RGB(240,240,240)
#define UICOL_CONVEX    RGB(192,192,192)
#define UICOL_WHITE     RGB(255,255,255)
#define UICOL_GRAY      RGB(128,128,128)
#define UICOL_LIGHT     RGB(227,227,227)
#define UICOL_DARK      RGB(160,160,160)


//Mine Region, Time Region
#define INFONUM_MIN     0
#define INFONUM_MAX     9
#define INFONUM_DASH    10
#define INUMS_MIN       (-99)
#define INUMS_MAX       999

#define INUMSF_SHOWALL  0x00
#define INUMSF_HIDEA    0x01
#define INUMSF_HIDEB    0x02
#define INUMSF_HIDEC    0x04
#define INUMSF_HIDEALL  0x07

#define INFONUM_WIDTH   15
#define INFONUM_HEIGHT  27
#define INUMS_WIDTH     (INFONUM_WIDTH * 3)
#define INUMS_HEIGHT    INFONUM_HEIGHT

#define INFO_EDGE       REGION_FEDGE
#define INFO_WIDTH      (INFO_EDGE + INUMS_WIDTH + INFO_EDGE)
#define INFO_HEIGHT     (INFO_EDGE + INUMS_HEIGHT + INFO_EDGE)

#define INCOL_BLANK     RGB(0,0,0)
#define INCOL_SEGON     RGB(255,0,0)
#define INCOL_SEGOFF    RGB(128,0,0)
#define ICOL_SURFACE    UICOL_CLTAREA
#define ICOL_BRIGHT     UICOL_WHITE
#define ICOL_DARK       UICOL_GRAY

#define MNUMS_WIDTH     INUMS_WIDTH
#define MNUMS_HEIGHT    INUMS_HEIGHT
#define MINE_WIDTH      INFO_WIDTH
#define MINE_HEIGHT     INFO_HEIGHT

#define TNUMS_WIDTH     INUMS_WIDTH
#define TNUMS_HEIGHT    INUMS_HEIGHT
#define TIME_WIDTH      INFO_WIDTH
#define TIME_HEIGHT     INFO_HEIGHT
//end Mine Region, Time Region

//ResetButton
#define RB_SIZE         32
#define BMP_SIZE        28

#define RBCOL_SURFACE   UICOL_CLTAREA
#define RBCOL_BRIGHT1   UICOL_WHITE
#define RBCOL_DARK1     UICOL_GRAY
#define RBCOL_BRIGHT2   UICOL_LIGHT
#define RBCOL_DARK2     UICOL_DARK
//end ResetButton

//Map
#define MUP_SIZE        24
#define MUP_NSIZE(n)    ((n) * MUP_SIZE)

#define MAP_WIDTH(nw)   MUP_NSIZE(nw)
#define MAP_HEIGHT(nh)  MUP_NSIZE(nh)

#define MUCOL_COVERSF   UICOL_CONVEX
#define MUCOL_COVERBT   UICOL_WHITE
#define MUCOL_COVERDK   UICOL_GRAY
#define MUCOL_BARESF    UICOL_CLTAREA
#define MUCOL_BAREEG    UICOL_GRAY
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
//end Map

//HeadArea Region
#define HEADAREA_EDGE       REGION_TEDGE
#define HEADAREA_WIDTH(nw)  (HEADAREA_EDGE + MAP_WIDTH(nw) + HEADAREA_EDGE)
#define HEADAREA_HEIGHT     (HEADAREA_EDGE + 36 + HEADAREA_EDGE)

#define HACOL_SURFACE       UICOL_CLTAREA
#define HACOL_BRIGHT        UICOL_WHITE
#define HACOL_DARK          UICOL_GRAY
//end HeadArea Region

//MapArea Region
#define MAPAREA_EDGE        REGION_TEDGE
#define MAPAREA_WIDTH(nw)   (MAPAREA_EDGE + MAP_WIDTH(nw) + MAPAREA_EDGE)
#define MAPAREA_HEIGHT(nh)  (MAPAREA_EDGE + MAP_HEIGHT(nh) + MAPAREA_EDGE)

#define MACOL_SURFACE       UICOL_CLTAREA
#define MACOL_BRIGHT        UICOL_WHITE
#define MACOL_DARK          UICOL_GRAY
//end MapArea Region

//Client Area
#define CLIENT_WIDTH(nw)    (AREA_FRAME + HEADAREA_WIDTH(nw) + AREA_FRAME)
#define CLIENT_HEIGHT(nh)   (AREA_FRAME + HEADAREA_HEIGHT + AREA_FRAME + MAPAREA_HEIGHT(nh) + AREA_FRAME)

#define CLTCOL_SURFACE      UICOL_CLTAREA
//end Client Area


//Component Positions
#define CLIENT_LEFT     0
#define CLIENT_TOP      0

#define HEADAREA_LEFT   (CLIENT_LEFT + AREA_FRAME)
#define HEADAREA_TOP    (CLIENT_TOP + AREA_FRAME)

#define MAPAREA_LEFT    (CLIENT_LEFT + AREA_FRAME)
#define MAPAREA_TOP     (CLIENT_TOP + AREA_FRAME + HEADAREA_HEIGHT + AREA_FRAME)

#define INFO_LDIST      (HEADAREA_EDGE + 6)
#define INFO_RDIST      INFO_LDIST
#define INFO_TDIST      (HEADAREA_EDGE + 3)

#define MINE_LEFT       (HEADAREA_LEFT + INFO_LDIST)
#define MINE_TOP        (HEADAREA_TOP + INFO_TDIST)
#define MNUMS_LEFT      (MINE_LEFT + INFO_EDGE)
#define MNUMS_TOP       (MINE_TOP + INFO_EDGE)

#define TIME_LEFT(nw)   (HEADAREA_LEFT + HEADAREA_WIDTH(nw) - INFO_RDIST - TIME_WIDTH)
#define TIME_TOP        (HEADAREA_TOP + INFO_TDIST)
#define TNUMS_LEFT(nw)  (TIME_LEFT(nw) + INFO_EDGE)
#define TNUMS_TOP       (TIME_TOP + INFO_EDGE)

#define RB_LEFT(nw)     (HEADAREA_LEFT + (HEADAREA_WIDTH(nw) - RB_SIZE) / 2)
#define RB_TOP          (HEADAREA_TOP + HEADAREA_EDGE + 2)
#define RB_BMPOFFSET    ((RB_SIZE - BMP_SIZE) / 2)

#define MAP_LEFT        (MAPAREA_LEFT + MAPAREA_EDGE)
#define MAP_TOP         (MAPAREA_TOP + MAPAREA_EDGE)
//end Component Positions



/* position related functions */

//Transform between pixel position and unit position of MapUnit.
//Assume the top-left of the whole Map is position (0, 0).
//The pixel position must be offset of MAP_LEFT and MAP_TOP.
int px2x(int px);
int py2y(int py);
int x2px(int x);
int y2py(int y);


/* These functions draw surface of each Region using
 * predefined color and size, check related macros.
 */

//w:CLIENT_WIDTH, h:CLIENT_HEIGHT
void drawClient(
    _In_ HDC hdc,
    _In_ int left,
    _In_ int top,
    _In_ int n_mapw,
    _In_ int n_maph
);

//w:HEADAREA_WIDTH, h:HEADAREA_HEIGHT
void drawHeadArea(
    _In_ HDC hdc,
    _In_ int left,
    _In_ int top,
    _In_ int n_mapw
);

//w:MAPAREA_WIDTH, h:MAPAREA_HEIGHT
void drawMapArea(
    _In_ HDC hdc,
    _In_ int left,
    _In_ int top,
    _In_ int n_mapw,
    _In_ int n_maph
);

//w:INFO_WIDTH, h:INFO_HEIGHT
void drawInfo(
    _In_ HDC hdc,
    _In_ int left,
    _In_ int top
);


/* These functions draw contents of each Object using
 * predefined color and size, check related macros.
 */

//Draw a single InfoNum by number.
//Accept numbers within [INFONUM_MIN, INFONUM_MAX] and INFONUM_DASH
//as symbol '-', otherwise, draws a blank 7-segment display.
//w:INFONUM_WIDTH, h:INFONUM_HEIGHT
void drawInfoNum(
    _In_ HDC hdc,
    _In_ int left,
    _In_ int top,
    _In_ int num
);

//Draw INums by number.
//Available range:[INUMS_MIN, INUMS_MAX], otherwise, draws '---'.
//The drawing behavior is controlled by 'flag',
//which is a combination of INUMSF* flags that controls whether
//a InfoNum turns off.
//w:INUMS_WIDTH, h:INUMS_HEIGHT
void drawINums(
    _In_ HDC hdc,
    _In_ int left,
    _In_ int top,
    _In_ int num,
    _In_ BYTE flag
);

//Draw a ResetButton with specified bitmap.
//Use NULL if no bitmap to be drawn.
//w:RB_SIZE, h:RB_SIZE
void drawResetButton(
    _In_ HDC hdc,
    _In_ int left,
    _In_ int top,
    _In_ HBITMAP hbmp,
    _In_ bool clicked
);

//Draw a covered MapUnit.
//w:MUP_SIZE, h:MUP_SIZE
void drawMapUnitCover(
    _In_ HDC hdc,
    _In_ int left,
    _In_ int top
);

//Draw a flagged MapUnit.
//w:MUP_SIZE, h:MUP_SIZE
void drawMapUnitFlag(
    _In_ HDC hdc,
    _In_ int left,
    _In_ int top
);

//Draw an uncovered MapUnit.
//w:MUP_SIZE, h:MUP_SIZE
void drawMapUnitBare(
    _In_ HDC hdc,
    _In_ int left,
    _In_ int top
);

//Draw a MapUnit with mine.
//w:MUP_SIZE, h:MUP_SIZE
void drawMapUnitMine(
    _In_ HDC hdc,
    _In_ int left,
    _In_ int top,
    _In_ bool bomb
);

//Draw a marked MapUnit.
//w:MUP_SIZE, h:MUP_SIZE
void drawMapUnitMark(
    _In_ HDC hdc,
    _In_ int left,
    _In_ int top,
    _In_ bool clicked
);

//Draw a MapUnit with flag and cross.
//w:MUP_SIZE, h:MUP_SIZE
void drawMapUnitFalse(
    _In_ HDC hdc,
    _In_ int left,
    _In_ int top
);

//Draw a MapUnit with number.
//w:MUP_SIZE, h:MUP_SIZE
void drawMapUnitNumber(
    _In_ HDC hdc,
    _In_ int left,
    _In_ int top,
    _In_ int num
);
