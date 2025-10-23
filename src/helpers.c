#include "ref.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

void handle_cd(char **tokens, char *curr_dir) {
	char move_to_dir[MAX_DIR_SIZE];
	if (tokens[1] == NULL) {
		// cd into home
		strcpy(move_to_dir, getenv("HOME"));
	} else if (tokens[1][0] != '/') {
		// relative cd
		strcpy(move_to_dir, curr_dir);
		strcat(move_to_dir, "/");
		strcat(move_to_dir, tokens[1]);
	} else {
		strcpy(move_to_dir, tokens[1]);
	}
	int ok = chdir(move_to_dir);
	if (ok == -1) {
		printf("cd: no such file or directory: %s\n", tokens[1]);
	}
}

bool handle_input(char *line) {
	scanf("%[^\n]", line);
	getchar();

	if (strcmp(line, "exit") == 0) {
		// kill(pid, SIGINT);
		return false;
	}
	line[strlen(line)] = '\n'; // terminate with new line
	return true;
}
