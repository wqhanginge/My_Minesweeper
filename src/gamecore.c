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
/*****************************************************************************\
 * gamecore.c
 *****************************************************************************
 * this file contains the minesweeper core functions and Game data
 * all basic operations of Game are defined in this file
 * 
 * this file contains TWO global variables described as 'extern', which are
 * Game -- for game data, Score -- for record data
 * you can READ infomation from Game/Score struct by directly accessing
 * its member AND use defined functions to WRITE/CHANGE it (RECOMMENDED)
\*****************************************************************************/


#include "gamecore.h"


//TWO global variables for game data storage
//described as 'extern'
GameInfo Game;
GameScore Score;

//transform GameMap index-type into xy-type
int index2x(int index)
{
	return (index % Game.width);
}

//transform GameMap index-type into xy-type
int index2y(int index)
{
	return (index / Game.width);
}

//transform GameMap xy-type into index-type
int xy2index(int x, int y)
{
	return (y * Game.width) + x;
}

//get all neighbors' index which around given unit
int getNeighbors(Neighbor* pneighbor, int x, int y)
{
	if (x < 0 || y < 0) return -1;
	if ((word)xy2index(x, y) >= Game.size) return -1;
	(*pneighbor)[0] = xy2index(x, y);
	(*pneighbor)[1] = (x > 0 && y > 0) ? xy2index(x - 1, y - 1) : -1;
	(*pneighbor)[2] = (y > 0) ? xy2index(x, y - 1) : -1;
	(*pneighbor)[3] = (x < Game.width - 1 && y > 0) ? xy2index(x + 1, y - 1) : -1;
	(*pneighbor)[4] = (x > 0) ? xy2index(x - 1, y) : -1;
	(*pneighbor)[5] = (x < Game.width - 1) ? xy2index(x + 1, y) : -1;
	(*pneighbor)[6] = (x > 0 && y < Game.height - 1) ? xy2index(x - 1, y + 1) : -1;
	(*pneighbor)[7] = (y < Game.height - 1) ? xy2index(x, y + 1) : -1;
	(*pneighbor)[8] = (x < Game.width - 1 && y < Game.height - 1) ? xy2index(x + 1, y + 1) : -1;
	return 0;
}

//set Game Mode by params
void setGameMode(byte mode, byte width, byte height, word mines)
{
	switch (mode) {
	case JUNIOR:
		Game.mode = JUNIOR;
		Game.state = INIT;
		Game.width = JUNIOR_WIDTH;
		Game.height = JUNIOR_HEIGHT;
		Game.size = JUNIOR_SIZE;
		Game.mines = JUNIOR_MINES;
		Game.mine_remains = JUNIOR_MINES;
		Game.uncov_units = 0;
		Game.time = 0;
		memset(Game.map, 0, sizeof(byte) * Game.size);
		break;
	case MIDDLE:
		Game.mode = MIDDLE;
		Game.state = INIT;
		Game.width = MIDDLE_WIDTH;
		Game.height = MIDDLE_HEIGHT;
		Game.size = MIDDLE_SIZE;
		Game.mines = MIDDLE_MINES;
		Game.mine_remains = MIDDLE_MINES;
		Game.uncov_units = 0;
		Game.time = 0;
		memset(Game.map, 0, sizeof(byte) * Game.size);
		break;
	case SENIOR:
		Game.mode = SENIOR;
		Game.state = INIT;
		Game.width = SENIOR_WIDTH;
		Game.height = SENIOR_HEIGHT;
		Game.size = SENIOR_SIZE;
		Game.mines = SENIOR_MINES;
		Game.mine_remains = SENIOR_MINES;
		Game.uncov_units = 0;
		Game.time = 0;
		memset(Game.map, 0, sizeof(byte) * Game.size);
		break;
	case CUSTOM:
		Game.mode = CUSTOM;
		Game.state = INIT;
		Game.width = (width < MIN_WIDTH) ? MIN_WIDTH : width;
		Game.width = (Game.width > MAX_WIDTH) ? MAX_WIDTH : Game.width;
		Game.height = (height < MIN_HEIGHT) ? MIN_HEIGHT : height;
		Game.height = (Game.height > MAX_HEIGHT) ? MAX_HEIGHT : Game.height;
		Game.size = Game.width * Game.height;
		Game.mines = (mines > MAX_MINES(Game.size)) ? MAX_MINES(Game.size) : mines;
		Game.mines = (Game.mines < MIN_MINES) ? MIN_MINES : Game.mines;
		Game.mine_remains = (short)Game.mines;
		Game.uncov_units = 0;
		Game.time = 0;
		memset(Game.map, 0, sizeof(byte) * Game.size);
		break;
	default:
		setGameMode(JUNIOR, 0, 0, 0);
		break;
	}
}

//change Game State by param
int setGameState(byte state)
{
	if (state >= UNKNOW) return -1;
	Game.state = state;
	return 0;
}

//set Game Mark value by param
void setMark(bool enable)
{
	Game.mark = enable;
}

