#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "Domain.h"
#include "Neighborhood.h"

typedef enum{
	FINISHED = 0,
	CONTROL_FINISHED = 4,
	RUNNING = 1,
	ABORTED = 2,
	ERROR = 3
} TransitionsExecState;

typedef struct {
	 int current;
	 int maximum;
	 int epoch;
} TransitionsEpochProgression;

typedef struct {	
	int timeStep;

	TransitionsExecState state;
} TransitionsExecControl;

typedef struct {
	int mask;
} TransitionsConfig;

typedef struct {
	TransitionsExecState state;
	int timeStep;
	int change;
} TransitionsExecResult;

typedef int(*TransitionsFunction)(domainCellValue, Neighborhood*);
typedef void(*TransitionsControlFunction)(Domain*, TransitionsExecControl*);
typedef void(*TransitionsEpochWatch)(Domain*, TransitionsEpochProgression);

TransitionsExecResult te_run(
	Domain *domain,
	TransitionsConfig config,
	TransitionsFunction transitionFunction,
	TransitionsControlFunction controlFunction,
	TransitionsEpochWatch epochWatch
);