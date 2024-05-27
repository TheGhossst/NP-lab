#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
    int fd1[2],fd2[2];

    pipe(fd1);
    pipe(fd2);
    pid_t pid = fork();

    if (pid > 0){
        char str[20],res[20];
        printf("\nParent Process\n");

        printf("\nEnter a string : ");
        fgets(str, sizeof(str), stdin);

        int len = strcspn(str, "\n");  
        str[len] = '\0';

        close(fd1[0]);
        write(fd1[1], str, sizeof(str));
        close(fd1[1]);

        close(fd2[1]);
        read(fd2[0], res, sizeof(res));
        
        printf("The string %s is %s\n",str,res);

        close(fd2[0]);
    }

    else if (pid == 0){
        char str[20],status[20];

        close(fd1[1]);
        read(fd1[0], str, sizeof(str));

        printf("Child Process\n");
        printf("\nString recieved from pipe1 : %s\n",str);

        int start = 0, end = strlen(str) - 1;
        int is_palindrome = 1;
        while (start < end) {
            if (str[start] != str[end]) {
                is_palindrome = 0;
                break;
            }
            start++;
            end--;
        }

        if (is_palindrome) strcpy(status, "palindrome");
        else strcpy(status, "Not Palindrome");
        

        close(fd1[0]);

        close(fd2[0]);
        write(fd2[1], status, sizeof(status));
        close(fd2[1]);
    }
}