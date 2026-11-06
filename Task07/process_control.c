
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    int status;

    printf("PROCESS CONTROL DEMONSTRATION\n");

    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("Child 1: Executing a new program\n");
        execlp("expr", "expr", "6", "*", "7", (char *)NULL);

        perror("execlp");
        _exit(1);
    }

    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
        printf("Parent: Child 1 exited with status %d\n",
               WEXITSTATUS(status));
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("Child 2: Exiting with status 3\n");
        _exit(3);
    }

    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
        printf("Parent: Child 2 exited with status %d\n",
               WEXITSTATUS(status));
    }

    printf("Parent: All child processes completed.\n");

    return 0;
}
