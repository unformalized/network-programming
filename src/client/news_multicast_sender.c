#include <arpa/inet.h>
#include <headers/common.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define TTL 64
#define BUF_SIZE 30

int main(int argc, char **argv) {
    int send_sock;
    struct sockaddr_in mul_addr;
    int time_live = TTL;
    FILE *fp;
    char buf[BUF_SIZE];

    if (argc != 3) {
        printf("Usage : %s <GroupIP> <PORT> \n", argv[0]);
        exit(1);
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (send_sock == -1) {
        error_handling("socket() error");
    }
    memset(&mul_addr, 0, sizeof(mul_addr));
    mul_addr.sin_family = AF_INET;
    mul_addr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_addr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&time_live,
               sizeof(time_live));

    if ((fp = fopen("data/news.txt", "r")) == NULL)
        error_handling("fopen error");

    while (!feof(fp)) {
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr *)&mul_addr,
               sizeof(mul_addr));
        sleep(1);
    }

    fclose(fp);
    close(send_sock);
    return 0;
}
