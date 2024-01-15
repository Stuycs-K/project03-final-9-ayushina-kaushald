#include "words.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "queue.h"

char *generateLetter() {
    char *result = malloc(2);
    result[0] = 'a' + rand() % 26;
    return result;
}

int checkWord(char *word, char letter) {
    if(word[0] == letter){
        return 1;
    } 
    return 0;
}


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

int shm_setup() {
    int *data;
    int shmid;
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        printf("shmget error %d: %s\n", errno, strerror(errno));
    }
    return shmid;
}

int wordValid(char * word, char letter){
    if(checkWord(word, letter)){
        if(find_word(word)){
            return 0;
        }
    }
    return 1;
}

void remove_shm() {
    int shmid = shmget(SHM_KEY, SHM_SIZE, 0);
    if (shmid == -1) {
        //printf("shmget error %d: %s\n", errno, strerror(errno));
    }
    else {
        shmctl(shmid, IPC_RMID, 0);
    }
}

// int main() {
//     file_setup();
//     add_word("Hello");
//     add_word("he he");
//     add_word("where");
//     // printf("%d\n", find_word("Hello"));
//     // printf("%d\n", find_word("bye"));
//     if(wordValid("Hello", 'H')){
//         printf("Word is valid!");
//     }
// }
