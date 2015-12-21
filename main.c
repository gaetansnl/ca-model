#include <stdio.h>
#include "Domain.h"
#include "Transition.h"
#include "Table.h"
#include <stdlib.h>
#include <time.h>

#include "Neighborhood.h"

int current = 1;
void transition(domainCellValue cell, Neighborhood *nbd) {
	return current;
}

void control(Domain *domain, TransitionsExecControl *control) {
	current++;
	system(clear);
	table_print_int(domain->array, domain->height, domain->width, 2);
	printf("Epoch %d\n", control->timeStep);
	printf("******************************************************\n");
	printf("*                      STATE CONTROL                 *\n");
	printf("* (ENTREE) RUNNING - (a) ABORTED - (e) FINISHED *\n");
	printf("******************************************************\n");

	char c;
	c = getchar();

	switch (c)
	{
		case 'a':
			control->state = ABORTED;
			break;
		case 'e':
			control->state = FINISHED;
			break;
	default:
		break;
	}
}

int main() {
	int table[4][4] = {
		{1,2,3,4},
		{ 1,2,3,4 },
		{ 1,2,3,4 },
		{ 1,2,3,4 }
	};

	Domain *a = domainCreate(10, 10);
	Domain *b = domainCopy(a);
	TransitionsConfig config = {1};
	te_run(a, config, transition, control);
	//domainCellCoord c = { 4,4 };
	//Neighborhood *n = createNeighborhood(a, c, 10);


//	table_print_int(table, 4, 4, 2);
//	getchar();
//	int b = TOP | RIGHT;
//	
//	te_run(a, transition, NULL);
//
//
//	domainCellCoord c = {9, 9};
//	setDomainCell(c, 100, a);
////	Cell b = getDomainCell(c, a);
//	//*(b.value) = 10;
//	//int aaaa = *(b.value);
//	int x;
//	freeDomain(a);
//	getchar(&x);
//	//int table[2][2] = { {11,21},{31,41} };
//	
//	
//	print_top(10, 6);
//	print_line(10, 6);
//	print_separator(10, 6);
//	print_line(10, 6);
//	print_bottom(10, 6);
//	//call(test);
//	//printf("%c",180 );
//	
//	getchar(&x);
}