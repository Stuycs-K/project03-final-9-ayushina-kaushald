#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "words.h"

int main() {
    struct addrinfo *hints, *results;
    hints = calloc(1, sizeof(struct addrinfo));
    char *PORT = "9998";

    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    getaddrinfo("127.0.0.1", PORT, hints, &results);

    int client_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

    if (connect(client_socket, results->ai_addr, results->ai_addrlen) == -1) {
        perror("Unable to connect to game");
        exit(EXIT_FAILURE);
    }

    printf("Welcome to Word Bomb!\n");

    fd_set read_fds;
    char *buff = malloc(BUFFER_SIZE);

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(client_socket, &read_fds);

        int i = select(client_socket + 1, &read_fds, NULL, NULL, NULL);

        if (i == -1) {
            perror("Error in select");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            fgets(buff, sizeof(buff), stdin);
            buff[strlen(buff)] = '\0';
            buff = strsep(&buff, "\n");
            int wbytes = write(client_socket, buff, sizeof(buff));
        }

        if (FD_ISSET(client_socket, &read_fds)) {
            ssize_t bytes_received = recv(client_socket, buff, sizeof(buff), 0);

            if (bytes_received <= 0) {
                if (bytes_received == 0) {
                    printf("Server closed connection.\n");
                } else {
                    perror("Error receiving data from server");
                }
                break;
            }

            buff[bytes_received] = '\0';
            printf("Received from server: '%s'\n", buff);
        }
    }

    free(hints);
    freeaddrinfo(results);
    close(client_socket);

    return 0;
}