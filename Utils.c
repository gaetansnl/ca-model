#include "Error.h"

void invariant(int value, int variable, char *error) {
	if (value != variable) {
		fatalError(error);
	}
}