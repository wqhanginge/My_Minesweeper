#include "gamecore.h"
#include "imlogistic.h"
#include <ctime>
#include <memory>
using namespace std;

GameInfo Game;
GameScore Score;

int indextox(int index)
{
	return (index % Game.width);
}

int indextoy(int index)
{
	return (index / Game.width);
}

int xytoindex(int x, int y)
{
	return (y * Game.width) + x;
}

void getneighbors(Neighbor & neighbor, int x, int y)
{
	neighbor[0] = xytoindex(x, y);
	neighbor[1] = (x > 0 && y > 0) ? xytoindex(x - 1, y - 1) : -1;
	neighbor[2] = (y > 0) ? xytoindex(x, y - 1) : -1;
	neighbor[3] = (x < Game.width - 1 && y > 0) ? xytoindex(x + 1, y - 1) : -1;
	neighbor[4] = (x > 0) ? xytoindex(x - 1, y) : -1;
	neighbor[5] = (x < Game.width - 1) ? xytoindex(x + 1, y) : -1;
	neighbor[6] = (x > 0 && y < Game.height - 1) ? xytoindex(x - 1, y + 1) : -1;
	neighbor[7] = (y < Game.height - 1) ? xytoindex(x, y + 1) : -1;
	neighbor[8] = (x < Game.width - 1 && y < Game.height - 1) ? xytoindex(x + 1, y + 1) : -1;
}

void getneighbors(Neighbor & neighbor, int index)
{
	getneighbors(neighbor, indextox(index), indextoy(index));
}

void setgamemode(byte mode, byte width/*=0*/, byte height/*=0*/, word mines/*=0*/)
{
	word mineslimit;
	if (Game.map != nullptr) delete Game.map;
	if (Game.mapchange != nullptr) delete Game.mapchange;
	switch (mode) {
	case JUNIOR:
		Game.mode = JUNIOR;
		Game.state = INITIAL;
		Game.width = JUNIOR_WIDTH;
		Game.height = JUNIOR_HEIGHT;
		Game.size = JUNIOR_SIZE;
		Game.mines = JUNIOR_MINES;
		Game.minesleft = JUNIOR_MINES;
		Game.unitsuncov = 0;
		Game.time = 0;
		Game.map = new byte[JUNIOR_SIZE]{};
		Game.mapchange = new bool[JUNIOR_SIZE] {};
		break;
	case MIDDLE:
		Game.mode = MIDDLE;
		Game.state = INITIAL;
		Game.width = MIDDLE_WIDTH;
		Game.height = MIDDLE_HEIGHT;
		Game.size = MIDDLE_SIZE;
		Game.mines = MIDDLE_MINES;
		Game.minesleft = MIDDLE_MINES;
		Game.unitsuncov = 0;
		Game.time = 0;
		Game.map = new byte[MIDDLE_SIZE]{};
		Game.mapchange = new bool[MIDDLE_SIZE] {};
		break;
	case SENIOR:
		Game.mode = SENIOR;
		Game.state = INITIAL;
		Game.width = SENIOR_WIDTH;
		Game.height = SENIOR_HEIGHT;
		Game.size = SENIOR_SIZE;
		Game.mines = SENIOR_MINES;
		Game.minesleft = SENIOR_MINES;
		Game.unitsuncov = 0;
		Game.time = 0;
		Game.map = new byte[SENIOR_SIZE]{};
		Game.mapchange = new bool[SENIOR_SIZE] {};
		break;
	case CUSTOM:
		Game.mode = CUSTOM;
		Game.state = INITIAL;
		Game.width = (width < MINWIDTH) ? MINWIDTH : width;
		Game.width = (Game.width > MAXWIDTH) ? MAXWIDTH : Game.width;
		Game.height = (height < MINHEIGHT) ? MINHEIGHT : height;
		Game.height = (Game.height > MAXHEIGHT) ? MAXHEIGHT : Game.height;
		Game.size = Game.width * Game.height;
		mineslimit = (word)(Game.size * MAXMINERATE);
		Game.mines = (mines > mineslimit) ? mineslimit : mines;
		Game.mines = (Game.mines < MINMINES) ? MINMINES : Game.mines;
		Game.minesleft = (short)Game.mines;
		Game.unitsuncov = 0;
		Game.time = 0;
		Game.map = new byte[Game.size]{};
		Game.mapchange = new bool[Game.size]{};
		break;
	default:
		setgamemode(JUNIOR);
		break;
	}
}

void resetgame()
{
	Game.state = INITIAL;
	Game.minesleft = Game.mines;
	Game.unitsuncov = 0;
	Game.time = 0;
	memset(Game.map, 0, sizeof(byte)*Game.size);
	memset(Game.mapchange, 0, sizeof(bool)*Game.size);
}

