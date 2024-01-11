#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#ifndef WORDS_H
#define WORDS_H
#define FILE_NAME "words"
#define BUFFER_SIZE 1024
#define SHM_KEY 0xdeadbeef
void file_setup();
void add_word(char *word);
int find_word(char *word);
#endif