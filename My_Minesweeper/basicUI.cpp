#include "basicUI.h"


/* this array stores InfoNum backgroud
 * 0 represent background color, non-0 represent dark color
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



/* these static functions are used only in this file */

//draw 2 pixel edge concave background,
//exchange 'light' and 'shadow' to draw a convex backgroung
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

//draw 2 pixel edge concave background with 2 layers of color,
//exchange colors to draw a convex backgroung
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
{
	HBRUSH hbinner, hblight, hbll, hbshadow, hbsh;
	hbinner = CreateSolidBrush(inner);
	hblight = CreateSolidBrush(light);
	hbll = CreateSolidBrush(lightlow);
	hbshadow = CreateSolidBrush(shadow);
	hbsh = CreateSolidBrush(shadowhigh);

	RECT rect = { left,top,left + width - 1,top + height - 1 };
	FillRect(hdestdc, &rect, hbshadow);
	rect = { left + 1,top + 1,left + width,top + height };
	FillRect(hdestdc, &rect, hblight);
	rect = { left + 1,top + 1,left + width - 2,top + height - 2 };
	FillRect(hdestdc, &rect, hbsh);
	rect = { left + 2,top + 2,left + width - 1,top + height - 1 };
	FillRect(hdestdc, &rect, hbll);
	rect = { left + 2,top + 2,left + width - 2,top + height - 2 };
	FillRect(hdestdc, &rect, hbinner);

	SetPixel(hdestdc, left, rect.bottom + 1, inner);
	SetPixel(hdestdc, left + 1, rect.bottom, inner);
	SetPixel(hdestdc, rect.right, top + 1, inner);
	SetPixel(hdestdc, rect.right + 1, top, inner);

	DeleteObject(hbinner);
	DeleteObject(hblight);
	DeleteObject(hbll);
	DeleteObject(hbshadow);
	DeleteObject(hbsh);
}

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
//no DC-Buffer
static inline void draw7sdbg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ COLORREF bg = COLOR_INBG,
	_In_ COLORREF dark = COLOR_INDK
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
//no DC-Buffer
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
//logical xor
static inline bool _xor(const bool A, const bool B)
{
	return (!A && B || A && !B);
}


//no DC-Buffer
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
	_In_ bool clicked,
	_In_ COLORREF mark = COLOR_MUMARK
)
{
	HBRUSH hbmark = CreateSolidBrush(mark);
	RECT rect = { left + clicked + 9,top + clicked + 4,left + clicked + 15,top + clicked + 5 };
	FillRect(hdestdc, &rect, hbmark);
	rect = { left + clicked + 7,top + clicked + 5,left + clicked + 10,top + clicked + 9 };
	FillRect(hdestdc, &rect, hbmark);
	rect = { left + clicked + 14,top + clicked + 5,left + clicked + 17,top + clicked + 10 };
	FillRect(hdestdc, &rect, hbmark);
	rect = { left + clicked + 12,top + clicked + 10,left + clicked + 15,top + clicked + 12 };
	FillRect(hdestdc, &rect, hbmark);
	rect = { left + clicked + 10,top + clicked + 12,left + clicked + 14,top + clicked + 15 };
	FillRect(hdestdc, &rect, hbmark);
	rect = { left + clicked + 10,top + clicked + 17,left + clicked + 14,top + clicked + 20 };
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


/* following functions can be called by external functions */

void drawClientBg(HDC hdestdc, int left, int top, COLORREF def)
{
	RECT rect = { left,top,left + CLIENT_WIDTH,top + CLIENT_HEIGHT };
	HBRUSH hbrush = CreateSolidBrush(def);
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}

void drawInfoBg(HDC hdestdc, int left, int top, COLORREF inner, COLORREF light, COLORREF shadow)
{
	drawthickedgebg(hdestdc, left, top, INFO_WIDTH, INFO_HEIGHT, inner, light, shadow);
}

void drawMapAreaBg(HDC hdestdc, int left, int top, COLORREF inner, COLORREF light, COLORREF shadow)
{
	drawthickedgebg(hdestdc, left, top, MAPAREA_WIDTH, MAPAREA_HEIGHT, inner, light, shadow);
}

void drawMineBg(HDC hdestdc, int left, int top, COLORREF inner, COLORREF light, COLORREF shadow)
{
	drawthinedgebg(hdestdc, left, top, MINE_WIDTH, MINE_HEIGHT, inner, light, shadow);
}

void drawTimeBg(HDC hdestdc, int left, int top, COLORREF inner, COLORREF light, COLORREF shadow)
{
	drawthinedgebg(hdestdc, left, top, TIME_WIDTH, TIME_HEIGHT, inner, light, shadow);
}

void drawIN(HDC hdestdc, int left, int top, int num, COLORREF bg, COLORREF dark, COLORREF bright)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, IN_WIDTH, IN_HEIGHT);
	SelectObject(hdcbuffer, hbmbuffer);
	drawINNB(hdcbuffer, 0, 0, num, bg, dark, bright);
	BitBlt(hdestdc, left, top, IN_WIDTH, IN_HEIGHT, hdcbuffer, 0, 0, SRCCOPY);
	DeleteObject(hbmbuffer);
	DeleteDC(hdcbuffer);
}

