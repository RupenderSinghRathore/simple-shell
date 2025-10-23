#include "ref.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

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

bool handle_input(char *line) {
	char curr_dir[MAX_DIR_SIZE];
	getcwd(curr_dir, MAX_DIR_SIZE);
	printf("%s $ ", curr_dir);

	scanf("%[^\n]", line);
	getchar();

	if (strcmp(line, "exit") == 0) {
		// kill(pid, SIGINT);
		return false;
	}
	line[strlen(line)] = '\n'; // terminate with new line
	return true;
}
