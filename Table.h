#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void table_print(int **table, void(*printFunction)(), int height, int width, int cell_size);
void table_print_with_params(int **table, void(*printFunction)(), int height, int width, int cell_size, int param_count, ...);

void table_print_int_line(int **table, int size, int row, int cell_size);
void table_print_char_line(int **table, int size, int row, int cell_size, int param_count, va_list *list);

typedef char(*charTransformer)(int);