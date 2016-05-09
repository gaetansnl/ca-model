#include <stdio.h>
#include "MazeGeneration.h"
#include "Transition.h"
#include "Neighborhood.h"

#include <math.h>
#include <time.h>

/*
	Fonction de transition pour normaliser l'�tat du domaine

	Normaliser l'�tat du domaine correspond � l'operation permetant de maintenir
	les zones r�li�es coherentes. Cette operation est appliqu� apr�s la suppresion 
	d'un mur o� une zone doit etre en general etendue.
*/
TransitionsFunction normalizeTransition(domainCellValue cell, Neighborhood *neighborhood) {
	domainCellType max = nbdValueGetMax(neighborhood);
	return cell.value != 1 && max > 1 && max > cell.value ? max : cell.value;
}

/*
	Obtient la valeur d'une des cellule du voisinage si leur valeur est differente. 
	Retourne 0 sinon.
*/
int getValue(Neighborhood* nbd) {
	if (nbdIsComplete(nbd) && (nbd->neighborhood[0].cellValue.value != nbd->neighborhood[1].cellValue.value) && nbdValueGetMin(nbd) > 1) {
		return nbdValueGetMax(nbd);
	}
	return 0;
}

/*
	Cr�e un labyrinthe ayant les dimensions sp�cifi�es
*/
Domain* mazeCreate(int width, int height) {
	srand(clock());

	/*
		Init domaine avec valeur unique pour chaque cellule vide
	*/

	Domain* d = domainCreate(width, height);
	for (size_t x = 0; x < width; x++)
	{
		for (size_t y = 0; y < height; y++)
		{
				d->array[x][y] = (y % 2 || x % 2) ? 1 : y * height + x + 2;
		}
	}

	TransitionsConfig config = { nbd_VON_NEUMANN };

	int domainSize = d->height*d->width - 1;
	int i, r; i = r = rand() % domainSize;

	do {
		domainCellCoord cellCoord = { i%domainSize%d->width, (i%domainSize - i%domainSize%d->width) / d->width };
		domainCell cell = domainGetCell(cellCoord, d);

		/*
			La cellule courante est un mur
		*/
		if (domainGetCellValue(cellCoord, d).value == 1) {

			Neighborhood *nbd1 = createNeighborhoodAndInit(d, cellCoord, nbd_TOP | nbd_BOTTOM);
			Neighborhood *nbd2 = createNeighborhoodAndInit(d, cellCoord, nbd_LEFT | nbd_RIGHT);

			int nValue = fmax(getValue(nbd1), getValue(nbd2));
			if (nValue) {
				(*cell.value) = nValue;
				te_run(d, config, normalizeTransition, NULL, NULL);
				i = r = rand() % domainSize;
			}

			freeNeighborhood(nbd1);
			freeNeighborhood(nbd2);
		}
		i++;

		/*
			Parcours des cellule � partir d'une cellule al�atoire. On s'arrete s'il n'y a plus de cellules � modifier.
		*/
	} while (i%domainSize != r);

	for (size_t x = 0; x < width; x++)
	{
		for (size_t y = 0; y < height; y++)
		{
			d->array[x][y] = d->array[x][y] > 1 ? 0 : d->array[x][y];
		}
	}

	return d;
}

/*
	D�finit les entr�es et sorties du labyrinthe sp�cifi�

	- domain Le domaine
	- entrance Les coordonn�es de l'entr�e
	- exit Les coordon�nes de la sortie
	- vEntr La valeur de la cellule d'entr�e
	- vExit la valeur de la cellule de sortie
*/
Domain* mazeSetPoints(Domain *domain, domainCellCoord entrance, domainCellCoord exit, int vEntr, int vExit) {
	domain->array[entrance.x][entrance.y] = vEntr;
	domain->array[exit.x][exit.y] = vExit;
}

int mazePositionToDirBasic(int position) {
	switch (position)
	{
	case nbd_LEFT:
		return maze_LEFT;
	case nbd_RIGHT:
		return maze_RIGHT;
	case nbd_BOTTOM:
		return maze_BOTTOM;
	case nbd_TOP:
		return maze_TOP;
	default:
		break;
	}
}