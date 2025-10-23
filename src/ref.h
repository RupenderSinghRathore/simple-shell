#include <stdbool.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_DIR_SIZE 256
#define MAX_PROC_COUNT 2

typedef struct {
	int len;
	int *pids;
} PROC_LIST;

void freeToken(char **tokens);
int get_last_idx(char **tokens);
char **tokenize(char *line);
void handle_cd(char **tokens, char *curr_dir);
bool handle_input(char *line);

// process_list
PROC_LIST *init_proc_list(int n);
void append_proc_list(PROC_LIST *proc_list, int pid);
void free_proc_list(PROC_LIST *proc_list);
void clean_dead_proc(PROC_LIST *proc_list);
