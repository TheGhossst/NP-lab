#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>
#include <string.h>

#include <stdio.h>
#include <string.h>

void get(char* filename) {
    char path[1024];
    snprintf(path, sizeof(path), "client/%s", filename); 

    FILE* f = fopen(path, "r");
    if (f == NULL) {
        printf("Error: File %s not found in the client directory.\n", filename);
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        printf("%s", buffer);
    }

    fclose(f);
}

void put(char* filename) {
    char path[1024];
    snprintf(path, sizeof(path), "client/%s", filename);  

    FILE* f = fopen(path, "r");
    if (f != NULL) {
        printf("Error: File %s already exists in the client directory.\n", filename);
        fclose(f);
        return;
    }

    f = fopen(path, "w");
    if (f == NULL) {
        printf("Error creating file %s\n", filename);
        return;
    }

    char buffer[1024];
    printf("Enter the content (type 'end' to stop):\n");
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        if (strcmp(buffer, "end\n") == 0) {
            break;
        }
        fputs(buffer, f);
    }

    fclose(f);
    printf("File %s written successfully.\n", filename);
}
int main(){
    printf("\nFTP Client\n");
    
    int client;
    struct sockaddr_in address;
    char read_buffer[1024], write_buffer[1024];

    client = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    socklen_t len = sizeof(address);
    if ((connect(client, (struct sockaddr *)&address,len)) < 0){
        perror("CONNECT error");
        return -1;
    }
    printf("\nConnected to server\n");

    while(1){
        int status = 0;

        printf("DNS MENU\n..............\n");
        printf("1.GET\n2.PUT\n3.Exit\n");
        int c;
        printf("Enter the choice: ");
        scanf("%d",&c);

        switch(c){
            case 1:
                send(client, "GET", sizeof("GET"), 0);
                printf("Enter the filename.extention: ");
                getchar();
                fgets(write_buffer, sizeof(write_buffer), stdin);
                send(client,write_buffer, sizeof(write_buffer), 0);
                recv(client,&status,sizeof(status), 0);
                if (status == 1){
                    get(write_buffer);
                }
                else
                    printf("Error 404\n");
                break;
            
            case 2:
                send(client, "PUT", sizeof("PUT"), 0);
                printf("Enter the filename.extention: ");
                getchar();
                fgets(write_buffer, sizeof(write_buffer), stdin);
                send(client,write_buffer, sizeof(write_buffer), 0);
                recv(client,&status,sizeof(status), 0);
                if (status == 1){
                    put(write_buffer);
                }
                else    printf("Error 404\n");
                break;
            
            case 3:
                send(client, "END", sizeof("END"), 0);
                printf("Client is exiting...\n");
                close(client);
                return 0;
            
            default:
                printf("Invalid choice\n");
                break;
        }
    }
}