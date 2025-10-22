#include "ref.h"
#include <stdlib.h>
#include <string.h>

void freeToken(char **tokens) {
	for (int i = 0; tokens[i] != NULL; i++) {
		free(tokens[i]);
	}
	free(tokens);
}

char **tokenize(char *line) {
	char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char token[MAX_TOKEN_SIZE];
	int i, tokenIndex = 0, tokenNo = 0;

	for (i = 0; i < strlen(line); i++) {

		char readChar = line[i];

		if (readChar == ' ' || readChar == '\n' || readChar == '\t') {
			token[tokenIndex] = '\0';
			if (tokenIndex != 0) {
				tokens[tokenNo++] = strdup(token);
				tokenIndex = 0;
			}
		} else {
			token[tokenIndex++] = readChar;
		}
	}

	tokens[tokenNo] = NULL;
	return tokens;
}

int get_last_idx(char **tokens) {
	int i;
	for (i = 0; tokens[i] != NULL; i++);
	return i - 1;
}
