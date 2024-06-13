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
 * gamecore.h
 *****************************************************************************
 * This file contains the minesweeper core data structures and functions.
 * All basic operations of Game are defined in this file.
 * 
 * NOTE:
 *   Accessing its member directly to READ from struct.
 *   Using defined functions to WRITE to struct (RECOMMENDED).
\*****************************************************************************/


#pragma once

#include "stdafx.h"



//Game Modes
#define JUNIOR          0
#define MIDDLE          1
#define SENIOR          2
#define CUSTOM          3
#define CRUSH           4
#define ISSTDMOD(Mode)  ((BYTE)(Mode) < CUSTOM)
#define ISCRUSH(Mode)   ((BYTE)(Mode) >= CRUSH)

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
#define MAX_MINES(Size) ((WORD)((WORD)(Size) * 9 / 10))
#define MIN_MINES       10
//end Game Modes

//Game States
#define INIT                0
#define RUNNING             1
#define FAIL                2
#define SUCCESS             3
#define UNKNOW              4
#define ISBADSTATE(State)   ((BYTE)(State) >= UNKNOW)
#define ISINPROGRESS(State) ((BYTE)(State) <= RUNNING)
#define ISGAMESET(State)    ((BYTE)(State) >= FAIL && (BYTE)(State) <= SUCCESS)
//end Game States

//Game Map Unit
/*  8 7 6 5 4 3 2 1
 * +-+-+-+-+-+-+-+-+
 * |M|  S  |   N   |
 * +-+-+-+-+-+-+-+-+
 *     map_unit
 * M:   If this unit is a mine.
 * S:   State of this unit:[covered, marked, flagged, uncovered, bombed, wrong flag].
 * N:   Contains the number of mines in neighbors, from 0x0 to 0x8.
 */

#define MUF_MINE        0x80
#define MUS_COVER       0x00
#define MUS_MARK        0x10
#define MUS_FLAG        0x20
#define MUS_UNCOV       0x30
#define MUS_BOMB        0x40
#define MUS_WRONG       0x50
#define GETMUNUMBER(unit)       ((BYTE)((unit) & 0x0F))
#define GETMUSTATE(unit)        ((BYTE)((unit) & 0x70))
#define SETMUNUMBER(n, unit)    (unit = ((BYTE)((BYTE)((n) & 0x0F) | (BYTE)((unit) & 0xF0))))
#define SETMUSTATE(S, unit)     (unit = ((BYTE)((BYTE)((S) & 0x70) | (BYTE)((unit) & 0x8F))))
#define ISMUMINE(unit)          ((unit) & MUF_MINE)
#define ISMUCLICKABLE(unit)     (GETMUSTATE(unit) <= MUS_MARK)
//end Game Map Unit

//Game Map Position
#define INV_INDEX   (-1)
#define NEI_TOTAL   9
//end Game Map Position

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
#define RETVAL_GAMEFAIL         (-6)
#define RETVAL_GAMESUCCESS      (-7)
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
    bool mark;          //if the QuestionMark is used
    BYTE state;         //GameState:[init, running, fail, success]
    short mine_remains; //counts of mines that haven't been flagged
    WORD uncov_units;   //counts of MapUnits that have been uncovered
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



/* GameMap position relative functions */

/* GameMap in 2D view
 *        x
 *      ----->
 *       0   1   2   3
 *     +---+---+---+---+
 * y| 0| 0 | 1 | 2 | 3 |    eg: index = 2  <==>  (x, y) = (2, 0)
 *  |  +---+---+---+---+
 *  v 1| 4 |...index...|
 *     +---+---+---+---+
 */

//Transform between GameMap index-type and xy-type.
//Start from 0 and NO ARG CHECK.
int index2x(PGameInfo pGame, int index);
int index2y(PGameInfo pGame, int index);
int xy2index(PGameInfo pGame, int x, int y);

//Check if the unit index is valid.
bool isxyinmap(PGameInfo pGame, int x, int y);
bool isidxinmap(PGameInfo pGame, int index);

//Get all neighbors' index which around the given unit, INV_INDEX means no such neighbor.
//See the description of Type Neighbor for details.
//Return RETVAL_NOERROR if the call succeeded,
//or RETVAL_INDEXOOR if the given unit is out of range.
int getNeighbors(PGameInfo pGame, Neighbor neighbor, int x, int y);


/* Game property relative functions */

//Set GameMode with JUNIOR by default if 'mode' is an undefined value.
//If 'mode' is a standard value(not CUSTOM), 'width', 'height' and 'mines' will be ignored.
//CUSTOM is limited by MAX_WIDTH, MAX_HEIGHT, MAX_MINES and MIN***.
//This function sets the GameState to UNKNOW.
//Make sure to call resetGame right after calling this function to init Game properly.
void setGameMode(PGameInfo pGame, BYTE mode, BYTE width, BYTE height, WORD mines);

