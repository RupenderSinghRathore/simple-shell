#include "ref.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

void handle_ctrl_c(int sig) {
	// printf("\tIntrupted\n");
	printf("\n");
	handle_prompt();
	fflush(stdout);
}
void handle_ctrl_c_running(int sig) {
}
