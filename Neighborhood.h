#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "Domain.h"
#include "Error.h"

//Pour 
#ifdef _MSC_VER
	#include <intrin.h>
	#define __builtin_popcount __popcnt
#endif

extern int NEIGHBORS_DIST;

typedef enum neighborhoodPosition {
	nbd_TOP_LEFT =     0b00100000,
	nbd_TOP =          0b01000000,
	nbd_TOP_RIGHT =    0b10000000,
	nbd_RIGHT =        0b00010000,
	nbd_BOTTOM_RIGHT = 0b00000100,
	nbd_BOTTOM =       0b00000010,
	nbd_BOTTOM_LEFT =  0b00000001,
	nbd_LEFT =         0b00001000
} neighborhoodPosition;

typedef struct nbdNeighbor {
	int mask;
	domainCellValue cellValue;
} nbdNeighbor;

typedef struct Neighborhood {
	nbdNeighbor *neighborhood;
	int size;
	int mask;
} Neighborhood;

Neighborhood *createNeighborhood(Domain *domain, domainCellCoord domainCellCoord, int mask);
void freeNeighborhood(Neighborhood * neighborhood);