void createmap(int x, int y)
{
	if (Game.state != INITIAL) return;

	Neighbor safepos;
	getneighbors(safepos, x, y);

	//generate mines, 9 units around where clicked won't have mines
	//use shuffle algorithm
	memset(&Game.map[Game.size - Game.mines], MU_MINE, Game.mines);
	dword neicount = 0;
	for (int i = 0; i < 9; i++) neicount += (Game.map[safepos[i]] != -1);	//remember how many safe positions needed
	for (dword k = Game.size - Game.mines; k < Game.size; k++) {	//shuffle algorithm
		dword index = imlogistic(neicount, k + 1);
		Game.map[k] = exchange(Game.map[index], Game.map[k]);
	}
	for (int i = 0; i < 9; i++) {	//move safe place to where it is at
		if (Game.map[safepos[i]] != -1) {
			neicount--;
			Game.map[neicount] = exchange(Game.map[safepos[i]], Game.map[neicount]);
		}
	}

	//write neighbor mines
	for (dword i = 0; i < Game.size; i++) {
		if (MUISMINE(Game.map[i])) continue;
		int m = 0;
		Neighbor neipos;
		getneighbors(neipos, i);
		for (int j = 1; j < 9; j++)
			if (neipos[j] != -1 && MUISMINE(Game.map[neipos[j]])) m++;
		Game.map[i] = MAKEMUMINES(m, Game.map[i]);
	}
}

void createmap(int index)
{
	createmap(indextox(index), indextoy(index));
}

int clickunit(int x, int y)
{
	return clickunit(xytoindex(x, y));
}

int clickunit(int index)
{
	if (Game.state != PROGRESS) return -2;
	if (index < 0 || index >= (int)Game.size) return -2;
	if (GETMUSTATE(Game.map[index]) != MUS_COVER && GETMUSTATE(Game.map[index]) != MUS_MARK) return -2;

	Game.unitsuncov++;
	Game.mapchange[index] = true;
	if (MUISMINE(Game.map[index])) {
		Game.map[index] = MAKEMUSTATE(MUS_BOMB, Game.map[index]);
		return -1;
	}
	else {
		Game.map[index] = MAKEMUSTATE(MUS_UNCOV, Game.map[index]);
		return GETMUMINES(Game.map[index]);
	}
}

int digneighbors(int x, int y)
{
	return digneighbors(xytoindex(x, y));
}

int digneighbors(int index)
{
	if (Game.state != PROGRESS) return -2;
	if (index < 0 || index >= (int)Game.size) return -2;
	if (GETMUSTATE(Game.map[index]) != MUS_UNCOV) return -2;

	Neighbor pos;
	getneighbors(pos, index);

	int flags = 0;
	for (int i = 1; i < 9; i++)
		if (pos[i] != -1 && GETMUSTATE(Game.map[pos[i]]) == MUS_FLAG) flags++;
	if (GETMUMINES(Game.map[pos[0]]) != flags) return -2;

	Neighbor info;
	//open neighbors
	for (int i = 1; i < 9; i++) {
		info[i] = clickunit(pos[i]);
		if (info[i] == 0) openblanks(pos[i]);
	}

	for (int i = 1; i < 9; i++) {
		if (info[i] == -1) {	//if bombed
			return -1;
		}
	}
	return 0;
}

int openblanks(int x, int y)
{
	return openblanks(xytoindex(x, y));
}

int openblanks(int index)
{
	if (Game.state != PROGRESS) return -1;
	if (index < 0 || index >= (int)Game.size) return -1;
	if (GETMUSTATE(Game.map[index]) != MUS_UNCOV) return -1;
	if (GETMUMINES(Game.map[index]) != 0) return -1;

	Neighbor pos;
	getneighbors(pos, index);
	for (int i = 1; i < 9; i++) {
		int ret = clickunit(pos[i]);
		if (ret == 0) openblanks(pos[i]);
	}
	return 0;
}

void uncovallmines()
{
	if (Game.state == FAIL) {
		for (word i = 0; i < Game.size; i++) {
			if (MUISMINE(Game.map[i]) && (GETMUSTATE(Game.map[i]) == MUS_COVER || GETMUSTATE(Game.map[i]) == MUS_MARK)) {
				Game.map[i] = MAKEMUSTATE(MUS_UNCOV, Game.map[i]);
				Game.mapchange[i] = true;
			}
			else if (!MUISMINE(Game.map[i]) && GETMUSTATE(Game.map[i]) == MUS_FLAG) {
				Game.map[i] = MAKEMUSTATE(MUS_WRONG, Game.map[i]);
				Game.mapchange[i] = true;
			}
		}
	}
	else if (Game.state == SUCCESS) {
		for (word i = 0; i < Game.size; i++) {
			if (MUISMINE(Game.map[i]) && (GETMUSTATE(Game.map[i]) == MUS_COVER || GETMUSTATE(Game.map[i]) == MUS_MARK)) {
				Game.map[i] = MAKEMUSTATE(MUS_FLAG, Game.map[i]);
				Game.mapchange[i] = true;
			}
		}
	}
}

void updatetime()
{
	switch (Game.mode) {
	case JUNIOR:
		Score.juniortime = Game.time;
		break;
	case MIDDLE:
		Score.middletime = Game.time;
		break;
	case SENIOR:
		Score.seniortime = Game.time;
		break;
	default:
		break;
	}
}

void resetrecord()
{
	Score = { MAXTIME,MAXTIME,MAXTIME,_T(DEFSCORENAMEEN),_T(DEFSCORENAMEEN),_T(DEFSCORENAMEEN) };
}

dword getrecordtime()
{
	switch (Game.mode) {
	case JUNIOR:	return Score.juniortime;
	case MIDDLE:	return Score.middletime;
	case SENIOR:	return Score.seniortime;
	default:		break;
	}
	return 0;
}

TCHAR *getrecordname()
{
	switch (Game.mode) {
	case JUNIOR:	return Score.juniorname;
	case MIDDLE:	return Score.middlename;
	case SENIOR:	return Score.seniorname;
	default:		break;
	}
	return nullptr;
}
