#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int status;
    pid_t pid = fork();

    if (pid == 0)
        return 3;
    else {
        printf("Child Pid: %d \n", pid);
        pid = fork();
        if (pid == 0)
            exit(7);
        else {
            printf("Child Pid: %d \n", pid);
            wait(&status);
            if (WIFEXITED(status))
                printf("Child send one: %d \n", WEXITSTATUS(status));

            wait(&status);
            if (WIFEXITED(status))
                printf("Child send two: %d \n", WEXITSTATUS(status));

            sleep(30);
        }
    }

    return 0;
}