#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OP_SIZE 4

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char const *argv[])
{
    int sock;
    char op_msg[BUF_SIZE];
    int result, operand_count, i;

    struct sockaddr_in server_addr;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error!");

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        printf("connect() error!");
    else
        printf("connected...\n");

    int n = 2178;
    char message[1024] = "h";
    printf("value: ");
    scanf("%d", (int *)&message[1]);
    printf("value: ");
    scanf("%d", (int *)&message[5]);
    printf("value: ");
    scanf("%d", (int *)&message[9]);
    write(sock, message, 15);
    // read(sock, &result, RLT_SIZE);

    // printf("Operation result: %d\n", result);
    close(sock);

    return 0;
}
