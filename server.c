#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "queue.h"
#include "words.h"

static void sighandler(int signo) {
    if (signo == SIGINT) {
        printf("\nInterrupted\n");
        remove_shm();
        exit(0);
    }
}

void err(int i, char*message){
    if(i < 0){
        printf("Error: %s - %s\n",message, strerror(errno));
        exit(1);
    }
}

int main(){
    signal(SIGINT, sighandler);

    struct addrinfo * hints, * results;
    hints = calloc(1,sizeof(struct addrinfo));
    char* PORT = "9998";


    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM; //TCP socket
    hints->ai_flags = AI_PASSIVE; //only needed on server
    getaddrinfo(NULL, PORT , hints, &results);  //NULL is localhost or 127.0.0.1

    //create socket
    int listen_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);\

    //this code allows the port to be freed after program exit.  (otherwise wait a few minutes)
    int yes = 1;
    if ( setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1 ) {
        printf("sockopt  error\n");
        printf("%s\n",strerror(errno));
        exit(-1);
    }

    int berr = bind(listen_socket, results->ai_addr, results->ai_addrlen);
    err(berr, "Error binding");
    listen(listen_socket, 3);//3 clients can wait to be processed
    

    // int* playerList = calloc(20, sizeof(int));
    // int players = 0;
    
    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);

    fd_set read_fds;

    // struct queue *plr_queue = create_queue(20);
    int shmid = shm_setup(); //player queue shm
    struct queue *player_queue = shmat(shmid, 0, 0);
    create_queue(player_queue, 20); //max capacity
    shmdt(player_queue);

    while(1){

        char *buff = malloc(BUFFER_SIZE);

        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(listen_socket,&read_fds);
        int i = select(listen_socket+1, &read_fds, NULL, NULL, NULL);

        // if(players > 1){
        //     printf("Game starting!\n");
        // }
        // printf("here");

        //if standard in, use fgets
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            fgets(buff, sizeof(buff), stdin);
            buff[strlen(buff)]=0;
            buff = strsep(&buff, "\n");
            printf("Recieved from terminal: '%s'\n",buff);
        }

        // if socket
        if (FD_ISSET(listen_socket, &read_fds)) {
            //accept the connection
            int client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);
            printf("Connected, waiting for data.\n");
            err(client_socket, "server accept error");
            printf("New player %d\n", client_socket);

            int shmid = shmget(SHM_KEY, sizeof(struct queue), 0);
            struct queue *plr_queue = shmat(shmid, 0, 0); //attach

            //debug_print(plr_queue);
            enqueue(plr_queue, client_socket);
            printf("CLIENT %d\n", client_socket);
            //print_queue(plr_queue);
            // playerList[players] = client_socket;
            // players++;
            printf("Player %d has joined the game(%d current players)\n", client_socket, plr_queue->size);
            printf("Player %d's turn(%d remaining players)\n", get_front(plr_queue), plr_queue->size);
            shmdt(plr_queue);
            
            int f = fork();
            if (f == 0) {
                while (1) {
                    //read the whole buff
                    int rbytes = read(client_socket,buff, BUFFER_SIZE);
                    if (rbytes == 0) {
                        printf("%d disconnected\n", client_socket);
                        break;
                    }

                    int shmid = shmget(SHM_KEY, sizeof(struct queue), 0);
                    plr_queue = shmat(shmid, 0, 0); //attach

                    //print_queue(plr_queue);
                    printf("CLIENT: %d\n", client_socket);
                    if (get_front(plr_queue) == client_socket) {
                        debug_print(plr_queue);
                        dequeue(plr_queue);
                        enqueue(plr_queue, client_socket);
                        //trim the string
                        buff[strlen(buff)]=0; //clear newline
                        if(buff[strlen(buff)]==13){
                            //clear windows line ending
                            buff[strlen(buff)]=0;
                        }
                        printf("\nRecieved from client '%s'\n",buff);
                        debug_print(plr_queue);
                        // printf("Player %d's turn(%d remaining players)\n", get_front(plr_queue), players);
                    } else {
                        char msg[BUFFER_SIZE] = "Wait your turn";
                        write(client_socket, msg, BUFFER_SIZE);
                    }

                    shmdt(plr_queue);
                }
            } 

            // if(players > 1){
            //     printf("Game starting!\n");
            // }
            // printf("here");
            //close(client_socket);
        }
    }

    free(hints);
    freeaddrinfo(results);
    return 0;
}