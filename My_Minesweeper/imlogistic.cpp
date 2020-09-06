#include "imlogistic.h"

qword x = 1;

unsigned logistic()
{
	x *= (~(unsigned)x + 1);
	x >>= LOGISTIC_BITW;
	x *= LOGISTIC_U;
	x >>= LOGISTIC_BITW;
	return (unsigned)x;
}

void slogistic(unsigned seed)
{
	x = seed;
}

qword x1 = 1, k1 = 1, x2 = (~1 & 0xffffffff), k2 = (~1 & 0xffffffff);

unsigned imlogistic()
{
	slogistic((unsigned)x1);
	k1 = x1 ^ logistic();
	x1 ^= k1;
	slogistic((unsigned)x2);
	x2 = logistic();
	k2 ^= x2;
	return (unsigned)(k1 ^ k2);
}

void simlogistic(unsigned seed)
{
	k1 = x1 = seed;
	k2 = x2 = (~seed == 0) ? 1 : ~seed;
}


/* improve structure
 *        L            L
 * x10--------->x11--------->x12
 *  |    xor    | |   xor    |
 *  +-----+-----+ +----+-----+
 *     k11|         k12|
 *     xor+->c1     xor+->c2       ...
 *        |            |
 *     k21+------>+ k22+----->
 *  +-----+-----+ +----+<----+
 *  |    xor    |     xor    |
 * x20--------->x21--------->x22
 *        L            L
 */