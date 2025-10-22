#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_DIR_SIZE 256

void freeToken(char **tokens);
int get_last_idx(char **tokens);
char **tokenize(char *line);
