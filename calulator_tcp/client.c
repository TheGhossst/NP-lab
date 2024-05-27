#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char input[1024];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("socket");
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("connect");
        return 1;
    }

    printf("\nEnter a number or an operator (+, -, *, /): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; 

    send(client_socket, input, strlen(input), 0);

    int result;
    recv(client_socket, &result, sizeof(int), 0);

    printf("\nResult -> %d", result);
    

    close(client_socket);
    return 0;
}