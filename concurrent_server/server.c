#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(){
    printf("\nUDP PRODUCT SERVER\n");

    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    struct sockaddr_in clients[1024];
    char readBuffer[10];

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if ((bind(server_socket, (struct sockaddr *)&server_addr,sizeof(server_addr))) < 0){
        printf("\nBIND ERROR\n");
        return -1;
    }

    int client_count = 0;
    int product = 1;

    while(1){
        socklen_t len = sizeof(client_addr);
        recvfrom(server_socket, readBuffer, sizeof(readBuffer), 0, (struct sockaddr *)&client_addr, &len);
        printf("\nClient has entered with datagram message -> %s",readBuffer);
        readBuffer[strcspn(readBuffer,"\n")] = '\0';

        int num = atoi(readBuffer);
        printf("\nNumber -> %d",num);
        product *= num;
        printf("\nProduct -> %d",product);

        clients[client_count++] = client_addr;

        if(client_count == 2){
            for(int i = 0; i < client_count; i++)  
                sendto(server_socket, &product, sizeof(int), 0, (struct sockaddr *)&clients[i], len);
            break;
        }
    }
    close(server_socket);
}