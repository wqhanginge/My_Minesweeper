/*****************************************************************************\
 *  My Minesweepper -- a classic minesweeper game
 *  Copyright (C) 2020-2023 Gee W.
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

#include "stdincludes.h"



//Game Modes
#define JUNIOR			0
#define MIDDLE			1
#define SENIOR			2
#define CUSTOM			3
#define CRUSH			4
#define ISSTDMOD(Mode)	((BYTE)(Mode) < CUSTOM)
#define ISCRUSH(Mode)	((BYTE)(Mode) >= CRUSH)

#define JUNIOR_WIDTH	9
#define JUNIOR_HEIGHT	9
#define JUNIOR_SIZE		(JUNIOR_WIDTH * JUNIOR_HEIGHT)
#define JUNIOR_MINES	10

#define MIDDLE_WIDTH	16
#define MIDDLE_HEIGHT	16
#define MIDDLE_SIZE		(MIDDLE_WIDTH * MIDDLE_HEIGHT)
#define MIDDLE_MINES	40

#define SENIOR_WIDTH	30
#define SENIOR_HEIGHT	16
#define SENIOR_SIZE		(SENIOR_WIDTH * SENIOR_HEIGHT)
#define SENIOR_MINES	99

#define MAX_WIDTH		30
#define MAX_HEIGHT		24
#define MAX_SIZE		(MAX_WIDTH * MAX_HEIGHT)
#define MIN_WIDTH		9
#define MIN_HEIGHT		9
#define MIN_SIZE		(MIN_WIDTH * MIN_HEIGHT)
#define MAX_MINES(Size)	((WORD)((WORD)(Size) * 9 / 10))
#define MIN_MINES		10
//end Game Modes

//Game States
#define INIT				0
#define RUNNING				1
#define SUCCESS				2
#define FAIL				3
#define UNKNOW				4
#define ISBADSTATE(State)	((BYTE)(State) >= UNKNOW)
#define ISGAMESET(State)	((BYTE)(State) > RUNNING)
//end Game States

//Game Map Unit
/*  8 7 6 5 4 3 2 1
 * +-+-+-+-+-+-+-+-+
 * |M|  S  |   N   |
 * +-+-+-+-+-+-+-+-+
 *     map_unit
 * M:	if this unit is a mine
 * S:	state of this unit:[covered, marked, flagged, uncovered, bombed, wrong flag]
 * N:	contains the number of mines in neighbors, from 0x0 to 0x8
 */

#define MU_MINE			0x80
#define MUS_COVER		0x00
#define MUS_MARK		0x10
#define MUS_FLAG		0x20
#define MUS_UNCOV		0x30
#define MUS_BOMB		0x40
#define MUS_WRONG		0x50
#define GETMUMINES(unit)		((BYTE)((unit) & 0x0F))
#define GETMUSTATE(unit)		((BYTE)((unit) & 0x70))
#define SETMUMINES(m, unit)		(unit = ((BYTE)((BYTE)((m) & 0x0F) | (BYTE)((unit) & 0xF0))))
#define SETMUSTATE(S, unit)		(unit = ((BYTE)((BYTE)((S) & 0x70) | (BYTE)((unit) & 0x8F))))
#define MUISMINE(unit)			((unit) & MU_MINE)
#define MUISCLICKABLE(unit)		(GETMUSTATE(unit) <= MUS_MARK)
//end Game Map Unit

//Game Score
#define SCORE_NAME_LEN		20
#define DEF_SCORE_NAME_EN	"anonymous"

#define MAX_TIME	999
//end Game Score



typedef struct _GameInfo {
	BYTE mode;			//[junior, middle, senior, custom]
	BYTE state;			//[init, running, success, fail]
	bool mark;			//if the QuestionMark is used
	BYTE width;			//GameMap width: map_units per line
	BYTE height;		//GameMap height: map_units per column
	WORD size;			//GameMap size: width x height
	WORD mines;			//counts of mines in GameMap
	short mine_remains;	//counts of mines that haven't been flagged
	WORD uncov_units;	//counts of map_units that have been uncovered
	WORD time;			//GameTime
	BYTE map[MAX_SIZE];	//GameMap data, avaliable area depends on GameMode
} GameInfo, * PGameInfo;

typedef struct _GameScore {
	WORD junior_time;
	WORD middle_time;
	WORD senior_time;
	TCHAR junior_name[SCORE_NAME_LEN];
	TCHAR middle_name[SCORE_NAME_LEN];
	TCHAR senior_name[SCORE_NAME_LEN];
} GameScore, * PGameScore;

