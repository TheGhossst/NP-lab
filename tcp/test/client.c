#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(){
    int client;
    struct sockaddr_in address;
    char read_buffer[1024], write_buffer[1024];

    client = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (connect(client,(struct sockaddr *)&address, sizeof(address)) < 0){
        perror("Client error");
        return -1;
    }

    while(1){
        printf("Enter a message to send to server: ");
        fgets(write_buffer, sizeof(write_buffer), stdin);

        send(client, write_buffer, sizeof(write_buffer), 0);

        if (strcmp("END\n", write_buffer) == 0 || strcmp("END", write_buffer) == 0){
            printf("Client is leaving\n");
            close(client);
            break;
        }    

        recv(client, read_buffer , sizeof(read_buffer), 0);

        printf("Server: %s\n",read_buffer);

        if (strcmp("END\n", read_buffer) == 0 || strcmp("END", read_buffer) == 0){
            printf("Server is exiting\n");
            close(client);
            break;
        }  

    }

}