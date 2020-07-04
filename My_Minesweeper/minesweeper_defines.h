#pragma once

/*
 * +-----------------------------+---
 * |                             | |
 * |            Info             |Info_heaght
 * |-------client_width----------| |
 * +-----------------------------+---
 * |                          |  | |
 * |                client_height| |
 * |           MapArea        |  |MapArea_height
 * |                          |  | |
 * |                          |  | |
 * +-----------------------------+---
 * |----------DLG_width----------|
 *
 * NOTE: these two dialogs have edge, so that the client_width
 *       is different from DLG_width, and so does the height.
 *
 * this frame is full of outest window's client area, i.e. the DLG_width is
 * the client_width of outest window.
 * Info Dialog has static height and flexible width which depends on
 * the map size. MapArea Dialog has both flexible width and height witch
 * depends on the map size.
 */

#define DLGEDGE		2
#define DDLGEDGE	(DLGEDGE * 2)
#define INFOCHEIGHT	36
#define INFOHEIGHT	(INFOCHEIGHT + DDLGEDGE)

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
 * NOTE: Mine Dialog and Time Dialog have edge, so that the nums_height is
 *       different from bitmap's height in the Dialog, the same as width.
 *
 * Mine Dialog, Time Dialog and Reset Button have static size,
 * but Info Dialog has flexible width, so that it's necessary to recalculate
 * the Control position when Game Mode changed.
 */

#define BM_BITWIDTH		24
#define BM_BYTECOUNT	(BM_BITWIDTH / 8)
#define BM_INFONUMS_WIDTH	15
#define BM_INFONUMS_HEIGHT	27
#define BM_INFONUMS_COUNT	11

#define MINE_DISTLEFT	6
#define MINE_DISTTOP	2
#define TIME_DISTRIGHT	MINE_DISTLEFT
#define TIME_DISTTOP	MINE_DISTTOP
#define RESETB_SIZE		32
#define MINE_CWIDTH		(BM_INFONUMS_WIDTH * 3)
#define MINE_CHEIGHT	BM_INFONUMS_HEIGHT
#define MINE_WIDTH		(MINE_CWIDTH + DDLGEDGE)
#define MINE_HEIGHT		(MINE_CHEIGHT + DDLGEDGE)
#define TIME_CWIDTH		MINE_CWIDTH
#define TIME_CHEIGHT	MINE_CHEIGHT
#define TIME_WIDTH		MINE_WIDTH
#define TIME_HEIGHT		MINE_HEIGHT
#define GAMETIMERID		1
#define GAMETIMERELAPSE	1000

/*
 * MapArea Dialog has flexible width and height depend on map size.
 * each unit in MapArea Dialog has static sizes, which are Small,
 * Middle, Large.
 */

#define BM_MAPUNIT_SIZES	16
#define BM_MAPUNIT_SIZEM	24
#define BM_MAPUNIT_SIZEL	32
#define BM_MAPNUMS_COUNT	9

/*
 * Game Modes
 */

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

/*
 * Info Dialog flexible sizes and MapArea Dialog flexible sizes
 */
//MapArea
#define MAE_CWS	(BM_MAPUNIT_SIZES * EASY_WIDTH)
#define MAE_CHS	(BM_MAPUNIT_SIZES * EASY_HEIGHT)
#define MAE_CWM	(BM_MAPUNIT_SIZEM * EASY_WIDTH)
#define MAE_CHM	(BM_MAPUNIT_SIZEM * EASY_HEIGHT)
#define MAE_CWL	(BM_MAPUNIT_SIZEL * EASY_WIDTH)
#define MAE_CHL	(BM_MAPUNIT_SIZEL * EASY_HEIGHT)

#define MAN_CWS	(BM_MAPUNIT_SIZES * NORMAL_WIDTH)
#define MAN_CHS	(BM_MAPUNIT_SIZES * NORMAL_HEIGHT)
#define MAN_CWM	(BM_MAPUNIT_SIZEM * NORMAL_WIDTH)
#define MAN_CHM	(BM_MAPUNIT_SIZEM * NORMAL_HEIGHT)
#define MAN_CWL	(BM_MAPUNIT_SIZEL * NORMAL_WIDTH)
#define MAN_CHL	(BM_MAPUNIT_SIZEL * NORMAL_HEIGHT)

#define MAH_CWS	(BM_MAPUNIT_SIZES * HARD_WIDTH)
#define MAH_CHS	(BM_MAPUNIT_SIZES * HARD_HEIGHT)
#define MAH_CWM	(BM_MAPUNIT_SIZEM * HARD_WIDTH)
#define MAH_CHM	(BM_MAPUNIT_SIZEM * HARD_HEIGHT)
#define MAH_CWL	(BM_MAPUNIT_SIZEL * HARD_WIDTH)
#define MAH_CHL	(BM_MAPUNIT_SIZEL * HARD_HEIGHT)

