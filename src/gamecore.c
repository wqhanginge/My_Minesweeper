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
    return (x >= 0 && x < pGame->width && y >= 0 && y < pGame->height);
}

bool isidxinmap(PGameInfo pGame, int index)
{
    return (index >= 0 && index < pGame->width * pGame->height);
}

bool isNeighbor(PGameInfo pGame, int first, int second)
{
    if (isidxinmap(pGame, first) && isidxinmap(pGame, second)) {
        bool xnei = abs(index2x(pGame, first) - index2x(pGame, second)) <= 1;
        bool ynei = abs(index2y(pGame, first) - index2y(pGame, second)) <= 1;
        return (xnei && ynei);
    }
    return false;
}

int getNeighbors(PGameInfo pGame, Neighbor neighbor, int index)
{
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;
    int x = index2x(pGame, index), y = index2y(pGame, index);
    neighbor[0] = index;
    neighbor[1] = isxyinmap(pGame, x - 1, y - 1) ? xy2index(pGame, x - 1, y - 1) : INV_INDEX;
    neighbor[2] = isxyinmap(pGame, x, y - 1) ? xy2index(pGame, x, y - 1) : INV_INDEX;
    neighbor[3] = isxyinmap(pGame, x + 1, y - 1) ? xy2index(pGame, x + 1, y - 1) : INV_INDEX;
    neighbor[4] = isxyinmap(pGame, x - 1, y) ? xy2index(pGame, x - 1, y) : INV_INDEX;
    neighbor[5] = isxyinmap(pGame, x + 1, y) ? xy2index(pGame, x + 1, y) : INV_INDEX;
    neighbor[6] = isxyinmap(pGame, x - 1, y + 1) ? xy2index(pGame, x - 1, y + 1) : INV_INDEX;
    neighbor[7] = isxyinmap(pGame, x, y + 1) ? xy2index(pGame, x, y + 1) : INV_INDEX;
    neighbor[8] = isxyinmap(pGame, x + 1, y + 1) ? xy2index(pGame, x + 1, y + 1) : INV_INDEX;
    return RETVAL_NOERROR;
}


/* Game property related functions */

void setGameMode(PGameInfo pGame, BYTE mode, BYTE width, BYTE height, WORD mines)
{
    switch (mode) {
    case GM_JUNIOR:
        pGame->mode = GM_JUNIOR;
        pGame->state = GS_UNKNOW;
        pGame->width = JUNIOR_WIDTH;
        pGame->height = JUNIOR_HEIGHT;
        pGame->mines = JUNIOR_MINES;
        break;
    case GM_MIDDLE:
        pGame->mode = GM_MIDDLE;
        pGame->state = GS_UNKNOW;
        pGame->width = MIDDLE_WIDTH;
        pGame->height = MIDDLE_HEIGHT;
        pGame->mines = MIDDLE_MINES;
        break;
    case GM_SENIOR:
        pGame->mode = GM_SENIOR;
        pGame->state = GS_UNKNOW;
        pGame->width = SENIOR_WIDTH;
        pGame->height = SENIOR_HEIGHT;
        pGame->mines = SENIOR_MINES;
        break;
    case GM_CUSTOM:
        pGame->mode = GM_CUSTOM;
        pGame->state = GS_UNKNOW;
        pGame->width = min(max(width, MIN_WIDTH), MAX_WIDTH);
        pGame->height = min(max(height, MIN_HEIGHT), MAX_HEIGHT);
        pGame->mines = min(max(mines, MIN_MINES), MAX_MINES(pGame->width * pGame->height));
        break;
    default:
        setGameMode(pGame, GM_JUNIOR, 0, 0, 0);
        break;
    }
}


/* Game runtime basic functions */

void resetGame(PGameInfo pGame)
{
    pGame->state = GS_INIT;
    pGame->flags = 0;
    pGame->secrets = pGame->width * pGame->height;
    pGame->time = 0;
    memset(pGame->map, 0, sizeof(BYTE) * pGame->width * pGame->height);
}

