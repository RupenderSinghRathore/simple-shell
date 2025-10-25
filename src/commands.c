#include "ref.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>

bool preprocess_commands(char **tokens, PROC_LIST *proc_list, bool *background_process_flag) {
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
		char move_to_dir[MAX_DIR_SIZE];
		if (tokens[1] == NULL) {
			// cd into home
			strcpy(move_to_dir, getenv("HOME"));
		} else if (tokens[1][0] != '/') {
			// relative cd
			char curr_dir[MAX_DIR_SIZE];
			getcwd(curr_dir, sizeof(curr_dir));
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
	} else {
		signal(SIGINT, handle_ctrl_c_running);
	}
	return true;
}
