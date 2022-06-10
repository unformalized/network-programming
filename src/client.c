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

  char message[30];
  int str_len;

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

  // 读取远程套接字返回的数据
  str_len = read(sock, message, sizeof(message) - 1);
  if (str_len == -1)
    error_handling("read() error!");

  printf("Message from server : %s \n", message);
  close(sock);
  return 0;
}