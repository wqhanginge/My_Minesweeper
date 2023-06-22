#pragma once

/* 
 * this file contains encapsulations of UI operations and IO functions
 * this file also contians some useful functions
 * NOTE:most functions do not have arg check process, use with care
 */

#include "stdincludes.h"
#include "basicUI.h"



/* IO defines and game init defines */

#define DEF_WND_LEFT	128
#define DEF_WND_TOP		128
#define DEF_FILENAME	"MyMinesweeper.ini"
#define DEF_FILEPATH_EV	"LOCALAPPDATA"
#define CONTENT_STRLEN	5
#define INIT_ANAME		"InitInfo"
#define INIT_XPOS		"xpos"
#define INIT_YPOS		"ypos"
#define INIT_MODE		"mode"
#define INIT_WIDTH		"width"
#define INIT_HEIGHT		"height"
#define INIT_MINES		"mines"
#define INIT_MARK		"mark"
#define SCORE_ANAME		"Record"
#define SCORE_JTIME		"junior_time"
#define SCORE_MTIME		"middle_time"
#define SCORE_STIME		"senior_time"
#define SCORE_JNAME		"junior_name"
#define SCORE_MNAME		"middle_name"
#define SCORE_SNAME		"senior_name"



//bitmap handle
extern HBITMAP hbm_rb, hbm_click, hbm_fail, hbm_success;
//a argument to remember current using bitmap
extern HBITMAP hbm_current;



/*        x
 *      ----->
 *       0   1   2   3
 *     +---+---+---+---+ --
 * y| 0| 0 | 1 | 2 | 3 |  h
 *  |  +---+---+---+---+ --
 *  v 1| 4 |...index...|
 *     +---+---+---+---+
 *     |<w>|
 * px and py means positon in pixel on window
 * x and y means position in unit on GameMap
 * px = x * w, py = y * h, ppos = (px, py)
 */

//transform pixels on MapArea into GameMap index
//use the first MapUnit left-top as position 0
//take care of the input, they must be offset of MAP_LEFT and MAP_TOP
int ppos2index(int px, int py);
int px2x(int px);
int py2y(int py);

//transform a GameMap index into MapArea pixels
//use the first MapUnit left-top as position zero
//take care of the output, they are offset of MAP_LEFT or MAP_TOP
int index2px(int index);
int index2py(int index);
int x2px(int x);
int y2py(int y);

//check if a mouse position is inside the ResetButton area
bool lparamIsInRB(LPARAM lparam);

//check if a mouse position is inside the MapUnits area
bool lparamIsInMap(LPARAM laparm);

//change a mouse position to map index
int lparam2index(LPARAM lparam);



//manage bitmaps
void loadBitmaps(HINSTANCE hinst);
void freeBitmaps();



/* manage MapUnit and ResetButton */

//draw a mapunit depends on MapUnit data with default color
//draw a covered mapunit by default
void paintMapUnit(HDC hdestdc, int muleft, int mutop, byte mapunit);

//draw a mapunit depends on MapUnitState with default color
//draw a covered mapunit by default
//this function will draw on DC directly without creating a DC-buffer
void paintMapUnitNB(HDC hdestdc, int muleft, int mutop, byte mapunit);

//paint GameMap, the left-top is position 0
//update map_units that have been changed and clear Update bit
//update all map_units if force is set
//it redraws the whole MapArea content
void paintMap(HDC hdestdc, int mapleft, int maptop, bool force);

//paint GameMap, the left-top is position 0
//update map_units that have been changed and clear Update bit
//update all map_units if force is set
//it will paint directly on DC without creating a DC-Buffer
void paintMapNB(HDC hdestdc, int mapleft, int maptop, bool force);

//paint ResetButton without changing its bitmap
void paintResetButton(HDC hdestdc, int rbleft, int rbtop, bool clicked);

//change bitmap of ResetButton
//NOTE:you need to redraw ResetButton after calling this function
void setRBBitmap(HBITMAP hbm);


//show clicked state when a MapUnit is clicked
//it will do nothing if the index is out of GameMap range
void showClickedMapUnit(HDC hdestdc, int mapleft, int maptop, int index);

//show clicked state when a group of MapUnits are clicked
//it jumps MapUnit which index is out of GameMap range
void showClickedMapUnit(HDC hdestdc, int mapleft, int maptop, Neighbor& indexes);



/* save file management */

//load infomation from a save file
//use default setting to init Game if error
void initGame(TCHAR* Path, POINT& lastwndpos);

//save Game infomation into a save file
void saveGame(TCHAR* Path, POINT& wndpos);


/* get program version information */
void getVersion(TCHAR* version, size_t size_in_ch);
