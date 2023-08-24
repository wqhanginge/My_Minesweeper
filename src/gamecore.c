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
    if (!isxyinmap(pGame, x, y)) return -1;
    neighbor[0] = xy2index(pGame, x, y);
    neighbor[1] = (x > 0 && y > 0) ? xy2index(pGame, x - 1, y - 1) : -1;
    neighbor[2] = (y > 0) ? xy2index(pGame, x, y - 1) : -1;
    neighbor[3] = (x < pGame->width - 1 && y > 0) ? xy2index(pGame, x + 1, y - 1) : -1;
    neighbor[4] = (x > 0) ? xy2index(pGame, x - 1, y) : -1;
    neighbor[5] = (x < pGame->width - 1) ? xy2index(pGame, x + 1, y) : -1;
    neighbor[6] = (x > 0 && y < pGame->height - 1) ? xy2index(pGame, x - 1, y + 1) : -1;
    neighbor[7] = (y < pGame->height - 1) ? xy2index(pGame, x, y + 1) : -1;
    neighbor[8] = (x < pGame->width - 1 && y < pGame->height - 1) ? xy2index(pGame, x + 1, y + 1) : -1;
    return 0;
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
        pGame->width = (width < MIN_WIDTH) ? MIN_WIDTH : width;
        pGame->width = (pGame->width > MAX_WIDTH) ? MAX_WIDTH : pGame->width;
        pGame->height = (height < MIN_HEIGHT) ? MIN_HEIGHT : height;
        pGame->height = (pGame->height > MAX_HEIGHT) ? MAX_HEIGHT : pGame->height;
        pGame->size = pGame->width * pGame->height;
        pGame->mines = (mines > MAX_MINES(pGame->size)) ? MAX_MINES(pGame->size) : mines;
        pGame->mines = (pGame->mines < MIN_MINES) ? MIN_MINES : pGame->mines;
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

int setGameState(PGameInfo pGame, BYTE state)
{
    if (ISBADSTATE(state)) return -1;
    pGame->state = state;
    return 0;
}

void setMark(PGameInfo pGame, bool enable)
{
    pGame->mark = enable;
}

