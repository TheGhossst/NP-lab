#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(){
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char read_buffer[1024], write_buffer[1024];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0){
        perror("Bind error");
        return -1;
    }

    listen(server_socket,5);

    while(1){
        socklen_t len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &len);\

        printf("\nClient has connected\n");

        while(1){
            recv(client_socket, read_buffer, sizeof(read_buffer), 0);

            printf("Client : %s\n",read_buffer);

            if(strcmp("END\n",read_buffer) == 0 || strcmp("END", read_buffer) == 0){
                printf("Client is  leaving\n");
                close(client_socket);
                break;
            }

            printf("Enter a response: ");
            fgets(write_buffer, sizeof(write_buffer), stdin);

            send(client_socket, write_buffer, sizeof(write_buffer), 0);

            if(strcmp("END\n",write_buffer) == 0 || strcmp("END", write_buffer) == 0){
                printf("Server is  Exiting..\n");
                close(server_socket);
                close(client_socket);
                return 0;
            }
        }
        printf("Do you want to continue or not \n");
        printf("To close the connection type END");
        fgets(write_buffer,sizeof(write_buffer),stdin);

        if(strcmp("END\n",write_buffer) == 0 || strcmp("END", write_buffer) == 0){
                printf("Server is  Exiting..\n");
                close(server_socket);
                break;
            }
    }

}