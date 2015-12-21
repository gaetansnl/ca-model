#include <stdlib.h>
#include "Transition.h"

/*
	Utils functions
*/
//
void setEpochProgression(TransitionsExecControl *control, int current, int maximum) {
	TransitionsEpochProgression p = { current, maximum };
	control->epochProgression = p;
}

//	Transition Execution
//
TransitionsExecResult te_run(
	Domain *domain, 
	TransitionsConfig config,
	int(*transitionFunction)(domainCellValue, Neighborhood*),
	void(*controlFunction)(Domain*, TransitionsExecControl*))
{
	TransitionsExecControl control;
	control.state = RUNNING;

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
		for (size_t i = 0; i < domain->height; i++)
		{
			for (size_t j = 0; j < domain->width; j++)
			{
				setEpochProgression(&control, i*domain->width + j, domain->height * domain->width - 1);

				/*
					Obtient la cellule courante sur le domain, il s'agit de la cellule et non de sa valeur,
					on expose donc cette cellule à des modifications...
				*/
				domainCellCoord coord = {j, i};
				domainCellValue c = domainGetCellValue(coord, domain);

				Neighborhood *nbd = createNeighborhood(nbdDomain, coord, config.mask);
				domain->array[c.cellCoord.x][c.cellCoord.y] = (*transitionFunction)(c, nbd);
				freeNeighborhood(nbd);
			}
		}

		domainFree(nbdDomain);

		(*controlFunction)(domain, &control);
		epoch++;
	}	
}