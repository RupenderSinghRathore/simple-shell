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

	for (i = 0; i < (int)strlen(line); i++) {

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

bool handle_input(char *line, PROC_LIST *proc_list) {
	char curr_dir[MAX_DIR_SIZE];
	getcwd(curr_dir, MAX_DIR_SIZE);
	printf("%s $ ", curr_dir);

	scanf("%[^\n]", line);
	getchar();

	if (strcmp(line, "exit") == 0) {
		for (int i = 0; i < proc_list->len; i++)
			kill(proc_list->pids[i], SIGINT);
		return false;
	}
	line[strlen(line)] = '\n'; // terminate with new line
	return true;
}
