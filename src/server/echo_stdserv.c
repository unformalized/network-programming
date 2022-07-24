#include <arpa/inet.h>
#include <headers/common.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char const *argv[]) {
    int serv_sock, client_sock;
    char message[BUF_SIZE];
    int str_len, i;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;

    FILE *readfp;
    FILE *writefp;

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
        -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    client_addr_size = sizeof(client_addr);

    for (int i = 0; i < 5; i++) {
        client_sock = accept(serv_sock, (struct sockaddr *)&client_addr,
                             &client_addr_size);
        if (client_sock == -1)
            error_handling("accept() error");
        else
            printf("Connected client \n");

        readfp = fdopen(client_sock, "r");
        writefp = fdopen(client_sock, "w");
        while (!feof(readfp)) {
            fgets(message, BUF_SIZE, readfp);
            fputs(message, writefp);
            fflush(writefp);
        }
        fclose(readfp);
        fclose(writefp);
    }

    close(serv_sock);
    return 0;
}