void drawINNB(HDC hdestdc, int left, int top, int num, COLORREF bg, COLORREF dark, COLORREF bright)
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
	drawInfoNum(hdestdc, left, top, a, bg, dark, bright);
	drawInfoNum(hdestdc, left + INFONUM_WIDTH, top, b, bg, dark, bright);
	drawInfoNum(hdestdc, left + INFONUM_WIDTH * 2, top, c, bg, dark, bright);
}

void drawInfoNum(HDC hdestdc, int left, int top, int num, COLORREF bg, COLORREF dark, COLORREF bright)
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

void drawResetButtonBg(HDC hdestdc, int left, int top, COLORREF inner, COLORREF light, COLORREF lightlow, COLORREF shadow, COLORREF shadowhigh)
{
	drawthickedgebg(hdestdc, left, top, RB_SIZE, RB_SIZE, inner, shadow, shadowhigh, light, lightlow);
}

void drawBmpOnResetButton(HDC hdestdc, int left, int top, HBITMAP hbm, bool clicked)
{
	if (hbm == NULL) return;
	HDC hdcbitmap = CreateCompatibleDC(hdestdc);
	SelectObject(hdcbitmap, hbm);
	BitBlt(hdestdc, left + clicked, top + clicked, BMP_SIZE - clicked, BMP_SIZE - clicked, hdcbitmap, 0, 0, SRCCOPY);
	DeleteDC(hdcbitmap);
}

void drawResetButton(HDC hdestdc, int left, int top, HBITMAP hbm, bool clicked, COLORREF inner, COLORREF light, COLORREF lightlow, COLORREF shadow, COLORREF shadowhigh)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, RB_SIZE, RB_SIZE);
	SelectObject(hdcbuffer, hbmbuffer);
	drawResetButtonNB(hdcbuffer, 0, 0, hbm, clicked, inner, light, lightlow, shadow, shadowhigh);
	BitBlt(hdestdc, left, top, RB_SIZE, RB_SIZE, hdcbuffer, 0, 0, SRCCOPY);
	DeleteObject(hbmbuffer);
	DeleteDC(hdcbuffer);
}

void drawResetButtonNB(HDC hdestdc, int left, int top, HBITMAP hbm, bool clicked, COLORREF inner, COLORREF light, COLORREF lightlow, COLORREF shadow, COLORREF shadowhigh)
{
	if (clicked) drawResetButtonBg(hdestdc, left, top, inner, shadow, shadowhigh, light, lightlow);
	else drawResetButtonBg(hdestdc, left, top, inner, light, lightlow, shadow, shadowhigh);
	drawBmpOnResetButton(hdestdc, left + 2, top + 2, hbm, clicked);
}

void drawMUCoverBg(HDC hdestdc, int left, int top, COLORREF inner, COLORREF light, COLORREF shadow)
{
	drawthickedgebg(hdestdc, left, top, MU_SIZE, MU_SIZE, inner, shadow, light);
}

