/*****************************************************************************\
 *  My Minesweepper -- a classic minesweeper game
 *  Copyright (C) 2020 Gee Wang
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
 * stdafx.h
 *****************************************************************************
 * This file is a precompiled header, including commonly used std libs.
\*****************************************************************************/


#pragma once

//Disable uncommonly used Windows APIs.
#define WIN32_LEAN_AND_MEAN
//Disable byte-count-based string functions.
#define STRSAFE_NO_CB_FUNCTIONS
//This is needed to workaround C preprocessor errors when using legacy versions of the Windows SDK.
#define MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS 0

#include <Windows.h>
#include <stdlib.h>
#include <stdbool.h>
#include <tchar.h>
#include <strsafe.h>
#include "../res/resource.h"
#pragma comment(lib, "Version.lib")