/* order in 2D:
 * +---+---+---+
 * | 1 | 2 | 3 |           transform to arrary:
 * +---+---+---+      +---+---+---+---+---+---+---+---+---+
 * | 4 | 0 | 5 | ---> | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
 * +---+---+---+      +---+---+---+---+---+---+---+---+---+
 * | 6 | 7 | 8 |
 * +---+---+---+
 * stores index of each neighbor, -1 means no such neighbor
 */
typedef int Neighbor[9];



/* GameMap position relative functions */

/* GameMap in 2D view
 *        x
 *      ----->
 *       0   1   2   3
 *     +---+---+---+---+
 * y| 0| 0 | 1 | 2 | 3 |	eg: index = 2  <==>  (x, y) = (2, 0)
 *  |  +---+---+---+---+
 *  v 1| 4 |...index...|
 *     +---+---+---+---+
 */

//Transform between GameMap index-type and xy-type.
//Start from 0 and NO ARG CHECK.
int index2x(PGameInfo pGame, int index);
int index2y(PGameInfo pGame, int index);
int xy2index(PGameInfo pGame, int x, int y);

//Check if the unit index is in the map area.
bool isxyinmap(PGameInfo pGame, int x, int y);
bool isidxinmap(PGameInfo pGame, int index);

//Get all neighbors' index which around given unit.
//See detail in Type Neighbor's description.
//Return -1 if error.
int getNeighbors(PGameInfo pGame, Neighbor neighbor, int x, int y);


/* Game property relative functions */

//Set GameMode with JUNIOR by default if 'mode' is an undefined value.
//If 'mode' is a standard value(not CUSTOM), 'width', 'height' and 'mines' will be ignored.
//CUSTOM is limited by MAX_WIDTH, MAX_HEIGHT, MAX_MINES and MIN_***.
//This function will set GameState to INIT and erase the whole GameMap.
void setGameMode(PGameInfo pGame, BYTE mode, BYTE width, BYTE height, WORD mines);

//Change GameState to specified state.
//Return -1 if 'state' is illegal.
int setGameState(PGameInfo pGame, BYTE state);

//Enable or disable QuestionMark mode.
void setMark(PGameInfo pGame, bool enable);

//Set GameTime with specified time.
//Return -1 if 'time' is over MAX_TIME.
int setGameTime(PGameInfo pGame, WORD time);

//Step in time.
void stepGameTime(PGameInfo pGame);


/* Game running basic functions */

//Erase the GameMap and reset Game infomation.
void resetGame(PGameInfo pGame);

//Create a new GameMap with a safe area at given position.
//Return -1 if the GameState is not INIT or index out of range.
int createGameMap(PGameInfo pGame, int index);


//Click a unit in GameMap.
//Return mines around neighbor,
//or return -1 if this unit is mine,
//or return -2 if this unit can't be clicked.
int clickOne(PGameInfo pGame, int index);

//Open all neighbors around a uncovered unit which has mines-value 0.
//Return -1 if error.
int openBlanks(PGameInfo pGame, int index);


//Show all mines' positions after GameSet.
void showAllMines(PGameInfo pGame);

//Check if all the non-mine unit is open.
bool isMapFullyOpen(PGameInfo pGame);

//Check if it is the first click of a Game.
bool isFirstClick(PGameInfo pGame, int index);


/* Game running functions */

//Click the given unit and open all blanks if it is blank,
//or open the given unit only if it is not blank.
//This function will start a new Game then click if it is the first click.
//Return mines around neighbor,
//or return -1 if this unit is mine(GameFail),
//or return -2 if GameSuccess,
//or return -3 if this unit can't be clicked.
int leftClick(PGameInfo pGame, int index);


//Open all neighbors around the uncovered unit.
//This function represents double click operation in Game.
//Return 0 if no error,
//or -1 if digged mine(GameFail),
//or -2 if GameSuccess,
//or -3 if error.
int clickAround(PGameInfo pGame, int index);


//Change the MapUnitState and remained mines when right clicked on a unit.
//Return 0 if changed,
//or -1 if not changed,
//or -2 if error.
int rightClick(PGameInfo pGame, int index);


/* GameScore relative functions */

//Reset all scores.
void resetRecord(PGameScore pScore);

//Get the best time under given GameMode.
//Return -1 if error.
DWORD getRecordTime(PGameScore pScore, BYTE gamemode);

//Get the player name under given GameMode.
//This function returns the head address of TCHAR[SCORE_NAME_LEN],
//you can directly edit Record content by this pointer but must
//be careful with array bound.
//Return NULL if error.
LPTSTR getpRecordName(PGameScore pScore, BYTE gamemode);

//Update the best time under given GameMode.
//Return -1 if 'gamemode' is illegal or 'besttime' is over MAX_TIME.
int setRecordTime(PGameScore pScore, BYTE gamemode, WORD besttime);


//Check if break the record.
bool isNewRecord(PGameInfo pGame, PGameScore pScore);
