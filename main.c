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
int transition(domainCellValue cell, Neighborhood *nbd) {
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

/*
	Char transform apr defaut pour remplacer les nombres du tableau par des caract�res
*/
char charTransform(int c) {
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
		return (char)(c + 48);
		break;
	}
}

/*
	Controle du debug entre les transitions
*/
void control(Domain *domain, TransitionsExecControl *control) {
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

/*
	Control avec arret des transitions lorsqu'un chemin arrive � proximit� du point d'entr�e
*/
void mazeCtrl(Domain *domain, TransitionsExecControl *transitionControl) {
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
void epoch(Domain *domain, TransitionsEpochProgression epochProgression) {
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

/*
	Affichage des r�sultats de l'execution
*/
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
	R�solution avec la solution B
*/
domainCellType mazeSolveAll(domainCellValue cell, Neighborhood *neighborhood) {
	invariant(nbd_VON_NEUMANN, neighborhood->mask, "Invalid mask");
	if (cell.value) return cell.value;

	int emptyCount = nbdValueCount(neighborhood, 0);
	int close = nbdGetMaskByValue(neighborhood, 8);

	if (emptyCount == 1 && !close) {
		return mazePositionToDirBasic(nbdGetMaskByValue(neighborhood, 0));
	}

	if (emptyCount == 0 && close) {
		return mazePositionToDirBasic(close);
	}

	return cell.value;
}

/*
R�solution avec la solution la solution A
*/
domainCellType mazeSolveSimple(domainCellValue cell, Neighborhood *neighborhood) {
	invariant(nbd_VON_NEUMANN, neighborhood->mask, "Invalid mask");
	if (cell.value) return cell.value;

	int wallCount = nbdValueCount(neighborhood, 1) + nbdUndefinedCount(neighborhood);

	if (wallCount == 3) {
		return 1;
	}

	return cell.value;
}

//D�finition des cellules de direction avec la cellule de sortie
domainCellType dirCells[5] = { maze_BOTTOM, maze_LEFT, maze_RIGHT, maze_TOP, 8 };


//Necessite une entr�e d�finie
/*
	R�solution avec la solution g�n�rale et la solution A
*/
domainCellType mazeSolveAndSimple(domainCellValue cell, Neighborhood *neighborhood) {
	invariant(nbd_VON_NEUMANN, neighborhood->mask, "Invalid mask");
	if (cell.value) return cell.value;

	int wallCount = nbdValueCount(neighborhood, 1) + nbdUndefinedCount(neighborhood);

	if (wallCount == 3) {
		return 1;
	}

	int dirCount = nbdGetMaskByValues(neighborhood, (int*)&dirCells, 5, 1);
	if (dirCount) {
		return mazePositionToDirBasic(dirCount);
	}

	return cell.value;
}

/*
	R�solution du labyrinthe avec la m�thode g�n�rale
*/
domainCellType mazeSolve(domainCellValue cell, Neighborhood *neighborhood) {
	invariant(nbd_VON_NEUMANN, neighborhood->mask, "Invalid mask");
	if (cell.value) return cell.value;

	int dirCount = nbdGetMaskByValues(neighborhood, (int*)&dirCells, 5, 1);
	if (dirCount) {
		return mazePositionToDirBasic(dirCount);
	}

	return cell.value;
}

/*
	G�n�ration d'un labyrinthe avec la r�gle B3 S12345 
	Les cellules prennent la valeur 1 si elles ont trois voisins
	Gardent la valeur 1 si 1,2,3,4,5 voisins
	Prennent la valeur 0 sinon
*/
domainCellType b3s12345(domainCellValue cell, Neighborhood *neighborhood) {
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

/*
	Obtient un labyrinthe non parfait et "fat maze"
*/
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
		/*
			Domaine pour chaque test
		*/
		Domain *DomainB = mazeCreate(11, 11);
		Domain *DomainSolve = domainCopy(DomainB);
		Domain *DomainSolve2 = domainCopy(DomainB);

		Domain *DomainA = domainCopy(DomainB);
		Domain *DomainSolveAndSimple = domainCopy(DomainB);

		/*
			Ajout des points de sortie et d'entr�e quand cela est necessaire
		*/
		domainCellCoord entrance = { 0, 0 };
		domainCellCoord exit = { 10, 10 };

		mazeSetPoints(DomainB, entrance, exit, 0, 8);
		mazeSetPoints(DomainSolve, entrance, exit, 0, 8);
		mazeSetPoints(DomainSolve2, entrance, exit, 0, 8);

		mazeSetPoints(DomainA, entrance, exit, 9, 8);
		mazeSetPoints(DomainSolveAndSimple, entrance, exit, 9, 8);

		clearConsole();

		/*
			Execution des transitions
		*/

		TransitionsExecResult results;

		printf("Solution A\n");
		results = te_run(DomainA, config, mazeSolveSimple, control, epoch);
		table_print_with_params(DomainA->array, table_print_char_line, DomainA->height, DomainA->width, 1, 1, charTransform);
		result(results);

		printf("Solution B\n");
		results = te_run(DomainB, config, mazeSolveAll, control, epoch);
		table_print_with_params(DomainB->array, table_print_char_line, DomainB->height, DomainB->width, 1, 1, charTransform);
		result(results);

		printf("Solution Generale\n");
		results = te_run(DomainSolve, config, mazeSolve, control, epoch);
		table_print_with_params(DomainSolve->array, table_print_char_line, DomainSolve->height, DomainSolve->width, 1, 1, charTransform);
		result(results);

		printf("Solution generale short circuit\n");
		//Notons qu'on change le control
		results = te_run(DomainSolve2, config, mazeSolve, mazeCtrl, epoch);
		table_print_with_params(DomainSolve2->array, table_print_char_line, DomainSolve2->height, DomainSolve2->width, 1, 1, charTransform);
		result(results);

		printf("Solution generale parcours dans les deux sens du DAG\n");
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
}