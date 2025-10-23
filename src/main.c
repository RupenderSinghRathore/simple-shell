#include "ref.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[]) {
	char line[MAX_INPUT_SIZE];
	char **tokens = NULL;
	PROC_LIST *proc_list = init_proc_list(MAX_PROC_COUNT);
	int i;

	while (1) {
		reap_backgound_procs(proc_list);

		char curr_dir[MAX_DIR_SIZE];
		getcwd(curr_dir, MAX_DIR_SIZE);
		printf("%s $ ", curr_dir);

		bzero(line, sizeof(line));
		if (!handle_input(line)) {
			break;
		}

		tokens = tokenize(line);

		bool background_process_flag = false;

		if (!preprocess_commands(tokens, proc_list, curr_dir, &background_process_flag)) {
			freeToken(tokens);
			continue;
		}

		// process spawning
		pid_t pid = fork();
		if (!execute_command(pid, tokens, proc_list)) {
			freeToken(tokens);
			continue;
		}

		// bookkeeping
		if (background_process_flag) {
			append_proc_list(proc_list, pid);
		} else {
			// forground
			int status;
			pid = waitpid(pid, &status, 0);
			printf("EXITSTATUS: %d\n", WEXITSTATUS(status));
		}
		freeToken(tokens);
	}
	free_proc_list(proc_list);
	return 0;
}
