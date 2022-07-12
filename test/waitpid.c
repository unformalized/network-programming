#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int status;
    pid_t pid = fork();

    if (pid == 0) {
        puts("child process");
        sleep(15);
        return 24;
    } else {
        while (!waitpid(-1, &status, WNOHANG)) {
            sleep(3);
            puts("sleep 3sec.");
        }

        if (WIFEXITED(status))
            printf("Child send two: %d \n", WEXITSTATUS(status));
    }

    return 0;
}
