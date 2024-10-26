/*****************************************************************************\
 *  My Minesweepper -- a classic minesweeper game
 *  Copyright (C) 2020-2024 Gee Wang
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
 * basicUI.c
 *****************************************************************************
 * This file contains the basic drawing functions.
 * Drawing functions use Win32 GDI and draw directly on Device Context (DC)
 * without creating a DC buffer.
 *
 * NOTE: Almost all functions do NOT check arguments, use with care.
\*****************************************************************************/


#include "stdafx.h"
#include "basicUI.h"


/* This array stores InfoNum backgroud,
 * 0 represent INCOL_BLANK, non-0 represent INCOL_SEGOFF.
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


/* These functions draw a concave or convex like region with different types
 * of edges.
 */

//Draw a solid color background.
static void drawsolidcolorbg(
    _In_ HDC hdstdc,
    _In_ int left,
    _In_ int top,
    _In_ int width,
    _In_ int height,
    _In_ COLORREF surface
)
{
    RECT rect = { left, top, left + width, top + height };
    SetDCBrushColor(hdstdc, surface);
    FillRect(hdstdc, &rect, GetStockObject(DC_BRUSH));
}

//Draw 1 pixel half edge 2D like background,
//draw left edge and top edge only.
static void drawhalfedgebg(
    _In_ HDC hdstdc,
    _In_ int left,
    _In_ int top,
    _In_ int width,
    _In_ int height,
    _In_ COLORREF inner,
    _In_ COLORREF edge
)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdstdc, edge);
    rect = (RECT){ left, top, left + width, top + height };
    FillRect(hdstdc, &rect, hdcbrush);

    SetDCBrushColor(hdstdc, inner);
    rect = (RECT){ left + 1, top + 1, left + width, top + height };
    FillRect(hdstdc, &rect, hdcbrush);
}

//Draw 1 pixel edge concave background,
//exchange 'bright' and 'dark' to draw a convex background.
static void drawfulledgebg(
    _In_ HDC hdstdc,
    _In_ int left,
    _In_ int top,
    _In_ int width,
    _In_ int height,
    _In_ COLORREF inner,
    _In_ COLORREF bright,
    _In_ COLORREF dark
)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdstdc, dark);
    rect = (RECT){ left, top, left + width - 1, top + height - 1 };
    FillRect(hdstdc, &rect, hdcbrush);
    SetDCBrushColor(hdstdc, bright);
    rect = (RECT){ left + 1, top + 1, left + width, top + height };
    FillRect(hdstdc, &rect, hdcbrush);

    SetDCBrushColor(hdstdc, inner);
    rect = (RECT){ left + 1, top + 1, left + width - 1, top + height - 1 };
    FillRect(hdstdc, &rect, hdcbrush);

    SetPixelV(hdstdc, left + width - 1, top, inner);
    SetPixelV(hdstdc, left, top + height - 1, inner);
}

//Draw 2 pixel edge concave background,
//exchange 'bright' and 'dark' to draw a convex background.
static void drawthickedgebg(
    _In_ HDC hdstdc,
    _In_ int left,
    _In_ int top,
    _In_ int width,
    _In_ int height,
    _In_ COLORREF inner,
    _In_ COLORREF bright,
    _In_ COLORREF dark
)
{
    drawfulledgebg(hdstdc, left, top, width, height, inner, bright, dark);
    drawfulledgebg(hdstdc, left + 1, top + 1, width - 2, height - 2, inner, bright, dark);
}

//Draw 2 pixel edge concave background with 2 layers of color,
//exchange 'bright*' and 'dark*' to draw a convex background.
static void drawdualedgebg(
    _In_ HDC hdstdc,
    _In_ int left,
    _In_ int top,
    _In_ int width,
    _In_ int height,
    _In_ COLORREF inner,
    _In_ COLORREF bright1,
    _In_ COLORREF bright2,
    _In_ COLORREF dark1,
    _In_ COLORREF dark2
)
{
    drawfulledgebg(hdstdc, left, top, width, height, inner, bright1, dark1);
    drawfulledgebg(hdstdc, left + 1, top + 1, width - 2, height - 2, inner, bright2, dark2);
}


/* These functions draw a seven-segment like region for InfoNum. */

