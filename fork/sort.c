#include <stdio.h>
#include <unistd.h>

int main(){
    int n, temp;

    printf("Enter the number of terms : ");
    scanf("%d",&n);

    int buffer[n];

    for(int i = 0;i < n ; i++){
        printf("Enter the element %d: ",i+1);
        scanf("%d",&buffer[i]);
    }

    pid_t pid = fork();

    if (pid == 0){
        printf("\nChild process\n");
        
        for (int i = 0 ; i < n ; i++){
            for (int j = 0 ;j < n - i -1; j++){
                if(buffer[j] < buffer[j+1]){
                    temp = buffer[j];
                    buffer[j] = buffer[j+1];
                    buffer[j+1] = temp;
                }
            }
        }

        printf("\n THe buffer sorted in the DESCENDING order is :\n");

        for (int i = 0; i< n;i++) printf("%d\t",buffer[i]);


    }

    else if (pid > 0){
        printf("\nParent Process\n");

        for (int i = 0 ; i < n ; i++){
            for (int j = 0 ;j < n - i -1; j++){
                if(buffer[j] > buffer[j+1]){
                    temp = buffer[j];
                    buffer[j] = buffer[j+1];
                    buffer[j+1] = temp;
                }
            }
        }

        printf("\n THe buffer sorted in the ASCENDING order is :\n");

        for (int i = 0; i< n;i++) printf("%d\t",buffer[i]);


    }
    else printf("Error in forking\n");
}