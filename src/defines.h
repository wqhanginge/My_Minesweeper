#pragma once

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned dword;

/* following defines for GameInfo struct, see details in gamecore.h
 */

//Game Modes
#define JUNIOR	0
#define MIDDLE	1
#define SENIOR	2
#define CUSTOM	3
#define CRUSH	4
#define ISSTANDARD(Mode)	((byte)(Mode) < CUSTOM)
#define ISCRUSH(Mode)		((byte)(Mode) >= CRUSH)

#define JUNIOR_WIDTH	9
#define JUNIOR_HEIGHT	9
#define JUNIOR_SIZE		(JUNIOR_WIDTH * JUNIOR_HEIGHT)
#define JUNIOR_MINES	10

#define MIDDLE_WIDTH	16
#define MIDDLE_HEIGHT	16
#define MIDDLE_SIZE		(MIDDLE_WIDTH * MIDDLE_HEIGHT)
#define MIDDLE_MINES	40

#define SENIOR_WIDTH	30
#define SENIOR_HEIGHT	16
#define SENIOR_SIZE		(SENIOR_WIDTH * SENIOR_HEIGHT)
#define SENIOR_MINES	99

#define MAXWIDTH	30
#define MAXHEIGHT	24
#define MINWIDTH	9
#define MINHEIGHT	9
#define MAXMINERATE	0.9
#define MINMINES	10
//end Game Modes

//Game States
#define INITIAL		0
#define PROGRESS	1
#define SUCCESS		2
#define FAIL		3
//end Game States

//Game Map
/*  7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+
 * |M|  S  | mines |
 * +-+-+-+-+-+-+-+-+
 *     map_unit
 * M:	this unit is a mine
 * S:	state of this unit, covered, flagged, marked, uncovered, bombed, wrong flag
 * mines:	mines of neighbors
 */

#define MU_MINE		0x80
#define MUS_COVER	0x00
#define MUS_FLAG	0x10
#define MUS_MARK	0x20
#define MUS_UNCOV	0x30
#define MUS_BOMB	0x40
#define MUS_WRONG	0x50
#define GETMUMINES(unit)		((byte)((unit) & 0x0F))
#define MAKEMUMINES(m, unit)	((byte)((byte)((m) & 0x0F) | (byte)((unit) & 0xF0)))
#define GETMUSTATE(unit)		((byte)((unit) & 0x70))
#define MAKEMUSTATE(S, unit)	((byte)((byte)(S) | (byte)((unit) & 0x8F)))
#define MUISMINE(unit)			((unit) & 0x80)
//end Game Map


/* following defines for GameScore struct, see details in gamecore.h
*/

#define SCORENAMELEN	20
#define DEFSCORENAMEEN	"anonymous"
#define DEFSCORENAMECH	"ÄäÃû"

#define MAXTIME			999


