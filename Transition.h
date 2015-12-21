#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "Domain.h"
#include "Neighborhood.h"

typedef enum{
	FINISHED = 0,
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
} TransitionsExecResult;

TransitionsExecResult te_run(
	Domain *domain,
	TransitionsConfig config,
	int(*transitionFunction)(domainCellValue, Neighborhood*),
	void(*controlFunction)(Domain*, TransitionsExecControl*),
	void(*epochWatch)(Domain*, TransitionsEpochProgression)
);