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
 * gamecore.c
 *****************************************************************************
 * This file contains the minesweeper core functions.
 * All basic operations of Game are defined in this file.
\*****************************************************************************/


#include "stdafx.h"
#include "gamecore.h"


/* GameMap position related functions */

int index2x(PGameInfo pGame, int index)
{
    return (index % pGame->width);
}

int index2y(PGameInfo pGame, int index)
{
    return (index / pGame->width);
}

int xy2index(PGameInfo pGame, int x, int y)
{
    return (y * pGame->width) + x;
}

bool isxyinmap(PGameInfo pGame, int x, int y)
{
    return (x >= 0 && x < pGame->width && y >=0 && y < pGame->height);
}

bool isidxinmap(PGameInfo pGame, int index)
{
    return (index >= 0 && index < pGame->size);
}

int getNeighbors(PGameInfo pGame, Neighbor neighbor, int x, int y)
{
    if (!isxyinmap(pGame, x, y)) return RETVAL_INDEXOOR;
    neighbor[0] = xy2index(pGame, x, y);
    neighbor[1] = (x > 0 && y > 0) ? xy2index(pGame, x - 1, y - 1) : INV_INDEX;
    neighbor[2] = (y > 0) ? xy2index(pGame, x, y - 1) : INV_INDEX;
    neighbor[3] = (x < pGame->width - 1 && y > 0) ? xy2index(pGame, x + 1, y - 1) : INV_INDEX;
    neighbor[4] = (x > 0) ? xy2index(pGame, x - 1, y) : INV_INDEX;
    neighbor[5] = (x < pGame->width - 1) ? xy2index(pGame, x + 1, y) : INV_INDEX;
    neighbor[6] = (x > 0 && y < pGame->height - 1) ? xy2index(pGame, x - 1, y + 1) : INV_INDEX;
    neighbor[7] = (y < pGame->height - 1) ? xy2index(pGame, x, y + 1) : INV_INDEX;
    neighbor[8] = (x < pGame->width - 1 && y < pGame->height - 1) ? xy2index(pGame, x + 1, y + 1) : INV_INDEX;
    return RETVAL_NOERROR;
}


/* Game property related functions */

void setGameMode(PGameInfo pGame, BYTE mode, BYTE width, BYTE height, WORD mines)
{
    switch (mode) {
    case JUNIOR:
        pGame->mode = JUNIOR;
        pGame->width = JUNIOR_WIDTH;
        pGame->height = JUNIOR_HEIGHT;
        pGame->size = JUNIOR_SIZE;
        pGame->mines = JUNIOR_MINES;
        pGame->state = GS_UNKNOW;
        break;
    case MIDDLE:
        pGame->mode = MIDDLE;
        pGame->width = MIDDLE_WIDTH;
        pGame->height = MIDDLE_HEIGHT;
        pGame->size = MIDDLE_SIZE;
        pGame->mines = MIDDLE_MINES;
        pGame->state = GS_UNKNOW;
        break;
    case SENIOR:
        pGame->mode = SENIOR;
        pGame->width = SENIOR_WIDTH;
        pGame->height = SENIOR_HEIGHT;
        pGame->size = SENIOR_SIZE;
        pGame->mines = SENIOR_MINES;
        pGame->state = GS_UNKNOW;
        break;
    case CUSTOM:
        pGame->mode = CUSTOM;
        pGame->width = min(max(width, MIN_WIDTH), MAX_WIDTH);
        pGame->height = min(max(height, MIN_HEIGHT), MAX_HEIGHT);
        pGame->size = pGame->width * pGame->height;
        pGame->mines = min(max(mines, MIN_MINES), MAX_MINES(pGame->size));
        pGame->state = GS_UNKNOW;
        break;
    default:
        setGameMode(pGame, JUNIOR, 0, 0, 0);
        break;
    }
}

void setMark(PGameInfo pGame, bool enable)
{
    pGame->mark = enable;
}


/* Game runtime basic functions */

void resetGame(PGameInfo pGame)
{
    pGame->state = GS_INIT;
    pGame->flags = 0;
    pGame->bare_units = 0;
    pGame->time = 0;
    memset(pGame->map, 0, sizeof(BYTE) * pGame->size);
}

void startGame(PGameInfo pGame)
{
    pGame->state = GS_RUNNING;
    pGame->flags = 0;
    pGame->bare_units = 0;
}

void finishGame(PGameInfo pGame, bool win)
{
    pGame->state = (win) ? GS_WIN : GS_LOSS;
    pGame->flags = (win) ? pGame->mines : pGame->flags;
    pGame->bare_units = pGame->size;
}

