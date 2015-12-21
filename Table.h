#pragma once

#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
#define clear "cls"
#else
#define clear "clear"
#endif

void table_print_int(int **table, int height, int width, int cell_size);