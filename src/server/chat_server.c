#include <arpa/inet.h>
#include <headers/common.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 100
#define MAX_CLIENT 256

void *handle_client(void *arg);
void send_msg(char *msg, int len);

int client_cnt = 0;
int clients_socks[MAX_CLIENT];
pthread_mutex_t mutex;

int main(int argc, char **argv) {
    int serv_sock, client_sock;
    struct sockaddr_in serv_addr, client_addr;
    int client_addr_size;
    pthread_t t_id;

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_family = AF_INET;

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    while (1) {
        client_addr_size = sizeof(client_addr);
        client_sock = accept(serv_sock, (struct sockaddr *)&client_addr,
                             &client_addr_size);

        pthread_mutex_lock(&mutex);
        clients_socks[client_cnt++] = client_sock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL, handle_client, (void *)&client_sock);
        pthread_detach(t_id);
        printf("Connected client IP: %s\n", inet_ntoa(client_addr.sin_addr));
    }

    close(serv_sock);
    return 0;
}

void *handle_client(void *arg) {
    int client_sock = *((int *)arg);
    int str_len = 0, i;
    char msg[BUF_SIZE];

    while ((str_len = read(client_sock, msg, sizeof(msg))) != 0)
        send_msg(msg, str_len);

    // 接受到消息后清除当前链接的客户端
    pthread_mutex_lock(&mutex);
    for (i = 0; i < client_cnt; i++) {
        if (client_sock == clients_socks[i]) {
            while (i++ < client_cnt - 1)
                clients_socks[i] = clients_socks[i + 1];
            break;
        }
    }
    client_cnt--;
    pthread_mutex_unlock(&mutex);
    close(client_sock);
    return NULL;
}

// 发送消息给全部客户端
void send_msg(char *msg, int len) {
    int i;
    pthread_mutex_lock(&mutex);
    for (i = 0; i < client_cnt; i++)
        write(clients_socks[i], msg, len);
    pthread_mutex_unlock(&mutex);
}
