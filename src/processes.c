#include "ref.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

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
void reaping_and_bookkeeping(pid_t pid, PROC_LIST *proc_list, bool background_process_flag) {
	if (background_process_flag) {
		append_proc_list(proc_list, pid);

	} else {
		int status;
		waitpid(pid, &status, 0);
		printf("EXITSTATUS: %d\n", WEXITSTATUS(status));
	}
}
