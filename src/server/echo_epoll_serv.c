#include <arpa/inet.h>
#include <headers/common.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50

int main(int argc, char *argv[]) {
    int serv_sock, client_sock;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t addr_size;
    return 0;
}
