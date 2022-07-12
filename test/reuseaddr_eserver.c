#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    int serv_sock, client_sock;
    char message[30];
    int option, str_len;
    socklen_t optlen, client_addr_size;
    struct sockaddr_in serv_addr, client_addr;

    if (argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error!");

    /*
    optlen = sizeof(option);
    option = TRUE;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)*option, optlen);
    */

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
        error_handling("bind() error!");

    if (listen(serv_sock, 5))
        error_handling("listen() error!");

    client_addr_size = sizeof(client_addr);
    client_sock =
        accept(serv_sock, (struct sockaddr *)&client_addr, &client_addr_size);
    while ((str_len = read(client_sock, message, sizeof(message))) != 0) {
        write(client_sock, message, str_len);
        write(1, message, str_len);
    }

    close(client_sock);
    close(serv_sock);

    return 0;
}
