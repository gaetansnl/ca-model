#pragma once
#define domainCellType int

typedef struct {
	domainCellType **array;
	int width;
	int height;	
} Domain;

typedef struct {
	int x;
	int y;
} domainCellCoord;

typedef struct {
	domainCellCoord cellCoord;
	Domain *domain;
	domainCellType *value;
} domainCell;

typedef struct {
	domainCellCoord cellCoord;
	domainCellType value;
} domainCellValue;

Domain *domainCreate(int width, int height);
void domainFree(Domain *domain);

Domain *domainCopy(Domain *domain);

domainCell domainGetCell(domainCellCoord p, Domain *domain);
domainCellValue domainGetCellValue(domainCellCoord p, Domain *domain);
//void domainSetCell(domainCellCoord p, domainCellType value, Domain *domain);
int domainIsOutside(domainCellCoord coord, Domain *domain);