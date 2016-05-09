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
		return 219;
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
			control->state = CONTROL_FINISHED;
			break;
		default:
			break;
	}
}

TransitionsControlFunction mazeCtrl(Domain *domain, TransitionsExecControl *transitionControl) {
	domainCellCoord c = { 0,0 };
	domainCellType dir[4] = {maze_BOTTOM, maze_LEFT, maze_RIGHT, maze_TOP};
	Neighborhood* nbd = createNeighborhoodAndInit(domain, c, nbd_VON_NEUMANN);

	if (domainGetCellValue(c, domain).value && nbdGetMaskByValues(nbd, dir, 4, 1)) {
		transitionControl->state = CONTROL_FINISHED;
	}else{
		control(domain, transitionControl);
	}

	freeNeighborhood(nbd);
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

/*
	Résolution avec la solution B
*/
TransitionsFunction mazeSolveAll(domainCellValue cell, Neighborhood *neighborhood) {
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

/*
Résolution avec la solution la solution A
*/
TransitionsFunction mazeSolveSimple(domainCellValue cell, Neighborhood *neighborhood) {
	invariant(nbd_VON_NEUMANN, neighborhood->mask, "Invalid mask");

	int wallCount = nbdValueCount(neighborhood, 1) + nbdUndefinedCount(neighborhood);

	if (cell.value == 0 && wallCount == 3) {
		return 1;
	}

	return cell.value;
}


//Necessite une entrée définie
/*
	Résolution avec la solution générale et la solution A
*/
TransitionsFunction mazeSolveAndSimple(domainCellValue cell, Neighborhood *neighborhood) {
	invariant(nbd_VON_NEUMANN, neighborhood->mask, "Invalid mask");

	domainCellType dirCells[5] = {maze_BOTTOM, maze_LEFT, maze_RIGHT, maze_TOP, 8};

	int wallCount = nbdValueCount(neighborhood, 1) + nbdUndefinedCount(neighborhood);

	if (cell.value == 0 && wallCount == 3) {
		return 1;
	}

	int dirCount = nbdGetMaskByValues(neighborhood, &dirCells, 5, 1);
	if (cell.value == 0 && dirCount) {
		return mazePositionToDirBasic(dirCount);
	}

	return cell.value;
}

/*
	Résolution du labyrinthe avec la méthode générale
*/
TransitionsFunction mazeSolve(domainCellValue cell, Neighborhood *neighborhood) {
	invariant(nbd_VON_NEUMANN, neighborhood->mask, "Invalid mask");

	domainCellType dirCells[5] = { maze_BOTTOM, maze_LEFT, maze_RIGHT, maze_TOP, 8 };

	int dirCount = nbdGetMaskByValues(neighborhood, &dirCells, 5, 1);
	if (cell.value == 0 && dirCount) {
		return mazePositionToDirBasic(dirCount);
	}

	return cell.value;
}

/*
	Génération d'un labyrinthe avec la règle B3 S12345 
	Les cellules prennent la valeur 1 si elles ont trois voisins
	Gardent la valeur 1 si 1,2,3,4,5 voisins
	Prennent la valeur 0 sinon
*/
TransitionsFunction b3s12345(domainCellValue cell, Neighborhood *neighborhood) {
	invariant(nbd_MOORE, neighborhood->mask, "Invalid mask");

	int count = nbdValueCount(neighborhood, 1);

	if (cell.value == 0 && count == 3) {
		return 1;
	}

	if (cell.value == 1 && count >= 1 && count <= 5) {
		return 1;
	}

	return 0;
}

Domain* getSpecialMaze(){
	Domain *a = domainCreate(11, 11);

	a->array[5][0] = 1;
	a->array[5][1] = 1;
	a->array[5][2] = 1;
	a->array[5][3] = 1;
	a->array[5][4] = 1;
	a->array[6][4] = 1;
	a->array[7][4] = 1;
	a->array[8][4] = 1;
	a->array[9][4] = 1;

	a->array[0][5] = 1;
	a->array[1][5] = 1;
	a->array[2][5] = 1;
	a->array[3][5] = 1;


	a->array[0][7] = 1;
	a->array[1][7] = 1;
	a->array[2][7] = 1;
	a->array[3][7] = 1;
	a->array[4][7] = 1;
	a->array[5][7] = 1;
	a->array[6][7] = 1;

	return a;
}

int main() {
	start();

	TransitionsConfig config = { nbd_VON_NEUMANN };

	while (1)
	{
		Domain *DomainB = mazeCreate(11, 11);
		Domain *DomainSolve = domainCopy(DomainB);
		Domain *DomainSolve2 = domainCopy(DomainB);

		Domain *DomainA = domainCopy(DomainB);
		Domain *DomainSolveAndSimple = domainCopy(DomainB);

		domainCellCoord entrance = { 0, 0 };
		domainCellCoord exit = { 10, 10 };

		mazeSetPoints(DomainB, entrance, exit, 0, 8);
		mazeSetPoints(DomainSolve, entrance, exit, 0, 8);
		mazeSetPoints(DomainSolve2, entrance, exit, 0, 8);

		mazeSetPoints(DomainA, entrance, exit, 9, 8);
		mazeSetPoints(DomainSolveAndSimple, entrance, exit, 9, 8);

		clearConsole();

		TransitionsExecResult results;
		results = te_run(DomainA, config, mazeSolveSimple, control, epoch);
		table_print_with_params(DomainA->array, table_print_char_line, DomainA->height, DomainA->width, 1, 1, charTransform);
		result(results);

		results = te_run(DomainB, config, mazeSolveAll, control, epoch);
		table_print_with_params(DomainB->array, table_print_char_line, DomainB->height, DomainB->width, 1, 1, charTransform);
		result(results);

		results = te_run(DomainSolve, config, mazeSolve, control, epoch);
		table_print_with_params(DomainSolve->array, table_print_char_line, DomainSolve->height, DomainSolve->width, 1, 1, charTransform);
		result(results);

		results = te_run(DomainSolve2, config, mazeSolve, mazeCtrl, epoch);
		table_print_with_params(DomainSolve2->array, table_print_char_line, DomainSolve2->height, DomainSolve2->width, 1, 1, charTransform);
		result(results);

		results = te_run(DomainSolveAndSimple, config, mazeSolveAndSimple, control, epoch);
		table_print_with_params(DomainSolveAndSimple->array, table_print_char_line, DomainSolveAndSimple->height, DomainSolveAndSimple->width, 1, 1, charTransform);
		result(results);

		domainFree(DomainA);
		domainFree(DomainB);
		domainFree(DomainSolve);
		domainFree(DomainSolve2);
		domainFree(DomainSolveAndSimple);

		getchar();	
	}

	//TransitionsConfig config = {1};
	//te_run(a, config, transition, control, epoch);
}