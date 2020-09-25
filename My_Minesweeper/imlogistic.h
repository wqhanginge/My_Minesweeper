#pragma once

/* this file use for generating random numbers by improved logistic function
 * 32bits long
 */

typedef unsigned long long qword;

#define LOGISTIC_MAX	0xFFFFFFFF
#define LOGISTIC_U		0x3B0A3D70A
#define LOGISTIC_BITW	32

//original logistic function
unsigned logistic();

//set original logistic seed
void slogistic(unsigned seed);

//return a random number within LOGISTIC_MAX
unsigned imlogistic();

//set logistic seed
void simlogistic(unsigned seed);