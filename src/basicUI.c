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
 * basicUI.c
 *****************************************************************************
 * This file contains the basic drawing and some transform functions.
 * Drawing functions use Win32 APIs and draw directly on Device Context(DC)
 * without creating a DC-buffer.
 * 
 * NOTE: Almost all functions have NO arg check process, use with care.
\*****************************************************************************/


#include "stdafx.h"
#include "basicUI.h"


/* this array stores InfoNum backgroud
 * 0 represent INCOL_BLANK, non-0 represent INCOL_SEGOFF
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

//Draw a solid color background.
static void drawsolidcolorbg(
    _In_ HDC hdestdc,
    _In_ int left,
    _In_ int top,
    _In_ int width,
    _In_ int height,
    _In_ COLORREF surface
)
{
    RECT rect = { left, top, left + width, top + height };
    SetDCBrushColor(hdestdc, surface);
    FillRect(hdestdc, &rect, GetStockObject(DC_BRUSH));
}

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
)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdestdc, edge);
    rect = (RECT){ left, top, left + width, top + height };
    FillRect(hdestdc, &rect, hdcbrush);

    SetDCBrushColor(hdestdc, inner);
    rect = (RECT){ left + 1, top + 1, left + width, top + height };
    FillRect(hdestdc, &rect, hdcbrush);
}

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
)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdestdc, dark);
    rect = (RECT){ left, top, left + width - 1, top + height - 1 };
    FillRect(hdestdc, &rect, hdcbrush);
    SetDCBrushColor(hdestdc, bright);
    rect = (RECT){ left + 1, top + 1, left + width, top + height };
    FillRect(hdestdc, &rect, hdcbrush);

    SetDCBrushColor(hdestdc, inner);
    rect = (RECT){ left + 1, top + 1, left + width - 1, top + height - 1 };
    FillRect(hdestdc, &rect, hdcbrush);

    SetPixel(hdestdc, left + width - 1, top, inner);
    SetPixel(hdestdc, left, top + height - 1, inner);
}

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
)
{
    drawfulledgebg(hdestdc, left, top, width, height, inner, bright, dark);
    drawfulledgebg(hdestdc, left + 1, top + 1, width - 2, height - 2, inner, bright, dark);
}

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
)
{
    drawfulledgebg(hdestdc, left, top, width, height, inner, bright1, dark1);
    drawfulledgebg(hdestdc, left + 1, top + 1, width - 2, height - 2, inner, bright2, dark2);
}


/*    a
 *  +---+
 * f| g |b
 *  +---+
 * e|   |c
 *  +---+
 *    d
 */
//Draw 7sd background.
static inline void draw7sdbg(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    for (int i = 0; i < INFONUM_WIDTH; i++) {
        for (int j = 0; j < INFONUM_HEIGHT; j++) {
            if (InfoNumBG[i][j]) SetPixel(hdestdc, left + i, top + j, INCOL_SEGOFF);
            else SetPixel(hdestdc, left + i, top + j, INCOL_BLANK);
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

//Draw a mine icon.
static void drawmuitemmine(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdestdc, MUCOL_MINESF);
    rect = (RECT){ left + 6, top + 9, left + 19, top + 16 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 7, left + 18, top + 18 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 9, top + 6, left + 16, top + 19 };
    FillRect(hdestdc, &rect, hdcbrush);

    rect = (RECT){ left + 3, top + 12, left + 22, top + 13 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 12, top + 3, left + 13, top + 22 };
    FillRect(hdestdc, &rect, hdcbrush);

    SetDCBrushColor(hdestdc, MUCOL_MINEHL);
    rect = (RECT){ left + 9, top + 9, left + 12, top + 12 };
    FillRect(hdestdc, &rect, hdcbrush);

    SetPixel(hdestdc, left + 6, top + 6, MUCOL_MINESF);
    SetPixel(hdestdc, left + 6, top + 18, MUCOL_MINESF);
    SetPixel(hdestdc, left + 18, top + 6, MUCOL_MINESF);
    SetPixel(hdestdc, left + 18, top + 18, MUCOL_MINESF);
}

//Draw a question mark icon.
static void drawmuitemmark(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ bool clicked)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdestdc, MUCOL_MARK);
    rect = (RECT){ left + clicked + 9, top + clicked + 4, left + clicked + 15, top + clicked + 5 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + clicked + 7, top + clicked + 5, left + clicked + 10, top + clicked + 9 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + clicked + 14, top + clicked + 5, left + clicked + 17, top + clicked + 10 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + clicked + 12, top + clicked + 10, left + clicked + 15, top + clicked + 12 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + clicked + 10, top + clicked + 12, left + clicked + 14, top + clicked + 15 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + clicked + 10, top + clicked + 17, left + clicked + 14, top + clicked + 20 };
    FillRect(hdestdc, &rect, hdcbrush);
}

