#include "userinterface.h"


/* this array stores infonum backgroud
 * 0 represent background color, 1 represent dark color
 */
const bool InfoNumBG[INFONUM_WIDTH][INFONUM_HEIGHT] =
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,6,0,6,0,6,0,6,0,6,0,6,0,5,0,5,0,5,0,5,0,5,0,5,0,0,
	0,0,0,6,0,6,0,6,0,6,0,6,0,0,0,5,0,5,0,5,0,5,0,5,0,0,0,
	0,1,0,0,6,0,6,0,6,0,6,0,0,7,0,0,5,0,5,0,5,0,5,0,0,4,0,
	0,0,1,0,0,0,0,0,0,0,0,0,7,0,7,0,0,0,0,0,0,0,0,0,4,0,0,
	0,1,0,1,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,4,0,4,0,
	0,0,1,0,0,0,0,0,0,0,0,0,7,0,7,0,0,0,0,0,0,0,0,0,4,0,0,
	0,1,0,1,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,4,0,4,0,
	0,0,1,0,0,0,0,0,0,0,0,0,7,0,7,0,0,0,0,0,0,0,0,0,4,0,0,
	0,1,0,1,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,4,0,4,0,
	0,0,1,0,0,0,0,0,0,0,0,0,7,0,7,0,0,0,0,0,0,0,0,0,4,0,0,
	0,1,0,0,2,0,2,0,2,0,2,0,0,7,0,0,3,0,3,0,3,0,3,0,0,4,0,
	0,0,0,2,0,2,0,2,0,2,0,2,0,0,0,3,0,3,0,3,0,3,0,3,0,0,0,
	0,0,2,0,2,0,2,0,2,0,2,0,2,0,3,0,3,0,3,0,3,0,3,0,3,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

void loadbitmaps()
{
	hbm_resetb = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RESETB));
	hbm_click = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CLICK));
	hbm_success = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SUCCESS));
	hbm_fail = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FAIL));
}

void freebitmaps()
{
	DeleteObject(hbm_resetb);
	DeleteObject(hbm_click);
	DeleteObject(hbm_success);
	DeleteObject(hbm_fail);
}

int mapunitswidth()
{
	switch (Game.mode) {
	case JUNIOR:	return MAPUNITSWIDTHJ;
	case MIDDLE:	return MAPUNITSWIDTHM;
	case SENIOR:	return MAPUNITSWIDTHS;
	case CUSTOM:	return (Game.width * MAPUNITSIZE);
	default:		break;
	}
	return -1;
}

int mapunitsheight()
{
	switch (Game.mode) {
	case JUNIOR:	return MAPUNITSHEIGHTJ;
	case MIDDLE:	return MAPUNITSHEIGHTM;
	case SENIOR:	return MAPUNITSHEIGHTS;
	case CUSTOM:	return (Game.height * MAPUNITSIZE);
	default:		break;
	}
	return -1;
}

int infowidth()
{
	switch (Game.mode) {
	case JUNIOR:	return INFOWIDTHJ;
	case MIDDLE:	return INFOWIDTHM;
	case SENIOR:	return INFOWIDTHS;
	case CUSTOM:	return (PARTEDGE + Game.width * MAPUNITSIZE + PARTEDGE);
	default:		break;
	}
	return -1;
}

int timeleft()
{
	switch (Game.mode) {
	case JUNIOR:	return TIMELEFTJ;
	case MIDDLE:	return TIMELEFTM;
	case SENIOR:	return TIMELEFTS;
	case CUSTOM:	return (INFOLEFT + (PARTEDGE + Game.width * MAPUNITSIZE + PARTEDGE) - RIGHTDIST - TIMEWIDTH);
	default:		break;
	}
	return -1;
}

int infotimenumsleft()
{
	switch (Game.mode) {
	case JUNIOR:	return ITNSLEFTJ;
	case MIDDLE:	return ITNSLEFTM;
	case SENIOR:	return ITNSLEFTS;
	case CUSTOM:	return ((INFOLEFT + (PARTEDGE + Game.width * MAPUNITSIZE + PARTEDGE) - RIGHTDIST - TIMEWIDTH) + INSEDGE);
	default:		break;
	}
	return -1;
}