int setGameTime(PGameInfo pGame, WORD time)
{
    if (time > MAX_TIME) return -1;
    pGame->time = time;
    return 0;
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
    if (pGame->state != INIT) return -1;
    if (!isidxinmap(pGame, index)) return -1;

    Neighbor safepos;
    getNeighbors(pGame, safepos, index2x(pGame, index), index2y(pGame, index));

    //generate mines, 8 units around where clicked won't have mines
    //use shuffle algorithm
    memset(pGame->map, 0, sizeof(BYTE) * pGame->size);  //clear the whole map
    memset(pGame->map, MU_MINE, sizeof(BYTE) * pGame->mines);   //generate mines
    DWORD neicount = 0;
    for (int i = 0; i < NEI_TOTAL; i++) neicount += (safepos[i] != -1); //remember how many safe positions needed
    for (DWORD k = 0; k < pGame->mines; k++) {  //shuffle algorithm, ignore reserved tail
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
        if (safepos[i] != -1) {
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
            if (neipos[j] != -1 && MUISMINE(pGame->map[neipos[j]])) m++;
        SETMUMINES(m, pGame->map[i]);
    }
    return 0;
}

int clickOne(PGameInfo pGame, int index)
{
    if (pGame->state != RUNNING) return -2;
    if (!isidxinmap(pGame, index)) return -2;
    if (!MUISCLICKABLE(pGame->map[index])) return -2;

    pGame->uncov_units++;
    if (MUISMINE(pGame->map[index])) {  //bomb
        SETMUSTATE(MUS_BOMB, pGame->map[index]);
        return -1;
    }
    else {  //safe
        SETMUSTATE(MUS_UNCOV, pGame->map[index]);
        return GETMUMINES(pGame->map[index]);
    }
}

int openBlanks(PGameInfo pGame, int index)
{
    if (pGame->state != RUNNING) return -1;
    if (!isidxinmap(pGame, index)) return -1;
    if (GETMUSTATE(pGame->map[index]) != MUS_UNCOV) return -1;
    if (GETMUMINES(pGame->map[index]) != 0) return -1;

    Neighbor pos;
    getNeighbors(pGame, pos, index2x(pGame, index), index2y(pGame, index));
    for (int i = 1; i < NEI_TOTAL; i++) {
        int ret = clickOne(pGame, pos[i]);
        if (ret == 0) openBlanks(pGame, pos[i]);    //do in a recursive manner
    }
    return 0;
}

void showAllMines(PGameInfo pGame)
{
    if (pGame->state == FAIL) {
        for (WORD i = 0; i < pGame->size; i++) {
            if (MUISMINE(pGame->map[i]) && MUISCLICKABLE(pGame->map[i])) {
                SETMUSTATE(MUS_UNCOV, pGame->map[i]);
            }
            else if (!MUISMINE(pGame->map[i]) && GETMUSTATE(pGame->map[i]) == MUS_FLAG) {
                SETMUSTATE(MUS_WRONG, pGame->map[i]);
            }
        }
    }
    else if (pGame->state == SUCCESS) {
        for (WORD i = 0; i < pGame->size; i++) {
            if (MUISMINE(pGame->map[i]) && MUISCLICKABLE(pGame->map[i])) {
                SETMUSTATE(MUS_FLAG, pGame->map[i]);
            }
        }
    }
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
    if (!isidxinmap(pGame, index)) return -3;

    //first click
    if (isFirstClick(pGame, index)) {
        //create map and init game infomation
        if (createGameMap(pGame, index) == -1) return -3;
        pGame->state = RUNNING;
        pGame->mine_remains = pGame->mines;
        pGame->uncov_units = 0;
        pGame->time = 0;
    }

    //normal click
    if (pGame->state != RUNNING) return -3;
    if (!MUISCLICKABLE(pGame->map[index])) return -3;

    int ret = clickOne(pGame, index);
    if (ret == 0) openBlanks(pGame, index);

    //after click
    if (ret == -1 || isMapFullyOpen(pGame)) {
        ret = (ret == -1) ? -1 : -2;
        pGame->state = (ret == -1) ? FAIL : SUCCESS;
        //show all mines' positions when game set
        pGame->mine_remains = 0;
        showAllMines(pGame);
    }
    return ret;
}

int clickAround(PGameInfo pGame, int index)
{
    if (pGame->state != RUNNING) return -3;
    if (!isidxinmap(pGame, index)) return -3;
    if (GETMUSTATE(pGame->map[index]) != MUS_UNCOV) return -3;

    Neighbor pos;
    getNeighbors(pGame, pos, index2x(pGame, index), index2y(pGame, index));

    //you can open neighbors only when flags EQ mines around neighbors
    int flags = 0;
    for (int i = 1; i < NEI_TOTAL; i++)
        if (pos[i] != -1 && GETMUSTATE(pGame->map[pos[i]]) == MUS_FLAG) flags++;
    if (GETMUMINES(pGame->map[pos[0]]) != flags) return -3;

    flags = 0;  //WARNING:the meaning of flags has been changed
    //open neighbors
    for (int i = 1; i < NEI_TOTAL; i++) {
        int ret = clickOne(pGame, pos[i]);
        if (ret == 0) openBlanks(pGame, pos[i]);
        if (ret == -1) flags = -1;  //if bombed
    }

    //after open
    if (flags == -1 || isMapFullyOpen(pGame)) {
        flags = (flags == -1) ? -1 : -2;
        pGame->state = (flags == -1) ? FAIL : SUCCESS;
        //show all mines' positions when game set
        pGame->mine_remains = 0;
        showAllMines(pGame);
    }
    return flags;
}

int rightClick(PGameInfo pGame, int index)
{
    if (ISGAMESET(pGame->state)) return -2;
    if (!isidxinmap(pGame, index)) return -2;

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
        return -1;
    }

    SETMUSTATE(new_mapunit_state, pGame->map[index]);
    return 0;
}

void resetRecord(PGameScore pScore)
{
    *pScore = (GameScore){ MAX_TIME,MAX_TIME,MAX_TIME,_T(DEF_SCORE_NAME_EN),_T(DEF_SCORE_NAME_EN),_T(DEF_SCORE_NAME_EN) };
}

WORD getRecordTime(PGameScore pScore, BYTE gamemode)
{
    switch (gamemode) {
    case JUNIOR: return pScore->junior_time;
    case MIDDLE: return pScore->middle_time;
    case SENIOR: return pScore->senior_time;
    }
    return -1;
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
    if (besttime > MAX_TIME) return -1;
    switch (gamemode) {
    case JUNIOR:
        pScore->junior_time = besttime;
        return 0;
    case MIDDLE:
        pScore->middle_time = besttime;
        return 0;
    case SENIOR:
        pScore->senior_time = besttime;
        return 0;
    }
    return -1;
}

bool isNewRecord(PGameInfo pGame, PGameScore pScore)
{
    return (ISSTDMOD(pGame->mode) && pGame->state == SUCCESS && pGame->time < getRecordTime(pScore, pGame->mode));
}
