#pragma once

#include <stdlib.h>
#include <stdint.h>
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
	nbd_TOP_LEFT =     0b00000001,
	nbd_TOP =          0b00001000,
	nbd_TOP_RIGHT =    0b00100000,
	nbd_RIGHT =        0b01000000,
	nbd_BOTTOM_RIGHT = 0b10000000,
	nbd_BOTTOM =       0b00010000,
	nbd_BOTTOM_LEFT =  0b00000100,
	nbd_LEFT =		   0b00000010
} neighborhoodPosition;

#define nbd_VON_NEUMANN 0b01011010
#define nbd_MOORE 0b11111111

typedef struct nbdNeighbor {
	int32_t mask;
	domainCellValue cellValue;
} nbdNeighbor;

typedef struct Neighborhood {
	nbdNeighbor *neighborhood;
	int size;
	int32_t mask;
} Neighborhood;

Neighborhood *createNeighborhood(int32_t mask);
Neighborhood *createNeighborhoodAndInit(Domain *domain, domainCellCoord domainCellCoord, int32_t mask);
Neighborhood *updateNeighborhood(Domain *domain, domainCellCoord domainCellCoord, Neighborhood *n);
void freeNeighborhood(Neighborhood * neighborhood);

int nbdValueCount(Neighborhood *neighborhood, domainCellType value);
int nbdUndefinedCount(Neighborhood *neighborhood);
domainCellType nbdValueGetMax(Neighborhood *neighborhood);
int nbdIsNeighborEquals(Neighborhood *neighborhood, int32_t mask, domainCellType value);
nbdNeighbor nbdGetByMask(Neighborhood *neighborhood, int32_t mask);
domainCellType nbdValueGetMin(Neighborhood *neighborhood);
int nbdIsComplete(Neighborhood* neighborhood);
int nbdGetMaskByValue(Neighborhood *neighborhood, domainCellType value);
int nbdGetMaskByValues(Neighborhood *neighborhood, domainCellType* values, int size, int first);