#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <time.h>
#include "queue.h"

#ifndef WORDS_H
#define WORDS_H
#define FILE_NAME "used_words.TEXT"
#define BUFFER_SIZE 1024
#define SHM_KEY 0xdeadbeef
#define SIGSHM_KEY 0xbeefdead
char *generateLetter();
int checkWord(char *word, char letter);
void file_setup();
void add_word(char *word);
int find_word(char *word);
int shm_setup();
int wordValid(char * word, char letter);
void remove_shm();
#endif
