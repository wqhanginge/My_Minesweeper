/*****************************************************************************\
 *  My Minesweepper -- a classic minesweeper game
 *  Copyright (C) 2020-2022 Gee W.
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

//disable uncommonly used APIs and reduce program size
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//use unicode in program
#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <stdbool.h>
#include "resource.h"
#pragma comment(lib, "Version.lib")


typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned dword;

#define nullptr NULL
