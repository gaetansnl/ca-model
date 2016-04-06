#include <stdio.h>
#include "Domain.h"
#include "Transition.h"
#include "Table.h"
#include <stdlib.h>
#include <time.h>
#include "io.h"
#include "Utils.h"

#include "MazeGeneration.h"

#include "Neighborhood.h"

/*
	Gestion de la console avec compatibilite win32
*/
#ifdef WIN32
void clearConsole() {
	system("cls");
}
#else
void clearConsole() {
	system("clear");
}
#endif

int current = 1;
TransitionsFunction transition(domainCellValue cell, Neighborhood *nbd) {
	return current;
}

/*
Obtient ou definit le debug entre les etapes de l'epoch
*/
int epochStep = 0;

/*
Obtient ou definit le debug entre les generations
*/
int timeStepStep = 1;

/*
Obtient ou definit si le debug est interrompu jusqu'a la fin de l'epoch
*/
int skipEpochStep = 0;

charTransformer charTransform(int c) {
	switch (c)
	{
	case 0:
		return ' ';
	case 1:
		return '*';
	case 2:
		return 'L';
	case 3:
		return 'R';
	case 4:
		return 'T';
	case 5:
		return 'B';
	default:
		return c + 48;
		break;
	}
}

/*
	Controle du debug entre les transitions
*/
TransitionsControlFunction control(Domain *domain, TransitionsExecControl *control) {
	skipEpochStep = 0;
	if (!timeStepStep) {
		return;
	}

	current++;

	clearConsole();
	table_print_with_params(domain->array, table_print_char_line, domain->height, domain->width, 1, 1, charTransform);

	printf("Time step %d\n", control->timeStep);
	printf("******************************************************\n");
	printf("*                      STATE CONTROL                 *\n");
	printf("*    (ENTREE) RUNNING - (a) ABORT - (e) FINISHED     *\n");
	printf("******************************************************\n");

	switch (getOneChar())
	{
		case 'a':
			control->state = ABORTED;
			break;
		case 'e':
			control->state = FINISHED;
			break;
		default:
			break;
	}
}

/*
	Controle du debug pendant l'epoch
*/
TransitionsEpochWatch epoch(Domain *domain, TransitionsEpochProgression epochProgression) {
	if (!epochStep || skipEpochStep) {
		return;
	}

	clearConsole();
	table_print_with_params(domain->array, table_print_char_line, domain->height, domain->width, 1, 1, charTransform);

	printf("Epoch Progression %d/%d\n", epochProgression.current, epochProgression.maximum);
	printf("Epoch %d \n", epochProgression.epoch);
	printf("(ENTER) Continuer - (n) Time Step suivant\n");
	skipEpochStep = getOneChar() == 'n';
}

void result(TransitionsExecResult result) {
	printf("Resultat de l'execution \n");
	printf("State : %d \n", result.state);
	printf("Time step : %d \n", result.timeStep);
	printf("Change : %d \n", result.change);
}

/*
	Start et configuration
*/
void start() {
	printf("Recherche de sortie d'un labyrinthe \n");
	printf("----------------------------------- \n");
	printf("CONFIGURATION : (n pour les resultats) \n");
	printf("Voulez-vous faire des pauses entre les time Step ? y/n\n");
	timeStepStep = yesno();
	printf("Voulez-vous faire des pauses pour visualiser la progression de l'epoch ? y/n\n");
	epochStep = yesno();
}

//TransitionsFunction mazeSolve(domainCellValue cell, Neighborhood *neighborhood) {
//	invariant(nbd_VON_NEUMANN, neighborhood->mask, "Invalid mask");
//
//	int emptyCount = nbdValueCount(neighborhood, 0);
//	int wallCount = nbdValueCount(neighborhood, 1);
//	int undefinedCount = nbdUndefinedCount(neighborhood);
//	int inaccessibleCount = undefinedCount + wallCount;
//	int max = nbdValueGetMax(neighborhood);
//
//	if (cell.value == 8 && emptyCount == 1) {
//		return 3;
//	}
//	
//	if (!cell.value && nbdValueCount(neighborhood, 10) == 1 && !emptyCount && inaccessibleCount == 2) {
//		return  mazePositionToDirBasic(nbdGetMaskByValue(neighborhood, 10));
//	}
//
//	if (max >= 2 && max <= 5 && cell.value == 0 && emptyCount == 1 && inaccessibleCount == 2) {
//		return mazePositionToDirBasic(nbdGetMaskByValue(neighborhood, max));
//	}
//
//	if (!cell.value && inaccessibleCount == 3) {
//		return 1;
//	}
//
//	return cell.value;
//}

TransitionsFunction mazeSolve(domainCellValue cell, Neighborhood *neighborhood) {
	invariant(nbd_VON_NEUMANN, neighborhood->mask, "Invalid mask");

	int emptyCount = nbdValueCount(neighborhood, 0);
	int close = nbdValueCount(neighborhood, 8);

	if (cell.value == 0 && emptyCount == 0 && close) {
		return mazePositionToDirBasic(nbdGetMaskByValue(neighborhood, 8));
	}

	if (cell.value == 0 && emptyCount == 1 && !close) {
		return mazePositionToDirBasic(nbdGetMaskByValue(neighborhood, 0));
	}

	return cell.value;
}

int main() {
	start();

	TransitionsConfig config = { nbd_VON_NEUMANN };

	Domain *a = mazeCreate(11,11);
	while (1)
	{
		clearConsole();
		Domain *a = mazeCreate(11, 11);

		domainCellCoord entrance = { 0, 0 };
		domainCellCoord exit = { 10, 10 };

		mazeSetPoints(a, entrance, exit, 0, 8);

		table_print_with_params(a->array, table_print_char_line, a->height, a->width, 1, 1, charTransform);

		TransitionsExecResult results = te_run(a, config, mazeSolve, control, epoch);

		table_print_with_params(a->array, table_print_char_line, a->height, a->width, 1,1, charTransform);		 
		result(results);

		domainFree(a);
		getchar();	
	}

	//TransitionsConfig config = {1};
	//te_run(a, config, transition, control, epoch);
}