#include <arpa/inet.h>
#include <headers/common.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char const *argv[]) {
    int serv_sd, client_sd;
    FILE *fp;
    char buf[BUF_SIZE];
    int read_cnt;

    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_addr_size;

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    fp = fopen("./src/server/file_server.c", "rb");

    serv_sd = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
        error_handling("bind() error!");

    if (listen(serv_sd, 5))
        error_handling("listen() error!");

    client_addr_size = sizeof(client_addr);
    client_sd =
        accept(serv_sd, (struct sockaddr *)&client_addr, &client_addr_size);

    while (1) {
        read_cnt = fread((void *)buf, 1, BUF_SIZE, fp);
        if (read_cnt < BUF_SIZE) {
            write(client_sd, buf, read_cnt);
            break;
        }
        write(client_sd, buf, BUF_SIZE);
    }

    shutdown(client_sd, SHUT_WR);
    read(client_sd, buf, BUF_SIZE);
    printf("Message from client: %s \n", buf);

    fclose(fp);
    close(client_sd);
    close(serv_sd);

    return 0;
}
