#include <stdlib.h>
#include "Transition.h"

//	Transition Execution
//
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
		Compteur des epoch
	*/
	int epoch = 1;

	while (control.state == RUNNING) {

		control.timeStep = epoch;

		/*
			Copie du domaine pour les neighbors
			Les valeurs ne seront pas celles en cours de modification
		*/
		Domain *nbdDomain = domainCopy(domain);

		/*
			Execution sur toute la grille
		*/
		for (size_t y = 0; y < domain->height; y++)
		{
			for (size_t x = 0; x < domain->width; x++)
			{
				/*
					Obtient la cellule courante sur le domain, il s'agit de la cellule et non de sa valeur,
					on expose donc cette cellule à des modifications...
				*/
				domainCellCoord coord = {x, y};
				domainCellValue c = domainGetCellValue(coord, domain);

				Neighborhood *nbd = createNeighborhood(nbdDomain, coord, config.mask);
				domain->array[c.cellCoord.x][c.cellCoord.y] = (*transitionFunction)(c, nbd);
				freeNeighborhood(nbd);

				/*
					On transmet la progressionde l'epoch au callback si elle est suivie
				*/

				if (watchEpoch) {
					TransitionsEpochProgression p = {y * domain->height + x + 1, domain->height * domain->width };
					(*epochWatch)(domain, p);
				}
			}
		}

		domainFree(nbdDomain);

		(*controlFunction)(domain, &control);
		epoch++;
	}	
}