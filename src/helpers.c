#include "ref.h"
#include <stdlib.h>

void freeToken(char **tokens) {
	for (int i = 0; tokens[i] != NULL; i++) {
		free(tokens[i]);
	}
	free(tokens);
}

int get_last_idx(char **tokens) {
	int i;
	for (i = 0; tokens[i] != NULL; i++);
	return i - 1;
}
