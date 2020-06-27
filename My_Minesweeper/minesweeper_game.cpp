#include "minesweeper_game.h"
#include <fstream>
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
	case EASY:
		Game.mode = EASY;
		Game.state = INITIAL;
		Game.width = EASY_WIDTH;
		Game.height = EASY_HEIGHT;
		Game.size = EASY_SIZE;
		Game.mines = EASY_MINES;
		Game.minesleft = EASY_MINES;
		Game.unitsuncov = 0;
		Game.time = 0;
		Game.map = new byte[EASY_SIZE]{};
		Game.mapchange = new bool[EASY_SIZE] {};
		break;
	case NORMAL:
		Game.mode = NORMAL;
		Game.state = INITIAL;
		Game.width = NORMAL_WIDTH;
		Game.height = NORMAL_HEIGHT;
		Game.size = NORMAL_SIZE;
		Game.mines = NORMAL_MINES;
		Game.minesleft = NORMAL_MINES;
		Game.unitsuncov = 0;
		Game.time = 0;
		Game.map = new byte[NORMAL_SIZE]{};
		Game.mapchange = new bool[NORMAL_SIZE] {};
		break;
	case HARD:
		Game.mode = HARD;
		Game.state = INITIAL;
		Game.width = HARD_WIDTH;
		Game.height = HARD_HEIGHT;
		Game.size = HARD_SIZE;
		Game.mines = HARD_MINES;
		Game.minesleft = HARD_MINES;
		Game.unitsuncov = 0;
		Game.time = 0;
		Game.map = new byte[HARD_SIZE]{};
		Game.mapchange = new bool[HARD_SIZE] {};
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
		Game.mode = CRUSH;
		Game.map = nullptr;
		Game.mapchange = nullptr;
		break;
	}
}

void cleanmap()
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
	//init map
	if(Game.state != INITIAL) cleanmap();

	Neighbor pos;
	getneighbors(pos, x, y);

	//generate mines, 9 units around where clicked won't have mines
	word k = 0, index, p;
	srand((dword)time(nullptr));
	while (k < Game.mines) {
		index = rand() % Game.size;
		for (p = 0; p < 9; p++) if (index == pos[p]) break;
		if (p >= 9 && !UNITISMINE(Game.map[index])) {
			Game.map[index] |= UNITMINE;
			k++;
		}
	}

	//write neighbor mines
	for (word i = 0; i < Game.size; i++) {
		if (UNITISMINE(Game.map[i])) continue;
		int m = 0;
		getneighbors(pos, i);
		for (int j = 1; j < 9; j++)
			if (pos[j] != -1 && UNITISMINE(Game.map[pos[j]])) m++;
		Game.map[i] = SETUNITMINES(m, Game.map[i]);
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
	if (GETUNITSTATE(Game.map[index]) != UNITCOVER && GETUNITSTATE(Game.map[index]) != UNITMARK) return -2;

	Game.unitsuncov++;
	Game.mapchange[index] = true;
	if (UNITISMINE(Game.map[index])) {
		Game.map[index] = SETUNITSTATE(UNITBOMB, Game.map[index]);
		return -1;
	}
	else {
		Game.map[index] = SETUNITSTATE(UNITUNCOV, Game.map[index]);
		return GETUNITMINES(Game.map[index]);
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
	if (GETUNITSTATE(Game.map[index]) != UNITUNCOV) return -2;

	Neighbor pos;
	getneighbors(pos, index);

	int flags = 0;
	for (int i = 1; i < 9; i++)
		if (pos[i] != -1 && GETUNITSTATE(Game.map[pos[i]]) == UNITFLAG) flags++;
	if (GETUNITMINES(Game.map[pos[0]]) != flags) return -2;

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
	if (GETUNITSTATE(Game.map[index]) != UNITUNCOV) return -1;
	if (GETUNITMINES(Game.map[index]) != 0) return -1;

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
			if (UNITISMINE(Game.map[i]) && (GETUNITSTATE(Game.map[i]) == UNITCOVER || GETUNITSTATE(Game.map[i]) == UNITMARK)) {
				Game.map[i] = SETUNITSTATE(UNITUNCOV, Game.map[i]);
				Game.mapchange[i] = true;
			}
			else if (!UNITISMINE(Game.map[i]) && GETUNITSTATE(Game.map[i]) == UNITFLAG) {
				Game.map[i] = SETUNITSTATE(UNITWRONG, Game.map[i]);
				Game.mapchange[i] = true;
			}
		}
	}
	else if (Game.state == SUCCESS) {
		for (word i = 0; i < Game.size; i++) {
			if (UNITISMINE(Game.map[i]) && (GETUNITSTATE(Game.map[i]) == UNITCOVER || GETUNITSTATE(Game.map[i]) == UNITMARK)) {
				Game.map[i] = SETUNITSTATE(UNITFLAG, Game.map[i]);
				Game.mapchange[i] = true;
			}
		}
	}
}