//Enable or disable QuestionMark mode.
void setMark(PGameInfo pGame, bool enable);

//Set GameTime with specified time.
//The time value will be limited to MAX_TIME automatically.
void setGameTime(PGameInfo pGame, WORD time);

//Step in time.
void stepGameTime(PGameInfo pGame);


/* Game runtime basic functions */

//Erase the GameMap and reset Game to INIT without changing the GameMode.
void resetGame(PGameInfo pGame);

//Create a new GameMap with a safe area at given position.
//Return RETVAL_NOERROR if the call succeeded,
//or RETVAL_INDEXOOR if the index is out of range,
//or RETVAL_BADGAMESTATE if the GameState is not INIT.
int createGameMap(PGameInfo pGame, int index);


//Click a unit in GameMap.
//Return the number of mines around neighbor,
//or RETVAL_GAMEFIAL if this unit is a mine,
//or RETVAL_INDEXOOR if the index is out of range,
//or RETVAL_BADGAMESTATE if the GameState is not RUNING,
//or RETVAL_BADMUSTATE if this unit can't be clicked.
int clickOne(PGameInfo pGame, int index);

//Open all neighbors around an uncovered unit which has mines-value 0.
//Return RETVAL_NOERROR if the call succeeded,
//or RETVAL_INDEXOOR if the index is out of range,
//or RETVAL_BADGAMESTATE if the GameState is not RUNING,
//or RETVAL_BADMUSTATE if this unit does not satisfied with requirements.
int openBlanks(PGameInfo pGame, int index);

//Set or remove the flag of a coverd unit in GameMap.
//This function changes the state of given unit in cycle:[covered -> flagged -> covered],
//if the QuestionMark mode is enabled, this will be:[covered -> flagged -> marked -> covered].
//Return RETVAL_NOERROR if the call succeeded,
//or RETVAL_INDEXOOR if the index is out of range,
//or RETVAL_BADGAMESTATE if the Game is already finished,
//or RETVAL_BADMUSTATE if this unit does not satisfied with requirements.
int flagOne(PGameInfo pGame, int index);


//Show all mines' positions after GameSet.
//Return RETVAL_NOERROR if the call succeeded,
//or RETVAL_BADGAMESTATE if the Game is not finishend.
int showAllMines(PGameInfo pGame);

//Check if all the non-mine unit is open.
bool isMapFullyOpen(PGameInfo pGame);

//Check if it is the first click of a Game.
bool isFirstClick(PGameInfo pGame, int index);


/* Game runtime high-level functions */

//Click the given unit and open all blanks if it is blank,
//or open the given unit only if it is not blank.
//This function will start a new Game then click if it is the first click.
//Return the number of mines around neighbor,
//or RETVAL_GAMEFIAL if this unit is a mine,
//or RETVAL_GAMESUCCESS if GameSuccess,
//or RETVAL_INDEXOOR if the index is out of range,
//or RETVAL_BADGAMESTATE if the GameState is not RUNING,
//or RETVAL_BADMUSTATE if this unit can't be clicked.
int leftClick(PGameInfo pGame, int index);


//Open all neighbors around the uncovered unit.
//This function represents double click operation in Game.
//Return RETVAL_NOERROR if the call succeeded,
//or RETVAL_GAMEFIAL if clicked a mine,
//or RETVAL_GAMESUCCESS if GameSuccess,
//or RETVAL_INDEXOOR if the index is out of range,
//or RETVAL_BADGAMESTATE if the GameState is not RUNING,
//or RETVAL_BADMUSTATE if this unit is not uncovered,
//or RETVAL_BADFLAGNUM if the number of flags around is not equal to mines-value.
int clickAround(PGameInfo pGame, int index);


//Flag, mark or remove flag on the unit.
//Return RETVAL_NOERROR if the call succeeded,
//or RETVAL_INDEXOOR if the index is out of range,
//or RETVAL_BADGAMESTATE if the Game is already finished,
//or RETVAL_BADMUSTATE if this unit can not be changed.
int rightClick(PGameInfo pGame, int index);


/* GameScore relative functions */

//Reset all scores.
void resetRecord(PGameScore pScore);

//Get the best time under given GameMode.
//Return INV_TIME if the given GameMode is not supported.
WORD getRecordTime(PGameScore pScore, BYTE gamemode);

//Get the player name under given GameMode.
//This function returns the head address of TCHAR[SCORE_NAME_LEN],
//you can directly edit Record content by this pointer but must
//be careful with array bound.
//Return NULL if the given GameMode is not supported.
LPTSTR getpRecordName(PGameScore pScore, BYTE gamemode);

//Update the best time under given GameMode.
//The time value will be limited to MAX_TIME automatically.
//Return RETVAL_BADGAMEMODE if the given GameMode is not supported.
int setRecordTime(PGameScore pScore, BYTE gamemode, WORD besttime);


//Check if break the record.
bool isNewRecord(PGameInfo pGame, PGameScore pScore);
