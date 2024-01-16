#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "words.h"

int main(int argc, char *argv[]) {
    struct addrinfo *hints, *results;
    hints = calloc(1, sizeof(struct addrinfo));
    char *PORT = "9998";

    char* IP = "127.0.0.1";
    if(argc == 2) {
        IP = argv[1];
        printf("Connecting to %s\n", IP);
    }
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    getaddrinfo(IP, PORT, hints, &results);

    int client_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

    if (connect(client_socket, results->ai_addr, results->ai_addrlen) == -1) {
        perror("Unable to connect to game");
        exit(EXIT_FAILURE);
    }

    printf("Welcome to Word Bomb!\n");

    int plr_num;
    read(client_socket, &plr_num, sizeof(int));
    printf("You are player %d.\n", plr_num);

    char received_letter;
    if (recv(client_socket, &received_letter, sizeof(received_letter), 0) < 0) {
        perror("Client receive error");
        exit(EXIT_FAILURE);
    }
    printf("The letter of the game is: %c\n", received_letter);

    fd_set read_fds;

    while (1) {


        char *buff = malloc(BUFFER_SIZE);

        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(client_socket, &read_fds);

        int i = select(client_socket + 1, &read_fds, NULL, NULL, NULL);

        if (i == -1) {
            perror("Error in select");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            fgets(buff, BUFFER_SIZE, stdin);
            buff[strlen(buff)] = '\0';
            buff = strsep(&buff, "\n");
            //printf("word inputted: %s\n", buff);
            if(checkWord(buff, received_letter)){
                // printf("Word is valid!\n");
                //printf("word inputted: %s\n", buff);
                // add_word(buff);
                int wbytes = write(client_socket, buff, BUFFER_SIZE);
            }
            else {
                printf("Invalid word.\n");
            }
        }

        if (FD_ISSET(client_socket, &read_fds)) {
            ssize_t bytes_received = recv(client_socket, buff, BUFFER_SIZE, 0);

            if (bytes_received <= 0) {
                if (bytes_received == 0) {
                    printf("Server closed connection.\n");
                } else {
                    perror("Error receiving data from server");
                }
                break;
            }
            buff[bytes_received] = '\0';
            printf("[SERVER] '%s'\n", buff);
        }
    }

    free(hints);
    freeaddrinfo(results);
    close(client_socket);

    return 0;
}