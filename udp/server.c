#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(){
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    char read_buffer[1024], write_buffer[1024];

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if ((bind(server_socket,(struct sockaddr *)&server_addr,sizeof(server_addr))) < 0){
        perror("Bind error");
        return -1;
    }

    while(1){
        //recvfrom(int __fd, void *restrict __buf, size_t __n, int __flags, struct sockaddr *restrict __addr, socklen_t *restrict __addr_len)
        socklen_t client_len = sizeof(client_addr);
        recvfrom(server_socket, read_buffer, sizeof(read_buffer), 0, (struct sockaddr *)&client_addr, &client_len);

        printf("Client: %s\n", read_buffer);

        if(strcmp("END\n",read_buffer) == 0 || strcmp("END", read_buffer) == 0){
            printf("Client is  leaving\n");
            continue;
        }

        printf("Enter a response: ");
        fgets(write_buffer, sizeof(write_buffer), stdin);

        //sendto(int __fd, const void *__buf, size_t __n, int __flags, const struct sockaddr *__addr, socklen_t __addr_len)
        sendto(server_socket, write_buffer, sizeof(write_buffer), 0, (struct sockaddr * )&client_addr,client_len);

        if(strcmp("END\n",write_buffer) == 0 || strcmp("END", write_buffer) == 0){
            printf("Server is  leaving\n");
            break;
        }
        
    }
    close(server_socket);
    return 0;
}