/* following defines for user interface
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
#define MAPUNITSWIDTHJ	JUNIOR_WIDTH * MAPUNITSIZE
#define MAPUNITSWIDTHM	MIDDLE_WIDTH * MAPUNITSIZE
#define MAPUNITSWIDTHS	SENIOR_WIDTH * MAPUNITSIZE
#define MAPUNITSHEIGHTJ	JUNIOR_HEIGHT * MAPUNITSIZE
#define MAPUNITSHEIGHTM	MIDDLE_HEIGHT * MAPUNITSIZE
#define MAPUNITSHEIGHTS	SENIOR_HEIGHT * MAPUNITSIZE

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
#define INFOWIDTHJ	(PARTEDGE + MAPUNITSWIDTHJ + PARTEDGE)
#define INFOWIDTHM	(PARTEDGE + MAPUNITSWIDTHM + PARTEDGE)
#define INFOWIDTHS	(PARTEDGE + MAPUNITSWIDTHS + PARTEDGE)
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
#define TIMELEFTJ	(INFOLEFT + INFOWIDTHJ - RIGHTDIST - TIMEWIDTH)
#define TIMELEFTM	(INFOLEFT + INFOWIDTHM - RIGHTDIST - TIMEWIDTH)
#define TIMELEFTS	(INFOLEFT + INFOWIDTHS - RIGHTDIST - TIMEWIDTH)
#define TIMETOP		MINETOP
#define ITNSLEFTJ	(TIMELEFTJ + INSEDGE)
#define ITNSLEFTM	(TIMELEFTM + INSEDGE)
#define ITNSLEFTS	(TIMELEFTS + INSEDGE)
#define ITNSTOP		(TIMETOP + INSEDGE)

#define COLOR_TIME	COLOR_MINE
#define COLOR_TIMEL	COLOR_MINEL
#define COLOR_TIMES	COLOR_MINES

#define RESETBSIZE	32
#define RESETBLEFTJ	(INFOLEFT + (INFOWIDTHJ - RESETBSIZE) / 2)
#define RESETBLEFTM	(INFOLEFT + (INFOWIDTHM - RESETBSIZE) / 2)
#define RESETBLEFTS	(INFOLEFT + (INFOWIDTHS - RESETBSIZE) / 2)
#define RESETBTOP	(INFOTOP + PARTEDGE + 2)
//end Mine part, Time part and Reset Button

//MapArea
#define MAPAREAWIDTHJ	INFOWIDTHJ
#define MAPAREAWIDTHM	INFOWIDTHM
#define MAPAREAWIDTHS	INFOWIDTHS
#define MAPAREAHEIGHTJ	(PARTEDGE + MAPUNITSHEIGHTJ + PARTEDGE)
#define MAPAREAHEIGHTM	(PARTEDGE + MAPUNITSHEIGHTM + PARTEDGE)
#define MAPAREAHEIGHTS	(PARTEDGE + MAPUNITSHEIGHTS + PARTEDGE)
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
#define CLIENTWIDTHJ	(AREAEDGE + INFOWIDTHJ + AREAEDGE)
#define CLIENTWIDTHM	(AREAEDGE + INFOWIDTHM + AREAEDGE)
#define CLIENTWIDTHS	(AREAEDGE + INFOWIDTHS + AREAEDGE)
#define CLIENTHEIGHTJ	(AREAEDGE + INFOHEIGHT + AREAEDGE + MAPAREAHEIGHTJ + AREAEDGE)
#define CLIENTHEIGHTM	(AREAEDGE + INFOHEIGHT + AREAEDGE + MAPAREAHEIGHTM + AREAEDGE)
#define CLIENTHEIGHTS	(AREAEDGE + INFOHEIGHT + AREAEDGE + MAPAREAHEIGHTS + AREAEDGE)
#define CLIENTLEFT		0
#define CLIENTTOP		0

#define COLOR_CLIENT	RGB(240,240,240)
#define COLOR_CLIENTL	RGB(255,255,255)
#define COLOR_CLIENTS	RGB(128,128,128)
//end Client Area

//Custom Dialog
#define CUSTOMEDITLEN	8
//end Custom Dialog

//GetName Dialog
#define NAMEEDITLEN	SCORENAMELEN
//end GetName Dialog

//Record Dialog
#define TIMESTRLEN		10
#define DEFTIMEUNITEN	" Sec"
#define DEFTIMEUNITCH	" Ãë"
//end Record Dialog


/* following defines for file io
 */

#define DEFFILENAME		"MyMinesweeper.ini"
#define DEFFILEPATHEV	"LOCALAPPDATA"
#define INITAPPNAME		"InitInfo"
#define SCOREAPPNAME	"Record"


/* following defines for miscellaneous
 */

#define GAMETIMERID		1
#define GAMETIMERELAPSE	1000
#define DEFWNDLEFT		128
#define DEFWNDTOP		128
//this use for generate probability, generation function: P = 1 / 2^x, x = mines
//value = 1 << 8
#define MINEPRONE		256