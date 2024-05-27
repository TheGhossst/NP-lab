#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(){
    printf("\nUDP PRODUCT CLIENT\n");

    int client;
    struct sockaddr_in address;
    char writeBuffer[10];
    int product;

    client = socket(AF_INET, SOCK_DGRAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port  = htons(8080);

    printf("\nEnter a number to send to the server: ");
    fgets(writeBuffer, sizeof(writeBuffer), stdin);

    socklen_t len = sizeof(address);
    sendto(client, writeBuffer, sizeof(writeBuffer), 0, (struct sockaddr *)&address, len);

    recvfrom(client, &product, sizeof(int), 0, (struct sockaddr *)&address, &len);

    printf("\nThe product is %d\n",product);

    close(client);
}