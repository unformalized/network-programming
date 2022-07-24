#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 3

int main(int argc, char **argv) {
    int fd1, fd2;
    int len;
    char buf[BUF_SIZE];

    fd1 = open("data/bigdata.txt", O_RDONLY);
    fd2 = open("data/syscpy_bigdata.txt", O_WRONLY | O_CREAT | O_TRUNC);

    while ((len = read(fd1, buf, sizeof(buf))) > 0)
        write(fd2, buf, len);

    return 0;
}