void startGame(PGameInfo pGame)
{
    pGame->state = GS_RUNNING;
    pGame->flags = 0;
    pGame->secrets = pGame->width * pGame->height;
}

void finishGame(PGameInfo pGame, bool win)
{
    pGame->state = (win) ? GS_WIN : GS_LOSS;
    pGame->flags = (win) ? pGame->mines : pGame->flags;
    pGame->secrets = 0;
}

int createGameMap(PGameInfo pGame, int index)
{
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;

    Neighbor safepos;
    getNeighbors(pGame, safepos, index);

    //generate mines, 8 Units around where is clicked will not have mines
    //use shuffle algorithm
    int safecnt = 0, mapsize = pGame->width * pGame->height;
    memset(pGame->map, MUS_COVER, sizeof(BYTE) * mapsize);  //init MapUnits
    memset(pGame->map, MUN_MINE, sizeof(BYTE) * pGame->mines);  //generate mines
    for (int i = 0; i < NEI_TOTAL; i++) safecnt += (safepos[i] != INV_INDEX);   //remember how many safe positions needed
    for (int k = 0; k < mapsize - safecnt; k++) {   //shuffle algorithm, exclude reserved tail
        int idx = rand() % (mapsize - safecnt - k) + k;
        BYTE temp = pGame->map[idx];
        pGame->map[idx] = pGame->map[k];
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
            BYTE temp = pGame->map[mapsize - safecnt];
            pGame->map[mapsize - safecnt] = pGame->map[safepos[i]];
            pGame->map[safepos[i]] = temp;
            safecnt--;
        }
    }

    //calculate mines around each Unit
    //reuse local variables
    for (int i = 0; i < mapsize; i++) {
        if (ISMUMINE(pGame->map[i])) continue;
        getNeighbors(pGame, safepos, i);
        safecnt = 0;
        for (int j = 1; j < NEI_TOTAL; j++)
            safecnt += (safepos[j] != INV_INDEX && ISMUMINE(pGame->map[safepos[j]]));
        pGame->map[i] = SETMUNUMBER(pGame->map[i], safecnt);
    }
    return RETVAL_NOERROR;
}

int clickOne(PGameInfo pGame, int index)
{
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;
    if (ISMUSTATIC(pGame->map[index])) return RETVAL_BADMUSTATE;

    BYTE mustate = (ISMUMINE(pGame->map[index])) ? MUS_BOMB : MUS_BARE;
    pGame->secrets--;
    pGame->map[index] = SETMUSTATE(pGame->map[index], mustate);
    return GETMUNUMBER(pGame->map[index]);
}

int openBlanks(PGameInfo pGame, int index)
{
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;
    if (GETMUSTATE(pGame->map[index]) != MUS_BARE) return RETVAL_BADMUSTATE;
    if (GETMUNUMBER(pGame->map[index]) != 0) return RETVAL_BADMUSTATE;

    Neighbor pos;
    getNeighbors(pGame, pos, index);
    for (int i = 1; i < NEI_TOTAL; i++) {   //no need to take care of INV_INDEX here
        int ret = clickOne(pGame, pos[i]);
        if (ret == 0) openBlanks(pGame, pos[i]);    //do in a recursive manner
    }
    return RETVAL_NOERROR;
}

int flagOne(PGameInfo pGame, int index, bool qmark)
{
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;

    //take effect only on covered Units
    BYTE new_mustate;
    switch (GETMUSTATE(pGame->map[index])) {
    case MUS_COVER:
        new_mustate = MUS_FLAG;
        pGame->flags++;
        break;
    case MUS_FLAG:
        new_mustate = (qmark) ? MUS_MARK : MUS_COVER;
        pGame->flags--;
        break;
    case MUS_MARK:
        new_mustate = MUS_COVER;
        break;
    default:
        return RETVAL_BADMUSTATE;
    }

    pGame->map[index] = SETMUSTATE(pGame->map[index], new_mustate);
    return RETVAL_NOERROR;
}

