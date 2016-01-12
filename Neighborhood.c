#include "Neighborhood.h"

int NEIGHBORS_DIST = 1;

Neighborhood *createNeighborhood(Domain *domain, domainCellCoord cellCoord, int mask) {
	/*
		Hamming weight
	*/
	int neighborsCount = __builtin_popcount(mask);

	/*
		Alloc memoire du voisinage et des voisins
	*/
	Neighborhood *n = malloc(sizeof(Neighborhood));
	n->neighborhood = malloc(neighborsCount * sizeof(nbdNeighbor));

	n->mask = mask;
	n->size = neighborsCount;

	int currentMask = 0b1;
	int index = 0;

	/*
		Parcours des neighbors.
		On pourra modifier plus tard la distance des neighboors à condition de modifier le type de mask
	*/
	for (int i = -NEIGHBORS_DIST; i <= NEIGHBORS_DIST; i++)
	{
		for (int j = -NEIGHBORS_DIST; j <= NEIGHBORS_DIST; j++)
		{
			/*
				Pour eviter l'incrementation du mask, etc quand la positions relative est nulle
			*/
			if (i == 0 && j == 0) {
				continue;
			}

			if (currentMask & n->mask) {
				domainCellCoord coord = { cellCoord.x + i, cellCoord.y + j };
				nbdNeighbor newNeighbor;

				/*
					Si le neighbor est hors du domaine on le differencie en lui appliquant un mask de 0 (aucune position relative)
				*/
				if (!domainIsOutside(coord, domain)) {
					domainCellValue value = domainGetCellValue(coord, domain);
					newNeighbor.mask = currentMask;
					newNeighbor.cellValue = value;
				}
				else {
					newNeighbor.mask = 0;
				}

				n->neighborhood[index] = newNeighbor;
				index++;
			}

			/*
				On passe au masque suivant avec bitwise
			*/
			currentMask = currentMask << 1;
		}	
	}

	return n;
}

/*
	Libere la mémoire pour le voisignage specifie
*/
void freeNeighborhood(Neighborhood *neighborhood) {
	free(neighborhood->neighborhood);
	free(neighborhood);
	neighborhood = NULL;
}

/*
	Obtient la valeur maximale parmi la valeur des voisins du voisinage specifie
*/
domainCellType nbdValueGetMax(Neighborhood *neighborhood) {
	if (neighborhood->size == 0) {
		fatalError("nbd GetMax Empty nbd");
	}

	domainCellType max = neighborhood->neighborhood[0].cellValue.value;

	for (size_t i = 0; i < neighborhood->size; i++)
	{
		if (neighborhood->neighborhood[i].mask && neighborhood->neighborhood[i].cellValue.value > max) {
			max = neighborhood->neighborhood[i].cellValue.value;
		}
	}
	return max;
}

/*
Obtient la valeur minimale parmi la valeur des voisins du voisinage specifie
*/
domainCellType nbdValueGetMin(Neighborhood *neighborhood) {
	if (neighborhood->size == 0) {
		fatalError("nbd GetMin Empty nbd");
	}

	domainCellType min = neighborhood->neighborhood[0].cellValue.value;

	for (size_t i = 0; i < neighborhood->size; i++)
	{
		if (neighborhood->neighborhood[i].mask && neighborhood->neighborhood[i].cellValue.value < min) {
			min = neighborhood->neighborhood[i].cellValue.value;
		}
	}
	return min;
}
/*
	Obtient le nombre de voisins ayant la valeur specifiee
*/
int nbdValueCount(Neighborhood *neighborhood, domainCellType value) {
	int count = 0;
	for (size_t i = 0; i < neighborhood->size; i++)
	{
		if (neighborhood->neighborhood[i].mask && neighborhood->neighborhood[i].cellValue.value == value) {
			count++;
		}
	}
	return count;
}

/*
	Obtient si oui ou non tous les voisins sont definis
*/
int nbdIsComplete(Neighborhood* neightborhood) {
	for (size_t i = 0; i < neightborhood->size; i++)
	{
		if (neightborhood->neighborhood[i].mask == 0) {
			return 0;
		}
	}
	return 1;
}

/*
	Obtient si oui ou non le voisin spécifié à sa valeur egale a value
*/
int nbdIsNeighborEquals(Neighborhood *neighborhood, int mask, domainCellType value) {
	for (size_t i = 0; i < neighborhood->size; i++)
	{
		if ((neighborhood->neighborhood[i].mask & mask) && neighborhood->neighborhood[i].cellValue.value != value) {
			return 0;
		}
	}
	return 1;
}

/*
	Obtient le voisin avec le mask specifie
*/
nbdNeighbor nbdGetByMask(Neighborhood *neighborhood, int mask) {
	for (size_t i = 0; i < neighborhood->size; i++)
	{
		if (neighborhood->neighborhood[i].mask & mask) {
			return neighborhood->neighborhood[i];
		}
	}
	nbdNeighbor n = {0};
	return n;
}