//Draw a flag icon.
static void drawmuitemflag(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdestdc, MUCOL_FLAGFLAG);
    rect = (RECT){ left + 6, top + 7, left + 7, top + 9 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 6, left + 10, top + 10 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 10, top + 4, left + 13, top + 12 };
    FillRect(hdestdc, &rect, hdcbrush);

    SetDCBrushColor(hdestdc, MUCOL_FLAGBASE);
    rect = (RECT){ left + 12, top + 12, left + 13, top + 15 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 9, top + 15, left + 15, top + 16 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 6, top + 16, left + 18, top + 19 };
    FillRect(hdestdc, &rect, hdcbrush);
}

//Draw a cross icon.
static void drawmuitemcross(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    HPEN hpcross = CreatePen(PS_SOLID, 2, MUCOL_CROSS);
    HPEN hpold = SelectObject(hdestdc, hpcross);

    MoveToEx(hdestdc, left + 4, top + 4, NULL);
    LineTo(hdestdc, left + 20, top + 20);
    MoveToEx(hdestdc, left + 4, top + 20, NULL);
    LineTo(hdestdc, left + 20, top + 4);

    SelectObject(hdestdc, hpold);
    DeleteObject(hpcross);
}

//Draw a number 1 icon.
static void drawmuitemnum1(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdestdc, MUCOL_NUMBER1);
    rect = (RECT){ left + 11, top + 5, left + 15, top + 6 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 9, top + 6, left + 15, top + 7 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 7, left + 15, top + 8 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 8, left + 15, top + 10 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 10, top + 10, left + 15, top + 17 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 17, left + 20, top + 20 };
    FillRect(hdestdc, &rect, hdcbrush);
}

//Draw a number 2 icon.
static void drawmuitemnum2(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdestdc, MUCOL_NUMBER2);
    rect = (RECT){ left + 5, top + 7, left + 9, top + 10 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 5, left + 18, top + 8 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 7, left + 20, top + 11 };
    FillRect(hdestdc, &rect, hdcbrush);

    rect = (RECT){ left + 15, top + 10, left + 19, top + 13 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 13, top + 11, left + 17, top + 14 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 11, top + 12, left + 15, top + 15 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 9, top + 13, left + 13, top + 16 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 14, left + 11, top + 17 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 15, left + 9, top + 18 };
    FillRect(hdestdc, &rect, hdcbrush);

    rect = (RECT){ left + 5, top + 17, left + 20, top + 20 };
    FillRect(hdestdc, &rect, hdcbrush);

    SetPixel(hdestdc, left + 6, top + 6, MUCOL_NUMBER2);
    SetPixel(hdestdc, left + 18, top + 6, MUCOL_NUMBER2);
}

//Draw a number 3 icon.
static void drawmuitemnum3(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdestdc, MUCOL_NUMBER3);
    rect = (RECT){ left + 5, top + 5, left + 19, top + 8 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 7, left + 20, top + 11 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 10, top + 11, left + 19, top + 14 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 14, left + 20, top + 19 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 17, left + 19, top + 20 };
    FillRect(hdestdc, &rect, hdcbrush);
}

//Draw a number 4 icon.
static void drawmuitemnum4(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdestdc, MUCOL_NUMBER4);
    rect = (RECT){ left + 8, top + 5, left + 13, top + 8 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 8, left + 11, top + 11 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 11, left + 20, top + 14 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 14, top + 5, left + 19, top + 20 };
    FillRect(hdestdc, &rect, hdcbrush);
}

//Draw a number 5 icon.
static void drawmuitemnum5(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdestdc, MUCOL_NUMBER5);
    rect = (RECT){ left + 5, top + 5, left + 20, top + 8 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 8, left + 10, top + 11 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 11, left + 19, top + 14 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 13, left + 20, top + 19 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 17, left + 19, top + 20 };
    FillRect(hdestdc, &rect, hdcbrush);
}