int resetbleft()
{
	switch (Game.mode) {
	case JUNIOR:	return RESETBLEFTJ;
	case MIDDLE:	return RESETBLEFTM;
	case SENIOR:	return RESETBLEFTS;
	case CUSTOM:	return (INFOLEFT + ((PARTEDGE + Game.width * MAPUNITSIZE + PARTEDGE) - RESETBSIZE) / 2);
	default:		break;
	}
	return -1;
}

int mapareawidth()
{
	switch (Game.mode) {
	case JUNIOR:	return MAPAREAWIDTHJ;
	case MIDDLE:	return MAPAREAWIDTHM;
	case SENIOR:	return MAPAREAWIDTHS;
	case CUSTOM:	return (PARTEDGE + Game.width * MAPUNITSIZE + PARTEDGE);
	default:		break;
	}
	return -1;
}

int mapareaheight()
{
	switch (Game.mode) {
	case JUNIOR:	return MAPAREAHEIGHTJ;
	case MIDDLE:	return MAPAREAHEIGHTM;
	case SENIOR:	return MAPAREAHEIGHTS;
	case CUSTOM:	return (PARTEDGE + Game.height * MAPUNITSIZE + PARTEDGE);
	default:		break;
	}
	return -1;
}

int clientwidth()
{
	switch (Game.mode) {
	case JUNIOR:	return CLIENTWIDTHJ;
	case MIDDLE:	return CLIENTWIDTHM;
	case SENIOR:	return CLIENTWIDTHS;
	case CUSTOM:	return (AREAEDGE + (PARTEDGE + Game.width * MAPUNITSIZE + PARTEDGE) + AREAEDGE);
	default:		break;
	}
	return -1;
}

int clientheight()
{
	switch (Game.mode) {
	case JUNIOR:	return CLIENTHEIGHTJ;
	case MIDDLE:	return CLIENTHEIGHTM;
	case SENIOR:	return CLIENTHEIGHTS;
	case CUSTOM:	return (AREAEDGE + INFOHEIGHT + AREAEDGE + (PARTEDGE + Game.height * MAPUNITSIZE + PARTEDGE) + AREAEDGE);
	default:		break;
	}
	return -1;
}


/* these static functions are used only in this file */
//draw 2 pixel edge concave background, exchange 'light' and 'shadow' to draw a convex backgroung
static void drawthickedgebg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int width,
	_In_ int height,
	_In_ COLORREF inner,
	_In_ COLORREF light,
	_In_ COLORREF shadow
)
{
	HBRUSH hbinner, hblight, hbshadow;
	hbinner = CreateSolidBrush(inner);
	hblight = CreateSolidBrush(light);
	hbshadow = CreateSolidBrush(shadow);

	RECT rect = { left,top,left + width - 2,top + height - 2 };
	FillRect(hdestdc, &rect, hbshadow);
	rect = { left + 2,top + 2,left + width,top + height };
	FillRect(hdestdc, &rect, hblight);
	rect = { left + 2,top + 2,left + width - 2,top + height - 2 };
	FillRect(hdestdc, &rect, hbinner);

	SetPixel(hdestdc, left, rect.bottom, shadow);
	SetPixel(hdestdc, rect.right, top, shadow);
	SetPixel(hdestdc, left + 1, rect.bottom + 1, light);
	SetPixel(hdestdc, rect.right + 1, top + 1, light);
	SetPixel(hdestdc, left, rect.bottom + 1, inner);
	SetPixel(hdestdc, left + 1, rect.bottom, inner);
	SetPixel(hdestdc, rect.right, top + 1, inner);
	SetPixel(hdestdc, rect.right + 1, top, inner);

	DeleteObject(hbinner);
	DeleteObject(hblight);
	DeleteObject(hbshadow);
}

