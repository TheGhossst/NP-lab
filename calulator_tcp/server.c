#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>


int main() {
    printf("\nTCP CALCULATOR\n");
    int server_socket, client_socket1, client_socket2, client_socket3;
    struct sockaddr_in server_address, client_address;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("bind");
        return 1;
    }

    listen(server_socket, 5);
     

    printf("TCP calculator server listening on port 8080...\n");

    socklen_t len = sizeof(client_address);
    char number1[2], number2[2];
    char operator;

    client_socket1 = accept(server_socket, (struct sockaddr *)&client_address, &len);
    recv(client_socket1, number1, sizeof(number1), 0);
    
    client_socket2 = accept(server_socket, (struct sockaddr *)&client_address, &len);
    recv(client_socket2, number2, sizeof(number2), 0);
     
    client_socket3 = accept(server_socket, (struct sockaddr *)&client_address, &len);
    recv(client_socket3, &operator, sizeof(operator), 0);
        

    int num1 = atoi(number1);
    int num2 = atoi(number2);
    int result;

    switch (operator) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
            } else {
                printf("Error: Division by zero\n");
                return 1;
            }
            break;
        default:
            printf("Error: Invalid operand\n");
            return 1;
    }

    send(client_socket1, &result, sizeof(int), 0);
    send(client_socket2, &result, sizeof(int), 0);
    send(client_socket3, &result, sizeof(int), 0);

    printf("Result -> %d\n", result);

    close(server_socket);
    return 0;
}