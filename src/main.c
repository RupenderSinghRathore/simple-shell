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
	int i;

	while (1) {
		int status, pre_pid;
		while ((pre_pid = waitpid(-1, &status, WNOHANG)) > 0) {
			printf("Background process exited with: %d\n", WEXITSTATUS(status));
		}

		// clear the input buffer
		bzero(line, sizeof(line));

		char curr_dir[MAX_DIR_SIZE];
		getcwd(curr_dir, MAX_DIR_SIZE);

		printf("%s $ ", curr_dir);

		scanf("%[^\n]", line);
		getchar();

		if (strcmp(line, "exit") == 0) {
			break;
		}
		line[strlen(line)] = '\n'; // terminate with new line
		tokens = tokenize(line);

		int background_process_flag = 0;
		int last_idx = get_last_idx(tokens);
		// printf("index: %d\n", last_idx);

		if (tokens[0] == NULL) {
			freeToken(tokens);
			continue;
		} else if (strcmp(tokens[last_idx], "&") == 0) { // if equal then
			free(tokens[last_idx]);
			tokens[last_idx] = NULL;
			background_process_flag = 1;
		} else if (strcmp(tokens[0], "cd") == 0) {
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
			exit(1);
		}

		if (background_process_flag == 0) {
			// wait(&status);
			pid = waitpid(pid, &status, 0);
		} else {
			pid = waitpid(pid, &status, WNOHANG);
		}
		if (!background_process_flag && WIFEXITED(status)) {
			printf("EXITSTATUS: %d\n", WEXITSTATUS(status));
		}
		freeToken(tokens);
	}
	return 0;
}