//set Game Time with a specific time
int setGameTime(word time)
{
	if (time > MAX_TIME) return -1;
	Game.time = time;
	return 0;
}

//step in time
void stepGameTime()
{
	Game.time++;
}

//erase the GameMap and reset Game infomation
void resetGame()
{
	Game.state = INIT;
	Game.mine_remains = Game.mines;
	Game.uncov_units = 0;
	Game.time = 0;
	memset(Game.map, 0, sizeof(byte) * Game.size);
}

//start a new Game by click one position
int startGame(int x, int y)
{
	if (x < 0 || y < 0 || xy2index(x, y) >= Game.size) return -2;

	//create map and init game infomation
	if (createGameMap(x, y) == -1) return -2;
	Game.state = PROGRESS;
	Game.mine_remains = Game.mines;
	Game.uncov_units = 0;
	Game.time = 0;

	//click on the given unit
	//because of the safe area, the first click will always be safe with 0 mines
	if (leftClick(xy2index(x, y)) == -1) return -1;

	//it is possible that you win the game at first click
	return isGameSuccessful();
}

//create a new GameMap with a safe area at given position
int createGameMap(int x, int y)
{
	if (Game.state != INIT) return -1;

	Neighbor safepos;
	getNeighbors(&safepos, x, y);

	//generate mines, 8 units around where clicked won't have mines
	//use shuffle algorithm
	memset(Game.map, 0, sizeof(byte) * Game.size);	//clear the whole map
	memset(Game.map, MUM_MINE, sizeof(byte) * Game.mines);	//generate mines
	dword neicount = 0;
	for (int i = 0; i < 9; i++) neicount += (safepos[i] != -1);	//remember how many safe positions needed
	for (dword k = 0; k < Game.mines; k++) {	//shuffle algorithm, ignore reserved tail
		dword index = rand() % (Game.size - neicount - k) + k;
		byte temp = Game.map[index];
		Game.map[index] = Game.map[k];
		Game.map[k] = temp;
	}

	//there may be overlap between safe area and neighbor area, need handling
	//if the safe area is at back-half of the whole map, move reserved tail to head
	if (safepos[0] >= Game.size / 2) {
		for (dword i = 0; i < neicount; i++) {
			byte temp = Game.map[Game.size - 1 - i];
			Game.map[Game.size - 1 - i] = Game.map[i];
			Game.map[i] = temp;
		}
		neicount = Game.size;	//WARNING:the meaning of neicount has been changed
	}

	//move safe area to where it is
	for (int i = 0; i < 9; i++) {
		if (safepos[i] != -1) {
			byte temp = Game.map[Game.size - neicount];
			Game.map[Game.size - neicount] = Game.map[safepos[i]];
			Game.map[safepos[i]] = temp;
			neicount--;
		}
	}

	//calculate mines around each unit
	for (dword i = 0; i < Game.size; i++) {
		if (MUISMINE(Game.map[i])) continue;
		int m = 0;
		Neighbor neipos;
		getNeighbors(&neipos, index2x(i), index2y(i));
		for (int j = 1; j < 9; j++)
			if (neipos[j] != -1 && MUISMINE(Game.map[neipos[j]])) m++;
		SETMUMINES(m, Game.map[i]);
	}
	return 0;
}

//click a unit in GameMap
int clickOne(int index)
{
	if (Game.state != PROGRESS) return -2;
	if (index < 0 || index >= (int)Game.size) return -2;
	if (GETMUSTATE(Game.map[index]) != MUS_COVER && GETMUSTATE(Game.map[index]) != MUS_MARK) return -2;

	Game.uncov_units++;
	SETMUUPDATE(Game.map[index]);
	if (MUISMINE(Game.map[index])) {	//bomb
		SETMUSTATE(MUS_BOMB, Game.map[index]);
		return -1;
	}
	else {	//safe
		SETMUSTATE(MUS_UNCOV, Game.map[index]);
		return GETMUMINES(Game.map[index]);
	}
}

//open all neighbors around a uncovered unit witch has mines-value 0
int openBlanks(int index)
{
	if (Game.state != PROGRESS) return -1;
	if (index < 0 || index >= (int)Game.size) return -1;
	if (GETMUSTATE(Game.map[index]) != MUS_UNCOV) return -1;
	if (GETMUMINES(Game.map[index]) != 0) return -1;

	Neighbor pos;
	getNeighbors(&pos, index2x(index), index2y(index));
	for (int i = 1; i < 9; i++) {
		int ret = clickOne(pos[i]);
		if (ret == 0) openBlanks(pos[i]);	//do in a recursive manner
	}
	return 0;
}

//click the given unit and open all blanks if it is blank
//or open the given unit only if it is not blank
int leftClick(int index)
{
	if (Game.state != PROGRESS) return -2;
	if (index < 0 || index >= (int)Game.size) return -2;
	if (GETMUSTATE(Game.map[index]) != MUS_COVER && GETMUSTATE(Game.map[index]) != MUS_MARK) return -2;

	int ret = clickOne(index);
	if (ret == 0) openBlanks(index);
	return ret;
}

