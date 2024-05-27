#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int fd = open("pipe2", O_RDONLY);

    char status[50];

    read(fd, status, sizeof(status));

    close(fd);

    printf("%s\n",status);
}