#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main() {
    int client;
    struct sockaddr_in address;
    char write_buffer[1024], read_buffer[1024];

    client = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (connect(client, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("CONNECT ERROR");
        return -1;
    }

    while (1) {
        printf("Enter a message to send to the client: ");
        fgets(write_buffer, sizeof(write_buffer), stdin);

        if (strcmp("END\n", write_buffer) == 0 || strcmp("END", write_buffer) == 0) {
            send(client, write_buffer, strlen(write_buffer), 0);
            printf("Client is exiting\n");
            close(client);
            break;
        }

        send(client, write_buffer, strlen(write_buffer), 0);

        int bytes_received = recv(client, read_buffer, sizeof(read_buffer), 0);
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                printf("Server has closed the connection\n");
            } else {
                perror("recv error");
            }
            close(client);
            break;
        }

        read_buffer[bytes_received] = '\0';
        printf("Server: %s\n", read_buffer);
    }

    return 0;
}