/*    a
 *  +---+
 * f| g |b
 *  +---+
 * e|   |c
 *  +---+
 *    d
 */
//Draw 7sd background.
static inline void draw7sdbg(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    for (int i = 0; i < INFONUM_WIDTH; i++) {
        for (int j = 0; j < INFONUM_HEIGHT; j++) {
            COLORREF color = (InfoNumBG[i][j]) ? INCOL_SEGOFF : INCOL_BLANK;
            SetPixelV(hdstdc, left + i, top + j, color);
        }
    }
}

//Draw specific segment on 7sd, MUST select pen before calling.
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


/* These functions draw different contents inside a MapUnit. */

//Draw a mine icon.
static void drawmuitemmine(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdstdc, MUCOL_MINESF);
    rect = (RECT){ left + 6, top + 9, left + 19, top + 16 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 7, left + 18, top + 18 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 9, top + 6, left + 16, top + 19 };
    FillRect(hdstdc, &rect, hdcbrush);

    rect = (RECT){ left + 3, top + 12, left + 22, top + 13 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 12, top + 3, left + 13, top + 22 };
    FillRect(hdstdc, &rect, hdcbrush);

    SetDCBrushColor(hdstdc, MUCOL_MINEHL);
    rect = (RECT){ left + 9, top + 9, left + 12, top + 12 };
    FillRect(hdstdc, &rect, hdcbrush);

    SetPixelV(hdstdc, left + 6, top + 6, MUCOL_MINESF);
    SetPixelV(hdstdc, left + 6, top + 18, MUCOL_MINESF);
    SetPixelV(hdstdc, left + 18, top + 6, MUCOL_MINESF);
    SetPixelV(hdstdc, left + 18, top + 18, MUCOL_MINESF);
}

//Draw a question mark icon.
static void drawmuitemmark(_In_ HDC hdstdc, _In_ int left, _In_ int top, _In_ bool clicked)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdstdc, MUCOL_MARK);
    rect = (RECT){ left + clicked + 9, top + clicked + 4, left + clicked + 15, top + clicked + 5 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + clicked + 7, top + clicked + 5, left + clicked + 10, top + clicked + 9 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + clicked + 14, top + clicked + 5, left + clicked + 17, top + clicked + 10 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + clicked + 12, top + clicked + 10, left + clicked + 15, top + clicked + 12 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + clicked + 10, top + clicked + 12, left + clicked + 14, top + clicked + 15 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + clicked + 10, top + clicked + 17, left + clicked + 14, top + clicked + 20 };
    FillRect(hdstdc, &rect, hdcbrush);
}

//Draw a flag icon.
static void drawmuitemflag(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdstdc, MUCOL_FLAGFLAG);
    rect = (RECT){ left + 6, top + 7, left + 7, top + 9 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 6, left + 10, top + 10 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 10, top + 4, left + 13, top + 12 };
    FillRect(hdstdc, &rect, hdcbrush);

    SetDCBrushColor(hdstdc, MUCOL_FLAGBASE);
    rect = (RECT){ left + 12, top + 12, left + 13, top + 15 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 9, top + 15, left + 15, top + 16 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 6, top + 16, left + 18, top + 19 };
    FillRect(hdstdc, &rect, hdcbrush);
}

//Draw a cross icon.
static void drawmuitemcross(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    HPEN hpcross = CreatePen(PS_SOLID, 2, MUCOL_CROSS);
    HPEN hpold = SelectObject(hdstdc, hpcross);

    MoveToEx(hdstdc, left + 4, top + 4, NULL);
    LineTo(hdstdc, left + 20, top + 20);
    MoveToEx(hdstdc, left + 4, top + 20, NULL);
    LineTo(hdstdc, left + 20, top + 4);

    SelectObject(hdstdc, hpold);
    DeleteObject(hpcross);
}

//Draw a number 1 icon.
static void drawmuitemnum1(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdstdc, MUCOL_NUMBER1);
    rect = (RECT){ left + 11, top + 5, left + 15, top + 6 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 9, top + 6, left + 15, top + 7 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 7, left + 15, top + 8 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 8, left + 15, top + 10 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 10, top + 10, left + 15, top + 17 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 17, left + 20, top + 20 };
    FillRect(hdstdc, &rect, hdcbrush);
}

