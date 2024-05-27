#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(){
    int fd = open("pipe1",O_RDONLY);

    char message[50];

    read(fd, message, sizeof(message));
    printf("message -> %s",message);
    printf("length -> %ld",strlen(message));

    message[strlen(message)- 1] = '\0';

    close(fd);

    int start = 0;
    int end = strlen(message) - 1;
    int is_palindrome = 1;

    while(start < end){
        if(message[start] != message[end]){
            is_palindrome = 0;
            break;
        }
        start++;
        end--;
    }
    
    char status[50];
    if (is_palindrome) strcpy(status, "palindrome");
    else strcpy(status, "Not Palindrome");

    mkfifo("pipe2", 07777);
    fd = open("pipe2", O_WRONLY);

    write(fd, status, sizeof(status));

    close(fd);
}