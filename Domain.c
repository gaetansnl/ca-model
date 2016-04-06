#include <stdio.h>
#include <stdlib.h>

#include "Domain.h"
#include "Error.h"

/*
	Alloue un nouveau domaine avec la taille spécifiée
*/
Domain *domainCreate(int width, int height) {
	Domain *r = calloc(1, sizeof(Domain));
	r->width = width;
	r->height = height;

	r->array = calloc(width, sizeof(domainCellType*));
	r->array[0] = calloc(width * height, sizeof(domainCellType));

	for (size_t i = 1; i < r->width; i++)
	{
		r->array[i] = r->array[0] + i * r->height;
	}

	return r;
}

/*
	Obtient une copie du domaine spéficifié
*/
Domain *domainCopy(Domain *domain) {
	Domain *r = domainCreate(domain->width, domain->height);

	for (size_t i = 0; i < domain->width; i++)
	{
		for (size_t j = 0; j < domain->height; j++)
		{
			r->array[i][j] = domain->array[i][j];
		}
	}

	return r;
}

/*
	Domain dispose
*/
void domainFree(Domain *domain) {
	free(domain->array[0]);
	free(domain->array);
	free(domain);
}

/*
	Obtient la cellule du domaine aux coordonnees specifiees
*/
domainCell domainGetCell(domainCellCoord p, Domain *domain){
	if (domainIsOutside(p, domain)) {
		fatalError("domainCellCoord not in domain");
	}

	domainCell c;
	c.domain = domain;
	c.value =  &(domain->array[p.x][p.y]);
	c.cellCoord = p;

	return c;
}

/*
	Obtient domainCellValue aux coordonnees specifiees.

	domainCellValue représente une valeur et ses coordonnées
*/
domainCellValue domainGetCellValue(domainCellCoord p, Domain *domain) {
	if (domainIsOutside(p, domain)) {
		fatalError("domainCellCoord not in domain");
	}

	domainCellValue c;
	c.value = domain->array[p.x][p.y];
	c.cellCoord = p;

	return c;
}

/*
	Obtient si la cellule aux coordonnees specifiees est dans le domaine specifie
*/
int domainIsOutside(domainCellCoord coord, Domain *domain) {
	return coord.x >= domain->width || coord.x < 0 || coord.y >= domain->height || coord.y < 0;
}

/*
	Obtient si les domaines specifiés leurs valeurs égales
*/
int domainCompare(Domain *domain, Domain *source) {
	if ((domain->width != source->width) || (domain->height != source->height)) {
		fatalError("domainCompare size not equal");
	}

	for (size_t i = 0; i < domain->width; i++)
	{
		for (size_t j = 0; j < domain->height; j++)
		{
			if (domain->array[i][j] != source->array[i][j]) {
				return 0;
			}
		}
	}

	return 1;
}