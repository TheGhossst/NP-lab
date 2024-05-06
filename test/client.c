#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(){
    printf("UPD CLIENT\n");

    int client;
    struct sockaddr_in address;
    char read_buffer[1024], write_buffer[1024];

    client = socket(AF_INET, SOCK_DGRAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);    

    while(1){
        socklen_t len = sizeof(address);
        printf("Enter the message to sent to server : ");
        fgets(write_buffer, sizeof(write_buffer), stdin);

        sendto(client, write_buffer ,sizeof(write_buffer), 0, (struct sockaddr *)&address,len);

        if (strcmp("END\n", write_buffer) == 0 || strcmp("END", write_buffer) == 0){
            printf("Client is leaving...\n");
            break;
        }

        recvfrom(client, read_buffer, sizeof(read_buffer), 0, (struct sockaddr *)&address, &len);

        printf("Server : %s\n",read_buffer);

        if (strcmp("END\n", read_buffer) == 0 || strcmp("END", read_buffer) == 0){
            printf("Server is exiting....\n");
            break;
        }
    }
    close(client);
}