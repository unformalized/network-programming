#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <headers/common.h>

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server_addr;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <PORT>\n", argv[0]);
        exit(1);
    }

    // 创建套接字
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&server_addr, 0, sizeof(server_addr));
    // 拼装远程 server 套接字地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // 连接远程服务地址, 发起一次请求
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        error_handling("connect() error!");
    else
        printf("Connected........\n");

    int operand_count = 0;
    int receive_server_data = 0;
    while (receive_server_data == FAIL)
    {
        printf("input operand count: ");
        scanf("%d", &operand_count);
        write(sock, &operand_count, sizeof(int));
        read(sock, &receive_server_data, sizeof(int));
        printf("receive server data: %d\n", receive_server_data);
    }

    int operand_value = 0;
    for (int i = 0; i < operand_count; ++i)
    {
        receive_server_data = 0;
        while (receive_server_data == FAIL)
        {
            printf("input operand value: ");
            scanf("%d", &operand_value);
            write(sock, &operand_value, sizeof(int));
            read(sock, &receive_server_data, sizeof(int));
            printf("receive server data: %d\n", receive_server_data);
        }
        operand_value = 0;
    }

    // 获取 operator 字符前，先清空缓冲区
    clear_scanf_buffer();
    char operator;
    printf("input operator: ");
    scanf("%c", &operator);
    write(sock, &operator, sizeof(char));

    int finish = -1;
    while (finish != FINISH)
    {
        read(sock, &finish, sizeof(int));
        if (finish == FINISH)
        {
            break;
        }
        printf("result: %d\n", finish);
    }
    printf("finished!\n");

    close(sock);
    return 0;
}