void updatetime()
{
	switch (Game.mode) {
	case EASY:
		Score.easytime = Game.time;
		break;
	case NORMAL:
		Score.normaltime = Game.time;
		break;
	case HARD:
		Score.hardtime = Game.time;
		break;
	default:
		break;
	}
}

void cleanscore()
{
	Score = { MAXTIME,MAXTIME,MAXTIME,DEFSCORENAME,DEFSCORENAME,DEFSCORENAME };
}

static bool readfile(char *Path)
{
	int temp;
	ifstream input(Path);
	if (!input.is_open()) return false;

	input.ignore(MAXIGNORE, '=');
	input >> temp;
	if (input.fail()) return false;
	Game.mode = (byte)temp;
	if (Game.mode >= CRUSH) return false;

	input.ignore(MAXIGNORE, '=');
	input >> temp;
	if (input.fail()) return false;
	Game.width = (byte)temp;

	input.ignore(MAXIGNORE, '=');
	input >> temp;
	if (input.fail()) return false;
	Game.height = (byte)temp;

	input.ignore(MAXIGNORE, '=');
	input >> temp;
	if (input.fail()) return false;
	Game.mines = (word)temp;

	input.ignore(MAXIGNORE, '=');
	input >> temp;
	if (input.fail()) return false;
	Game.mark = (bool)temp;

	input.ignore(MAXIGNORE, '=');
	input >> Score.easytime;
	if (input.fail()) return false;
	if (Score.easytime > MAXTIME) return false;
	input.ignore(MAXIGNORE, '\n');
	input.getline(Score.easyname, SCORENAMELEN, '\n');

	input.ignore(MAXIGNORE, '=');
	input >> Score.normaltime;
	if (input.fail()) return false;
	if (Score.normaltime > MAXTIME) return false;
	input.ignore(MAXIGNORE, '\n');
	input.getline(Score.normalname, SCORENAMELEN, '\n');

	input.ignore(MAXIGNORE, '=');
	input >> Score.hardtime;
	if (input.fail()) return false;
	if (Score.hardtime > MAXTIME) return false;
	input.ignore(MAXIGNORE, '\n');
	input.getline(Score.hardname, SCORENAMELEN, '\n');

	input.close();
	return true;
}

void initgame(char * Path)
{
	if (readfile(Path)) {
		if (Game.mode == EASY) setgamemode(EASY);
		else if (Game.mode == NORMAL) setgamemode(NORMAL);
		else if (Game.mode == HARD) setgamemode(HARD);
		else if (Game.mode == CUSTOM) setgamemode(CUSTOM, Game.width, Game.height, Game.mines);
		else setgamemode(EASY);
	}
	else {
		setgamemode(EASY);
		Game.mark = false;
		Score = { MAXTIME,MAXTIME,MAXTIME,DEFSCORENAME,DEFSCORENAME,DEFSCORENAME };
	}
}

void savegame(char * Path)
{
	ofstream output(Path);
	if (output.is_open()) {
		output << "game mode = " << (int)Game.mode << "\n";
		output << "width = " << (int)Game.width << "\n";
		output << "height = " << (int)Game.height << "\n";
		output << "mines = " << (int)Game.mines << "\n";
		output << "enable mark = " << (int)Game.mark << "\n";
		output << "\n";
		output << "records" << "\n";
		output << "easy = " << Score.easytime << "\n" << Score.easyname << "\n";
		output << "normal = " << Score.normaltime << "\n" << Score.normalname << "\n";
		output << "hard = " << Score.hardtime << "\n" << Score.hardname << "\n";
		output.close();
	}
}
