/*  
    Usage: ./tee-custom [-a] <FILE>...
    Flags:
        -a : append input to existing files
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>


#define BUFFER_SIZE 1024


int main (int argc, char* argv[]) {
    
    if (argc < 2 || (argc == 2 && !strcmp(argv[1], "-a"))) {
        printf("Usage: ./tee-custom [-a] <FILE>...\n");
        return 1;
    }

    int fd, ofs = 0;
    char buf[1000];

    if (!strcmp(argv[1], "-a")) {
        ofs = 1;
    }

    // printf("ofs: %d", strcmp(argv[1], "-a"));

    while(1) {
        
        memset(buf, 0, sizeof(buf));
        int bytes_received = read(STDIN_FILENO, buf, sizeof(buf));

        if (!bytes_received) {
            fprintf(stderr, "Failed to read().\n");
            return 1;
        }

        printf("%s", buf);

        for(int i = ofs+1; i < argc; ++i) {
            
            if (ofs) {
                fd = open(argv[i], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
            }
            else {
                fd = open(argv[i], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
            }
            
            if (fd == -1) {
                fprintf(stderr, "Failed to open(). %s\n", strerror(errno));
                return 1;
            }

            if (write(fd, buf, strlen(buf)) <= 0) {
                fprintf(stderr, "Failed to write(). %s\n", strerror(errno));
                return 1;
            }

            if (close(fd) == -1) {
                fprintf(stderr, "Failed to close().\n");
                return 1;
            }
        }
    }
    return 0; 
}