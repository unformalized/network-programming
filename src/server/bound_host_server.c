#include <arpa/inet.h>
#include <headers/common.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 20

int main(int argc, char const *argv[]) {
    int sock;
    char message[BUF_SIZE];
    struct sockaddr_in my_addr, your_addr;
    socklen_t addr_size;
    int str_len, i;

    if (argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = htons(atoi(argv[1]));

    if (bind(sock, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
        error_handling("bind() error");

    for (i = 0; i < 3; i++) {
        sleep(5);
        addr_size = sizeof(your_addr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0,
                           (struct sockaddr *)&your_addr, &addr_size);

        printf("Message %d: %s \n", i + 1, message);
    }

    close(sock);
    return 0;
}
