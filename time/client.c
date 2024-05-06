#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock, choice;
    struct sockaddr_in server_addr;
    char time_buffer[26];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connection failed\n");
        return 1;
    }

    while (1) {
        printf("\nTime Server Client Menu:\n");
        printf("1. Get Time\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (recv(sock, time_buffer, sizeof(time_buffer), 0) < 0) {
                    printf("Receive failed\n");
                    break;
                }

                printf("The current time is: %s", time_buffer);
                break;

            case 2:
                printf("Exiting client...\n");
                close(sock);
                return 0;

            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    }

    return 0;
}