//draw 1 pixel edge concave background, exchange 'light' and 'shadow' to draw a convex background
static void drawthinedgebg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ int width,
	_In_ int height,
	_In_ COLORREF inner,
	_In_ COLORREF light,
	_In_ COLORREF shadow
)
{
	HBRUSH hbinner, hblight, hbshadow;
	hbinner = CreateSolidBrush(inner);
	hblight = CreateSolidBrush(light);
	hbshadow = CreateSolidBrush(shadow);

	RECT rect = { left,top,left + width - 1,top + height - 1 };
	FillRect(hdestdc, &rect, hbshadow);
	rect = { left + 1,top + 1,left + width,top + height };
	FillRect(hdestdc, &rect, hblight);
	rect = { left + 1,top + 1,left + width - 1,top + height - 1 };
	FillRect(hdestdc, &rect, hbinner);

	SetPixel(hdestdc, left, rect.bottom, inner);
	SetPixel(hdestdc, rect.right, top, inner);

	DeleteObject(hbinner);
	DeleteObject(hblight);
	DeleteObject(hbshadow);
}

//draw 7sd background
static inline void draw7sdbg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF bg = COLOR_INFONUMSBG,
	_In_ COLORREF dark = COLOR_INFONUMSDK
)
{
	for (int i = 0; i < INFONUM_WIDTH; i++) {
		for (int j = 0; j < INFONUM_HEIGHT; j++) {
			if (InfoNumBG[i][j]) SetPixel(hdestdc, left + i, top + j, dark);
			else SetPixel(hdestdc, left + i, top + j, bg);
		}
	}
}

/*   a
 * +---+
 *f| g |b
 * +---+
 *e|   |c
 * +---+
 *   d
 */
 //draw specific line on 7sd, must select pen before calling
static inline void draw7sda(_In_ HDC h7sddc, _In_ int left, _In_ int top)
{
	MoveToEx(h7sddc, left + 2, top + 1, NULL);
	LineTo(h7sddc, left + 13, top + 1);
	MoveToEx(h7sddc, left + 3, top + 2, NULL);
	LineTo(h7sddc, left + 12, top + 2);
	MoveToEx(h7sddc, left + 4, top + 3, NULL);
	LineTo(h7sddc, left + 11, top + 3);
}
static inline void draw7sdb(_In_ HDC h7sddc, _In_ int left, _In_ int top)
{
	MoveToEx(h7sddc, left + 11, top + 4, NULL);
	LineTo(h7sddc, left + 11, top + 11);
	MoveToEx(h7sddc, left + 12, top + 3, NULL);
	LineTo(h7sddc, left + 12, top + 12);
	MoveToEx(h7sddc, left + 13, top + 2, NULL);
	LineTo(h7sddc, left + 13, top + 13);
}
static inline void draw7sdc(_In_ HDC h7sddc, _In_ int left, _In_ int top)
{
	MoveToEx(h7sddc, left + 11, top + 16, NULL);
	LineTo(h7sddc, left + 11, top + 23);
	MoveToEx(h7sddc, left + 12, top + 15, NULL);
	LineTo(h7sddc, left + 12, top + 24);
	MoveToEx(h7sddc, left + 13, top + 14, NULL);
	LineTo(h7sddc, left + 13, top + 25);
}
static inline void draw7sdd(_In_ HDC h7sddc, _In_ int left, _In_ int top)
{
	MoveToEx(h7sddc, left + 4, top + 23, NULL);
	LineTo(h7sddc, left + 11, top + 23);
	MoveToEx(h7sddc, left + 3, top + 24, NULL);
	LineTo(h7sddc, left + 12, top + 24);
	MoveToEx(h7sddc, left + 2, top + 25, NULL);
	LineTo(h7sddc, left + 13, top + 25);
}
static inline void draw7sde(_In_ HDC h7sddc, _In_ int left, _In_ int top)
{
	MoveToEx(h7sddc, left + 1, top + 14, NULL);
	LineTo(h7sddc, left + 1, top + 25);
	MoveToEx(h7sddc, left + 2, top + 15, NULL);
	LineTo(h7sddc, left + 2, top + 24);
	MoveToEx(h7sddc, left + 3, top + 16, NULL);
	LineTo(h7sddc, left + 3, top + 23);
}
static inline void draw7sdf(_In_ HDC h7sddc, _In_ int left, _In_ int top)
{
	MoveToEx(h7sddc, left + 1, top + 2, NULL);
	LineTo(h7sddc, left + 1, top + 13);
	MoveToEx(h7sddc, left + 2, top + 3, NULL);
	LineTo(h7sddc, left + 2, top + 12);
	MoveToEx(h7sddc, left + 3, top + 4, NULL);
	LineTo(h7sddc, left + 3, top + 11);
}
static inline void draw7sdg(_In_ HDC h7sddc, _In_ int left, _In_ int top)
{
	MoveToEx(h7sddc, left + 3, top + 12, NULL);
	LineTo(h7sddc, left + 12, top + 12);
	MoveToEx(h7sddc, left + 2, top + 13, NULL);
	LineTo(h7sddc, left + 13, top + 13);
	MoveToEx(h7sddc, left + 3, top + 14, NULL);
	LineTo(h7sddc, left + 12, top + 14);
}


