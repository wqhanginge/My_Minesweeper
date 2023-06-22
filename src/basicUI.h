#pragma once

/* 
 * this file contains the Game UI design and basic drawing functions
 * drawing functions use Win32 API
 * NOTE:almost all functions have no arg check process, use with care
 */

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
 * NOTE: these two Parts have edges
 *
 * Info_height is static
 * MapArea_height and width are flexible
 */

/*                  Info
 * +----------------------------------------+---
 * |  +------+      +------+ --   +------+  |--- top_dist
 * |  | Mine |      |Reset | |    | Time |  |nums_height
 * |  +------+      |      | |D   +------+  |---
 * |                +------+ --             |bottom_dist
 * +----------------------------------------+---
 * |--|------|------|------|------|------|--|
 *  A    B      C      D      E      F    G
 * A:left_dist	B:nums_width	C:mid_dist	D:reset_button_size
 * E:mid_dist	F:nums_width	G:right_dist
 *
 * static: top_dist, bottom_dist, nums_height, left_dist, right_dist,
 *         nums_width, reset_button_size
 * flexible: mid_dist
 * NOTE: the reset_button_size is different from nums_height or nums_width,
 *       i.e. the bottom_dist may has different value, but the top_dist has
 *       only one value.
 *
 * Mine Part, Time Part and Reset Button have static size,
 * but Info Part has flexible width, so that it's necessary to recalculate
 * the position when Game Mode changed.
 */

/*     Mine            Time
 * +---+---+---+   +---+---+---+
 * |   |   |   |   |   |   |   |
 * | N | N | N |   | N | N | N |
 * |   |   |   |   |   |   |   |
 * +---+---+---+   +---+---+---+
 * 
 * Mine Part and Time Part are composed by three-digit number
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
 * a Part have edges, and its Content may has another Part
 * there are edges between Main Client Area and two main Parts
 * 
 * NOTE: the child Part may not fully fill the parent Part's Content
 */

#define MU_SIZE			24
#define AREA_EDGE		6
#define PART_EDGE		2
#define COLOR_DEFBG		RGB(240,240,240)
#define COLOR_DEFDARK	RGB(192,192,192)
#define COLOR_DEFLIGHT	RGB(255,255,255)
#define COLOR_DEFLL		RGB(227,227,227)
#define COLOR_DEFSHADOW	RGB(128,128,128)
#define COLOR_DEFSH		RGB(160,160,160)

#define MAPUNITS_WIDTH	(Game.width * MU_SIZE)
#define MAPUNITS_HEIGHT	(Game.height * MU_SIZE)

//Info Part
#define INFO_WIDTH	(PART_EDGE + MAPUNITS_WIDTH + PART_EDGE)
#define INFO_HEIGHT	(PART_EDGE + 36 + PART_EDGE)
#define INFO_LEFT	AREA_EDGE
#define INFO_TOP	AREA_EDGE

#define COLOR_INFO	COLOR_DEFBG
#define COLOR_INFOL	COLOR_DEFLIGHT
#define COLOR_INFOS	COLOR_DEFSHADOW
//end Info Part

//Mine Part, Time Part and Reset Button
#define LEFT_DIST		(PART_EDGE + 6)
#define RIGHT_DIST		LEFT_DIST
#define INFONUM_WIDTH	15
#define INFONUM_HEIGHT	27
#define IN_WIDTH		(INFONUM_WIDTH * 3)
#define IN_HEIGHT		INFONUM_HEIGHT
#define IN_EDGE			1

#define COLOR_INBG	RGB(0,0,0)
#define COLOR_INBT	RGB(255,0,0)
#define COLOR_INDK	RGB(128,0,0)

#define MINE_WIDTH	(IN_EDGE + IN_WIDTH + IN_EDGE)
#define MINE_HEIGHT	(IN_EDGE + IN_HEIGHT + IN_EDGE)
#define MINE_LEFT	(INFO_LEFT + LEFT_DIST)
#define MINE_TOP	(INFO_TOP + PART_EDGE + 3)
#define MN_WIDTH	IN_WIDTH
#define MN_HEIGHT	IN_HEIGHT
#define MN_LEFT		(MINE_LEFT + IN_EDGE)
#define MN_TOP		(MINE_TOP + IN_EDGE)

