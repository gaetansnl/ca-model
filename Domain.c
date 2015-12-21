#include <stdio.h>
#include <stdlib.h>

#include "Domain.h"
#include "Error.h"

size_t domainArraySize(int width, int height) {
	return width * sizeof(domainCellType*) + width * height * sizeof(domainCellType);
}

Domain *domainCreate(int width, int height) {
	Domain *r = malloc(sizeof(Domain) + domainArraySize(width, height));
	r->width = width;
	r->height = height;

	r->array = r + 1;

	for (size_t i = 0; i < r->width; i++)
	{
		r->array[i] = r->array + r->width + i * r->height;
	}

	return r;
}

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

void domainFree(Domain *domain) {
	free(domain);
}

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

domainCellValue domainGetCellValue(domainCellCoord p, Domain *domain) {
	if (domainIsOutside(p, domain)) {
		fatalError("domainCellCoord not in domain");
	}

	domainCellValue c;
	c.value = domain->array[p.x][p.y];
	c.cellCoord = p;

	return c;
}

//void domainSetCell(domainCellCoord p, domainCellType value, Domain *domain) {
//	if (p.x >= domain->width || p.y >= domain->height) {
//		fatalError("domainCellCoord not in domain");
//	}
//
//	*(domain->array + p.x + domain->width * p.y) = value;
//}

int domainIsOutside(domainCellCoord coord, Domain *domain) {
	return coord.x > domain->width || coord.x < 0 || coord.y > domain->height || coord.y < 0;
}