//draw a mine icon
static void drawmuitemmine(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF mine = COLOR_MUMINE,
	_In_ COLORREF light = COLOR_MUMINEL
)
{
	HBRUSH hbmine, hblight;
	hbmine = CreateSolidBrush(mine);
	hblight = CreateSolidBrush(light);

	RECT rect = { left + 6,top + 9,left + 19,top + 16 };
	FillRect(hdestdc, &rect, hbmine);
	rect = { left + 7,top + 7,left + 18,top + 18 };
	FillRect(hdestdc, &rect, hbmine);
	rect = { left + 9,top + 6,left + 16,top + 19 };
	FillRect(hdestdc, &rect, hbmine);

	rect = { left + 3,top + 12,left + 22,top + 13 };
	FillRect(hdestdc, &rect, hbmine);
	rect = { left + 12,top + 3,left + 13,top + 22 };
	FillRect(hdestdc, &rect, hbmine);

	rect = { left + 9,top + 9,left + 12,top + 12 };
	FillRect(hdestdc, &rect, hblight);

	SetPixel(hdestdc, left + 6, top + 6, mine);
	SetPixel(hdestdc, left + 6, top + 18, mine);
	SetPixel(hdestdc, left + 18, top + 6, mine);
	SetPixel(hdestdc, left + 18, top + 18, mine);

	DeleteObject(hbmine);
	DeleteObject(hblight);
}
//draw a question mark icon
static void drawmuitemmark(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ bool moved,
	_In_ COLORREF mark = COLOR_MUMARK
)
{
	HBRUSH hbmark = CreateSolidBrush(mark);
	RECT rect = { left + moved + 9,top + moved + 4,left + moved + 15,top + moved + 5 };
	FillRect(hdestdc, &rect, hbmark);
	rect = { left + moved + 7,top + moved + 5,left + moved + 10,top + moved + 9 };
	FillRect(hdestdc, &rect, hbmark);
	rect = { left + moved + 14,top + moved + 5,left + moved + 17,top + moved + 10 };
	FillRect(hdestdc, &rect, hbmark);
	rect = { left + moved + 12,top + moved + 10,left + moved + 15,top + moved + 12 };
	FillRect(hdestdc, &rect, hbmark);
	rect = { left + moved + 10,top + moved + 12,left + moved + 14,top + moved + 15 };
	FillRect(hdestdc, &rect, hbmark);
	rect = { left + moved + 10,top + moved + 17,left + moved + 14,top + moved + 20 };
	FillRect(hdestdc, &rect, hbmark);
	DeleteObject(hbmark);
}
//draw a flag icon
static void drawmuitemflag(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF flag = COLOR_MUFLAGF,
	_In_ COLORREF base = COLOR_MUFLAGB
)
{
	HBRUSH hbflag, hbbase;
	hbflag = CreateSolidBrush(flag);
	hbbase = CreateSolidBrush(base);

	RECT rect = { left + 6,top + 7,left + 7,top + 9 };
	FillRect(hdestdc, &rect, hbflag);
	rect = { left + 7,top + 6,left + 10,top + 10 };
	FillRect(hdestdc, &rect, hbflag);
	rect = { left + 10,top + 4,left + 13,top + 12 };
	FillRect(hdestdc, &rect, hbflag);

	rect = { left + 12,top + 12,left + 13,top + 15 };
	FillRect(hdestdc, &rect, hbbase);
	rect = { left + 9,top + 15,left + 15,top + 16 };
	FillRect(hdestdc, &rect, hbbase);
	rect = { left + 6,top + 16,left + 18,top + 19 };
	FillRect(hdestdc, &rect, hbbase);

	DeleteObject(hbflag);
	DeleteObject(hbbase);
}
//draw a cross mark icon
static void drawmuitemcross(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF cross = COLOR_MUCROSS
)
{
	HPEN hpcross = CreatePen(PS_SOLID, 2, cross);
	SelectObject(hdestdc, hpcross);
	MoveToEx(hdestdc, left + 4, top + 4, NULL);
	LineTo(hdestdc, left + 20, top + 20);
	MoveToEx(hdestdc, left + 4, top + 20, NULL);
	LineTo(hdestdc, left + 20, top + 4);
	DeleteObject(hpcross);
}
//draw a number 1 icon
static void drawmuitemnum1(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF number = COLOR_MUNUM1
)
{
	HBRUSH hbrush = CreateSolidBrush(number);
	RECT rect = { left + 11,top + 5,left + 15,top + 6 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 9,top + 6,left + 15,top + 7 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 7,top + 7,left + 15,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 5,top + 8,left + 15,top + 10 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 10,top + 10,left + 15,top + 17 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 5,top + 17,left + 20,top + 20 };
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}
//draw a number 2 icon
static void drawmuitemnum2(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF number = COLOR_MUNUM2
)
{
	HBRUSH hbrush = CreateSolidBrush(number);

	RECT rect = { left + 5,top + 7,left + 9,top + 10 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 7,top + 5,left + 18,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 16,top + 7,left + 20,top + 11 };
	FillRect(hdestdc, &rect, hbrush);

	rect = { left + 15,top + 10,left + 19,top + 13 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 13,top + 11,left + 17,top + 14 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 11,top + 12,left + 15,top + 15 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 9,top + 13,left + 13,top + 16 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 7,top + 14,left + 11,top + 17 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 5,top + 15,left + 9,top + 18 };
	FillRect(hdestdc, &rect, hbrush);

	rect = { left + 5,top + 17,left + 20,top + 20 };
	FillRect(hdestdc, &rect, hbrush);

	SetPixel(hdestdc, left + 6, top + 6, number);
	SetPixel(hdestdc, left + 18, top + 6, number);

	DeleteObject(hbrush);
}
//draw a number 3 icon
static void drawmuitemnum3(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF number = COLOR_MUNUM3
)
{
	HBRUSH hbrush = CreateSolidBrush(number);
	RECT rect = { left + 5,top + 5,left + 19,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 16,top + 7,left + 20,top + 11 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 10,top + 11,left + 19,top + 14 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 16,top + 14,left + 20,top + 19 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 5,top + 17,left + 19,top + 20 };
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}
//draw a number 4 icon
static void drawmuitemnum4(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF number = COLOR_MUNUM4
)
{
	HBRUSH hbrush = CreateSolidBrush(number);
	RECT rect = { left + 8,top + 5,left + 13,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 7,top + 8,left + 11,top + 11 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 5,top + 11,left + 20,top + 14 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 14,top + 5,left + 19,top + 20 };
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}
//draw a number 5 icon
static void drawmuitemnum5(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF number = COLOR_MUNUM5
)
{
	HBRUSH hbrush = CreateSolidBrush(number);
	RECT rect = { left + 5,top + 5,left + 20,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 5,top + 8,left + 10,top + 11 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 5,top + 11,left + 19,top + 14 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 16,top + 13,left + 20,top + 19 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 5,top + 17,left + 19,top + 20 };
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}
//draw a number 6 icon
static void drawmuitemnum6(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF number = COLOR_MUNUM6
)
{
	HBRUSH hbrush = CreateSolidBrush(number);
	RECT rect = { left + 7,top + 5,left + 19,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 5,top + 7,left + 10,top + 19 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 10,top + 11,left + 19,top + 14 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 16,top + 13,left + 20,top + 19 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 7,top + 17,left + 19,top + 20 };
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}
//draw a number 7 icon
static void drawmuitemnum7(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF number = COLOR_MUNUM7
)
{
	HBRUSH hbrush = CreateSolidBrush(number);
	RECT rect = { left + 5,top + 5,left + 20,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 16,top + 8,left + 20,top + 11 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 14,top + 11,left + 19,top + 14 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 13,top + 14,left + 17,top + 17 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 11,top + 17,left + 16,top + 20 };
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}
//draw a number 8 icon
static void drawmuitemnum8(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF number = COLOR_MUNUM8
)
{
	HBRUSH hbrush = CreateSolidBrush(number);
	RECT rect = { left + 7,top + 5,left + 19,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 5,top + 7,left + 10,top + 11 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 16,top + 7,left + 20,top + 11 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 7,top + 11,left + 19,top + 14 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 5,top + 14,left + 10,top + 19 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 16,top + 14,left + 20,top + 19 };
	FillRect(hdestdc, &rect, hbrush);
	rect = { left + 7,top + 17,left + 19,top + 20 };
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}

