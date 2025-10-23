#include "ref.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

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

void reap_backgound_procs(PROC_LIST *proc_list) {
	int status, pre_pid;
	while ((pre_pid = waitpid(-1, &status, WNOHANG)) > 0) {
		for (int i = 0; i < proc_list->len; i++) {
			if (proc_list->pids[i] == pre_pid) {
				proc_list->pids[i] = 0;
			}
		}
		clean_dead_proc(proc_list);
		printf("Background process exited with: %d\n", WEXITSTATUS(status));
	}
}

bool preprocess_commands(char **tokens, PROC_LIST *proc_list, char *curr_dir, bool *background_process_flag) {
	int last_idx = get_last_idx(tokens);
	if (tokens[0] == NULL) {
		return false;
	} else if (strcmp(tokens[last_idx], "&") == 0) { // if equal then
		if (proc_list->len == MAX_PROC_COUNT) {
			printf("Error Max processes running in background\n");
			return false;
		}
		free(tokens[last_idx]);
		tokens[last_idx] = NULL;
		*background_process_flag = true;
	} else if (strcmp(tokens[0], "cd") == 0) {
		handle_cd(tokens, curr_dir);
		return false;
	}
	return true;
}

bool execute_command(pid_t pid, char **tokens, PROC_LIST *proc_list) {
	if (pid == -1) {
		printf("Error!!\n");
		return false;
	} else if (pid == 0) {
		int ok = execvp(tokens[0], tokens);
		printf("xsh: command not found: %s\n", tokens[0]);
		freeToken(tokens);
		free_proc_list(proc_list);
		exit(1);
	}
	return true;
}
