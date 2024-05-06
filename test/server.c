#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(){
    printf("UDP SERVER\n");

    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    char write_buffer[1024], read_buffer[1024];

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0){
        perror("BIND error");
        return -1;
    }

    while(1){
        socklen_t len = sizeof(client_addr);

        recvfrom(server_socket, read_buffer, sizeof(read_buffer), 0, (struct sockaddr *)&client_addr, &len);

        printf("Client: %s\n",read_buffer);

        if (strcmp("END\n", read_buffer) == 0 || strcmp("END", read_buffer) == 0){
            printf("Client is leaving...\n");
            continue;
        }

        printf("Enter the response : ");
        fgets(write_buffer, sizeof(write_buffer), stdin);

        sendto(server_socket, write_buffer, sizeof(write_buffer), 0, (struct sockaddr *)&client_addr, len);

        if (strcmp("END\n", write_buffer) == 0 || strcmp("END", write_buffer) == 0){
            printf("Server is exiting...\n");
            break;
        }
    }
    close(server_socket);

}