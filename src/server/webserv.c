//
// Created by liubin on 22-8-6.
//

#include <arpa/inet.h>
#include <headers/common.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void *request_handler(void *arg);
char *content_type(char *file);
void send_data(FILE *fp, char *ct, char *file_name);
void send_error(FILE *fp);

int main(int argc, char *argv[]) {
    int serv_sock, client_sock;
    struct sockaddr_in serv_addr, client_addr;
    int client_addr_size;
    char buf[BUF_SIZE];
    pthread_t t_id;

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (serv_sock == -1)
        error_handling("socket() error");

    int reuse_addr = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse_addr,
               sizeof(reuse_addr));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_family = AF_INET;

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 20) == -1)
        error_handling("listen() error");

    while (1) {
        client_addr_size = sizeof(client_addr);
        client_sock = accept(serv_sock, (struct sockaddr *)&client_addr,
                             &client_addr_size);

        if (client_sock == -1)
            error_handling("accept() error");

        printf("Connection Request: %s:%d\n", inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
        pthread_create(&t_id, NULL, request_handler, &client_sock);
        pthread_detach(t_id);
    }

    close(serv_sock);
    return 0;
}

void *request_handler(void *arg) {
    int client_sock = *((int *)arg);
    char req_line[SMALL_BUF];
    FILE *client_read;
    FILE *client_write;

    char method[10];
    char ct[15];
    char file_name[30];

    client_read = fdopen(client_sock, "r");
    client_write = fdopen(dup(client_sock), "w");
    fgets(req_line, SMALL_BUF, client_read);
    if (strstr(req_line, "HTTP/") == NULL) {
        printf("req_line error\n");
        send_error(client_write);
        fclose(client_read);
        fclose(client_write);
        return NULL;
    }

    strcpy(method, strtok(req_line, " /"));
    strcpy(file_name, strtok(NULL, " /"));
    strcpy(ct, content_type(file_name));
    if (strcmp(method, "GET") != 0) {
        printf("method error\n");
        send_error(client_write);
        fclose(client_read);
        fclose(client_write);
        return NULL;
    }

    fclose(client_read);
    send_data(client_write, ct, file_name);

    return NULL;
}

void send_data(FILE *fp, char *ct, char *file_name) {
    char protocol[] = "HTTP/1.0 OK\r\n";
    char server[] = "Server: Linux Web Server \r\n";
    char cnt_len[] = "Content-length: 2048\r\n";
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE *send_file;

    char data_file[] = "./data/";
    strcat(data_file, file_name);

    printf("%s\n", data_file);

    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    send_file = fopen(data_file, "r");
    if (send_file == NULL) {
        printf("file_name error\n");
        send_error(fp);
        return;
    }

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);

    while (fgets(buf, BUF_SIZE, send_file) != NULL) {
        fputs(buf, fp);
        fflush(fp);
    }

    fflush(fp);
    fclose(fp);
    printf("close request");
}

char *content_type(char *file) {
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];
    strcpy(file_name, file);
    strtok(file_name, ".");
    strcpy(extension, strtok(NULL, "."));

    if (!strcmp(extension, "html") || !strcmp(extension, "HTML"))
        return "text/html";
    else
        return "text/plain";
}

void send_error(FILE *fp) {
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server: Linux Web Server\r\n";
    char cnt_len[] = "Content-length: 105\r\n";
    char cnt_type[] = "Content-type:text/html\r\n";
    char content[] = "<html>"
                     "<head><title>NETWORK</title></head>"
                     "<body><font size=+5><br>Error ! Please Check filename!"
                     "</font></body></html>";
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    // 请求头后需要接一个空行，才能传送消息体
    fputs("\r\n", fp);
    fputs(content, fp);
    fflush(fp);
}
