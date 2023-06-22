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

#pragma once

/* gamecore.h
 * this file contains the minesweeper core functions and data structure
 * all basic operations of Game are defined in this file
 * you can READ infomation from Game/Score struct by directly accessing its member
 * AND use defined functions to WRITE/CHANGE it (RECOMMENDED)
 */

#include "stdincludes.h"



//Game Modes
#define JUNIOR	0
#define MIDDLE	1
#define SENIOR	2
#define CUSTOM	3
#define CRUSH	4
#define ISCRUSH(Mode)	((byte)(Mode) >= CRUSH)

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
#define MAX_MINES(Size)	((word)((word)(Size) * 9 / 10))
#define MIN_MINES		10
//end Game Modes

//Game States
#define INIT		0
#define PROGRESS	1
#define SUCCESS		2
#define FAIL		3
#define UNKNOW		4
#define ISGAMESET(State)	((byte)(State) > PROGRESS)
//end Game States

//Game Map Unit
/*  7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+
 * |U|  S  |   M   |
 * +-+-+-+-+-+-+-+-+
 *     map_unit
 * U:	if this unit is need to update/redraw
 * S:	state of this unit:[covered, flagged, marked, uncovered, bombed, wrong flag]
 * M:	counts of mines in neighbors, from 0x0 to 0x8, 0xF if this unit is a mine
 */

#define MU_UPDATE	0x80
#define MUS_COVER	0x00
#define MUS_FLAG	0x10
#define MUS_MARK	0x20
#define MUS_UNCOV	0x30
#define MUS_BOMB	0x40
#define MUS_WRONG	0x50
#define MUM_MINE	0x0F
#define GETMUMINES(unit)		((byte)((unit) & 0x0F))
#define GETMUSTATE(unit)		((byte)((unit) & 0x70))
#define SETMUMINES(m, unit)		unit = ((byte)((byte)((m) & 0x0F) | (byte)((unit) & 0xF0)))
#define SETMUSTATE(S, unit)		unit = ((byte)((byte)((S) & 0x70) | (byte)((unit) & 0x8F)))
#define MUISMINE(unit)			(((unit) & 0x0F) == MUM_MINE)
#define SETMUUPDATE(unit)		unit |= MU_UPDATE
#define REMMUUPDATE(unit)		unit &= (~MU_UPDATE)
#define MUISUPDATE(unit)		((bool)((unit) & MU_UPDATE))
//end Game Map Unit

//Game Score
#define SCORE_NAME_LEN		20
#define DEF_SCORE_NAME_EN	"anonymous"
#define DEF_SCORE_NAME_CH	"ÄäÃû"

#define MAX_TIME	999
//end Game Score



typedef struct GameInfo {
	byte mode;			//junior, middle, senior or custom
	byte state;			//init, progress, success or fail
	bool mark;			//if the Question Mark is used
	byte width;			//GameMap width: map_units per line
	byte height;		//GameMap height: map_units per column
	word size;			//GameMap size: width x height
	word mines;			//counts of mines in GameMap
	short mine_remains;	//counts of mines that haven't been flagged
	word uncov_units;	//counts of map_units that have been uncovered
	word time;			//Game time
	byte map[MAX_SIZE];	//GameMap data, avaliable area depends on Game Mode
} GameInfo;

typedef struct GameScore {
	word junior_time;
	word middle_time;
	word senior_time;
	TCHAR junior_name[SCORE_NAME_LEN];
	TCHAR middle_name[SCORE_NAME_LEN];
	TCHAR senior_name[SCORE_NAME_LEN];
} GameScore;

/* order in 2D:
 * +---+---+---+
 * | 1 | 2 | 3 |           transform to arrary:
 * +---+---+---+      +---+---+---+---+---+---+---+---+---+
 * | 4 | 0 | 5 | ---> | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
 * +---+---+---+      +---+---+---+---+---+---+---+---+---+
 * | 6 | 7 | 8 |
 * +---+---+---+
 * store index of each neighbor, -1 means no such neighbor
 */
typedef int Neighbor[9];



extern GameInfo Game;
extern GameScore Score;

/*        x
 *      ----->
 *       0   1   2   3
 *     +---+---+---+---+
 * y| 0| 0 | 1 | 2 | 3 |	eg: index = 2  <==>  (x, y) = (2, 0)
 *  |  +---+---+---+---+
 *  v 1| 4 |...index...|
 *     +---+---+---+---+
 */

//transform GameMap index-type into xy-type
//start from 0, no arg check
int index2x(int index);
int index2y(int index);

//transform GameMap xy-type into index-type
//start from 0, no arg check
int xy2index(int x, int y);


//see detail in Type Neighbor's description
//return -1 if error
int getNeighbors(Neighbor* pneighbor, int x, int y);


//set Game Mode with JUNIOR by default if 'mode' is undefined value
//if 'mode' is a standard value(not CUSTOM), 'width', 'height' and 'mines' will be ignored
//CUSTOM is limited by MAX_WIDTH, MAX_HEIGHT, MAX_MINES and MIN_*** as well
//won't set Update bit, will set Game State to INIT and erase the whole Game Map
void setGameMode(byte mode, byte width, byte height, word mines);


//on/off Question Mark mode
void setMark(bool enable);


//set Game Time with a specific time
//return -1 if time bigger than MAX_TIME
int setGameTime(word time);

//step in time
void stepGameTime();


//change Game State
//return -1 if state is illegal
//won't set Update bit
int setGameState(byte state);


//erase the GameMap and reset Game infomation
//won't set Update bit
void resetGame();


//create a new GameMap
//set Update bit before return
//return -1 if the Game State is not INIT
int createGameMap(int x, int y);


//click a unit in GameMap, return mines around neighbor, or return -1 if this unit is mine,
//or return -2 if this unit can't be clicked
//set Update bit before return
int clickOne(int index);


//open all neighbors around a uncovered unit witch has 0 mines value
//return 0 if no error, or -1 if error
//set Update bit before return
int openBlanks(int index);


//open all neighbors around the open unit
//return 0 if no error, or -1 if digged mine, or -2 if error
//set Update bit before return
int clickAround(int index);


//change the MapUnitState and remained mines when right clicked on a unit
//return 0 if normal, return -1 if not changed, return -2 if error
//set Update bit before return
int rightClick(int index);


//open all mines' positions after Game Fail
//set Update bit before return
void uncovAllMines();


//start a new Game by click one position
//set Update bit before return
//return 0 if no exception, return -2 if error,
//return 1 if game success, return -1 if digged mine(game fail)
int gameStart(int x, int y);

//judge if the current Game is Successful
bool isGameSuccessful();

//update Game Map and open all mines' positions
//clear mine_remains and check if breaking record
//set Update bit before return
//return 1 if breaking record, or 0 if no
int gameSet();


//reset all scores
void resetRecord();

//this function return -1 if error
dword getRecordTime(byte gamemode);

//this function return a TCHAR[SCORE_NAME_LEN] pointer,
//you can directly edit Record content by this pointer
//be careful with array bound
//return nullptr if error
TCHAR *getpRecordName(byte gamemode);

//update best time, return -1 if gamemode is illegal or besttime bigger than MAX_TIME
int setRecordTime(byte gamemode, word besttime);