#define COLOR_MINE	COLOR_DEFBG
#define COLOR_MINEL	COLOR_DEFLIGHT
#define COLOR_MINES	COLOR_DEFSHADOW

#define TIME_WIDTH	MINE_WIDTH
#define TIME_HEIGHT	MINE_HEIGHT
#define TIME_LEFT	(INFO_LEFT + INFO_WIDTH - RIGHT_DIST - TIME_WIDTH)
#define TIME_TOP	MINE_TOP
#define TN_WIDTH	IN_WIDTH
#define TN_HEIGHT	IN_HEIGHT
#define TN_LEFT		(TIME_LEFT + IN_EDGE)
#define TN_TOP		(TIME_TOP + IN_EDGE)

#define COLOR_TIME	COLOR_MINE
#define COLOR_TIMEL	COLOR_MINEL
#define COLOR_TIMES	COLOR_MINES

#define RB_SIZE		32
#define RB_LEFT		(INFO_LEFT + (INFO_WIDTH - RB_SIZE) / 2)
#define RB_TOP		(INFO_TOP + PART_EDGE + 2)
#define BMP_SIZE	28
#define BMP_LEFT	(RB_LEFT + 2)
#define BMP_TOP		(RB_TOP + 2)

#define COLOR_RB	COLOR_DEFBG
#define COLOR_RBL	COLOR_DEFLIGHT
#define COLOR_RBS	COLOR_DEFSHADOW
#define COLOR_RBLL	COLOR_DEFLL
#define COLOR_RBSH	COLOR_DEFSH
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
#define COLOR_MUNUMDEF	(-1)
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



/* this array stores infonum backgroud, it is private */
/*
const bool InfoNumBG[INFONUM_WIDTH][INFONUM_HEIGHT];
*/



/* following functions draw specific bitmaps at appointed position */

/* these function implementations are private
 * they are invisible outside
 */
/*
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
static void drawthickedgebg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int width,
	_In_ int height,
	_In_ COLORREF inner,
	_In_ COLORREF light,
	_In_ COLORREF lightlow,
	_In_ COLORREF shadow,
	_In_ COLORREF shadowhigh
)
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
//use for 7sd
//no DC-Buffer
static inline void draw7sdbg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF bg = COLOR_INBG,
	_In_ COLORREF dark = COLOR_INDK
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

//w:CLIENT_WIDTH, h:CLIENT_HEIGHT
//no DC-Buffer
void drawClientBg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF def = COLOR_CLIENT
);

//w:INFO_WIDTH, h:INFO_HEIGHT
//no DC-Buffer
void drawInfoBg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF inner = COLOR_INFO,
	_In_ COLORREF light = COLOR_INFOL,
	_In_ COLORREF shadow = COLOR_INFOS
);

//w:MAPAREA_WIDTH, h:MAPAREA_HEIGHT
//no DC-Buffer
void drawMapAreaBg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF inner = COLOR_MAPAREA,
	_In_ COLORREF light = COLOR_MAPAREAL,
	_In_ COLORREF shadow = COLOR_MAPAREAS
);

//w:MINE_WIDTH, h:MINE_HEIGHT
//no DC-Buffer
void drawMineBg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF inner = COLOR_MINE,
	_In_ COLORREF light = COLOR_MINEL,
	_In_ COLORREF shadow = COLOR_MINES
);

//w:TIME_WIDTH, h:TIME_HEIGHT
//no DC-Buffer
void drawTimeBg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF inner = COLOR_TIME,
	_In_ COLORREF light = COLOR_TIMEL,
	_In_ COLORREF shadow = COLOR_TIMES
);


//if num is out of range, it draws '---'
//-100 < num < 1000
//w:IN_WIDTH, h:IN_HEIGHT
void drawIN(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int num,
	_In_ COLORREF bg = COLOR_INBG,
	_In_ COLORREF dark = COLOR_INDK,
	_In_ COLORREF bright = COLOR_INBT
);
//if num is out of range, it draws '---'
//-100 < num < 1000
//this function will draw on DC directly without creating a DC-buffer
//w:IN_WIDTH, h:IN_HEIGHT
void drawINNB(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int num,
	_In_ COLORREF bg = COLOR_INBG,
	_In_ COLORREF dark = COLOR_INDK,
	_In_ COLORREF bright = COLOR_INBT
);

//if num EQ -1, it draws '-', if num GE 10, it draws nothing
//w:INFONUM_WIDTH, h:INFONUM_HEIGHT
//no DC-Buffer
void drawInfoNum(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int num,
	_In_ COLORREF bg = COLOR_INBG,
	_In_ COLORREF dark = COLOR_INDK,
	_In_ COLORREF bright = COLOR_INBT
);


//w:RB_SIZE, h:RB_SIZE
//no DC-Buffer
void drawResetButtonBg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF inner = COLOR_RB,
	_In_ COLORREF light = COLOR_RBL,
	_In_ COLORREF lightlow = COLOR_RBLL,
	_In_ COLORREF shadow = COLOR_RBS,
	_In_ COLORREF shadowhigh = COLOR_RBSH
);

//draw bitmap on Reset Button
//it will do nothing if the hbm is NULL
//w:BMP_SIZE, h:BMP_SIZE
//no DC-Buffer
void drawBmpOnResetButton(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ HBITMAP hbm,
	_In_ bool clicked
);


//draw a Reset Button
//use NULL for hbm if no bitmap
//w:RB_SIZE, h:RB_SIZE
void drawResetButton(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ HBITMAP hbm,
	_In_ bool clicked,
	_In_ COLORREF inner = COLOR_RB,
	_In_ COLORREF light = COLOR_RBL,
	_In_ COLORREF lightlow = COLOR_RBLL,
	_In_ COLORREF shadow = COLOR_RBS,
	_In_ COLORREF shadowhigh = COLOR_RBSH
);
//draw a Reset Button
//use NULL for hbm if no bitmap
//w:RB_SIZE, h:RB_SIZE
//this function will draw on DC directly without creating a DC-buffer
void drawResetButtonNB(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ HBITMAP hbm,
	_In_ bool clicked,
	_In_ COLORREF inner = COLOR_RB,
	_In_ COLORREF light = COLOR_RBL,
	_In_ COLORREF lightlow = COLOR_RBLL,
	_In_ COLORREF shadow = COLOR_RBS,
	_In_ COLORREF shadowhigh = COLOR_RBSH
);


/* these functions are private, they are invisible outside */
/*
//no DC-Buffer
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
	_In_ bool clicked,
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
);
*/


