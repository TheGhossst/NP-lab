#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    char message[50];
    printf("Enter a message: ");
    fgets(message, sizeof(message), stdin);

    printf("message -> %s",message);
    
    //sys/stat.h
    mkfifo("pipe1",0777);  //fifo file 

    //fcntl.h
    int fd = open("pipe1",O_WRONLY);
    
    write(fd, message, sizeof(message));

    close(fd);
}