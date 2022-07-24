#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv) {
    FILE *fp;
    int fd = open("data/data.dat", O_RDONLY);

    if (fd == -1) {
        fputs("file open error\n", stdout);
        return -1;
    }

    printf("First file descriptor: %d\n", fd);
    fp = fdopen(fd, "r");
    fputs("TCP/IP SOCKET PROGRAMMING \n", fp);
    printf("Second file descriptor: %d \n", fileno(fp));
    fclose(fp);

    return 0;
}