//Draw a number 2 icon.
static void drawmuitemnum2(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdstdc, MUCOL_NUMBER2);
    rect = (RECT){ left + 5, top + 7, left + 9, top + 10 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 5, left + 18, top + 8 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 7, left + 20, top + 11 };
    FillRect(hdstdc, &rect, hdcbrush);

    rect = (RECT){ left + 15, top + 10, left + 19, top + 13 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 13, top + 11, left + 17, top + 14 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 11, top + 12, left + 15, top + 15 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 9, top + 13, left + 13, top + 16 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 14, left + 11, top + 17 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 15, left + 9, top + 18 };
    FillRect(hdstdc, &rect, hdcbrush);

    rect = (RECT){ left + 5, top + 17, left + 20, top + 20 };
    FillRect(hdstdc, &rect, hdcbrush);

    SetPixelV(hdstdc, left + 6, top + 6, MUCOL_NUMBER2);
    SetPixelV(hdstdc, left + 18, top + 6, MUCOL_NUMBER2);
}

//Draw a number 3 icon.
static void drawmuitemnum3(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdstdc, MUCOL_NUMBER3);
    rect = (RECT){ left + 5, top + 5, left + 19, top + 8 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 7, left + 20, top + 11 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 10, top + 11, left + 19, top + 14 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 14, left + 20, top + 19 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 17, left + 19, top + 20 };
    FillRect(hdstdc, &rect, hdcbrush);
}

//Draw a number 4 icon.
static void drawmuitemnum4(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdstdc, MUCOL_NUMBER4);
    rect = (RECT){ left + 8, top + 5, left + 13, top + 8 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 8, left + 11, top + 11 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 11, left + 20, top + 14 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 14, top + 5, left + 19, top + 20 };
    FillRect(hdstdc, &rect, hdcbrush);
}

//Draw a number 5 icon.
static void drawmuitemnum5(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdstdc, MUCOL_NUMBER5);
    rect = (RECT){ left + 5, top + 5, left + 20, top + 8 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 8, left + 10, top + 11 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 11, left + 19, top + 14 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 13, left + 20, top + 19 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 17, left + 19, top + 20 };
    FillRect(hdstdc, &rect, hdcbrush);
}

//Draw a number 6 icon.
static void drawmuitemnum6(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdstdc, MUCOL_NUMBER6);
    rect = (RECT){ left + 7, top + 5, left + 19, top + 8 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 7, left + 10, top + 19 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 10, top + 11, left + 19, top + 14 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 13, left + 20, top + 19 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 17, left + 19, top + 20 };
    FillRect(hdstdc, &rect, hdcbrush);
}

//Draw a number 7 icon.
static void drawmuitemnum7(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdstdc, MUCOL_NUMBER7);
    rect = (RECT){ left + 5, top + 5, left + 20, top + 8 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 8, left + 20, top + 11 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 14, top + 11, left + 19, top + 14 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 13, top + 14, left + 17, top + 17 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 11, top + 17, left + 16, top + 20 };
    FillRect(hdstdc, &rect, hdcbrush);
}

//Draw a number 8 icon.
static void drawmuitemnum8(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdstdc, MUCOL_NUMBER8);
    rect = (RECT){ left + 7, top + 5, left + 19, top + 8 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 7, left + 10, top + 11 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 7, left + 20, top + 11 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 11, left + 19, top + 14 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 14, left + 10, top + 19 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 14, left + 20, top + 19 };
    FillRect(hdstdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 17, left + 19, top + 20 };
    FillRect(hdstdc, &rect, hdcbrush);
}

//logical xor
static inline bool _xor(const bool A, const bool B)
{
    return (!A && B || A && !B);
}


/* Transform between pixel position and unit position of MapUnit. */

int px2x(int px)
{
    return (px / MUP_SIZE);
}

int py2y(int py)
{
    return (py / MUP_SIZE);
}

int x2px(int x)
{
    return (x * MUP_SIZE);
}

int y2py(int y)
{
    return (y * MUP_SIZE);
}


/* These functions draw backgroung with edge for each SubArea
 * using predefined color and size, check related macros.
 */