#define MAE_WS	(MAE_CWS + DDLGEDGE)
#define MAE_HS	(MAE_CHS + DDLGEDGE)
#define MAE_WM	(MAE_CWM + DDLGEDGE)
#define MAE_HM	(MAE_CHM + DDLGEDGE)
#define MAE_WL	(MAE_CWL + DDLGEDGE)
#define MAE_HL	(MAE_CHL + DDLGEDGE)

#define MAN_WS	(MAN_CWS + DDLGEDGE)
#define MAN_HS	(MAN_CHS + DDLGEDGE)
#define MAN_WM	(MAN_CWM + DDLGEDGE)
#define MAN_HM	(MAN_CHM + DDLGEDGE)
#define MAN_WL	(MAN_CWL + DDLGEDGE)
#define MAN_HL	(MAN_CHL + DDLGEDGE)

#define MAH_WS	(MAH_CWS + DDLGEDGE)
#define MAH_HS	(MAH_CHS + DDLGEDGE)
#define MAH_WM	(MAH_CWM + DDLGEDGE)
#define MAH_HM	(MAH_CHM + DDLGEDGE)
#define MAH_WL	(MAH_CWL + DDLGEDGE)
#define MAH_HL	(MAH_CHL + DDLGEDGE)

#define MA_LEFT	0
#define MA_TOP	INFOHEIGHT
//end MapArea

//Info
#define IE_CWS	MAE_CWS
#define IE_CWM	MAE_CWM
#define IE_CWL	MAE_CWL

#define IN_CWS	MAN_CWS
#define IN_CWM	MAN_CWM
#define IN_CWL	MAN_CWL

#define IH_CWS	MAH_CWS
#define IH_CWM	MAH_CWM
#define IH_CWL	MAH_CWL

#define INFO_CH	INFOCHEIGHT

#define IE_WS	MAE_WS
#define IE_WM	MAE_WM
#define IE_WL	MAE_WL

#define IN_WS	MAN_WS
#define IN_WM	MAN_WM
#define IN_WL	MAN_WL

#define IH_WS	MAH_WS
#define IH_WM	MAH_WM
#define IH_WL	MAH_WL

#define INFO_H	INFOHEIGHT

#define INFO_LEFT	0
#define INFO_TOP	0
//end Info

/*
 * Control flexible positions in Info Dialog
 */

#define MINE_LEFT		MINE_DISTLEFT
#define MINE_TOP		MINE_DISTTOP

//distance between Time Dialog left edge and Info Dialog client area right edge
#define TIME_ROFFSET	(TIME_WIDTH + TIME_DISTRIGHT)
#define TIME_TOP		TIME_DISTTOP

#define TIMEE_LEFTS		(IE_CWS - TIME_ROFFSET)
#define TIMEE_LEFTM		(IE_CWM - TIME_ROFFSET)
#define TIMEE_LEFTL		(IE_CWL - TIME_ROFFSET)

#define TIMEN_LEFTS		(IN_CWS - TIME_ROFFSET)
#define TIMEN_LEFTM		(IN_CWM - TIME_ROFFSET)
#define TIMEN_LEFTL		(IN_CWL - TIME_ROFFSET)

#define TIMEH_LEFTS		(IH_CWS - TIME_ROFFSET)
#define TIMEH_LEFTM		(IH_CWM - TIME_ROFFSET)
#define TIMEH_LEFTL		(IH_CWL - TIME_ROFFSET)

#define RESETB_TOP		MINE_TOP

#define RESETBE_LEFTS	((IE_CWS - RESETB_SIZE) / 2)
#define RESETBE_LEFTM	((IE_CWM - RESETB_SIZE) / 2)
#define RESETBE_LEFTL	((IE_CWL - RESETB_SIZE) / 2)

#define RESETBN_LEFTS	((IN_CWS - RESETB_SIZE) / 2)
#define RESETBN_LEFTM	((IN_CWM - RESETB_SIZE) / 2)
#define RESETBN_LEFTL	((IN_CWL - RESETB_SIZE) / 2)

#define RESETBH_LEFTS	((IH_CWS - RESETB_SIZE) / 2)
#define RESETBH_LEFTM	((IH_CWM - RESETB_SIZE) / 2)
#define RESETBH_LEFTL	((IH_CWL - RESETB_SIZE) / 2)

/*
 * following defines for game logic
 */

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned dword;

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
#define SCORENAME_COUNT	20
#define DEFSCORENAME	"Player"
#define DEFFILENAMEA	"\\AppData\\LocalLow\\MyMinesweeper.ini"
#define DEFFILENAMEW	L"\\AppData\\LocalLow\\MyMinesweeper.ini"
#define DEFFILEPATHEVA	"USERPROFILE"
#define DEFFILEPATHEVW	L"USERPROFILE"
#define MAXPATHBUF		100
#define MAXIGNORE		1000
#define SCOREEDITLEN	SCORENAME_COUNT

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