void showMines(PGameInfo pGame)
{
    for (int i = 0; i < pGame->width * pGame->height; i++) {
        BYTE mapunit = pGame->map[i];
        if (ISMUMINE(mapunit) && !ISMUSTATIC(mapunit)) {
            BYTE mustate = (pGame->state == GS_WIN) ? MUS_FLAG : MUS_BARE;
            pGame->map[i] = SETMUSTATE(mapunit, mustate);
        } else if (!ISMUMINE(mapunit) && GETMUSTATE(mapunit) == MUS_FLAG) {
            pGame->map[i] = SETMUSTATE(mapunit, MUS_FALSE);
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
    if (ISMUSTATIC(pGame->map[index])) return RETVAL_BADMUSTATE;

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
    if (isbomb || pGame->secrets == pGame->mines) {
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
    getNeighbors(pGame, pos, index);

    //you can open neighbors only when flags EQ mines
    int flags = 0;
    for (int i = 1; i < NEI_TOTAL; i++)
        flags += (pos[i] != INV_INDEX && GETMUSTATE(pGame->map[pos[i]]) == MUS_FLAG);
    if (GETMUNUMBER(pGame->map[index]) != flags) return RETVAL_BADFLAGNUM;

    //open neighbors
    bool isbomb = false;
    for (int i = 1; i < NEI_TOTAL; i++) {
        int ret = clickOne(pGame, pos[i]);
        if (ret == 0) openBlanks(pGame, pos[i]);
        isbomb |= (ret == MUN_MINE);
    }

    //after open, show positions of all mines when GameOver
    if (isbomb || pGame->secrets == pGame->mines) {
        finishGame(pGame, !isbomb);
        showMines(pGame);
        return (isbomb) ? RETVAL_GAMELOSS : RETVAL_GAMEWIN;
    }
    return RETVAL_NOERROR;
}

int rightClick(PGameInfo pGame, int index, bool qmark)
{
    return (ISGAMEACTIVE(pGame->state)) ? flagOne(pGame, index, qmark) : RETVAL_BADGAMESTATE;
}


/* GameScore related functions */

void resetRecord(PGameScore pScore)
{
    *pScore = (GameScore){ MAX_TIME, MAX_TIME, MAX_TIME, _T(SCORE_NAME_DEFT), _T(SCORE_NAME_DEFT), _T(SCORE_NAME_DEFT) };
}

WORD getRecordTime(PGameScore pScore, BYTE mode)
{
    switch (mode) {
    case GM_JUNIOR: return pScore->junior_time;
    case GM_MIDDLE: return pScore->middle_time;
    case GM_SENIOR: return pScore->senior_time;
    }
    return INV_TIME;
}

LPTSTR getRecordName(PGameScore pScore, BYTE mode)
{
    switch (mode) {
    case GM_JUNIOR: return pScore->junior_name;
    case GM_MIDDLE: return pScore->middle_name;
    case GM_SENIOR: return pScore->senior_name;
    }
    return NULL;
}

int setRecordTime(PGameScore pScore, BYTE mode, WORD time)
{
    time = min(time, MAX_TIME);
    switch (mode) {
    case GM_JUNIOR:
        pScore->junior_time = time;
        return RETVAL_NOERROR;
    case GM_MIDDLE:
        pScore->middle_time = time;
        return RETVAL_NOERROR;
    case GM_SENIOR:
        pScore->senior_time = time;
        return RETVAL_NOERROR;
    }
    return RETVAL_BADGAMEMODE;
}

bool isNewRecord(PGameInfo pGame, PGameScore pScore)
{
    return (ISSTDMOD(pGame->mode) && pGame->state == GS_WIN && pGame->time < getRecordTime(pScore, pGame->mode));
}
