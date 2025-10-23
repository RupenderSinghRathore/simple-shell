#include "ref.h"
#include <stdbool.h>
#include <stdlib.h>

PROC_LIST *init_proc_list(int n) {
	PROC_LIST *proc_list = malloc(sizeof(*proc_list));
	proc_list->pids = malloc(n * sizeof(*proc_list->pids));
	proc_list->len = 0;
	return proc_list;
}

void append_proc_list(PROC_LIST *proc_list, int pid) {
	proc_list->pids[proc_list->len] = pid;
	proc_list->len++;
}
void free_proc_list(PROC_LIST *proc_list) {
	free(proc_list->pids);
	free(proc_list);
}
void clean_dead_proc(PROC_LIST *proc_list) {
	int i, j = 0;
	for (i = 0; i < proc_list->len; i++) {
		proc_list->pids[j] = proc_list->pids[i];
		if (proc_list->pids[i] != 0)
			j++;
		else
			proc_list->len--;
	}
}
