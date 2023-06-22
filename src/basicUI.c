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
	rect = (RECT){ left + 2,top + 2,left + width,top + height };
	FillRect(hdestdc, &rect, hblight);
	rect = (RECT){ left + 2,top + 2,left + width - 2,top + height - 2 };
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
static void drawdoubleedgebg(
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
	rect = (RECT){ left + 1,top + 1,left + width,top + height };
	FillRect(hdestdc, &rect, hblight);
	rect = (RECT){ left + 1,top + 1,left + width - 2,top + height - 2 };
	FillRect(hdestdc, &rect, hbsh);
	rect = (RECT){ left + 2,top + 2,left + width - 1,top + height - 1 };
	FillRect(hdestdc, &rect, hbll);
	rect = (RECT){ left + 2,top + 2,left + width - 2,top + height - 2 };
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
	rect = (RECT){ left + 1,top + 1,left + width,top + height };
	FillRect(hdestdc, &rect, hblight);
	rect = (RECT){ left + 1,top + 1,left + width - 1,top + height - 1 };
	FillRect(hdestdc, &rect, hbinner);

	SetPixel(hdestdc, left, rect.bottom, inner);
	SetPixel(hdestdc, rect.right, top, inner);

	DeleteObject(hbinner);
	DeleteObject(hblight);
	DeleteObject(hbshadow);
}

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
)
{
	HPEN hpedge = CreatePen(PS_SOLID, 1, edge);
	HBRUSH hbinner = CreateSolidBrush(inner);
	SelectObject(hdestdc, hpedge);
	SelectObject(hdestdc, hbinner);

	RECT rect = { left + 1,top + 1,left + width,top + height };
	Rectangle(hdestdc, left, top, rect.right, rect.bottom);
	FillRect(hdestdc, &rect, hbinner);

	DeleteObject(hpedge);
	DeleteObject(hbinner);
}


