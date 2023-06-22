#pragma once

#include "defines.h"
#include <tchar.h>

struct GameInfo {
	byte mode;			//accessed by setgamemode()
	byte state;			//accessed by cleanmap() and external function
	bool mark;			//accessed by external function
	byte width;			//accessed by setgamemode()
	byte height;		//accessed by setgamemode()
	word size;			//accessed by setgamemode()
	word mines;			//accessed by setgamemode()
	short minesleft;	//accessed by external function
	word unitsuncov;	//accessed by clickunit()
	word time;			//accessed by external function
	byte *map;			//write by internal function, read by external function
	bool *mapchange;	//indicate changes in map, set by clickunit(), reset by cleanmap() and external function
};

struct GameScore {
	word juniortime;
	word middletime;
	word seniortime;
	TCHAR juniorname[SCORENAMELEN];
	TCHAR middlename[SCORENAMELEN];
	TCHAR seniorname[SCORENAMELEN];
};

/*
 * order: unit, leftup, up, rightup, left, right, leftdown, down, rightdown
 */
typedef int Neighbor[9];

extern GameInfo Game;
extern GameScore Score;

//transform GameMap index-type into xy-type
int indextox(int index);
int indextoy(int index);
//transform GameMap xy-type into index-type
int xytoindex(int x, int y);

//require a 9 members array with input in first member
void getneighbors(Neighbor &neighbor, int x, int y);
//require a 9 members array with input in first member
void getneighbors(Neighbor &neighbor, int index);

//without check
void setgamemode(byte mode, byte width = 0, byte height = 0, word mines = 0);

//reset a map to a blank map
void resetgame();

//create a new map
void createmap(int x, int y);
//create a new map
void createmap(int index);

//click a unit in map, return mines in neighbor, or return -1 if this unit is mine,
//or return -2 if this unit can't be clicked
int clickunit(int x, int y);
//click a unit in map, return mines in neighbor, or return -1 if this unit is mine,
//or return -2 if this unit can't be clicked
int clickunit(int index);

//open all neighbors around the open unit
//return 0 if no error, or -1 if digged mine, or -2 if error
int digneighbors(int x, int y);
//open all neighbors around the open unit
//return 0 if no error, or -1 if digged mine, or -2 if error
int digneighbors(int index);

//open all neighbors around a open unit witch has 0 mines value
//return 0 if no error, or -1 if error
int openblanks(int x, int y);
//open all neighbors around a open unit witch has 0 mines value
//return 0 if no error, or -1 if error
int openblanks(int index);

//mark all mines' positions after game fail
void uncovallmines();

//update best score
void updatetime();

//reset all scores
void resetrecord();

//get best score info
dword getrecordtime();
TCHAR *getrecordname();