void drawDCClientBg(_In_ HDC hdstdc, _In_ int left, _In_ int top, _In_ int n_map_width, _In_ int n_map_height)
{
    drawsolidcolorbg(hdstdc, left, top, CLIENT_WIDTH(n_map_width), CLIENT_HEIGHT(n_map_height), CLTCOL_SURFACE);
}

void drawDCHeadAreaBg(_In_ HDC hdstdc, _In_ int left, _In_ int top, _In_ int n_map_width)
{
    drawthickedgebg(hdstdc, left, top, HEADAREA_WIDTH(n_map_width), HEADAREA_HEIGHT, HACOL_SURFACE, HACOL_BRIGHT, HACOL_DARK);
}

void drawDCMapAreaBg(_In_ HDC hdstdc, _In_ int left, _In_ int top, _In_ int n_map_width, _In_ int n_map_height)
{
    drawthickedgebg(hdstdc, left, top, MAPAREA_WIDTH(n_map_width), MAPAREA_HEIGHT(n_map_height), MACOL_SURFACE, MACOL_BRIGHT, MACOL_DARK);
}

void drawDCInfoBg(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    drawfulledgebg(hdstdc, left, top, INFO_WIDTH, INFO_HEIGHT, ICOL_SURFACE, ICOL_BRIGHT, ICOL_DARK);
}


/* These functions draw specfic numbers inside Info SubArea
 * using predefined color and size, check related macros.
 */

void drawDCInfoNumBg(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    draw7sdbg(hdstdc, left, top);
}

void drawDCInfoNumNumber(_In_ HDC hdstdc, _In_ int left, _In_ int top, _In_ int num)
{
    /* assert INFONUM_MIN == 0 && INFONUM_MAX == 9 */
    bool valid = (num >= INFONUM_MIN && num <= INFONUM_MAX);
    num = min(max(num, INFONUM_MIN), INFONUM_MAX);  //limit possible states
    bool A = num & 0x1, B = num & 0x2, C = num & 0x4, D = num & 0x8;

    HPEN hpold = SelectObject(hdstdc, GetStockObject(DC_PEN));
    SetDCPenColor(hdstdc, INCOL_SEGON);

    if (valid && (B + D + !_xor(A, C))) draw7sda(hdstdc, left, top);
    if (valid && (!C + D + !_xor(A, B))) draw7sdb(hdstdc, left, top);
    if (valid && (A + !B + C + D)) draw7sdc(hdstdc, left, top);
    if (valid && (D + B * !C + !_xor(A, !B * C))) draw7sdd(hdstdc, left, top);
    if (valid && (!A * (B + !C))) draw7sde(hdstdc, left, top);
    if (valid && (D + !A * !B + !A * C + !B * C)) draw7sdf(hdstdc, left, top);
    if (!valid || (D + !A * B + _xor(B, C))) draw7sdg(hdstdc, left, top);

    SelectObject(hdstdc, hpold);
}

void drawDCInfoNum(_In_ HDC hdstdc, _In_ int left, _In_ int top, _In_ int num, _In_ bool hide)
{
    drawDCInfoNumBg(hdstdc, left, top);
    if (!hide) drawDCInfoNumNumber(hdstdc, left, top, num);
}

void drawDCINums(_In_ HDC hdstdc, _In_ int left, _In_ int top, _In_ int num, _In_ BYTE flag)
{
    /* assert INUMS_MIN == -99 && INUMS_MAX == 999 */
    bool valid = (num >= INUMS_MIN && num <= INUMS_MAX);
    int a = abs(num) / 100 % 10 - ((!valid + (num < 0)) * 10);  //let A show '-' when num < 0
    int b = abs(num) / 10 % 10 - (!valid * 10);
    int c = abs(num) % 10 - (!valid * 10);

    if (!(flag & INUMSF_SKIPA))
        drawDCInfoNum(hdstdc, left, top, a, (flag & INUMSF_HIDEA));
    if (!(flag & INUMSF_SKIPB))
        drawDCInfoNum(hdstdc, left + INFONUM_WIDTH, top, b, (flag & INUMSF_HIDEB));
    if (!(flag & INUMSF_SKIPC))
        drawDCInfoNum(hdstdc, left + INFONUM_WIDTH * 2, top, c, (flag & INUMSF_HIDEC));
}


/* These functions draw a ResetButton inside HeadArea SubArea
 * using predefined color and size, check related macros.
 */

