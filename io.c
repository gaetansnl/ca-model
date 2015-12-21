#include "io.h"

char getOneChar() {
	char c = getchar();
	while (c != '\n' && getchar() != '\n');
	return c;
}
int yesno() {
	return getOneChar() == 'y';
}