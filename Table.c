#include "Table.h"

void table_print_horizontal(int cell_size) {
	for (size_t j = 0; j < cell_size; j++)
	{
		printf("%c", 0xC4);
	}
}

void table_print_top(int size, int cell_size) {
	printf("%c", 218);
	for (size_t i = 0; i < size; i++)
	{
		table_print_horizontal(cell_size);
		printf("%c", i != size - 1 ? 194 : 191);
	}
	printf("\n");
}

void table_print_bottom(int size, int cell_size) {
	printf("%c", 192);
	for (size_t i = 0; i < size; i++)
	{
		table_print_horizontal(cell_size);
		printf("%c", i != size - 1 ? 193 : 217);
	}
	printf("\n");
}

void table_print_separator(int size, int cell_size) {
	printf("%c", 195);
	for (size_t i = 0; i < size; i++)
	{
		table_print_horizontal(cell_size);
		printf("%c", i != size - 1 ? 197 : 180);
	}
	printf("\n");
}

void table_print_int_line(int **table, int size, int row, int cell_size) {
	printf("%c", 179);
	for (size_t i = 0; i < size; i++)
	{
		printf("%*d", cell_size, table[i][row]);
		printf("%c", 179);
	}
	printf("\n");
}

void table_print_int(int **table,int height, int width, int cell_size) {
	table_print_top(width, cell_size);
	for (size_t i = 0; i < height; i++)
	{
		table_print_int_line(table, width, i, cell_size);
		if (i < height - 1) {
			table_print_separator(width, cell_size);
		}	
	}
	table_print_bottom(width, cell_size);
}