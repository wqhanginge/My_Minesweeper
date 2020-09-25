#include "lrand.h"
unsigned x = 0;

unsigned lrand()
{
	x = ((unsigned long long)x * LRAND_MODA + LRAND_MODB);
	return x;
}

void slrand(unsigned seed)
{
	x = seed;
}