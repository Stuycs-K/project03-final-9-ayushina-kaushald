#include "words.h"

void add_word(char *word) {
    char *buff = malloc(BUFFER_SIZE);
    strncpy(buff, word, BUFFER_SIZE - 1);
    buff[BUFFER_SIZE-1] = 0;

    int file = open(FILE_NAME, O_WRONLY | O_CREAT, 0644);
    int wbytes = write(file, buff, BUFFER_SIZE);
}

int find_word(char *word) {
    return 0;
}