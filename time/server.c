#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>

#define PORT 8080

void *handle_client(void *socket_desc);

int main() {
    int server_sock, client_sock, c;
    struct sockaddr_in server_addr, client_addr;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("BIND error");
        return -1;
    }

    listen(server_sock, 10);

    // Accept and handle incoming connections concurrently
    printf("Server is listening on port %d\n", PORT);
    c = sizeof(struct sockaddr_in);
    while ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, (socklen_t *)&c))) {
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, (void *)&client_sock) < 0) {
            printf("Could not create thread\n");
            return 1;
        }
    }

    if (client_sock < 0) {
        perror("ACCPET error");
        return -1;
    }
    return 0;
}

void *handle_client(void *socket_desc) {
    int sock = *(int *)socket_desc;
    time_t current_time;
    char time_buffer[26];
    int recv_size;

    time(&current_time);
    strcpy(time_buffer, ctime(&current_time));
    send(sock, time_buffer, strlen(time_buffer), 0);
    close(sock);
    return 0;
}