#pragma once

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned dword;

/*
 * following defines for game logic
 */

/*  7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+
 * |M|  S  | mines |
 * +-+-+-+-+-+-+-+-+
 *     map_unit
 * M:	this unit is a mine
 * S:	state of this unit, covered, flagged, marked, uncovered, bombed, wrong flag
 * mines:	mines of neighbors
 */

#define UNITMINE	0x80
#define UNITCOVER	0x00
#define UNITFLAG	0x10
#define UNITMARK	0x20
#define UNITUNCOV	0x30
#define UNITBOMB	0x40
#define UNITWRONG	0x50
#define GETUNITMINES(unit)		((byte)((unit) & 0x0F))
#define SETUNITMINES(x, unit)	((byte)((byte)((x) & 0x0F) | (byte)((unit) & 0xF0)))
#define GETUNITSTATE(unit)		((byte)((unit) & 0x70))
#define SETUNITSTATE(S, unit)	((byte)((byte)(S) | (byte)((unit) & 0x8F)))
#define UNITISMINE(unit)		((unit) & 0x80)

//game modes
#define EASY	0
#define NORMAL	1
#define HARD	2
#define CUSTOM	3
#define CRUSH	4

#define EASY_WIDTH		9
#define EASY_HEIGHT		9
#define EASY_SIZE		(EASY_WIDTH * EASY_HEIGHT)
#define EASY_MINES		10
#define NORMAL_WIDTH	16
#define NORMAL_HEIGHT	16
#define NORMAL_SIZE		(NORMAL_WIDTH * NORMAL_HEIGHT)
#define NORMAL_MINES	40
#define HARD_WIDTH		30
#define HARD_HEIGHT		16
#define HARD_SIZE		(HARD_WIDTH * HARD_HEIGHT)
#define HARD_MINES		99

//game states
#define INITIAL		0
#define PROGRESS	1
#define SUCCESS		2
#define FAIL		3

//game map custom controls
#define MAXWIDTH	30
#define MAXHEIGHT	24
#define MINWIDTH	9
#define MINHEIGHT	9
#define MAXMINERATE	0.9
#define MINMINES	10
#define MAPEDITLEN	8

//game scores info
#define SCORENAMELEN	20
#define DEFSCORENAME	"Player"
#define DEFFILENAMEA	"\\AppData\\LocalLow\\MyMinesweeper.ini"
#define DEFFILENAMEW	L"\\AppData\\LocalLow\\MyMinesweeper.ini"
#define DEFFILEPATHEVA	"USERPROFILE"
#define DEFFILEPATHEVW	L"USERPROFILE"
#define MAXPATHBUF		100
#define MAXIGNORE		1000
#define SCOREEDITLEN	SCORENAMELEN

#define MAXTIME			999
#define DEFTIMEUNITA	" Sec"
#define DEFTIMEUNITW	L" Sec"
#define TIMESTRBUF		10

#ifdef UNICODE
#define DEFFILENAME		DEFFILENAMEW
#define DEFFILEPATHEV	DEFFILEPATHEVW
#define DEFTIMEUNIT		DEFTIMEUNITW
#else
#define DEFFILENAME		DEFFILENAMEA
#define DEFFILEPATHEV	DEFFILEPATHEVA
#define DEFTIMEUNIT		DEFTIMEUNITA
#endif

#define GAMETIMERID		1
#define GAMETIMERELAPSE	1000

/*
 * following defines for game ui
 */

/*         Main Client Area
 * +-----------------------------+---
 * |                             | |
 * |            Info             |Info_heaght
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
 * NOTE: these two parts have edge
 *
 * Info_height is static
 * MapArea_height and width are flexible
 */

/*                  Info
 * +----------------------------------------+---
 * |  +------+      +------+ --   +------+  |--- dist_top
 * |  | Mine |      |Reset | |    | Time |  |nums_height
 * |  +------+      |      | |D   +------+  |---
 * |                +------+ --             |dist_bottom
 * +----------------------------------------+---
 * |--|------|------|------|------|------|--|
 *  A    B      C      D      E      F    G
 * A:dist_left	B:nums_width	C:dist_mid	D:reset_button_size
 * E:dist_mid	F:nums_width	G:dist_right
 *
 * static: dist_top, dist_bottom, nums_height, dist_left, dist_right,
 *         nums_width, reset_button_size
 * flexible: dist_mid
 * NOTE: the reset_button_size is different from nums_height or nums_width,
 *       i.e. the dist_down may has different value, but the dist_up has
 *       only one value.
 *
 * Mine part, Time part and Reset Button have static size,
 * but Info part has flexible width, so that it's necessary to recalculate
 * the position when Game Mode changed.
 */

#define MAPUNITSIZE		24
#define MAPUNITSWIDTHE	(MAPUNITSIZE * EASY_WIDTH)
#define MAPUNITSWIDTHN	(MAPUNITSIZE * NORMAL_WIDTH)
#define MAPUNITSWIDTHH	(MAPUNITSIZE * HARD_WIDTH)
#define MAPUNITSHEIGHTE	(MAPUNITSIZE * EASY_HEIGHT)
#define MAPUNITSHEIGHTN	(MAPUNITSIZE * NORMAL_HEIGHT)
#define MAPUNITSHEIGHTH	(MAPUNITSIZE * HARD_HEIGHT)

#define AREAEDGE		6
#define THICKEDGE		2
#define THINEDGE		1
#define PARTEDGE		THICKEDGE
#define COLOR_DEFBG		RGB(240,240,240)
#define COLOR_DEFDEEP	RGB(192,192,192)
#define COLOR_DEFBLACK	RGB(0,0,0)
#define COLOR_DEFLIGHT	RGB(255,255,255)
#define COLOR_DEFSHADOW	RGB(128,128,128)

