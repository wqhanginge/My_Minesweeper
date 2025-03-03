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
 * gamecore.h
 *****************************************************************************
 * This file contains the minesweeper core data structures and functions.
 * All basic operations of Game are defined in this file.
 *
 * NOTE:
 *   Access its members directly to READ from a struct.
 *   Use defined functions to WRITE to a struct (RECOMMENDED).
\*****************************************************************************/


#pragma once

#include "stdafx.h"



//Game Modes
#define JUNIOR          0
#define MIDDLE          1
#define SENIOR          2
#define CUSTOM          3
#define CRUSH           4
#define ISSTDMOD(mode)  ((BYTE)(mode) < CUSTOM)
#define ISCRUSH(mode)   ((BYTE)(mode) >= CRUSH)

#define JUNIOR_WIDTH    9
#define JUNIOR_HEIGHT   9
#define JUNIOR_SIZE     (JUNIOR_WIDTH * JUNIOR_HEIGHT)
#define JUNIOR_MINES    10

#define MIDDLE_WIDTH    16
#define MIDDLE_HEIGHT   16
#define MIDDLE_SIZE     (MIDDLE_WIDTH * MIDDLE_HEIGHT)
#define MIDDLE_MINES    40

#define SENIOR_WIDTH    30
#define SENIOR_HEIGHT   16
#define SENIOR_SIZE     (SENIOR_WIDTH * SENIOR_HEIGHT)
#define SENIOR_MINES    99

#define MAX_WIDTH       30
#define MAX_HEIGHT      24
#define MAX_SIZE        (MAX_WIDTH * MAX_HEIGHT)
#define MIN_WIDTH       9
#define MIN_HEIGHT      9
#define MIN_SIZE        (MIN_WIDTH * MIN_HEIGHT)
#define MAX_MINES(size) ((WORD)((WORD)(size) * 9 / 10))
#define MIN_MINES       10
//end Game Modes

//Game States
#define GS_INIT             0
#define GS_RUNNING          1
#define GS_WIN              2
#define GS_LOSS             3
#define GS_UNKNOW           4
#define ISBADSTATE(state)   ((BYTE)(state) >= GS_UNKNOW)
#define ISGAMEACTIVE(state) ((BYTE)(state) <= GS_RUNNING)
#define ISGAMEOVER(state)   ((BYTE)(state) >= GS_WIN && (BYTE)(state) <= GS_LOSS)
//end Game States

//Game Map Unit
/*  8 7 6 5 4 3 2 1
 * +-+-+-+-+-+-+-+-+
 * |F|  S  |   N   |
 * +-+-+-+-+-+-+-+-+
 *     map_unit
 * F: Set this bit if this Unit is a mine.
 * S: State of this Unit:[covered, marked, flagged, uncovered, bombed, false-flagged].
 * N: The number of mines in its neighbors, from 0x0 to 0x8. Set to MUN_MINE if this Unit is a mine.
 */

#define MUF_MINE        0x80
#define MUN_MINE        0x0F
#define MUS_COVER       0x00
#define MUS_MARK        0x10
#define MUS_FLAG        0x20
#define MUS_BARE        0x30
#define MUS_BOMB        0x40
#define MUS_FALSE       0x50
#define GETMUNUMBER(unit)       ((BYTE)((BYTE)(unit) & 0x0F))
#define GETMUSTATE(unit)        ((BYTE)((BYTE)(unit) & 0x70))
#define SETMUNUMBER(unit, n)    (unit = ((BYTE)((BYTE)((BYTE)(unit) & 0xF0) | (BYTE)((BYTE)(n) & 0x0F))))
#define SETMUSTATE(unit, s)     (unit = ((BYTE)((BYTE)((BYTE)(unit) & 0x8F) | (BYTE)((BYTE)(s) & 0x70))))
#define ISMUMINE(unit)          ((BYTE)(unit) & MUF_MINE)
#define ISMUCLICKABLE(unit)     (GETMUSTATE(unit) <= MUS_MARK)
//end Game Map Unit

//Game Map Index
#define INV_INDEX   (-1)
#define NEI_TOTAL   9
//end Game Map Index

//Game Score
#define SCORE_NAME_LEN      20
#define DEF_SCORE_NAME_EN   "anonymous"