//w:MU_SIZE, h:MU_SIZE
void drawMUCoverBg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF inner = COLOR_MUCOVER,
	_In_ COLORREF light = COLOR_MUCOVERL,
	_In_ COLORREF shadow = COLOR_MUCOVERS
);

//w:MU_SIZE, h:MU_SIZE
void drawMUUncovBg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF inner = COLOR_MUUNCOV,
	_In_ COLORREF edge = COLOR_MUUNCOVE
);

//w:MU_SIZE, h:MU_SIZE
void drawMUFlag(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF flag = COLOR_MUFLAGF,
	_In_ COLORREF base = COLOR_MUFLAGB,
	_In_ COLORREF bginner = COLOR_MUCOVER,
	_In_ COLORREF bglight = COLOR_MUCOVERL,
	_In_ COLORREF bgshadow = COLOR_MUCOVERS
);

//w:MU_SIZE, h:MU_SIZE
void drawMUMark(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ bool clicked,
	_In_ COLORREF mark = COLOR_MUMARK,
	_In_ COLORREF bginner = COLOR_MUCOVER,
	_In_ COLORREF bglight = COLOR_MUCOVERL,
	_In_ COLORREF bgshadow = COLOR_MUCOVERS,
	_In_ COLORREF bgclickedinner = COLOR_MUUNCOV,
	_In_ COLORREF bgclickededge = COLOR_MUUNCOVE
);

//w:MU_SIZE, h:MU_SIZE
void drawMUMine(
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

//w:MU_SIZE, h:MU_SIZE
void drawMUWrong(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF mine = COLOR_MUMINE,
	_In_ COLORREF light = COLOR_MUMINEL,
	_In_ COLORREF cross = COLOR_MUCROSS,
	_In_ COLORREF bginner = COLOR_MUUNCOV,
	_In_ COLORREF bgedge = COLOR_MUUNCOVE
);

//w:MU_SIZE, h:MU_SIZE
void drawMUNum(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int num,
	_In_ COLORREF numcolor = COLOR_MUNUMDEF,
	_In_ COLORREF bginner = COLOR_MUUNCOV,
	_In_ COLORREF bgedge = COLOR_MUUNCOVE
);
