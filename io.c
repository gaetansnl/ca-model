#include <stdio.h>
#include "io.h"

char getOneChar() {
	char c = getchar();
	while (c != '\n' && getchar() != '\n');
	return c;
}
int yesno() {
	switch (getOneChar())
	{
		case 'y':
			return 1;
		break;
		case 'n':
			return 0;
			break;
	default:
		printf("Reponse invalide. Entrez y/n \n");
		return yesno();
		break;
	}
}