//logical xor
static inline bool _xor(const bool A, const bool B)
{
	return (!A && B || A && !B);
}


int rpostoindex(int rleft, int rtop)
{
	return xytoindex(rlefttox(rleft), rtoptoy(rtop));
}

int rlefttox(int rleft)
{
	return (rleft / MAPUNITSIZE);
}

int rtoptoy(int rtop)
{
	return (rtop / MAPUNITSIZE);
}

int indextorleft(int index)
{
	return xtorleft(indextox(index));
}

int indextortop(int index)
{
	return ytortop(indextoy(index));
}

int xtorleft(int x)
{
	return (x * MAPUNITSIZE);
}

int ytortop(int y)
{
	return (y * MAPUNITSIZE);
}

void drawclientbg(HDC hdestdc, int left, int top, COLORREF def)
{
	RECT rect = { left,top,left + CLIENTWIDTH,top + CLIENTHEIGHT };
	HBRUSH hbrush = CreateSolidBrush(def);
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}

void drawinfobg(HDC hdestdc, int left, int top, COLORREF inner, COLORREF light, COLORREF shadow)
{
	drawthickedgebg(hdestdc, left, top, INFOWIDTH, INFOHEIGHT, inner, light, shadow);
}

void drawmapareabg(HDC hdestdc, int left, int top, COLORREF inner, COLORREF light, COLORREF shadow)
{
	drawthickedgebg(hdestdc, left, top, MAPAREAWIDTH, MAPAREAHEIGHT, inner, light, shadow);
}

