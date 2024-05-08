#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(){
    printf("\nTIME CLient\n");

    int client;
    struct sockaddr_in address;
    char buffer[100];

    client = socket(AF_INET, SOCK_DGRAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    while(1){
        socklen_t len = sizeof(address);
        printf("\nTime MENU\n");
        printf("1.Get time\n2.Exit\n");
        int choice;

        printf("\nEnter the choice : ");
        scanf("%d",&choice);

        switch(choice){
            case 1:
                sendto(client, "time", strlen("time") ,0, (struct sockaddr *)&address, len);

                recvfrom(client,buffer, sizeof(buffer), 0, (struct sockaddr *)&address, &len);

                printf("Time is %s\n",buffer);
                break;
            
            case 2:
                printf("Exiting...\n");
                close(client);
                return 0;
            
            default:
                printf("\nInvalid choice\n");
                break;
        }
    }

}