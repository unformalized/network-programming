#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <headers/common.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    int server_sock, client_sock;
    socklen_t clnt_addr_size;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;

    server_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (server_sock == -1)
        error_handling("socket() error!");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    clnt_addr_size = sizeof(clnt_addr);
    if (bind(server_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error!");

    if (listen(server_sock, 5) == -1)
        error_handling("listen() error!");

    int operand_count = 0, receive_len = 0;

    // 只需要调用依次 accept 接受一个客户端连接
    client_sock = accept(server_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (client_sock == -1)
        error_handling("accept() error!");
    else
        printf("connected client");

    while (operand_count <= 0)
    {
        // 第一次先获取到 operand 数量
        receive_len = read(client_sock, &operand_count, sizeof(int));
        printf("receive client data: %d\n", operand_count);
        if (receive_len <= 0)
        {
            write(client_sock, &FAIL_VAL, sizeof(int));
            continue;
        }
        printf("send client success\n");
        write(client_sock, &RECEIVE_OPERAND_COUNT_SUCCESS_VAL, sizeof(int));
    }
    printf("operand count: %d\n", operand_count);

    int operands[operand_count];
    int cur_count = 0;
    int cur_operand_value = 0;

    while (cur_count < operand_count)
    {
        // 依次获取 operand 值
        receive_len = read(client_sock, &cur_operand_value, sizeof(int));
        printf("receive client data: %d\n", cur_operand_value);
        if (receive_len <= 0)
        {
            printf("receive_len: %d\n", receive_len);
            write(client_sock, &FAIL_VAL, sizeof(int));
            continue;
        }

        operands[cur_count] = cur_operand_value;
        printf("operand value %d: %d\n", cur_count + 1, operands[cur_count]);
        write(client_sock, &RECEIVE_OPERAND_VALUE_SUCCESS_VAL, sizeof(int));
        cur_count++;
    }

    // 接受 operator
    char operator;

    receive_len = read(client_sock, &operator, sizeof(char));
    if (receive_len <= 0)
        write(client_sock, &FAIL_VAL, sizeof(int));

    printf("operator: %c\n", operator);

    int final_result = 0, i = 0;
    switch (operator)
    {
    case '*':
        final_result = 1;
        for (i = 0; i < operand_count; ++i)
        {
            final_result = final_result * operands[i];
        }
        write(client_sock, &final_result, sizeof(int));
        break;
    case '+':
        final_result = 0;
        for (i = 0; i < operand_count; ++i)
        {
            final_result = final_result + operands[i];
        }
        write(client_sock, &final_result, sizeof(int));
        break;
    case '-':
        final_result = operands[0];
        for (i = 1; i < operand_count; ++i)
        {
            final_result = final_result - operands[i];
        }
        write(client_sock, &final_result, sizeof(int));
        break;
    case '/':
        final_result = operands[0];
        for (i = 1; i < operand_count; ++i)
        {
            final_result = final_result / operands[i];
        }
        write(client_sock, &final_result, sizeof(int));
        break;
    }
    printf("final result: %d\n", final_result);
    write(client_sock, &FINISH, sizeof(int));

    close(client_sock);
    close(server_sock);

    return 0;
}
