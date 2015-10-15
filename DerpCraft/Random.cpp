#include "stdafx.h"
#include "Random.h"
#include <stdlib.h>

static int nRandomSeed;


void SetXSeed( int nSeed )
{
	nRandomSeed = nSeed;
}

int xRand()
{
	nRandomSeed = ( 214013 * nRandomSeed + 2531011 );
	return ( nRandomSeed >> 16 ) & 0x7FFF;
}

int xRandom(int min, int max) 
{ 
	return min + xRand() % (max - min + 1); 
} 

int xRandom()
{
	return xRand() % (RAND_MAX + 1 );
}
