#include <arpa/inet.h>
#include <headers/common.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char **argv) {
    int serv_sock, client_sock;
    FILE *readfp;
    FILE *writefp;

    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_addr_size;
    char buf[BUF_SIZE] = {
        0,
    };
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    client_addr_size = sizeof(client_addr);
    client_sock =
        accept(serv_sock, (struct sockaddr *)&client_addr, &client_addr_size);

    readfp = fdopen(client_sock, "r");
    writefp = fdopen(dup(client_sock), "w");

    fputs("FORM SERVER: Hi~ client? \n", writefp);
    fputs("I love all of the world \n", writefp);
    fputs("You are awesome! \n", writefp);
    fflush(writefp);

    shutdown(fileno(writefp), SHUT_WR);
    fclose(writefp);

    fgets(buf, sizeof(buf), readfp);
    fputs(buf, stdout);
    fclose(readfp);
    return 0;
}
