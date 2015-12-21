#include <stdio.h>
#include <stdlib.h>

#include "Domain.h"
#include "Error.h"

/*Helper Function
	Obtient la taille du tableau representant le domaine
*/

size_t domainArraySize(int width, int height) {
	return width * sizeof(domainCellType*) + width * height * sizeof(domainCellType);
}

/*
	Obtient un nouveau domaine
*/
Domain *domainCreate(int width, int height) {
	Domain *r = calloc(1, sizeof(Domain) + domainArraySize(width, height));
	r->width = width;
	r->height = height;

	r->array = r + 1;

	for (size_t i = 0; i < r->width; i++)
	{
		r->array[i] = r->array + r->width + i * r->height;
	}

	return r;
}

/*
	Obtient une copie du domaine precise
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
	Obtient la valeur de la cellule aux coordonnees specifiees
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
	Obtient si la la cellule aux coordonnees specifiees est dans le domaine specifie
*/
int domainIsOutside(domainCellCoord coord, Domain *domain) {
	return coord.x > domain->width || coord.x < 0 || coord.y > domain->height || coord.y < 0;
}
