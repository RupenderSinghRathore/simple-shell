#include "ref.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[]) {
	char line[MAX_INPUT_SIZE];
	char **tokens = NULL;
	PROC_LIST *proc_list = init_proc_list(MAX_PROC_COUNT);
	int i;

	while (1) {
		int status, pre_pid;
		while ((pre_pid = waitpid(-1, &status, WNOHANG)) > 0) {
			for (i = 0; i < proc_list->len; i++) {
				if (proc_list->pids[i] == pre_pid) {
					proc_list->pids[i] = 0;
				}
			}
			clean_dead_proc(proc_list);
			printf("Background process exited with: %d\n", WEXITSTATUS(status));
		}
		printf("len: %d\n", proc_list->len);

		char curr_dir[MAX_DIR_SIZE];
		getcwd(curr_dir, MAX_DIR_SIZE);
		printf("%s $ ", curr_dir);

		bzero(line, sizeof(line));
		if (!handle_input(line)) {
			break;
		}

		tokens = tokenize(line);

		bool background_process_flag = false;
		int last_idx = get_last_idx(tokens);
		// printf("index: %d\n", last_idx);

		if (tokens[0] == NULL) {
			freeToken(tokens);
			continue;
		} else if (strcmp(tokens[last_idx], "&") == 0) { // if equal then
			if (proc_list->len == MAX_PROC_COUNT) {
				printf("Error Max processes running in background\n");
				continue;
			}
			free(tokens[last_idx]);
			tokens[last_idx] = NULL;
			background_process_flag = 1;
		} else if (strcmp(tokens[0], "cd") == 0) {
			handle_cd(tokens, curr_dir);
			freeToken(tokens);
			continue;
		}

		// Child process
		int pid = fork();
		if (pid == -1) {
			printf("Error!!\n");
		} else if (pid == 0) {
			int ok = execvp(tokens[0], tokens);
			printf("xsh: command not found: %s\n", tokens[0]);
			freeToken(tokens);
			free_proc_list(proc_list);
			exit(1);
		}
		if (background_process_flag) {
			append_proc_list(proc_list, pid);
		} else {
			// forground
			pid = waitpid(pid, &status, 0);
			printf("EXITSTATUS: %d\n", WEXITSTATUS(status));
		}
		freeToken(tokens);
	}
	free_proc_list(proc_list);
	return 0;
}