//open all neighbors around the uncovered unit
int clickAround(int index)
{
	if (Game.state != PROGRESS) return -2;
	if (index < 0 || index >= (int)Game.size) return -2;
	if (GETMUSTATE(Game.map[index]) != MUS_UNCOV) return -2;

	Neighbor pos;
	getNeighbors(&pos, index2x(index), index2y(index));

	//you can open neighbors only when flags EQ mines around neighbors
	int flags = 0;
	for (int i = 1; i < 9; i++)
		if (pos[i] != -1 && GETMUSTATE(Game.map[pos[i]]) == MUS_FLAG) flags++;
	if (GETMUMINES(Game.map[pos[0]]) != flags) return -2;

	Neighbor info;
	flags = 0;	//WARNING:the meaning of flags has been changed
	//open neighbors
	for (int i = 1; i < 9; i++) {
		info[i] = clickOne(pos[i]);
		if (info[i] == 0) openBlanks(pos[i]);
		if (info[i] == -1) flags = -1;	//if bombed
	}
	return flags;
}

//change the MapUnitState and remained mines when right clicked on a unit
int rightClick(int index)
{
	if (ISGAMESET(Game.state)) return -2;
	if (index < 0 || index >= (int)Game.size) return -2;

	//take effect only on covered units
	byte new_mapunit_state;
	switch (GETMUSTATE(Game.map[index])) {
	case MUS_COVER:
		new_mapunit_state = MUS_FLAG;
		Game.mine_remains--;
		break;
	case MUS_FLAG:
		new_mapunit_state = (Game.mark) ? MUS_MARK : MUS_COVER;
		Game.mine_remains++;
		break;
	case MUS_MARK:
		new_mapunit_state = MUS_COVER;
		break;
	default:
		return -1;
	}

	SETMUSTATE(new_mapunit_state, Game.map[index]);
	SETMUUPDATE(Game.map[index]);
	return 0;
}

//update Game Map and open all mines' positions when game finish
int finishGame()
{
	Game.state = isGameSuccessful() ? SUCCESS : FAIL;
	//show all mines' positions when game set
	Game.mine_remains = 0;
	showAllMines();
	//if break record
	return (Game.mode < CUSTOM && Game.state == SUCCESS && Game.time < getRecordTime(Game.mode));
}

//show all mines' positions after Game Set
void showAllMines()
{
	if (Game.state == FAIL) {
		for (word i = 0; i < Game.size; i++) {
			if (MUISMINE(Game.map[i]) && (GETMUSTATE(Game.map[i]) == MUS_COVER || GETMUSTATE(Game.map[i]) == MUS_MARK)) {
				SETMUSTATE(MUS_UNCOV, Game.map[i]);
				SETMUUPDATE(Game.map[i]);
			}
			else if (!MUISMINE(Game.map[i]) && GETMUSTATE(Game.map[i]) == MUS_FLAG) {
				SETMUSTATE(MUS_WRONG, Game.map[i]);
				SETMUUPDATE(Game.map[i]);
			}
		}
	}
	else if (Game.state == SUCCESS) {
		for (word i = 0; i < Game.size; i++) {
			if (MUISMINE(Game.map[i]) && (GETMUSTATE(Game.map[i]) == MUS_COVER || GETMUSTATE(Game.map[i]) == MUS_MARK)) {
				SETMUSTATE(MUS_FLAG, Game.map[i]);
				SETMUUPDATE(Game.map[i]);
			}
		}
	}
}

//judge if the current Game is Successful
bool isGameSuccessful()
{
	return (Game.mines == Game.size - Game.uncov_units);
}

//reset all scores
void resetRecord()
{
	Score = (GameScore){ MAX_TIME,MAX_TIME,MAX_TIME,_T(DEF_SCORE_NAME_EN),_T(DEF_SCORE_NAME_EN),_T(DEF_SCORE_NAME_EN) };
}

//get best time under given Game Mode
dword getRecordTime(byte gamemode)
{
	switch (gamemode) {
	case JUNIOR:	return Score.junior_time;
	case MIDDLE:	return Score.middle_time;
	case SENIOR:	return Score.senior_time;
	default:		break;
	}
	return -1;
}

//get player name under given Game Mode
TCHAR *getpRecordName(byte gamemode)
{
	switch (gamemode) {
	case JUNIOR:	return Score.junior_name;
	case MIDDLE:	return Score.middle_name;
	case SENIOR:	return Score.senior_name;
	default:		break;
	}
	return nullptr;
}

//update best time under given Game Mode
int setRecordTime(byte gamemode, word besttime)
{
	if (besttime > MAX_TIME) return -1;
	switch (gamemode) {
	case JUNIOR:
		Score.junior_time = besttime;
		break;
	case MIDDLE:
		Score.middle_time = besttime;
		break;
	case SENIOR:
		Score.senior_time = besttime;
		break;
	default:
		return -1;
	}
	return 0;
}
