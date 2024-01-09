#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *generateLetter() {
    char *result = malloc(2);
    result[0] = 'a' + rand() % 26;
    return result;
}

int checkWord(char *word) {
    return 0;
}

int main() {
    srand(time(NULL));
    printf("%s\n", generateLetter());
}