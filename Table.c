#include "Table.h"
#include <stdarg.h>

//bad & quick linux bypass
#ifdef WIN32
void printChar(int c) {
	printf("%c", c);
}
#else
void printChar(int c) {
	switch (c)
	{		
	case 179:printf("\u2502");break;	
	case 180:printf("\u2524");break;
	case 191:printf("\u2510");break;
	case 192:printf("\u2514");break;
	case 193:printf("\u2534");break;
	case 194:printf("\u252c");break;	
	case 195:printf("\u251c");break;	
	case 196:printf("\u2500");break;
	case 197:printf("\u253c");break;
	case 217:printf("\u2518");break;
	case 218:printf("\u250c");break;

	default:break;
	}
}
#endif

void table_print_horizontal(int cell_size) {
	for (int j = 0; j < cell_size; j++)
	{
		printChar(196);
	}
}

void table_print_top(int size, int cell_size) {
	printChar(218);
	for (int i = 0; i < size; i++)
	{
		table_print_horizontal(cell_size);
		printChar(i != size - 1 ? 194 : 191);
	}
	printf("\n");
}

void table_print_bottom(int size, int cell_size) {
	printChar(192);
	for (int i = 0; i < size; i++)
	{
		table_print_horizontal(cell_size);
		printChar(i != size - 1 ? 193 : 217);
	}
	printf("\n");
}

void table_print_separator(int size, int cell_size) {
	printChar( 195);
	for (int i = 0; i < size; i++)
	{
		table_print_horizontal(cell_size);
		printChar(i != size - 1 ? 197 : 180);
	}
	printf("\n");
}

/*
	Line print functions
*/

void table_print_int_line(int **table, int size, int row, int cell_size) {
	printChar(179);
	for (int i = 0; i < size; i++)
	{
		printf("%*d", cell_size, table[i][row]);
		printChar(179);
	}
	printf("\n");
}
void table_print_char_line(int **table, int size, int row, int cell_size, int param_count, va_list *list) {
	charTransformer function = va_arg(list, charTransformer);

	printChar(179);
	for (int i = 0; i < size; i++)
	{
		printf("%*c", cell_size, (*function)(table[i][row]));
		printChar(179);
	}
	printf("\n");
}

/*
	Table print function
*/

void table_print(int **table, void(*printFunction)(), int height, int width, int cell_size) {
	table_print_top(width, cell_size);
	for (int i = 0; i < height; i++)
	{
		(*printFunction)(table, width, i, cell_size);
		if (i < height - 1) {
			table_print_separator(width, cell_size);
		}
	}
	table_print_bottom(width, cell_size);
}

void table_print_with_params(int **table, void(*printFunction)(), int height, int width, int cell_size, int param_count, ...) {
	table_print_top(width, cell_size);
	for (int i = 0; i < height; i++)
	{
		va_list ap;
		va_start(ap, param_count);
		(*printFunction)(table, width, i, cell_size, param_count, ap);
		va_end(ap);

		if (i < height - 1) {
			table_print_separator(width, cell_size);
		}	
	}
	table_print_bottom(width, cell_size);
}