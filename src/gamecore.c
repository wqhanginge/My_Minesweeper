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
 * gamecore.c
 *****************************************************************************
 * This file contains the minesweeper core functions.
 * All basic operations of Game are defined in this file.
\*****************************************************************************/


#include "stdafx.h"
#include "gamecore.h"


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

void setGameMode(PGameInfo pGame, BYTE mode, BYTE width, BYTE height, WORD mines)
{
    switch (mode) {
    case JUNIOR:
        pGame->mode = JUNIOR;
        pGame->state = INIT;
        pGame->width = JUNIOR_WIDTH;
        pGame->height = JUNIOR_HEIGHT;
        pGame->size = JUNIOR_SIZE;
        pGame->mines = JUNIOR_MINES;
        pGame->mine_remains = JUNIOR_MINES;
        pGame->uncov_units = 0;
        pGame->time = 0;
        memset(pGame->map, 0, sizeof(BYTE) * pGame->size);
        break;
    case MIDDLE:
        pGame->mode = MIDDLE;
        pGame->state = INIT;
        pGame->width = MIDDLE_WIDTH;
        pGame->height = MIDDLE_HEIGHT;
        pGame->size = MIDDLE_SIZE;
        pGame->mines = MIDDLE_MINES;
        pGame->mine_remains = MIDDLE_MINES;
        pGame->uncov_units = 0;
        pGame->time = 0;
        memset(pGame->map, 0, sizeof(BYTE) * pGame->size);
        break;
    case SENIOR:
        pGame->mode = SENIOR;
        pGame->state = INIT;
        pGame->width = SENIOR_WIDTH;
        pGame->height = SENIOR_HEIGHT;
        pGame->size = SENIOR_SIZE;
        pGame->mines = SENIOR_MINES;
        pGame->mine_remains = SENIOR_MINES;
        pGame->uncov_units = 0;
        pGame->time = 0;
        memset(pGame->map, 0, sizeof(BYTE) * pGame->size);
        break;
    case CUSTOM:
        pGame->mode = CUSTOM;
        pGame->state = INIT;
        pGame->width = min(max(width, MIN_WIDTH), MAX_WIDTH);
        pGame->height = min(max(height, MIN_HEIGHT), MAX_HEIGHT);
        pGame->size = pGame->width * pGame->height;
        pGame->mines = min(max(mines, MIN_MINES), MAX_MINES(pGame->size));
        pGame->mine_remains = (short)pGame->mines;
        pGame->uncov_units = 0;
        pGame->time = 0;
        memset(pGame->map, 0, sizeof(BYTE) * pGame->size);
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

void setGameTime(PGameInfo pGame, WORD time)
{
    pGame->time = min(time, MAX_TIME);
}

void stepGameTime(PGameInfo pGame)
{
    pGame->time++;
}

void resetGame(PGameInfo pGame)
{
    pGame->state = INIT;
    pGame->mine_remains = pGame->mines;
    pGame->uncov_units = 0;
    pGame->time = 0;
    memset(pGame->map, 0, sizeof(BYTE) * pGame->size);
}

int createGameMap(PGameInfo pGame, int index)
{
    if (pGame->state != INIT) return RETVAL_BADGAMESTATE;
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;

    Neighbor safepos;
    getNeighbors(pGame, safepos, index2x(pGame, index), index2y(pGame, index));

    //generate mines, 8 units around where clicked won't have mines
    //use shuffle algorithm
    memset(pGame->map, 0, sizeof(BYTE) * pGame->size);  //clear the whole map
    memset(pGame->map, MU_MINE, sizeof(BYTE) * pGame->mines);   //generate mines
    DWORD neicount = 0;
    for (int i = 0; i < NEI_TOTAL; i++) neicount += (safepos[i] != INV_INDEX);  //remember how many safe positions needed
    for (DWORD k = 0; k < pGame->size - neicount; k++) {    //shuffle algorithm, ignore reserved tail
        DWORD index = rand() % (pGame->size - neicount - k) + k;
        BYTE temp = pGame->map[index];
        pGame->map[index] = pGame->map[k];
        pGame->map[k] = temp;
    }

    //shift the safe area center unit index to the middle of Neighbor list
    for (int i = 0; i < NEI_TOTAL / 2; i++) {
        int temp = safepos[i];
        safepos[i] = safepos[i + 1];
        safepos[i + 1] = temp;
    }

    //move safe area to where it is
    for (int i = 0; i < NEI_TOTAL; i++) {
        if (safepos[i] != INV_INDEX) {
            BYTE temp = pGame->map[pGame->size - neicount];
            pGame->map[pGame->size - neicount] = pGame->map[safepos[i]];
            pGame->map[safepos[i]] = temp;
            neicount--;
        }
    }

    //calculate mines around each unit
    for (DWORD i = 0; i < pGame->size; i++) {
        if (MUISMINE(pGame->map[i])) continue;
        int m = 0;
        Neighbor neipos;
        getNeighbors(pGame, neipos, index2x(pGame, i), index2y(pGame, i));
        for (int j = 1; j < NEI_TOTAL; j++)
            if (neipos[j] != INV_INDEX && MUISMINE(pGame->map[neipos[j]])) m++;
        SETMUMINES(m, pGame->map[i]);
    }
    return RETVAL_NOERROR;
}

int clickOne(PGameInfo pGame, int index)
{
    if (pGame->state != RUNNING) return RETVAL_BADGAMESTATE;
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;
    if (!MUISCLICKABLE(pGame->map[index])) return RETVAL_BADMUSTATE;

    pGame->uncov_units++;
    if (MUISMINE(pGame->map[index])) {  //bomb
        SETMUSTATE(MUS_BOMB, pGame->map[index]);
        return RETVAL_GAMEFAIL;
    }
    else {  //safe
        SETMUSTATE(MUS_UNCOV, pGame->map[index]);
        return GETMUMINES(pGame->map[index]);
    }
}

int openBlanks(PGameInfo pGame, int index)
{
    if (pGame->state != RUNNING) return RETVAL_BADGAMESTATE;
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;
    if (GETMUSTATE(pGame->map[index]) != MUS_UNCOV) return RETVAL_BADMUSTATE;
    if (GETMUMINES(pGame->map[index]) != 0) return RETVAL_BADMUSTATE;

    Neighbor pos;
    getNeighbors(pGame, pos, index2x(pGame, index), index2y(pGame, index));
    for (int i = 1; i < NEI_TOTAL; i++) {   //no need to take care of INV_INDEX
        int ret = clickOne(pGame, pos[i]);
        if (ret == 0) openBlanks(pGame, pos[i]);    //do in a recursive manner
    }
    return RETVAL_NOERROR;
}

int flagOne(PGameInfo pGame, int index)
{
    if (ISGAMESET(pGame->state)) return RETVAL_BADGAMESTATE;
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;

    //take effect only on covered units
    BYTE new_mapunit_state;
    switch (GETMUSTATE(pGame->map[index])) {
    case MUS_COVER:
        new_mapunit_state = MUS_FLAG;
        pGame->mine_remains--;
        break;
    case MUS_FLAG:
        new_mapunit_state = (pGame->mark) ? MUS_MARK : MUS_COVER;
        pGame->mine_remains++;
        break;
    case MUS_MARK:
        new_mapunit_state = MUS_COVER;
        break;
    default:
        return RETVAL_BADMUSTATE;
    }

    SETMUSTATE(new_mapunit_state, pGame->map[index]);
    return RETVAL_NOERROR;
}

int showAllMines(PGameInfo pGame)
{
    if (!ISGAMESET(pGame->state)) return RETVAL_BADGAMESTATE;
    for (WORD i = 0; i < pGame->size; i++) {
        BYTE mapunit = pGame->map[i];
        if (MUISMINE(mapunit) && MUISCLICKABLE(mapunit)) {
            BYTE mustate = (pGame->state == SUCCESS) ? MUS_FLAG : MUS_UNCOV;
            SETMUSTATE(mustate, pGame->map[i]);
        }
        else if (!MUISMINE(mapunit) && GETMUSTATE(mapunit) == MUS_FLAG) {
            SETMUSTATE(MUS_WRONG, pGame->map[i]);
        }
    }
    return RETVAL_NOERROR;
}

bool isMapFullyOpen(PGameInfo pGame)
{
    return (pGame->uncov_units == pGame->size - pGame->mines);
}

bool isFirstClick(PGameInfo pGame, int index)
{
    return (pGame->state == INIT && MUISCLICKABLE(pGame->map[index]));
}

int leftClick(PGameInfo pGame, int index)
{
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;

    //first click
    if (isFirstClick(pGame, index)) {
        //create map and init game infomation
        createGameMap(pGame, index);
        pGame->state = RUNNING;
        pGame->mine_remains = pGame->mines;
        pGame->uncov_units = 0;
        pGame->time = 0;
    }

    //normal click
    int ret = clickOne(pGame, index);
    if (ret == 0) openBlanks(pGame, index);

    //after click
    if (ret == RETVAL_GAMEFAIL || isMapFullyOpen(pGame)) {
        ret = (ret == RETVAL_GAMEFAIL) ? RETVAL_GAMEFAIL : RETVAL_GAMESUCCESS;
        pGame->state = (ret == RETVAL_GAMEFAIL) ? FAIL : SUCCESS;
        //show all mines' positions when game set
        pGame->mine_remains = 0;
        showAllMines(pGame);
    }
    return ret;
}

int clickAround(PGameInfo pGame, int index)
{
    if (pGame->state != RUNNING) return RETVAL_BADGAMESTATE;
    if (!isidxinmap(pGame, index)) return RETVAL_INDEXOOR;
    if (GETMUSTATE(pGame->map[index]) != MUS_UNCOV) return RETVAL_BADMUSTATE;

    Neighbor pos;
    getNeighbors(pGame, pos, index2x(pGame, index), index2y(pGame, index));

    //you can open neighbors only when flags EQ mines around neighbors
    int flags = 0;
    for (int i = 1; i < NEI_TOTAL; i++)
        if (pos[i] != INV_INDEX && GETMUSTATE(pGame->map[pos[i]]) == MUS_FLAG) flags++;
    if (GETMUMINES(pGame->map[pos[0]]) != flags) return RETVAL_BADFLAGNUM;

    flags = RETVAL_NOERROR; //WARNING:the meaning of flags has been changed
    //open neighbors
    for (int i = 1; i < NEI_TOTAL; i++) {
        int ret = clickOne(pGame, pos[i]);
        if (ret == 0) openBlanks(pGame, pos[i]);
        if (ret == RETVAL_GAMEFAIL) flags = RETVAL_GAMEFAIL;    //if bombed
    }

    //after open
    if (flags == RETVAL_GAMEFAIL || isMapFullyOpen(pGame)) {
        flags = (flags == RETVAL_GAMEFAIL) ? RETVAL_GAMEFAIL : RETVAL_GAMESUCCESS;
        pGame->state = (flags == RETVAL_GAMEFAIL) ? FAIL : SUCCESS;
        //show all mines' positions when game set
        pGame->mine_remains = 0;
        showAllMines(pGame);
    }
    return flags;
}

int rightClick(PGameInfo pGame, int index)
{
    return flagOne(pGame, index);
}

void resetRecord(PGameScore pScore)
{
    *pScore = (GameScore){ MAX_TIME, MAX_TIME, MAX_TIME, _T(DEF_SCORE_NAME_EN), _T(DEF_SCORE_NAME_EN), _T(DEF_SCORE_NAME_EN) };
}

WORD getRecordTime(PGameScore pScore, BYTE gamemode)
{
    switch (gamemode) {
    case JUNIOR: return pScore->junior_time;
    case MIDDLE: return pScore->middle_time;
    case SENIOR: return pScore->senior_time;
    }
    return INV_TIME;
}

LPTSTR getpRecordName(PGameScore pScore, BYTE gamemode)
{
    switch (gamemode) {
    case JUNIOR: return pScore->junior_name;
    case MIDDLE: return pScore->middle_name;
    case SENIOR: return pScore->senior_name;
    }
    return NULL;
}

int setRecordTime(PGameScore pScore, BYTE gamemode, WORD besttime)
{
    WORD btime = min(besttime, MAX_TIME);
    switch (gamemode) {
    case JUNIOR:
        pScore->junior_time = btime;
        return RETVAL_NOERROR;
    case MIDDLE:
        pScore->middle_time = btime;
        return RETVAL_NOERROR;
    case SENIOR:
        pScore->senior_time = btime;
        return RETVAL_NOERROR;
    }
    return RETVAL_BADGAMEMODE;
}

bool isNewRecord(PGameInfo pGame, PGameScore pScore)
{
    return (ISSTDMOD(pGame->mode) && pGame->state == SUCCESS && pGame->time < getRecordTime(pScore, pGame->mode));
}