//Draw a number 6 icon.
static void drawmuitemnum6(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdestdc, MUCOL_NUMBER6);
    rect = (RECT){ left + 7, top + 5, left + 19, top + 8 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 7, left + 10, top + 19 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 10, top + 11, left + 19, top + 14 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 13, left + 20, top + 19 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 17, left + 19, top + 20 };
    FillRect(hdestdc, &rect, hdcbrush);
}

//Draw a number 7 icon.
static void drawmuitemnum7(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdestdc, MUCOL_NUMBER7);
    rect = (RECT){ left + 5, top + 5, left + 20, top + 8 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 8, left + 20, top + 11 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 14, top + 11, left + 19, top + 14 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 13, top + 14, left + 17, top + 17 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 11, top + 17, left + 16, top + 20 };
    FillRect(hdestdc, &rect, hdcbrush);
}

//Draw a number 8 icon.
static void drawmuitemnum8(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    HBRUSH hdcbrush = GetStockObject(DC_BRUSH);
    RECT rect;

    SetDCBrushColor(hdestdc, MUCOL_NUMBER8);
    rect = (RECT){ left + 7, top + 5, left + 19, top + 8 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 7, left + 10, top + 11 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 7, left + 20, top + 11 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 11, left + 19, top + 14 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 5, top + 14, left + 10, top + 19 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 16, top + 14, left + 20, top + 19 };
    FillRect(hdestdc, &rect, hdcbrush);
    rect = (RECT){ left + 7, top + 17, left + 19, top + 20 };
    FillRect(hdestdc, &rect, hdcbrush);
}

//logical xor
static inline bool _xor(const bool A, const bool B)
{
    return (!A && B || A && !B);
}


/* following functions can be called by external functions */

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


void drawDCClientBg(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ int n_map_width, _In_ int n_map_height)
{
    drawsolidcolorbg(hdestdc, left, top, CLIENT_WIDTH(n_map_width), CLIENT_HEIGHT(n_map_height), CLTCOL_SURFACE);
}

void drawDCHeadAreaBg(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ int n_map_width)
{
    drawthickedgebg(hdestdc, left, top, HEADAREA_WIDTH(n_map_width), HEADAREA_HEIGHT, HACOL_SURFACE, HACOL_BRIGHT, HACOL_DARK);
}

void drawDCMapAreaBg(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ int n_map_width, _In_ int n_map_height)
{
    drawthickedgebg(hdestdc, left, top, MAPAREA_WIDTH(n_map_width), MAPAREA_HEIGHT(n_map_height), MACOL_SURFACE, MACOL_BRIGHT, MACOL_DARK);
}

void drawDCInfoBg(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    drawfulledgebg(hdestdc, left, top, INFO_WIDTH, INFO_HEIGHT, ICOL_SURFACE, ICOL_BRIGHT, ICOL_DARK);
}


void drawDCInfoNumBg(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    draw7sdbg(hdestdc, left, top);
}

void drawDCInfoNumNumber(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ int num)
{
    /* assert INFONUM_MIN == 0 && INFONUM_MAX == 9 */
    bool valid = (num >= INFONUM_MIN && num <= INFONUM_MAX);
    num = min(max(num, INFONUM_MIN), INFONUM_MAX);  //limit possible states
    bool A = num & 0x01, B = num & 0x02, C = num & 0x04, D = num & 0x08;

    HPEN hpold = SelectObject(hdestdc, GetStockObject(DC_PEN));
    SetDCPenColor(hdestdc, INCOL_SEGON);

    if (valid && (B + D + !_xor(A, C))) draw7sda(hdestdc, left, top);
    if (valid && (!C + D + !_xor(A, B))) draw7sdb(hdestdc, left, top);
    if (valid && (A + !B + C + D)) draw7sdc(hdestdc, left, top);
    if (valid && (D + B * !C + !_xor(A, !B * C))) draw7sdd(hdestdc, left, top);
    if (valid && (!A * (B + !C))) draw7sde(hdestdc, left, top);
    if (valid && (D + !A * !B + !A * C + !B * C)) draw7sdf(hdestdc, left, top);
    if (!valid || (D + !A * B + _xor(B, C))) draw7sdg(hdestdc, left, top);

    SelectObject(hdestdc, hpold);
}

void drawDCInfoNum(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ int num, _In_ bool hide)
{
    drawDCInfoNumBg(hdestdc, left, top);
    if (!hide) drawDCInfoNumNumber(hdestdc, left, top, num);
}

