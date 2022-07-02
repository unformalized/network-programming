#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <headers/common.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OP_SIZE 4

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
        error_handling("connect() error!");
    else
        printf("connected...\n");

    fputs("Operand count: ", stdout);
    scanf("%d", &operand_count);
    op_msg[0] = (char)operand_count;

    for (i = 0; i < operand_count; ++i)
    {
        printf("Operand %d: ", i + 1);
        scanf("%d", (int *)&op_msg[i * OP_SIZE + 1]);
    }
    fgetc(stdin);
    fputs("Operator: ", stdout);
    scanf("%c", &op_msg[operand_count * OP_SIZE + 1]);
    write(sock, op_msg, operand_count * OP_SIZE + 2);
    read(sock, &result, RLT_SIZE);

    printf("Operation result: %d\n", result);
    close(sock);

    return 0;
}
