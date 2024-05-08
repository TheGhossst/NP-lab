#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main(){
    printf("\nTIME SERVER\n");

    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[50];

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("BIND ERROR");
        return -1;
    }

    while(1){
        socklen_t len = sizeof(client_addr);
        recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &len);

        time_t t = time(NULL);
        strcpy(buffer, ctime(&t));
        printf("\nA client has connected time = %s\n",buffer);

        sendto(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, len);
    }
    close(server_socket);
}