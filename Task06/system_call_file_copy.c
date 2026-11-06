
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    const char *source = "records.dat";
    const char *destination = "copied_records.dat";
    int buffers[] = {1, 512, 4096};
    char data[10000];
    int i;

    memset(data, 'A', sizeof(data));

    int fd = open(source, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0) {
        perror("open source");
        return 1;
    }

    if (write(fd, data, sizeof(data)) != sizeof(data)) {
        perror("write source");
        close(fd);
        return 1;
    }

    close(fd);

    printf("SYSTEM CALL COST - FILE COPY\n");
    printf("File size: %lu bytes\n\n",
           (unsigned long)sizeof(data));

    for (i = 0; i < 3; i++) {
        char buffer[4096];
        int input, output;
        ssize_t bytes;
        long read_calls = 0;

        input = open(source, O_RDONLY);

        if (input < 0) {
            perror("open input");
            return 1;
        }

        output = open(destination,
                      O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (output < 0) {
            perror("open output");
            close(input);
            return 1;
        }

        while ((bytes = read(input, buffer, buffers[i])) > 0) {
            read_calls++;

            ssize_t written = 0;

            while (written < bytes) {
                ssize_t result = write(output,
                                       buffer + written,
                                       bytes - written);

                if (result < 0) {
                    perror("write");
                    close(input);
                    close(output);
                    return 1;
                }

                written += result;
            }
        }

        read_calls++;

        if (bytes < 0) {
            perror("read");
            close(input);
            close(output);
            return 1;
        }

        close(input);
        close(output);

        printf("Buffer size: %d bytes\n", buffers[i]);
        printf("Read system calls: %ld\n\n", read_calls);
    }

    unlink(source);
    unlink(destination);

    return 0;
}