//Info
#define INFOWIDTHE	(PARTEDGE + MAPUNITSWIDTHE + PARTEDGE)
#define INFOWIDTHN	(PARTEDGE + MAPUNITSWIDTHN + PARTEDGE)
#define INFOWIDTHH	(PARTEDGE + MAPUNITSWIDTHH + PARTEDGE)
#define INFOHEIGHT	(PARTEDGE + 36 + PARTEDGE)
#define INFOLEFT	AREAEDGE
#define INFOTOP		AREAEDGE

#define COLOR_INFO	COLOR_DEFBG
#define COLOR_INFOL	COLOR_DEFLIGHT
#define COLOR_INFOS	COLOR_DEFSHADOW
//end Info

//Mine part, Time part and Reset Button
#define LEFTDIST		(PARTEDGE + 6)
#define RIGHTDIST		LEFTDIST
#define INFONUM_WIDTH	15
#define INFONUM_HEIGHT	27
#define INSWIDTH		(INFONUM_WIDTH * 3)
#define INSHEIGHT		INFONUM_HEIGHT
#define INSEDGE			THINEDGE

#define COLOR_INFONUMSBG	RGB(0,0,0)
#define COLOR_INFONUMSBT	RGB(255,0,0)
#define COLOR_INFONUMSDK	RGB(128,0,0)

#define MINEWIDTH	(INSEDGE + INSWIDTH + INSEDGE)
#define MINEHEIGHT	(INSEDGE + INSHEIGHT + INSEDGE)
#define MINELEFT	(INFOLEFT + LEFTDIST)
#define MINETOP		(INFOTOP + PARTEDGE + 3)
#define IMNSLEFT	(MINELEFT + INSEDGE)
#define IMNSTOP		(MINETOP + INSEDGE)

#define COLOR_MINE	COLOR_DEFBG
#define COLOR_MINEL	COLOR_DEFLIGHT
#define COLOR_MINES	COLOR_DEFSHADOW

#define TIMEWIDTH	MINEWIDTH
#define TIMEHEIGHT	MINEHEIGHT
#define TIMELEFTE	(INFOLEFT + INFOWIDTHE - RIGHTDIST - TIMEWIDTH)
#define TIMELEFTN	(INFOLEFT + INFOWIDTHN - RIGHTDIST - TIMEWIDTH)
#define TIMELEFTH	(INFOLEFT + INFOWIDTHH - RIGHTDIST - TIMEWIDTH)
#define TIMETOP		MINETOP
#define ITNSLEFTE	(TIMELEFTE + INSEDGE)
#define ITNSLEFTN	(TIMELEFTN + INSEDGE)
#define ITNSLEFTH	(TIMELEFTH + INSEDGE)
#define ITNSTOP		(TIMETOP + INSEDGE)

#define COLOR_TIME	COLOR_MINE
#define COLOR_TIMEL	COLOR_MINEL
#define COLOR_TIMES	COLOR_MINES

#define RESETBSIZE	32
#define RESETBLEFTE	(INFOLEFT + (INFOWIDTHE - RESETBSIZE) / 2)
#define RESETBLEFTN	(INFOLEFT + (INFOWIDTHN - RESETBSIZE) / 2)
#define RESETBLEFTH	(INFOLEFT + (INFOWIDTHH - RESETBSIZE) / 2)
#define RESETBTOP	(INFOTOP + PARTEDGE + 2)
//end Mine part, Time part and Reset Button

//MapArea
#define MAPAREAWIDTHE	INFOWIDTHE
#define MAPAREAWIDTHN	INFOWIDTHN
#define MAPAREAWIDTHH	INFOWIDTHH
#define MAPAREAHEIGHTE	(PARTEDGE + MAPUNITSHEIGHTE + PARTEDGE)
#define MAPAREAHEIGHTN	(PARTEDGE + MAPUNITSHEIGHTN + PARTEDGE)
#define MAPAREAHEIGHTH	(PARTEDGE + MAPUNITSHEIGHTH + PARTEDGE)
#define MAPAREALEFT		AREAEDGE
#define MAPAREATOP		(AREAEDGE + INFOHEIGHT + AREAEDGE)

#define MAPUNITSLEFT	(MAPAREALEFT + PARTEDGE)
#define MAPUNITSTOP		(MAPAREATOP + PARTEDGE)

#define COLOR_MAPAREA	COLOR_DEFBG
#define COLOR_MAPAREAL	COLOR_DEFLIGHT
#define COLOR_MAPAREAS	COLOR_DEFSHADOW

#define COLOR_MUCOVER	COLOR_DEFDEEP
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
#define CLIENTWIDTHE	(AREAEDGE + INFOWIDTHE + AREAEDGE)
#define CLIENTWIDTHN	(AREAEDGE + INFOWIDTHN + AREAEDGE)
#define CLIENTWIDTHH	(AREAEDGE + INFOWIDTHH + AREAEDGE)
#define CLIENTHEIGHTE	(AREAEDGE + INFOHEIGHT + AREAEDGE + MAPAREAHEIGHTE + AREAEDGE)
#define CLIENTHEIGHTN	(AREAEDGE + INFOHEIGHT + AREAEDGE + MAPAREAHEIGHTN + AREAEDGE)
#define CLIENTHEIGHTH	(AREAEDGE + INFOHEIGHT + AREAEDGE + MAPAREAHEIGHTH + AREAEDGE)
#define CLIENTLEFT		0
#define CLIENTTOP		0

#define COLOR_CLIENT	RGB(240,240,240)
#define COLOR_CLIENTL	RGB(255,255,255)
#define COLOR_CLIENTS	RGB(128,128,128)
//end Client Area