#pragma once
#include "Domain.h"
Domain* mazeCreate(int width, int height);

typedef enum mazeDirection {
	maze_LEFT = 2,
	maze_RIGHT = 3,
	maze_TOP = 4,
	maze_BOTTOM = 5
} mazeDirection;

Domain* mazeSetPoints(Domain *domain, domainCellCoord entrance, domainCellCoord exit, int vEntr, int vExit);

int mazePositionToDirBasic(int position);
int mazeReverseDirBasic(int dir);
