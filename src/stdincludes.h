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
 * stdincludes.h
 *****************************************************************************
 * This file is a overall include of std-libs used in program,
 * and global type and variable defines as well.
\*****************************************************************************/


#pragma once

//use unicode in program
#ifndef UNICODE
#define UNICODE
#endif
//disable uncommonly used APIs to reduce program size
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//This is needed to workaround C preprocessor errors when using legacy versions of the Windows SDK. (dmex)
#ifndef MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS
#define MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS 0
#endif

#include <Windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <stdbool.h>
#include "../res/resource.h"
#pragma comment(lib, "Version.lib")
