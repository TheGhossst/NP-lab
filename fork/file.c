#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_WORD_LENGTH 100

int main() {

    pid_t pid = fork();

    if (pid > 0){
        FILE *file;
        char word[MAX_WORD_LENGTH];
        char largest_word[MAX_WORD_LENGTH] = "";
        int word_length, largest_length = 0;

        file = fopen("hello.txt", "r");
        if (file == NULL) {
            printf("Error opening file.\n");
            return 1;
        }

        while (fscanf(file, "%s", word) != EOF) {
            word_length = strlen(word);
            if (word_length > largest_length) {
                largest_length = word_length;
                strcpy(largest_word, word);
            }
        }

        fclose(file);

        printf("The largest word in the file is: %s\n", largest_word);
    }
    else if (pid == 0){
        FILE *file;
        char word[MAX_WORD_LENGTH];
        char shortest[MAX_WORD_LENGTH] = "";
        int word_length, shortest_length = 0;

        file = fopen("hello.txt", "r");
        if (file == NULL) {
            printf("Error opening file.\n");
            return 1;
        }

        while (fscanf(file, "%s", word) != EOF) {
            word_length = strlen(word);
            if (word_length <= shortest_length) {
                shortest_length = word_length;
                strcpy(shortest, word);
            }
        }

        fclose(file);

        printf("The shortest word in the file is: %s\n", shortest);
    }

    return 0;
}