#include <stdlib.h>
#include "Transition.h"

/*
	Execution des transitions sur un domaine sp�cifi� avec les param�tres :

	- config La configuration pour l'execution. Elle contient notamment le mask
	- transitionFunction Fonction de transition
	- controlFunction Fonction de controle de transition OPTIONAL 
	- epochWatch Fonction pour suivre les transitions pendant une epoch OPTIONAL

	Retourne struct TransitionsExecResult contenant les statistiques sur l'execution.


	Cette fonction cr�e le neighborhood pour chaque cellule du domaine 
	avant de lui appliquer la fonction de transition. La valeur retourn�e par
	la fonction de transition sera la nouvelle valeur de la cellule.

	L'execution est effectu�e de telle mani�re que la focntion de transition 
	peut utiliser uniquement sa valeur et la valeur des voisins.

	Il est possible de controler l'execution des transition � l'aide 
	de controlFunction. Cette fonction est appel�e � chaque fin d'epoch.
	On peut modifier le TransitionsExecControl en param�tre pour stopper 
	l'execution ou afficher des informations sur lepoch en cours

	Il est possible de suivre tr�s pr�cisement chaque transition de l'epoch 
	en cours avec epochWatch. La fonction sp�cifi�e sera appel�e apr�s chaque 
	transition.

*/
TransitionsExecResult te_run(
	Domain *domain, 
	TransitionsConfig config,
	int(*transitionFunction)(domainCellValue, Neighborhood*),
	void(*controlFunction)(Domain*, TransitionsExecControl*),
	void(*epochWatch)(Domain*, TransitionsEpochProgression))
{
	TransitionsExecControl control;
	control.state = RUNNING;
	int watchEpoch = epochWatch != NULL;

	/*
		Compteurs statistiques
	*/
	unsigned long epoch = 1;
	unsigned long totalChange = 0;

	Neighborhood *nbd = NULL;

	while (control.state == RUNNING) {

		control.timeStep = epoch;

		/*
			Copie du domaine pour les neighbors
			Les valeurs ne seront pas celles en cours de modification
		*/
		Domain *nbdDomain = domainCopy(domain);
		if(nbd == NULL) nbd = createNeighborhood(config.mask);

		int change = 0;

		/*
			Execution sur toute la grille
		*/
		for (size_t y = 0; y < domain->height; y++)
		{
			for (size_t x = 0; x < domain->width; x++)
			{
				/*
					Obtient la cellule courante sur le domain, il s'agit de la cellule et non de sa valeur,
					on expose donc cette cellule � des modifications...
				*/
				domainCellCoord coord = {x, y};
				domainCellValue c = domainGetCellValue(coord, domain);

				updateNeighborhood(nbdDomain, coord, nbd);

				domainCellType v = (*transitionFunction)(c, nbd);
				if (domain->array[c.cellCoord.x][c.cellCoord.y] != v) {
					change++;
				}

				domain->array[c.cellCoord.x][c.cellCoord.y] = v;

				/*
					On transmet la progressionde l'epoch au callback si elle est suivie
				*/

				if (watchEpoch) {
					TransitionsEpochProgression p = {y * domain->height + x + 1, domain->height * domain->width, epoch };
					(*epochWatch)(domain, p);
				}
			}
		}

		totalChange = totalChange + change;
		domainFree(nbdDomain);

		if (!change) {
			control.state = FINISHED;
			continue;
		}

		if(controlFunction != NULL) {
			(*controlFunction)(domain, &control);
		}

		epoch++;
	}

	freeNeighborhood(nbd);

	TransitionsExecResult result;
	result.state = control.state;
	result.timeStep = control.timeStep;
	result.change = totalChange;

	return result;
}