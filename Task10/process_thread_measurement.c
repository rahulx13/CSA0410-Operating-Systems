
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>

#define COUNT 1000

void *thread_function(void *arg)
{
    return NULL;
}

double get_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

int main(void)
{
    int i;
    double start, end;

    printf("PROCESS AND THREAD PERFORMANCE\n");
    printf("Number of operations: %d\n\n", COUNT);

    // Measure process creation and termination
    start = get_time();

    for (i = 0; i < COUNT; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {
            _exit(0);
        }

        if (waitpid(pid, NULL, 0) < 0) {
            perror("waitpid");
            return 1;
        }
    }

    end = get_time();

    printf("PROCESS MEASUREMENT\n");
    printf("Processes created: %d\n", COUNT);
    printf("Total time: %.6f seconds\n", end - start);

    // Measure thread creation and joining
    pthread_t thread;

    start = get_time();

    for (i = 0; i < COUNT; i++) {
        int result = pthread_create(
            &thread, NULL, thread_function, NULL
        );

        if (result != 0) {
            printf("Thread creation failed: %d\n", result);
            return 1;
        }

        result = pthread_join(thread, NULL);

        if (result != 0) {
            printf("Thread joining failed: %d\n", result);
            return 1;
        }
    }

    end = get_time();

    printf("\nTHREAD MEASUREMENT\n");
    printf("Threads created: %d\n", COUNT);
    printf("Total time: %.6f seconds\n", end - start);

    printf("\nMeasurement completed successfully.\n");

    return 0;
}
