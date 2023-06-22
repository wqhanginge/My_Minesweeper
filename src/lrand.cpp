#include "lrand.h"

unsigned long long x = 0;

unsigned lrand()
{
    x = (LRAND_MODA * x + LRAND_MODB) % LRAND_MODN;
    return (unsigned)x;
}

void slrand(unsigned seed)
{
    x = seed;
}
