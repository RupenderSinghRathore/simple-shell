#include "ref.h"
#include <signal.h>
#include <strings.h>
#include <unistd.h>

int main() {
	char line[MAX_INPUT_SIZE];
	char **tokens = NULL;
	PROC_LIST *proc_list = init_proc_list(MAX_PROC_COUNT);

	while (1) {
		signal(SIGINT, handle_ctrl_c);
		reap_backgound_procs(proc_list);

		handle_prompt();
		bzero(line, sizeof(line));
		if (!handle_input(line, proc_list)) {
			break;
		}

		tokens = tokenize(line);

		bool background_process_flag = false;

		if (!preprocess_commands(tokens, proc_list, &background_process_flag)) {
			freeToken(tokens);
			continue;
		}

		pid_t pid = fork();
		if (!execute_command(pid, tokens, proc_list)) {
			freeToken(tokens);
			continue;
		}

		reaping_and_bookkeeping(pid, proc_list, background_process_flag);
		freeToken(tokens);
	}
	free_proc_list(proc_list);
	return 0;
}
