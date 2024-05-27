//B21CS1235
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define bufferSize 1024

int main(){
    printf("\nSMTP SERVER\n");
    int serverSocket,clientSocket;
    struct sockaddr_in server_addr,client_addr;
    int len = sizeof(server_addr);
    char buffer[bufferSize];
    
    printf("\nSMTP Server starting ....");
    
    serverSocket = socket(AF_INET,SOCK_STREAM,0);
    
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if(bind(serverSocket,(struct sockaddr *)&server_addr,sizeof(server_addr) ) < 0){
        perror("Bind Error");
        exit(EXIT_FAILURE);
    }
    
    listen(serverSocket,1);
    
    printf("\nConnection Established!\n");
    int val;
    char readArray[bufferSize];
    while(1){
        clientSocket = accept(serverSocket,(struct sockaddr *)&server_addr,(socklen_t *)&len);
        char to_email[1024],from_email[1024],subject[1024],message[1024];

        recv(clientSocket, to_email , sizeof(to_email), 0);
        recv(clientSocket, from_email , sizeof(from_email), 0);
        recv(clientSocket, subject , sizeof(subject), 0);
        recv(clientSocket, message , sizeof(message), 0);

        char *token = strtok(to_email,"@"); //nandu
        token = strtok(NULL,"@"); //123 -> to domain
              
        char *token1 = strtok(from_email,"@"); //nandu
        token1 = strtok(NULL,"@"); //123*/ -> from domain


        printf("\nTo email -> %s",to_email);
        printf("\nFrom email -> %s",from_email);
        printf("\nSubject -> %s",subject);
        printf("\nMessage -> %s",message);
        printf("\nHost domain -> %s", token);
        printf("\nTarget domain -> %s", token1);
        
    }
    close(clientSocket);
    close(serverSocket);
}