#include <stdbool.h>
#include <unistd.h>

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
bool handle_input(char *line, PROC_LIST *proc_list);
void reap_backgound_procs(PROC_LIST *proc_list);
bool preprocess_commands(char **tokens, PROC_LIST *proc_list, bool *background_process_flag);
bool execute_command(pid_t pid, char **tokens, PROC_LIST *proc_list);
void reaping_and_bookkeeping(pid_t pid, PROC_LIST *proc_list, bool background_process_flag);
void handle_prompt();

// process_list
PROC_LIST *init_proc_list(int n);
void append_proc_list(PROC_LIST *proc_list, int pid);
void free_proc_list(PROC_LIST *proc_list);
void clean_dead_proc(PROC_LIST *proc_list);

// signal
void handle_ctrl_c(int sig);
void handle_ctrl_c_running(int sig);