#define INV_TIME    ((WORD)-1)
#define MAX_TIME    999
//end Game Score

//Function Returns
#define RETVAL_NOERROR          0
#define RETVAL_INDEXOOR         (-1)
#define RETVAL_BADGAMEMODE      (-2)
#define RETVAL_BADGAMESTATE     (-3)
#define RETVAL_BADMUSTATE       (-4)
#define RETVAL_BADFLAGNUM       (-5)
#define RETVAL_GAMESTART        (-6)
#define RETVAL_GAMEWIN          (-7)
#define RETVAL_GAMELOSS         (-8)
//end Function Returns



/* order in 2D:
 * +---+---+---+
 * | 1 | 2 | 3 |           transform to arrary:
 * +---+---+---+      +---+---+---+---+---+---+---+---+---+
 * | 4 | 0 | 5 | ---> | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
 * +---+---+---+      +---+---+---+---+---+---+---+---+---+
 * | 6 | 7 | 8 |
 * +---+---+---+
 * Stores index of each neighbor in a list with NEI_TOTAL elements,
 * value INV_INDEX means no such neighbor.
 */
typedef int Neighbor[NEI_TOTAL];

typedef struct _GameInfo {
    BYTE mode;          //GameMode:[junior, middle, senior, custom]
    BYTE width;         //GameMap width: map_units per line
    BYTE height;        //GameMap height: map_units per column
    WORD size;          //GameMap size: width x height
    WORD mines;         //counts of mines in GameMap
    bool mark;          //if the QuestionMark is enabled
    BYTE state;         //GameState:[init, running, win, loss]
    WORD flags;         //counts of MapUnits that have been flagged
    WORD bare_units;    //counts of MapUnits that have been uncovered
    WORD time;          //GameTime
    BYTE map[MAX_SIZE]; //GameMap data, avaliable area depends on GameMode
} GameInfo, * PGameInfo;

typedef struct _GameScore {
    WORD junior_time;
    WORD middle_time;
    WORD senior_time;
    TCHAR junior_name[SCORE_NAME_LEN];
    TCHAR middle_name[SCORE_NAME_LEN];
    TCHAR senior_name[SCORE_NAME_LEN];
} GameScore, * PGameScore;



/* GameMap position related functions */

/* GameMap in 2D view
 *        x
 *      ----->
 *       0   1   2   3
 *     +---+---+---+---+
 * y| 0| 0 | 1 | 2 | 3 |    eg: {index = 2} <==> {(x, y) = (2, 0)}
 *  |  +---+---+---+---+
 *  v 1| 4 |...index...|
 *     +---+---+---+---+
 */

//Transform between GameMap index-type and xy-type.
//Start from 0 and NO ARG CHECK.
int index2x(PGameInfo pGame, int index);
int index2y(PGameInfo pGame, int index);
int xy2index(PGameInfo pGame, int x, int y);

//Check if the MapUnit index is valid.
bool isxyinmap(PGameInfo pGame, int x, int y);
bool isidxinmap(PGameInfo pGame, int index);

//Get the index of each neighbor of the given Unit, INV_INDEX means no such neighbor.
//See the description of Type Neighbor for details.
//Return RETVAL_NOERROR if the call succeeded,
//or RETVAL_INDEXOOR if the given Unit is out of range.
int getNeighbors(PGameInfo pGame, Neighbor neighbor, int x, int y);


/* Game property related functions */

//Set GameMode with specified value.
//If 'mode' is an undefined value, set GameMode to JUNIOR by default.
//If 'mode' is a standard GameMode(not CUSTOM), 'width', 'height' and 'mines' will be ignored.
//CUSTOM is limited by MAX_WIDTH, MAX_HEIGHT, MAX_MINES and MIN***.
//This function sets the Game to an uninit state,
//make sure to call resetGame() right after calling this function to init Game properly.
void setGameMode(PGameInfo pGame, BYTE mode, BYTE width, BYTE height, WORD mines);

//Enable or disable QuestionMark function.
void setMark(PGameInfo pGame, bool enable);


/* Game runtime basic functions */

//Erase the GameMap and reset Game to the init state without changing the GameMode.
void resetGame(PGameInfo pGame);

//Switch the Game to the running state and reset the MapUnit counters.
void startGame(PGameInfo pGame);

