#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024
int main (int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: ./cp-custom <FILEFROM> <FILETO>");
        return 1;
    }

    char buf[1];
    int fd_in, fd_out;
    int bytes_read;
    off_t counter = 0;

    fd_in = open(argv[1], O_RDWR);
    if (fd_in == -1) {
        fprintf(stderr, "open() failed.\n");
        return 1;
    }

    fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_in == -1) {
        fprintf(stderr, "open() failed.\n");
        return 1;
    }

    while ((bytes_read = read(fd_in, buf, sizeof(buf))) > 0)
    {
        if (buf[0] == '\0') {
            counter ++;
        } else {
            if (counter > 0) {
                printf("counter = %ld\n", counter);

                if (lseek(fd_out, counter, SEEK_CUR) == -1) {
                    fprintf(stderr, "lseek() failed.\n");
                    return 1;
                }

                counter = 0;
            }

            if (write(fd_out, buf, bytes_read) == -1) {
                fprintf(stderr, "write failed().\n");
                return 1;
            }   
        }
    }

    if (close(fd_in) == -1) {
        fprintf(stderr, "close() failed.\n");
        return 1;
    }

    if (close(fd_out) == -1) {
        fprintf(stderr, "close() failed.\n");
        return 1;
    }

    return 0;
}