int createGameMap(PGameInfo pGame, int index)
{
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;

    Neighbor safepos;
    getNeighbors(pGame, safepos, index2x(pGame, index), index2y(pGame, index));

    //generate mines, 8 Units around where clicked will not have mines
    //use shuffle algorithm
    memset(pGame->map, MUS_COVER, sizeof(BYTE) * pGame->size);  //init MapUnits
    memset(pGame->map, MUF_MINE | MUN_MINE, sizeof(BYTE) * pGame->mines);   //generate mines
    int neicount = 0;
    for (int i = 0; i < NEI_TOTAL; i++) neicount += (safepos[i] != INV_INDEX);  //remember how many safe positions needed
    for (int k = 0; k < pGame->size - neicount; k++) {  //shuffle algorithm, exclude reserved tail
        int index = rand() % (pGame->size - neicount - k) + k;
        BYTE temp = pGame->map[index];
        pGame->map[index] = pGame->map[k];
        pGame->map[k] = temp;
    }

    //shift the index of safe area center Unit to the middle of Neighbor list
    //this process is required to avoid overlapping issues between safe area and reserved area
    for (int i = 0; i < NEI_TOTAL / 2; i++) {
        int temp = safepos[i];
        safepos[i] = safepos[i + 1];
        safepos[i + 1] = temp;
    }

    //move safe area to where it is, the order of Units matters
    for (int i = 0; i < NEI_TOTAL; i++) {
        if (safepos[i] != INV_INDEX) {
            BYTE temp = pGame->map[pGame->size - neicount];
            pGame->map[pGame->size - neicount] = pGame->map[safepos[i]];
            pGame->map[safepos[i]] = temp;
            neicount--;
        }
    }

    //calculate mines around each Unit
    for (int i = 0; i < pGame->size; i++) {
        if (ISMUMINE(pGame->map[i])) continue;
        int mcnt = 0;
        Neighbor neipos;
        getNeighbors(pGame, neipos, index2x(pGame, i), index2y(pGame, i));
        for (int j = 1; j < NEI_TOTAL; j++)
            mcnt += (neipos[j] != INV_INDEX && ISMUMINE(pGame->map[neipos[j]]));
        SETMUNUMBER(pGame->map[i], mcnt);
    }
    return RETVAL_NOERROR;
}

int clickOne(PGameInfo pGame, int index)
{
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;
    if (!ISMUCLICKABLE(pGame->map[index])) return RETVAL_BADMUSTATE;

    BYTE mustate = (ISMUMINE(pGame->map[index])) ? MUS_BOMB : MUS_BARE;
    pGame->bare_units++;
    SETMUSTATE(pGame->map[index], mustate);
    return GETMUNUMBER(pGame->map[index]);
}

int openBlanks(PGameInfo pGame, int index)
{
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;
    if (GETMUSTATE(pGame->map[index]) != MUS_BARE) return RETVAL_BADMUSTATE;
    if (GETMUNUMBER(pGame->map[index]) != 0) return RETVAL_BADMUSTATE;

    Neighbor pos;
    getNeighbors(pGame, pos, index2x(pGame, index), index2y(pGame, index));
    for (int i = 1; i < NEI_TOTAL; i++) {   //no need to take care of INV_INDEX here
        int ret = clickOne(pGame, pos[i]);
        if (ret == 0) openBlanks(pGame, pos[i]);    //do in a recursive manner
    }
    return RETVAL_NOERROR;
}

int flagOne(PGameInfo pGame, int index)
{
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;

    //take effect only on covered Units
    BYTE new_mapunit_state;
    switch (GETMUSTATE(pGame->map[index])) {
    case MUS_COVER:
        new_mapunit_state = MUS_FLAG;
        pGame->flags++;
        break;
    case MUS_FLAG:
        new_mapunit_state = (pGame->mark) ? MUS_MARK : MUS_COVER;
        pGame->flags--;
        break;
    case MUS_MARK:
        new_mapunit_state = MUS_COVER;
        break;
    default:
        return RETVAL_BADMUSTATE;
    }

    SETMUSTATE(pGame->map[index], new_mapunit_state);
    return RETVAL_NOERROR;
}

void showMines(PGameInfo pGame)
{
    for (int i = 0; i < pGame->size; i++) {
        BYTE mapunit = pGame->map[i];
        if (ISMUMINE(mapunit) && ISMUCLICKABLE(mapunit)) {
            BYTE mustate = (pGame->state == GS_WIN) ? MUS_FLAG : MUS_BARE;
            SETMUSTATE(pGame->map[i], mustate);
        }
        else if (!ISMUMINE(mapunit) && GETMUSTATE(mapunit) == MUS_FLAG) {
            SETMUSTATE(pGame->map[i], MUS_FALSE);
        }
    }
}