//draw 7sd background
//no DC-Buffer
static inline void draw7sdbg(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
)
{
	for (int i = 0; i < INFONUM_WIDTH; i++) {
		for (int j = 0; j < INFONUM_HEIGHT; j++) {
			if (InfoNumBG[i][j]) SetPixel(hdestdc, left + i, top + j, COLOR_INDK);
			else SetPixel(hdestdc, left + i, top + j, COLOR_INBG);
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
	_In_ int top
)
{
	HBRUSH hbmine, hblight;
	hbmine = CreateSolidBrush(COLOR_MUMINE);
	hblight = CreateSolidBrush(COLOR_MUMINEL);

	RECT rect = { left + 6,top + 9,left + 19,top + 16 };
	FillRect(hdestdc, &rect, hbmine);
	rect = (RECT){ left + 7,top + 7,left + 18,top + 18 };
	FillRect(hdestdc, &rect, hbmine);
	rect = (RECT){ left + 9,top + 6,left + 16,top + 19 };
	FillRect(hdestdc, &rect, hbmine);

	rect = (RECT){ left + 3,top + 12,left + 22,top + 13 };
	FillRect(hdestdc, &rect, hbmine);
	rect = (RECT){ left + 12,top + 3,left + 13,top + 22 };
	FillRect(hdestdc, &rect, hbmine);

	rect = (RECT){ left + 9,top + 9,left + 12,top + 12 };
	FillRect(hdestdc, &rect, hblight);

	SetPixel(hdestdc, left + 6, top + 6, COLOR_MUMINE);
	SetPixel(hdestdc, left + 6, top + 18, COLOR_MUMINE);
	SetPixel(hdestdc, left + 18, top + 6, COLOR_MUMINE);
	SetPixel(hdestdc, left + 18, top + 18, COLOR_MUMINE);

	DeleteObject(hbmine);
	DeleteObject(hblight);
}
//draw a question mark icon
static void drawmuitemmark(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top,
	_In_ bool clicked
)
{
	HBRUSH hbmark = CreateSolidBrush(COLOR_MUMARK);
	RECT rect = { left + clicked + 9,top + clicked + 4,left + clicked + 15,top + clicked + 5 };
	FillRect(hdestdc, &rect, hbmark);
	rect = (RECT){ left + clicked + 7,top + clicked + 5,left + clicked + 10,top + clicked + 9 };
	FillRect(hdestdc, &rect, hbmark);
	rect = (RECT){ left + clicked + 14,top + clicked + 5,left + clicked + 17,top + clicked + 10 };
	FillRect(hdestdc, &rect, hbmark);
	rect = (RECT){ left + clicked + 12,top + clicked + 10,left + clicked + 15,top + clicked + 12 };
	FillRect(hdestdc, &rect, hbmark);
	rect = (RECT){ left + clicked + 10,top + clicked + 12,left + clicked + 14,top + clicked + 15 };
	FillRect(hdestdc, &rect, hbmark);
	rect = (RECT){ left + clicked + 10,top + clicked + 17,left + clicked + 14,top + clicked + 20 };
	FillRect(hdestdc, &rect, hbmark);
	DeleteObject(hbmark);
}
//draw a flag icon
static void drawmuitemflag(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
)
{
	HBRUSH hbflag, hbbase;
	hbflag = CreateSolidBrush(COLOR_MUFLAGF);
	hbbase = CreateSolidBrush(COLOR_MUFLAGB);

	RECT rect = { left + 6,top + 7,left + 7,top + 9 };
	FillRect(hdestdc, &rect, hbflag);
	rect = (RECT){ left + 7,top + 6,left + 10,top + 10 };
	FillRect(hdestdc, &rect, hbflag);
	rect = (RECT){ left + 10,top + 4,left + 13,top + 12 };
	FillRect(hdestdc, &rect, hbflag);

	rect = (RECT){ left + 12,top + 12,left + 13,top + 15 };
	FillRect(hdestdc, &rect, hbbase);
	rect = (RECT){ left + 9,top + 15,left + 15,top + 16 };
	FillRect(hdestdc, &rect, hbbase);
	rect = (RECT){ left + 6,top + 16,left + 18,top + 19 };
	FillRect(hdestdc, &rect, hbbase);

	DeleteObject(hbflag);
	DeleteObject(hbbase);
}
//draw a cross mark icon
static void drawmuitemcross(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
)
{
	HPEN hpcross = CreatePen(PS_SOLID, 2, COLOR_MUCROSS);
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
	_In_ int top
)
{
	HBRUSH hbrush = CreateSolidBrush(COLOR_MUNUM1);
	RECT rect = { left + 11,top + 5,left + 15,top + 6 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 9,top + 6,left + 15,top + 7 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 7,top + 7,left + 15,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 5,top + 8,left + 15,top + 10 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 10,top + 10,left + 15,top + 17 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 5,top + 17,left + 20,top + 20 };
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}
//draw a number 2 icon
static void drawmuitemnum2(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
)
{
	HBRUSH hbrush = CreateSolidBrush(COLOR_MUNUM2);

	RECT rect = { left + 5,top + 7,left + 9,top + 10 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 7,top + 5,left + 18,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 16,top + 7,left + 20,top + 11 };
	FillRect(hdestdc, &rect, hbrush);

	rect = (RECT){ left + 15,top + 10,left + 19,top + 13 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 13,top + 11,left + 17,top + 14 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 11,top + 12,left + 15,top + 15 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 9,top + 13,left + 13,top + 16 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 7,top + 14,left + 11,top + 17 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 5,top + 15,left + 9,top + 18 };
	FillRect(hdestdc, &rect, hbrush);

	rect = (RECT){ left + 5,top + 17,left + 20,top + 20 };
	FillRect(hdestdc, &rect, hbrush);

	SetPixel(hdestdc, left + 6, top + 6, COLOR_MUNUM2);
	SetPixel(hdestdc, left + 18, top + 6, COLOR_MUNUM2);

	DeleteObject(hbrush);
}
//draw a number 3 icon
static void drawmuitemnum3(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
)
{
	HBRUSH hbrush = CreateSolidBrush(COLOR_MUNUM3);
	RECT rect = { left + 5,top + 5,left + 19,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 16,top + 7,left + 20,top + 11 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 10,top + 11,left + 19,top + 14 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 16,top + 14,left + 20,top + 19 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 5,top + 17,left + 19,top + 20 };
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}
//draw a number 4 icon
static void drawmuitemnum4(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
)
{
	HBRUSH hbrush = CreateSolidBrush(COLOR_MUNUM4);
	RECT rect = { left + 8,top + 5,left + 13,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 7,top + 8,left + 11,top + 11 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 5,top + 11,left + 20,top + 14 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 14,top + 5,left + 19,top + 20 };
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}
//draw a number 5 icon
static void drawmuitemnum5(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
)
{
	HBRUSH hbrush = CreateSolidBrush(COLOR_MUNUM5);
	RECT rect = { left + 5,top + 5,left + 20,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 5,top + 8,left + 10,top + 11 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 5,top + 11,left + 19,top + 14 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 16,top + 13,left + 20,top + 19 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 5,top + 17,left + 19,top + 20 };
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}
//draw a number 6 icon
static void drawmuitemnum6(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
)
{
	HBRUSH hbrush = CreateSolidBrush(COLOR_MUNUM6);
	RECT rect = { left + 7,top + 5,left + 19,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 5,top + 7,left + 10,top + 19 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 10,top + 11,left + 19,top + 14 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 16,top + 13,left + 20,top + 19 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 7,top + 17,left + 19,top + 20 };
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}
//draw a number 7 icon
static void drawmuitemnum7(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
)
{
	HBRUSH hbrush = CreateSolidBrush(COLOR_MUNUM7);
	RECT rect = { left + 5,top + 5,left + 20,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 16,top + 8,left + 20,top + 11 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 14,top + 11,left + 19,top + 14 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 13,top + 14,left + 17,top + 17 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 11,top + 17,left + 16,top + 20 };
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}
//draw a number 8 icon
static void drawmuitemnum8(
	_In_ HDC hdestdc,
	_In_ int left,
	_In_ int top
)
{
	HBRUSH hbrush = CreateSolidBrush(COLOR_MUNUM8);
	RECT rect = { left + 7,top + 5,left + 19,top + 8 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 5,top + 7,left + 10,top + 11 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 16,top + 7,left + 20,top + 11 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 7,top + 11,left + 19,top + 14 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 5,top + 14,left + 10,top + 19 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 16,top + 14,left + 20,top + 19 };
	FillRect(hdestdc, &rect, hbrush);
	rect = (RECT){ left + 7,top + 17,left + 19,top + 20 };
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}


/* following functions can be called by external functions */

void drawClientBg(HDC hdestdc, int left, int top)
{
	RECT rect = { left,top,left + CLIENT_WIDTH,top + CLIENT_HEIGHT };
	HBRUSH hbrush = CreateSolidBrush(COLOR_CLIENT);
	FillRect(hdestdc, &rect, hbrush);
	DeleteObject(hbrush);
}

void drawInfoBg(HDC hdestdc, int left, int top)
{
	drawthickedgebg(hdestdc, left, top, INFO_WIDTH, INFO_HEIGHT, COLOR_INFO, COLOR_INFOL, COLOR_INFOS);
}

void drawMapAreaBg(HDC hdestdc, int left, int top)
{
	drawthickedgebg(hdestdc, left, top, MAPAREA_WIDTH, MAPAREA_HEIGHT, COLOR_MAPAREA, COLOR_MAPAREAL, COLOR_MAPAREAS);
}

void drawMineBg(HDC hdestdc, int left, int top)
{
	drawthinedgebg(hdestdc, left, top, MINE_WIDTH, MINE_HEIGHT, COLOR_MINE, COLOR_MINEL, COLOR_MINES);
}

void drawTimeBg(HDC hdestdc, int left, int top)
{
	drawthinedgebg(hdestdc, left, top, TIME_WIDTH, TIME_HEIGHT, COLOR_TIME, COLOR_TIMEL, COLOR_TIMES);
}

void drawIN(HDC hdestdc, int left, int top, int num)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, IN_WIDTH, IN_HEIGHT);
	SelectObject(hdcbuffer, hbmbuffer);
	drawINNB(hdcbuffer, 0, 0, num);
	BitBlt(hdestdc, left, top, IN_WIDTH, IN_HEIGHT, hdcbuffer, 0, 0, SRCCOPY);
	DeleteObject(hbmbuffer);
	DeleteDC(hdcbuffer);
}