void drawminebg(HDC hdestdc, int left, int top, COLORREF inner, COLORREF light, COLORREF shadow)
{
	drawthinedgebg(hdestdc, left, top, MINEWIDTH, MINEHEIGHT, inner, light, shadow);
}

void drawtimebg(HDC hdestdc, int left, int top, COLORREF inner, COLORREF light, COLORREF shadow)
{
	drawthinedgebg(hdestdc, left, top, TIMEWIDTH, TIMEHEIGHT, inner, light, shadow);
}

void drawinfonums(HDC hdestdc, int left, int top, int num, COLORREF bg, COLORREF dark, COLORREF bright)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, INSWIDTH, INSHEIGHT);
	SelectObject(hdcbuffer, hbmbuffer);
	drawinfonumsNB(hdcbuffer, 0, 0, num, bg, dark, bright);
	BitBlt(hdestdc, left, top, INSWIDTH, INSHEIGHT, hdcbuffer, 0, 0, SRCCOPY);
	DeleteObject(hbmbuffer);
	DeleteDC(hdcbuffer);
}

void drawinfonumsNB(HDC hdestdc, int left, int top, int num, COLORREF bg, COLORREF dark, COLORREF bright)
{
	int a, b, c;
	if (num > -100 && num < 0) {
		num = -num;
		a = -1;
		b = num / 10;
		c = num % 10;
	}
	else if (num >= 0 && num < 1000) {
		a = num / 100;
		b = (num / 10) % 10;
		c = num % 10;
	}
	else {
		a = b = c = -1;
	}
	drawinfonum(hdestdc, left, top, a, bg, dark, bright);
	drawinfonum(hdestdc, left + INFONUM_WIDTH, top, b, bg, dark, bright);
	drawinfonum(hdestdc, left + INFONUM_WIDTH * 2, top, c, bg, dark, bright);
}

