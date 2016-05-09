#pragma once
#define domainCellType int

/*
	Structure representant le domaine
*/
typedef struct {
	domainCellType **array;
	int width;
	int height;	
} Domain;

/*
	Structure representant les coordonnees d'une cellule
*/
typedef struct {
	unsigned int x;
	unsigned int y;
} domainCellCoord;

/*
	Structure representant une cellule associee a un domaine
*/
typedef struct {
	domainCellCoord cellCoord;
	Domain *domain;
	domainCellType *value;
} domainCell;

/*
	Structure representant la valeur d'une cellule associee a des coordonnees
*/
typedef struct {
	domainCellCoord cellCoord;
	domainCellType value;
} domainCellValue;

Domain *domainCreate(int width, int height);
void domainFree(Domain *domain);
Domain *domainCopy(Domain *domain);
int domainCompare(Domain *domain, Domain *source);

domainCell domainGetCell(domainCellCoord p, Domain *domain);
domainCellValue domainGetCellValue(domainCellCoord p, Domain *domain);
int domainIsOutside(domainCellCoord coord, Domain *domain);