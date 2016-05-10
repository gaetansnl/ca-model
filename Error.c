#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Error.h"

void fatalError(char *message) {
	fprintf(stderr, "%s", message);
	fprintf(stderr, "\n");
	exit(EXIT_FAILURE);
}