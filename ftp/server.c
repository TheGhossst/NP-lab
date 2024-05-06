#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>
#include <string.h>

int fileExists(const char *path) {
    FILE *file = fopen(path, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

int get(char *filename) {
    char serverPath[1024];
    char clientPath[1024];

    snprintf(serverPath, sizeof(serverPath), "server/%s", filename);
    snprintf(clientPath, sizeof(clientPath), "client/%s", filename);

    if (!fileExists(serverPath)) {
        printf("File %s does not exist in the server directory.\n", filename);
        return 0;
    }

    if (fileExists(clientPath)) {
        printf("File %s already exists in the client directory.\n", filename);
        return 0;
    }

    FILE *serverFile = fopen(serverPath, "r");
    FILE *clientFile = fopen(clientPath, "w");

    if (serverFile == NULL || clientFile == NULL) {
        printf("Error opening files.\n");
        if (serverFile) fclose(serverFile);
        if (clientFile) fclose(clientFile);
        return 0;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), serverFile) != NULL) {
        fputs(buffer, clientFile);
    }

    fclose(serverFile);
    fclose(clientFile);

    printf("File %s copied from server to client directory.\n", filename);
    return 1;
}

int put(char *filename) {
    char serverPath[1024];
    char clientPath[1024];

    snprintf(serverPath, sizeof(serverPath), "server/%s", filename);
    snprintf(clientPath, sizeof(clientPath), "client/%s", filename);

    if (!fileExists(clientPath)) {
        printf("File %s does not exist in the client directory.\n", filename);
        return 0;
    }
    if (fileExists(serverPath)) {
        printf("File %s already exists in the server directory.\n", filename);
        return 0;
    }

    FILE *serverFile = fopen(serverPath, "w");
    FILE *clientFile = fopen(clientPath, "r");

    if (serverFile == NULL || clientFile == NULL) {
        printf("Error opening files.\n");
        if (serverFile) fclose(serverFile);
        if (clientFile) fclose(clientFile);
        return 0;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), clientFile) != NULL) {
        fputs(buffer, serverFile);
    }

    fclose(serverFile);
    fclose(clientFile);

    printf("File %s copied from client to server directory.\n", filename);
    return 1;
}

int main(){
    printf("\nFTP Server\n");

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char write_buffer[1024],read_buffer[1024];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0){
        perror("BIND error");
        return -1;
    }

    listen(server_socket,5);

    while(1){
        socklen_t len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr,&len);
        printf("A client has connected\n");
        while(1){
            recv(client_socket,read_buffer,sizeof(read_buffer),0);
            if (strcmp("GET\n",read_buffer) == 0 || strcmp("GET",read_buffer)== 0 || strcmp("get\n",read_buffer) == 0 || strcmp("get",read_buffer)== 0){
                recv(client_socket,read_buffer,sizeof(read_buffer),0);
                int len = strcspn(read_buffer, "\n");  
                read_buffer[len] = '\0';
                int status = get(read_buffer);
                if (status == 1){
                    printf("Success file put in server directory\n");
                }
                else{
                    printf("File doesnt exist in the server direcory\n");
                }
                send(client_socket, &status, sizeof(status), 0);
            }
            else if (strcmp("PUT\n", read_buffer) == 0 || strcmp("PUT", read_buffer) == 0){
                recv(client_socket,read_buffer,sizeof(read_buffer),0);
                int len = strcspn(read_buffer, "\n");  
                read_buffer[len] = '\0';
                int status = put(read_buffer);
                if (status == 1){
                    printf("Success file put in server directory\n");
                }
                else{
                    printf("File doesnt exist in the client direcory or file already exists\n");
                }
                send(client_socket, &status, sizeof(status), 0);
            }
            else if (strcmp("END\n", read_buffer) == 0 || strcmp("END", read_buffer) == 0){
                printf("Client is leaving\n");
                close(client_socket);
                break;
            }

            printf("Do you want to continue\n");
            printf("Type END to exit\n");
            fgets(write_buffer, sizeof(write_buffer), stdin);

            if (strcmp("END\n", write_buffer) == 0 || strcmp("END",write_buffer) == 0){
                printf("Server is exiting...\n");
                send(client_socket, write_buffer, sizeof(write_buffer), 0);
                close(client_socket);
                close(server_socket);
                return 0;
            }   
        }
        printf("Do you want to continue\n");
        printf("Type END to exit\n");
        fgets(write_buffer, sizeof(write_buffer), stdin);

        if (strcmp("END\n", write_buffer) == 0 || strcmp("END",write_buffer) == 0){
            printf("Server is exiting...\n");
            send(client_socket, write_buffer, sizeof(write_buffer), 0);
            close(server_socket);
            return 0;
        }
    }
}