void setGameTime(PGameInfo pGame, WORD time)
{
    pGame->time = min(time, MAX_TIME);
}

void stepGameTime(PGameInfo pGame)
{
    pGame->time++;
}


/* Game runtime high-level functions */

int leftClick(PGameInfo pGame, int index)
{
    if (!ISGAMEACTIVE(pGame->state)) return RETVAL_BADGAMESTATE;
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;
    if (!ISMUCLICKABLE(pGame->map[index])) return RETVAL_BADMUSTATE;

    //first click, create new Map and start new Game
    bool isfirst = (pGame->state == GS_INIT);
    if (isfirst) {
        createGameMap(pGame, index);
        startGame(pGame);
    }

    //normal click
    int ret = clickOne(pGame, index);
    if (ret == 0) openBlanks(pGame, index);
    bool isbomb = (ret == MUN_MINE);

    //after click, show positions of all mines when GameOver,
    //or return a Game start message if is first click
    if (isfirst) ret = RETVAL_GAMESTART;
    if (isbomb || pGame->bare_units == pGame->size - pGame->mines) {
        finishGame(pGame, !isbomb);
        showMines(pGame);
        ret = (isbomb) ? RETVAL_GAMELOSS : RETVAL_GAMEWIN;
    }
    return ret;
}

int simulClick(PGameInfo pGame, int index)
{
    if (pGame->state != GS_RUNNING) return RETVAL_BADGAMESTATE;
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;
    if (GETMUSTATE(pGame->map[index]) != MUS_BARE) return RETVAL_BADMUSTATE;

    Neighbor pos;
    getNeighbors(pGame, pos, index2x(pGame, index), index2y(pGame, index));

    //you can open neighbors only when flags EQ mines
    int flags = 0;
    for (int i = 1; i < NEI_TOTAL; i++)
        flags += (pos[i] != INV_INDEX && GETMUSTATE(pGame->map[pos[i]]) == MUS_FLAG);
    if (GETMUNUMBER(pGame->map[pos[0]]) != flags) return RETVAL_BADFLAGNUM;

    //open neighbors
    bool isbomb = false;
    for (int i = 1; i < NEI_TOTAL; i++) {
        int ret = clickOne(pGame, pos[i]);
        if (ret == 0) openBlanks(pGame, pos[i]);
        isbomb |= (ret == MUN_MINE);
    }

    //after open, show positions of all mines when GameOver
    if (isbomb || pGame->bare_units == pGame->size - pGame->mines) {
        finishGame(pGame, !isbomb);
        showMines(pGame);
        return (isbomb) ? RETVAL_GAMELOSS : RETVAL_GAMEWIN;
    }
    return RETVAL_NOERROR;
}

int rightClick(PGameInfo pGame, int index)
{
    return (ISGAMEACTIVE(pGame->state)) ? flagOne(pGame, index) : RETVAL_BADGAMESTATE;
}


/* GameScore related functions */

void resetRecord(PGameScore pScore)
{
    *pScore = (GameScore){ MAX_TIME, MAX_TIME, MAX_TIME, _T(DEF_SCORE_NAME_EN), _T(DEF_SCORE_NAME_EN), _T(DEF_SCORE_NAME_EN) };
}

WORD getRecordTime(PGameScore pScore, BYTE mode)
{
    switch (mode) {
    case JUNIOR: return pScore->junior_time;
    case MIDDLE: return pScore->middle_time;
    case SENIOR: return pScore->senior_time;
    }
    return INV_TIME;
}

LPTSTR getRecordName(PGameScore pScore, BYTE mode)
{
    switch (mode) {
    case JUNIOR: return pScore->junior_name;
    case MIDDLE: return pScore->middle_name;
    case SENIOR: return pScore->senior_name;
    }
    return NULL;
}

int setRecordTime(PGameScore pScore, BYTE mode, WORD time)
{
    time = min(time, MAX_TIME);
    switch (mode) {
    case JUNIOR:
        pScore->junior_time = time;
        return RETVAL_NOERROR;
    case MIDDLE:
        pScore->middle_time = time;
        return RETVAL_NOERROR;
    case SENIOR:
        pScore->senior_time = time;
        return RETVAL_NOERROR;
    }
    return RETVAL_BADGAMEMODE;
}

bool isNewRecord(PGameInfo pGame, PGameScore pScore)
{
    return (ISSTDMOD(pGame->mode) && pGame->state == GS_WIN && pGame->time < getRecordTime(pScore, pGame->mode));
}
