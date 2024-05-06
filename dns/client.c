#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(){
    printf("\nDNS CLIENT\n");

    int client;
    struct sockaddr_in address;
    char read_buffer[1024], write_buffer[1024];

    client = socket(AF_INET,SOCK_STREAM,0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    socklen_t len = sizeof(address);

    if ((connect(client,(struct sockaddr *)&address, len)) < 0){
        perror("CONNECT error");
        return -1;
    }
    printf("\nConnected to DNS Server\n");

    while(1){
        printf("Enter the hostname : ");
        fgets(write_buffer, sizeof(write_buffer), stdin);

        int len = strcspn(write_buffer, "\n");  
        write_buffer[len] = '\0';

        send(client, write_buffer, sizeof(write_buffer), 0);

        if (strcmp("END\n", write_buffer) == 0 || strcmp("END", write_buffer) == 0){
            printf("\nClient is leaving\n");
            break;
        }

        recv(client, read_buffer, sizeof(read_buffer), 0);

        if (strcmp("END\n", read_buffer) == 0 || strcmp("END", read_buffer) == 0){
            printf("\nServer is exiting...\n");
            break;
        }
        
        printf("\nIP: %s\n",read_buffer);
    }
    close(client);
}