void drawMUUncovBg(HDC hdestdc, int left, int top, COLORREF inner, COLORREF edge)
{
	RECT rect = { left + 1,top + 1,left + MU_SIZE,top + MU_SIZE };
	HPEN hpedge = CreatePen(PS_SOLID, 1, edge);
	HBRUSH hbinner = CreateSolidBrush(inner);
	SelectObject(hdestdc, hpedge);
	SelectObject(hdestdc, hbinner);

	Rectangle(hdestdc, left, top, rect.right, rect.bottom);
	FillRect(hdestdc, &rect, hbinner);

	DeleteObject(hpedge);
	DeleteObject(hbinner);
}

void drawMUFlag(HDC hdestdc, int left, int top, COLORREF flag, COLORREF base, COLORREF bginner, COLORREF bglight, COLORREF bgshadow)
{
	drawMUCoverBg(hdestdc, left, top, bginner, bglight, bgshadow);
	drawmuitemflag(hdestdc, left, top, flag, base);
}

void drawMUMark(HDC hdestdc, int left, int top, bool clicked, COLORREF mark, COLORREF bginner, COLORREF bglight, COLORREF bgshadow, COLORREF bgclickedinner, COLORREF bgclickededge)
{
	if (clicked) drawMUUncovBg(hdestdc, left, top, bgclickedinner, bgclickededge);
	else drawMUCoverBg(hdestdc, left, top, bginner, bglight, bgshadow);
	drawmuitemmark(hdestdc, left, top, clicked, mark);
}

void drawMUMine(HDC hdestdc, int left, int top, bool bomb, COLORREF mine, COLORREF light, COLORREF bginner, COLORREF bgbombinner, COLORREF bgedge)
{
	if (bomb) drawMUUncovBg(hdestdc, left, top, bgbombinner, bgedge);
	else drawMUUncovBg(hdestdc, left, top, bginner, bgedge);
	drawmuitemmine(hdestdc, left, top, mine, light);
}

void drawMUWrong(HDC hdestdc, int left, int top, COLORREF mine, COLORREF light, COLORREF cross, COLORREF bginner, COLORREF bgedge)
{
	drawMUUncovBg(hdestdc, left, top, bginner, bgedge);
	drawmuitemmine(hdestdc, left, top, mine, light);
	drawmuitemcross(hdestdc, left, top, cross);
}

void drawMUNum(HDC hdestdc, int left, int top, int num, COLORREF numcolor, COLORREF bginner, COLORREF bgedge)
{
	drawMUUncovBg(hdestdc, left, top, bginner, bgedge);
	switch (num) {
	case 0:	break;
	case 1:	drawmuitemnum1(hdestdc, left, top, ((numcolor == COLOR_MUNUMDEF) ? COLOR_MUNUM1 : numcolor)); break;
	case 2:	drawmuitemnum2(hdestdc, left, top, ((numcolor == COLOR_MUNUMDEF) ? COLOR_MUNUM2 : numcolor)); break;
	case 3:	drawmuitemnum3(hdestdc, left, top, ((numcolor == COLOR_MUNUMDEF) ? COLOR_MUNUM3 : numcolor)); break;
	case 4:	drawmuitemnum4(hdestdc, left, top, ((numcolor == COLOR_MUNUMDEF) ? COLOR_MUNUM4 : numcolor)); break;
	case 5:	drawmuitemnum5(hdestdc, left, top, ((numcolor == COLOR_MUNUMDEF) ? COLOR_MUNUM5 : numcolor)); break;
	case 6:	drawmuitemnum6(hdestdc, left, top, ((numcolor == COLOR_MUNUMDEF) ? COLOR_MUNUM6 : numcolor)); break;
	case 7:	drawmuitemnum7(hdestdc, left, top, ((numcolor == COLOR_MUNUMDEF) ? COLOR_MUNUM7 : numcolor)); break;
	case 8:	drawmuitemnum8(hdestdc, left, top, ((numcolor == COLOR_MUNUMDEF) ? COLOR_MUNUM8 : numcolor)); break;
	default:break;
	}
}
