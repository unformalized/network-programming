#include <arpa/inet.h>
#include <headers/common.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char **argv) {
    int send_sock;
    struct sockaddr_in broad_addr;
    FILE *fp;
    char buf[BUF_SIZE];
    int so_broad = 1;
    if (argc != 3) {
        printf("Usage : %s <Broadcast IP> <PORT> \n", argv[0]);
        exit(1);
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&broad_addr, 0, sizeof(broad_addr));
    broad_addr.sin_family = AF_INET;
    broad_addr.sin_addr.s_addr = inet_addr(argv[1]);
    broad_addr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void *)&so_broad,
               sizeof(so_broad));
    if ((fp = fopen("data/news.txt", "r")) == NULL)
        error_handling("fopen() error");

    while (!feof(fp)) {
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr *)&broad_addr,
               sizeof(broad_addr));
        sleep(2);
    }

    close(send_sock);
    return 0;
}