void drawINNB(HDC hdestdc, int left, int top, int num)
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
	drawInfoNum(hdestdc, left, top, a);
	drawInfoNum(hdestdc, left + INFONUM_WIDTH, top, b);
	drawInfoNum(hdestdc, left + INFONUM_WIDTH * 2, top, c);
}

void drawInfoNum(HDC hdestdc, int left, int top, int num)
{
	HPEN hpen = CreatePen(PS_SOLID, 1, COLOR_INBT);
	SelectObject(hdestdc, hpen);
	draw7sdbg(hdestdc, left, top);

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

void drawResetButtonBg(HDC hdestdc, int left, int top, bool clicked)
{
	if (clicked) drawdoubleedgebg(hdestdc, left, top, RB_SIZE, RB_SIZE, COLOR_RB, COLOR_RBL, COLOR_RBLL, COLOR_RBS, COLOR_RBSH);
	else drawdoubleedgebg(hdestdc, left, top, RB_SIZE, RB_SIZE, COLOR_RB, COLOR_RBS, COLOR_RBSH, COLOR_RBL, COLOR_RBLL);
}

void drawBmpOnResetButton(HDC hdestdc, int left, int top, HBITMAP hbm, bool clicked)
{
	if (hbm == NULL) return;
	HDC hdcbitmap = CreateCompatibleDC(hdestdc);
	SelectObject(hdcbitmap, hbm);
	BitBlt(hdestdc, left + clicked, top + clicked, BMP_SIZE - clicked, BMP_SIZE - clicked, hdcbitmap, 0, 0, SRCCOPY);
	DeleteDC(hdcbitmap);
}

void drawResetButton(HDC hdestdc, int left, int top, HBITMAP hbm, bool clicked)
{
	HDC hdcbuffer = CreateCompatibleDC(hdestdc);
	HBITMAP hbmbuffer = CreateCompatibleBitmap(hdestdc, RB_SIZE, RB_SIZE);
	SelectObject(hdcbuffer, hbmbuffer);
	drawResetButtonNB(hdcbuffer, 0, 0, hbm, clicked);
	BitBlt(hdestdc, left, top, RB_SIZE, RB_SIZE, hdcbuffer, 0, 0, SRCCOPY);
	DeleteObject(hbmbuffer);
	DeleteDC(hdcbuffer);
}

void drawResetButtonNB(HDC hdestdc, int left, int top, HBITMAP hbm, bool clicked)
{
	drawResetButtonBg(hdestdc, left, top, clicked);
	drawBmpOnResetButton(hdestdc, left + 2, top + 2, hbm, clicked);
}

void drawMUCoverBg(HDC hdestdc, int left, int top)
{
	drawthickedgebg(hdestdc, left, top, MU_SIZE, MU_SIZE, COLOR_MUCOVER, COLOR_MUCOVERS, COLOR_MUCOVERL);
}

void drawMUUncovBg(HDC hdestdc, int left, int top)
{
	drawhalfedgebg(hdestdc, left, top, MU_SIZE, MU_SIZE, COLOR_MUUNCOV, COLOR_MUUNCOVE);
}

void drawMUFlag(HDC hdestdc, int left, int top)
{
	drawMUCoverBg(hdestdc, left, top);
	drawmuitemflag(hdestdc, left, top);
}

void drawMUMark(HDC hdestdc, int left, int top, bool clicked)
{
	if (clicked) drawMUUncovBg(hdestdc, left, top);
	else drawMUCoverBg(hdestdc, left, top);
	drawmuitemmark(hdestdc, left, top, clicked);
}

void drawMUMine(HDC hdestdc, int left, int top, bool bomb)
{
	if (bomb) drawhalfedgebg(hdestdc, left, top, MU_SIZE, MU_SIZE, COLOR_MUUNCOVB, COLOR_MUUNCOVE);
	else drawhalfedgebg(hdestdc, left, top, MU_SIZE, MU_SIZE, COLOR_MUUNCOV, COLOR_MUUNCOVE);
	drawmuitemmine(hdestdc, left, top);
}

void drawMUWrong(HDC hdestdc, int left, int top)
{
	drawMUUncovBg(hdestdc, left, top);
	drawmuitemmine(hdestdc, left, top);
	drawmuitemcross(hdestdc, left, top);
}

void drawMUNum(HDC hdestdc, int left, int top, int num)
{
	drawMUUncovBg(hdestdc, left, top);
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