//Stop the Game and set it to win or loss.
void finishGame(PGameInfo pGame, bool win);


//Create a new GameMap with a safe area at the given position.
//Return RETVAL_NOERROR if the call succeeded,
//or RETVAL_INDEXOOR if the index is out of range.
int createGameMap(PGameInfo pGame, int index);

//Click a Unit in GameMap.
//Return the number of mines in its neighbors, where MUN_MINE means this Unit is a mine,
//or RETVAL_INDEXOOR if the index is out of range,
//or RETVAL_BADMUSTATE if this Unit cannot be clicked.
int clickOne(PGameInfo pGame, int index);

//Open all neighbors around an uncovered Unit which has no mine in its neighbors.
//Return RETVAL_NOERROR if the call succeeded,
//or RETVAL_INDEXOOR if the index is out of range,
//or RETVAL_BADMUSTATE if this Unit does not satisfied with requirements.
int openBlanks(PGameInfo pGame, int index);

//Set or remove the flag on a coverd Unit in GameMap.
//This function changes the state of given Unit in cycle:[covered -> flagged -> covered],
//if the QuestionMark is enabled, the cycle will be:[covered -> flagged -> marked -> covered].
//Return RETVAL_NOERROR if the call succeeded,
//or RETVAL_INDEXOOR if the index is out of range,
//or RETVAL_BADMUSTATE if the MapUnitState is not one of the three states mentioned above.
int flagOne(PGameInfo pGame, int index);


//Show positions of all mines.
void showMines(PGameInfo pGame);

//Set GameTime with specified time.
//The time value will be truncated to MAX_TIME automatically.
void setGameTime(PGameInfo pGame, WORD time);

//Increment GameTime by one.
void stepGameTime(PGameInfo pGame);


/* Game runtime high-level functions */

//Click the given Unit and open all blanks if it is blank,
//or open the given Unit only if it is not blank.
//This function will start a new Game then click if it is the first click.
//Return the number of mines in neighbors around the given Unit,
//or RETVAL_GAMESTART if a new Game starts,
//or RETVAL_GAMEWIN if all blanks are already open,
//or RETVAL_GAMELOSS if this Unit is a mine,
//or RETVAL_INDEXOOR if the index is out of range,
//or RETVAL_BADGAMESTATE if the Game is already finished,
//or RETVAL_BADMUSTATE if this Unit cannot be clicked.
int leftClick(PGameInfo pGame, int index);


//Open all neighbors around the uncovered Unit.
//This function implements the operation of pressing both mouse buttons at the same time.
//Return RETVAL_NOERROR if the call succeeded,
//or RETVAL_GAMEWIN if all blanks are already open,
//or RETVAL_GAMELOSS if a mine is open,
//or RETVAL_INDEXOOR if the index is out of range,
//or RETVAL_BADGAMESTATE if the Game is not running,
//or RETVAL_BADMUSTATE if this Unit is not uncovered,
//or RETVAL_BADFLAGNUM if the number of flags around this Unit does not match the value stored in it.
int simulClick(PGameInfo pGame, int index);


//Flag, mark or remove flag on the covered Unit.
//Return RETVAL_NOERROR if the call succeeded,
//or RETVAL_INDEXOOR if the index is out of range,
//or RETVAL_BADGAMESTATE if the Game is already finished,
//or RETVAL_BADMUSTATE if this Unit cannot be flagged.
int rightClick(PGameInfo pGame, int index);


/* GameScore related functions */

//Reset all scores.
void resetRecord(PGameScore pScore);

//Get the time record under given GameMode.
//Return INV_TIME if the given GameMode is not valid.
WORD getRecordTime(PGameScore pScore, BYTE mode);

//Get the player name under given GameMode.
//This function returns the head address of TCHAR[SCORE_NAME_LEN].
//You can directly edit Record content using this pointer,
//but you must be careful with array bound.
//Return NULL if the given GameMode is not valid.
LPTSTR getRecordName(PGameScore pScore, BYTE mode);

//Update the time record under given GameMode.
//The time value will be truncated to MAX_TIME automatically.
//Return RETVAL_BADGAMEMODE if the given GameMode is not valid.
int setRecordTime(PGameScore pScore, BYTE mode, WORD time);


//Check if break the record.
bool isNewRecord(PGameInfo pGame, PGameScore pScore);
