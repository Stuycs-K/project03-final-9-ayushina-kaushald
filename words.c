#include "words.h"

void file_setup() {
    int file = open(FILE_NAME, O_CREAT | O_TRUNC, 0644);
}

void add_word(char *word) {
    char *buff = malloc(BUFFER_SIZE + 1);
    strncpy(buff, word, BUFFER_SIZE - 1);
    buff[BUFFER_SIZE-1] = '\n';
    buff[BUFFER_SIZE] = 0;

    int file = open(FILE_NAME, O_WRONLY | O_APPEND, 0);
    int wbytes = write(file, buff, BUFFER_SIZE + 1);
}

//return 1 if found, 0 if not found
int find_word(char *word) {
    char *buff = malloc(BUFFER_SIZE + 1);

    FILE *fstream = fopen(FILE_NAME, "r");
    while (fgets(buff, BUFFER_SIZE + 1, fstream)) {
        if (strstr(buff, word)) {
            return 1;
        }
    }
    return 0;
}

int main() {
    file_setup();
    add_word("Hello");
    add_word("he he");
    add_word("where");
    printf("%d\n", find_word("Hello"));
    printf("%d\n", find_word("bye"));
}