void drawDCResetButtonBg(_In_ HDC hdstdc, _In_ int left, _In_ int top, _In_ bool clicked)
{
    if (clicked) drawdualedgebg(hdstdc, left, top, RB_SIZE, RB_SIZE, RBCOL_SURFACE, RBCOL_BRIGHT1, RBCOL_BRIGHT2, RBCOL_DARK1, RBCOL_DARK2);
    else drawdualedgebg(hdstdc, left, top, RB_SIZE, RB_SIZE, RBCOL_SURFACE, RBCOL_DARK1, RBCOL_DARK2, RBCOL_BRIGHT1, RBCOL_BRIGHT2);
}

void drawDCResetButtonBmp(_In_ HDC hdstdc, _In_ int left, _In_ int top, _In_ HBITMAP hbm, _In_ bool clicked)
{
    if (hbm) {
        HDC hdcbitmap = CreateCompatibleDC(hdstdc);
        SelectObject(hdcbitmap, hbm);
        BitBlt(hdstdc, left + clicked, top + clicked, BMP_SIZE - clicked, BMP_SIZE - clicked, hdcbitmap, 0, 0, SRCCOPY);
        DeleteDC(hdcbitmap);
    }
}

void drawDCResetButton(_In_ HDC hdstdc, _In_ int left, _In_ int top, _In_ HBITMAP hbm, _In_ bool clicked)
{
    drawDCResetButtonBg(hdstdc, left, top, clicked);
    drawDCResetButtonBmp(hdstdc, left + RB_BMPOFFSET, top + RB_BMPOFFSET, hbm, clicked);
}


/* These functions draw a MapUnit on MapArea SubArea
 * using predefined color and size, check related macros.
 */

void drawDCMUCover(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    drawthickedgebg(hdstdc, left, top, MUP_SIZE, MUP_SIZE, MUCOL_COVERSF, MUCOL_COVERDK, MUCOL_COVERBT);
}

void drawDCMUUncov(_In_ HDC hdstdc, _In_ int left, _In_ int top, _In_ bool bomb)
{
    if (bomb) drawhalfedgebg(hdstdc, left, top, MUP_SIZE, MUP_SIZE, MUCOL_BOMBBG, MUCOL_UNCOVEG);
    else drawhalfedgebg(hdstdc, left, top, MUP_SIZE, MUP_SIZE, MUCOL_UNCOVSF, MUCOL_UNCOVEG);
}

void drawDCMUFlag(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    drawDCMUCover(hdstdc, left, top);
    drawmuitemflag(hdstdc, left, top);
}

void drawDCMUMark(_In_ HDC hdstdc, _In_ int left, _In_ int top, _In_ bool clicked)
{
    if (clicked) drawDCMUUncov(hdstdc, left, top, false);
    else drawDCMUCover(hdstdc, left, top);
    drawmuitemmark(hdstdc, left, top, clicked);
}

void drawDCMUMine(_In_ HDC hdstdc, _In_ int left, _In_ int top, _In_ bool bomb)
{
    drawDCMUUncov(hdstdc, left, top, bomb);
    drawmuitemmine(hdstdc, left, top);
}

void drawDCMUWrong(_In_ HDC hdstdc, _In_ int left, _In_ int top)
{
    drawDCMUUncov(hdstdc, left, top, false);
    drawmuitemmine(hdstdc, left, top);
    drawmuitemcross(hdstdc, left, top);
}

void drawDCMUNum(_In_ HDC hdstdc, _In_ int left, _In_ int top, _In_ int num)
{
    drawDCMUUncov(hdstdc, left, top, false);
    switch (num) {
    case 0: break;
    case 1: drawmuitemnum1(hdstdc, left, top); break;
    case 2: drawmuitemnum2(hdstdc, left, top); break;
    case 3: drawmuitemnum3(hdstdc, left, top); break;
    case 4: drawmuitemnum4(hdstdc, left, top); break;
    case 5: drawmuitemnum5(hdstdc, left, top); break;
    case 6: drawmuitemnum6(hdstdc, left, top); break;
    case 7: drawmuitemnum7(hdstdc, left, top); break;
    case 8: drawmuitemnum8(hdstdc, left, top); break;
    }
}
