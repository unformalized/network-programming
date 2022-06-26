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
  int str_len = 0;
  int idx = 0, read_len = 0;

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
  while ((read_len = read(sock, &message[idx++], 1)) > 0)
  {
    if (read_len == -1)
      error_handling("read() error!");

    str_len += read_len;
  }

  printf("Message from server : %s \n", message);
  printf("Function read call count: %d \n", str_len);
  close(sock);
  return 0;
}