void drawinfonum(HDC hdestdc, int left, int top, int num, COLORREF bg, COLORREF dark, COLORREF bright)
{
	HPEN hpen = CreatePen(PS_SOLID, 1, bright);
	SelectObject(hdestdc, hpen);
	draw7sdbg(hdestdc, left, top, bg, dark);

	bool D = num & 0x8, C = num & 0x4, B = num & 0x2, A = num & 0x1;
	if (_xor(D, B) || (!D && !_xor(C, A))) draw7sda(hdestdc, left, top);
	if ((!B && (D || !A)) || (!D && (!C || B && A))) draw7sdb(hdestdc, left, top);
	if (!B || !D && (C || A)) draw7sdc(hdestdc, left, top);
	if (D && !B || !C && B && !A || !(_xor(C, _xor(B, A)))) draw7sdd(hdestdc, left, top);
	if ((!C || B) && !A) draw7sde(hdestdc, left, top);
	if ((D || C || !A) && !B || C && !A) draw7sdf(hdestdc, left, top);
	if (D || _xor(C, B) || B && !A) draw7sdg(hdestdc, left, top);

	DeleteObject(hpen);
}

void drawmucoverbg(HDC hdestdc, int left, int top, COLORREF inner, COLORREF light, COLORREF shadow)
{
	drawthickedgebg(hdestdc, left, top, MAPUNITSIZE, MAPUNITSIZE, inner, shadow, light);
}

void drawmuuncovbg(HDC hdestdc, int left, int top, COLORREF inner, COLORREF edge)
{
	RECT rect = { left + 1,top + 1,left + MAPUNITSIZE,top + MAPUNITSIZE };
	HPEN hpedge = CreatePen(PS_SOLID, 1, edge);
	HBRUSH hbinner = CreateSolidBrush(inner);
	SelectObject(hdestdc, hpedge);
	SelectObject(hdestdc, hbinner);

	Rectangle(hdestdc, left, top, rect.right, rect.bottom);
	FillRect(hdestdc, &rect, hbinner);

	DeleteObject(hpedge);
	DeleteObject(hbinner);
}

void drawmuflag(HDC hdestdc, int left, int top, COLORREF flag, COLORREF base, COLORREF bginner, COLORREF bglight, COLORREF bgshadow)
{
	drawmucoverbg(hdestdc, left, top, bginner, bglight, bgshadow);
	drawmuitemflag(hdestdc, left, top, flag, base);
}

void drawmumark(HDC hdestdc, int left, int top, bool down, COLORREF mark, COLORREF bginner, COLORREF bglight, COLORREF bgshadow, COLORREF bgdowninner, COLORREF bgdownedge)
{
	if (down) drawmuuncovbg(hdestdc, left, top, bgdowninner, bgdownedge);
	else drawmucoverbg(hdestdc, left, top, bginner, bglight, bgshadow);
	drawmuitemmark(hdestdc, left, top, down, mark);
}

