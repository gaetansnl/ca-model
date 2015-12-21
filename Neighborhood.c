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
	Neighborhood *n = malloc(sizeof(Neighborhood) + neighborsCount * sizeof(nbdNeighbor));
	n->neighborhood = n + 1;
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
			if ((i != 0 || j != 0) && (currentMask & n->mask)) {
				domainCellCoord coord = { cellCoord.x + i, cellCoord.y + j };
				nbdNeighbor newNeighbor;

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

void freeNeighborhood(Neighborhood *neighborhood) {
	free(neighborhood);
	neighborhood = NULL;
}

int nbdCountByValue(Neighborhood *neighborhood, int mask, domainCellType value) {
	int count = 0;
	for (size_t i = 0; i < neighborhood->size; i++)
	{
		if ((neighborhood->neighborhood[i].mask & mask) && neighborhood->neighborhood[i].cellValue.value == value) {
			count++;
		}
	}
	return count;
}

int nbdIsNeighborEquals(Neighborhood *neighborhood, int mask, domainCellType value) {
	for (size_t i = 0; i < neighborhood->size; i++)
	{
		if ((neighborhood->neighborhood[i].mask & mask) && neighborhood->neighborhood[i].cellValue.value != value) {
			return 0;
		}
	}
	return 1;
}

nbdNeighbor nbdGetByMask(Neighborhood *neighborhood, int mask) {
	for (size_t i = 0; i < neighborhood->size; i++)
	{
		if (neighborhood->neighborhood[i].mask & mask) {
			return neighborhood->neighborhood[i];
		}
	}
	fatalError("Neighbor not found by mask");
}