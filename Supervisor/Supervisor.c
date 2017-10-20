#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

// include child process name and its args
#define MAX_NUM_ARGS 100

int main(int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <exe_path> <args...>\n", argv[0]);
        exit(-1);
    }

    char *child_argv[MAX_NUM_ARGS];
    for (int i = 1; i < argc; i++) {
        child_argv[i - 1] = (char *)malloc(strlen(argv[i]) + 1);
        strncpy(child_argv[i - 1], argv[i], strlen(argv[i]));
        child_argv[i - 1][strlen(argv[i])] = '\0';
    }

    pid_t pid;
    int ret;
    int status;

    while (1) {
        pid = fork();
        if (pid == -1) {
            fprintf(stderr, "fork() error.errno: %d error: %s\n", errno, strerror(errno));
            exit(-1);
        }

        if (pid == 0) {
            ret = execv(child_argv[0], (char **)child_argv);
            if (ret < 0) {
                fprintf(stderr, "execv ret: %d errno: %d error: %s\n", ret, errno, strerror(errno));
                continue;
            }
            exit(-2);
        }

        if (pid > 0) {
            pid = wait(&status);
            fprintf(stdout, "wait return");
        }
    }
    return 0;
}