void drawmumine(HDC hdestdc, int left, int top, bool bomb, COLORREF mine, COLORREF light, COLORREF bginner, COLORREF bgbombinner, COLORREF bgedge)
{
	if (bomb) drawmuuncovbg(hdestdc, left, top, bgbombinner, bgedge);
	else drawmuuncovbg(hdestdc, left, top, bginner, bgedge);
	drawmuitemmine(hdestdc, left, top, mine, light);
}

void drawmuwrong(HDC hdestdc, int left, int top, COLORREF mine, COLORREF light, COLORREF cross, COLORREF bginner, COLORREF bgedge)
{
	drawmuuncovbg(hdestdc, left, top, bginner, bgedge);
	drawmuitemmine(hdestdc, left, top, mine, light);
	drawmuitemcross(hdestdc, left, top, cross);
}

void drawmunum(HDC hdestdc, int left, int top, int num, COLORREF number, COLORREF bginner, COLORREF bgedge)
{
	drawmuuncovbg(hdestdc, left, top, bginner, bgedge);
	if (number == COLOR_MUNUMDEF) {
		switch (num) {
		case 0:	break;
		case 1:	drawmuitemnum1(hdestdc, left, top); break;
		case 2:	drawmuitemnum2(hdestdc, left, top); break;
		case 3:	drawmuitemnum3(hdestdc, left, top); break;
		case 4:	drawmuitemnum4(hdestdc, left, top); break;
		case 5:	drawmuitemnum5(hdestdc, left, top); break;
		case 6:	drawmuitemnum6(hdestdc, left, top); break;
		case 7:	drawmuitemnum7(hdestdc, left, top); break;
		case 8:	drawmuitemnum8(hdestdc, left, top); break;
		default:break;
		}
	}
	else {
		switch (num) {
		case 0:	break;
		case 1:	drawmuitemnum1(hdestdc, left, top, number); break;
		case 2:	drawmuitemnum2(hdestdc, left, top, number); break;
		case 3:	drawmuitemnum3(hdestdc, left, top, number); break;
		case 4:	drawmuitemnum4(hdestdc, left, top, number); break;
		case 5:	drawmuitemnum5(hdestdc, left, top, number); break;
		case 6:	drawmuitemnum6(hdestdc, left, top, number); break;
		case 7:	drawmuitemnum7(hdestdc, left, top, number); break;
		case 8:	drawmuitemnum8(hdestdc, left, top, number); break;
		default:break;
		}
	}
}

void drawmapunit(HDC hdestdc, int left, int top, int index)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, MAPUNITSIZE, MAPUNITSIZE);
	SelectObject(hdcbuffer, hbmbuffer);
	drawmapunitNB(hdcbuffer, 0, 0, index);
	BitBlt(hdestdc, left, top, MAPUNITSIZE, MAPUNITSIZE, hdcbuffer, 0, 0, SRCCOPY);
	DeleteObject(hdcbuffer);
	DeleteObject(hbmbuffer);
}

void drawmapunit(HDC hdestdc, int left, int top, int x, int y)
{
	drawmapunit(hdestdc, left, top, xytoindex(x, y));
}

void drawmapunitNB(HDC hdestdc, int left, int top, int index)
{
	switch (GETMUSTATE(Game.map[index])) {
	case MUS_COVER:
		drawmucoverbg(hdestdc, left, top);
		break;
	case MUS_FLAG:
		drawmuflag(hdestdc, left, top);
		break;
	case MUS_MARK:
		drawmumark(hdestdc, left, top, false);
		break;
	case MUS_UNCOV:
		if (MUISMINE(Game.map[index])) drawmumine(hdestdc, left, top, false);
		else drawmunum(hdestdc, left, top, GETMUMINES(Game.map[index]));
		break;
	case MUS_BOMB:
		drawmumine(hdestdc, left, top, true);
		break;
	case MUS_WRONG:
		drawmuwrong(hdestdc, left, top);
		break;
	default:
		drawmucoverbg(hdestdc, left, top);
		break;
	}
}

void drawmapunitNB(HDC hdestdc, int left, int top, int x, int y)
{
	drawmapunitNB(hdestdc, left, top, xytoindex(x, y));
}