void drawDCINums(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ int num, _In_ BYTE flag)
{
    /* assert INUMS_MIN == -99 && INUMS_MAX == 999 */
    bool valid = (num >= INUMS_MIN && num <= INUMS_MAX);
    int a = abs(num) / 100 % 10 - (!valid * (num < 0) * 10);    //let A show '-' when num < 0
    int b = abs(num) / 10 % 10 - (!valid * 10);
    int c = abs(num) % 10 - (!valid * 10);

    if (!(flag & INUMSF_SKIPA))
        drawDCInfoNum(hdestdc, left, top, a, (flag & INUMSF_HIDEA));
    if (!(flag & INUMSF_SKIPB))
        drawDCInfoNum(hdestdc, left + INFONUM_WIDTH, top, b, (flag & INUMSF_HIDEB));
    if (!(flag & INUMSF_SKIPC))
        drawDCInfoNum(hdestdc, left + INFONUM_WIDTH * 2, top, c, (flag & INUMSF_HIDEC));
}


void drawDCResetButtonBg(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ bool clicked)
{
    if (clicked) drawdualedgebg(hdestdc, left, top, RB_SIZE, RB_SIZE, RBCOL_SURFACE, RBCOL_BRIGHT1, RBCOL_BRIGHT2, RBCOL_DARK1, RBCOL_DARK2);
    else drawdualedgebg(hdestdc, left, top, RB_SIZE, RB_SIZE, RBCOL_SURFACE, RBCOL_DARK1, RBCOL_DARK2, RBCOL_BRIGHT1, RBCOL_BRIGHT2);
}

void drawDCResetButtonBmp(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ HBITMAP hbm, _In_ bool clicked)
{
    if (hbm == NULL) return;
    HDC hdcbitmap = CreateCompatibleDC(hdestdc);
    SelectObject(hdcbitmap, hbm);
    BitBlt(hdestdc, left + clicked, top + clicked, BMP_SIZE - clicked, BMP_SIZE - clicked, hdcbitmap, 0, 0, SRCCOPY);
    DeleteDC(hdcbitmap);
}

void drawDCResetButton(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ HBITMAP hbm, _In_ bool clicked)
{
    drawDCResetButtonBg(hdestdc, left, top, clicked);
    drawDCResetButtonBmp(hdestdc, left + 2, top + 2, hbm, clicked);
}


void drawDCMUCover(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    drawthickedgebg(hdestdc, left, top, MUP_SIZE, MUP_SIZE, MUCOL_COVERSF, MUCOL_COVERDK, MUCOL_COVERBT);
}

void drawDCMUUncov(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ bool bomb)
{
    if (bomb) drawhalfedgebg(hdestdc, left, top, MUP_SIZE, MUP_SIZE, MUCOL_BOMBBG, MUCOL_UNCOVEG);
    else drawhalfedgebg(hdestdc, left, top, MUP_SIZE, MUP_SIZE, MUCOL_UNCOVSF, MUCOL_UNCOVEG);
}

void drawDCMUFlag(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    drawDCMUCover(hdestdc, left, top);
    drawmuitemflag(hdestdc, left, top);
}

void drawDCMUMark(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ bool clicked)
{
    if (clicked) drawDCMUUncov(hdestdc, left, top, false);
    else drawDCMUCover(hdestdc, left, top);
    drawmuitemmark(hdestdc, left, top, clicked);
}

void drawDCMUMine(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ bool bomb)
{
    drawDCMUUncov(hdestdc, left, top, bomb);
    drawmuitemmine(hdestdc, left, top);
}

void drawDCMUWrong(_In_ HDC hdestdc, _In_ int left, _In_ int top)
{
    drawDCMUUncov(hdestdc, left, top, false);
    drawmuitemmine(hdestdc, left, top);
    drawmuitemcross(hdestdc, left, top);
}

void drawDCMUNum(_In_ HDC hdestdc, _In_ int left, _In_ int top, _In_ int num)
{
    drawDCMUUncov(hdestdc, left, top, false);
    switch (num) {
    case 0: break;
    case 1: drawmuitemnum1(hdestdc, left, top); break;
    case 2: drawmuitemnum2(hdestdc, left, top); break;
    case 3: drawmuitemnum3(hdestdc, left, top); break;
    case 4: drawmuitemnum4(hdestdc, left, top); break;
    case 5: drawmuitemnum5(hdestdc, left, top); break;
    case 6: drawmuitemnum6(hdestdc, left, top); break;
    case 7: drawmuitemnum7(hdestdc, left, top); break;
    case 8: drawmuitemnum8(hdestdc, left, top); break;
    }
}
