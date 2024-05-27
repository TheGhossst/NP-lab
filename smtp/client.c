#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define bufferSize 1024

int main(){
	printf("SMTP Client\n.......\n");
	int clientSocket;
	struct sockaddr_in server_addr;
	
	int len = sizeof(server_addr);
	char buffer[bufferSize] = {0};
	
	if((clientSocket = socket(AF_INET,SOCK_STREAM, 0)) == 0 ){
		perror("Socket Error");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

    if(connect(clientSocket,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
		perror("Connection Error");
		exit(EXIT_FAILURE);
	}

	printf("The message should be in the format To:From:Subject:Body \n");
	printf("\nEnter the message :\n");

    printf("\nEnter the to email : ");
	fgets(buffer,sizeof(buffer),stdin);
    buffer[strcspn(buffer,"\n")] = '\0';
	send(clientSocket,buffer,strlen(buffer),0);

    printf("\nEnter the from email : ");
	fgets(buffer,sizeof(buffer),stdin);
    buffer[strcspn(buffer,"\n")] = '\0';
	send(clientSocket,buffer,strlen(buffer),0);

    printf("\nEnter the subject : ");
	fgets(buffer,sizeof(buffer),stdin);
    buffer[strcspn(buffer,"\n")] = '\0';
	send(clientSocket,buffer,strlen(buffer),0);

    printf("\nEnter the message : ");
	fgets(buffer,sizeof(buffer),stdin);
    buffer[strcspn(buffer,"\n")] = '\0';
	send(clientSocket,buffer,strlen(buffer),0);

	printf("\nSuccesfully send message to SMTP server\n");
	close(clientSocket);
}
