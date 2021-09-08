#pragma once

#define WIN32_LEAN_AND_MEAN
#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <tchar.h>
#include <ctime>
#include <memory>
#include "resource.h"
#include <strsafe.h>
#pragma comment(lib, "Version.lib")
using namespace std;


typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned dword;


#ifdef UNICODE
#define tcsscanf_s	swscanf_s
#else
#define tcsscanf_s	swscanf_s
#endif
#define tcsprintf StringCchPrintf
