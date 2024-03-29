#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netdb.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include "queue.h"
#include "words.h"

#define TIME_LIMIT 10

int child_processes[MAX_CAPACITY + 10];
int no_processes = 0;

// int shmid_timer;
// int *timer_flag;

void err(int i, char*message){
    if(i < 0){
        printf("Error: %s - %s\n",message, strerror(errno));
        exit(1);
    }
}

static void sighandler(int signo) {
    if (signo == SIGINT) {
        // printf("\nInterrupted\n");
        remove_shm();
        for (int i = 0; i < no_processes; i++) {
            kill(child_processes[i], SIGTERM);
        }
        exit(0);
    }
    if (signo == SIGALRM) {
        // printf("SIGALRM pid: %d\n", getpid());
        // printf("Time ran out. ");

        // int shmid = shmget(SHM_KEY, sizeof(struct queue), 0);
        // int *data = shmat(shmid, 0, 0); //attach
        // struct queue *plr_queue = deserialize(data);
        // int skipped_client = dequeue(plr_queue);
        // enqueue(plr_queue, skipped_client);

        // printf("Player %d goes next.\n", get_front(plr_queue));

        // char *buff = malloc(BUFFER_SIZE);
        // strcpy(buff, "Your time ran out.");
        // int wbytes = write(skipped_client, buff, BUFFER_SIZE);
        // err(wbytes, "sigalrm write to client error");

        // serialize(plr_queue, data);
        // shmdt(data);

    //     int shmid_timer = shmget(SIGSHM_KEY, sizeof(int), 0);
    //     int *timer_flag = shmat(shmid_timer, 0, 0);
    //     // err(timer_flag, " timer shmat error");
    //     *timer_flag = 1;
    //     shmdt(timer_flag);
    }
}

void set_timer(int seconds) {
    // printf("set_timer from pid: %d\n", getpid());
    // struct itimerval timer;
    // timer.it_value.tv_sec = seconds;
    // timer.it_value.tv_usec = 0;
    // timer.it_interval = timer.it_value;
    // setitimer(ITIMER_REAL, &timer, NULL);
    int f = fork();
    if (f == 0) {
        while (1) {
            // printf("Starting while!\n");
            time_t start = time(NULL);
            while ((time(NULL) - start) < seconds) {
                usleep(0.1 * 1000000);
                int shmid_timer = shmget(SIGSHM_KEY, sizeof(int) * 2, 0);
                int *timer_flag = shmat(shmid_timer, 0, 0);
                if (timer_flag[1] == 1) {
                    // printf("Timer was reset!!!\n");
                    //reset timer
                    shmdt(timer_flag);
                    break;
                }
            }
            // printf("Time passed!!\n");
            int shmid_timer = shmget(SIGSHM_KEY, sizeof(int) * 2, 0);
            int *timer_flag = shmat(shmid_timer, 0, 0);
            if (timer_flag[1] == 0) {
                // printf("[1] == 0!!\n");
                timer_flag[0] = 1; //time is up
            }
            else {
                // printf("[1] == 1!!\n");
                timer_flag[1] = 0; //timer is reset
            }
            shmdt(timer_flag);
        }
    }
    else {
        child_processes[no_processes] = f;
        no_processes++;
    }
}

void reset_timer() {
    // set_timer(0);
    // printf("Resetting timer pid: %d\n", getpid());
    int shmid_timer = shmget(SIGSHM_KEY, sizeof(int) * 2, 0);
    int *timer_flag = shmat(shmid_timer, 0, 0);
    timer_flag[1] = 1; //reset timer
    shmdt(timer_flag);
}

int main(){
    srand(time(NULL));

    file_setup();

    int shmid_start = shmget(STARTSHM_KEY, sizeof(int), IPC_CREAT | 0666);
    int *game_started = shmat(shmid_start, 0, 0);
    *game_started = 0;
    shmdt(game_started);

    char *letter = generateLetter();
    printf("The letter of this game is: %s\n", letter);

    signal(SIGINT, sighandler);
    signal(SIGALRM, sighandler);

    struct addrinfo * hints, * results;
    hints = calloc(1,sizeof(struct addrinfo));
    char* PORT = "9998";


    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM; //TCP socket
    hints->ai_flags = AI_PASSIVE; //only needed on server
    getaddrinfo(NULL, PORT , hints, &results);  //NULL is localhost or 127.0.0.1

    //create socket
    int listen_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

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

    {
        int shmid = shm_setup(); //player queue shm
        int *data = shmat(shmid, 0, 0);
        serialize(create_queue(), data);
        shmdt(data);
    }
    int shmid_timer = shmget(SIGSHM_KEY, sizeof(int) * 2, IPC_CREAT | 0666);
    // err(shmid_timer, "shmget error");
    int *timer_flag = (int *)shmat(shmid_timer, 0, 0);
    // err(timer_flag, "shmat error");
    timer_flag[0] = 0; // 1 when time is up
    timer_flag[1] = 0; // 1 when resetting timer
    shmdt(timer_flag);

    int sigf = fork();
    if (sigf == 0) {
        while(1) {
            int shmid_timer = shmget(SIGSHM_KEY, sizeof(int) * 2, 0);
            int *timer_flag = (int *)shmat(shmid_timer, 0, 0);
            if (timer_flag[0] == 1) { //time is up
                // printf("Timer!!!\n");

                // printf("pid %d\n", getpid());
                printf("Time ran out. ");

                int shmid = shmget(SHM_KEY, sizeof(struct queue), 0);
                int *data = shmat(shmid, 0, 0); //attach
                struct queue *plr_queue = deserialize(data);
                int skipped_client = dequeue(plr_queue);

                // enqueue(plr_queue, skipped_client);

                if (plr_queue->size == 1) {
                    printf("Eliminated player %d.\n", skipped_client - PLR_OFFSET);
                    printf("\nGame over. The winner is player %d\n", get_front(plr_queue) - PLR_OFFSET);
                    serialize(plr_queue, data);
                    shmdt(data);

                    kill(getppid(), SIGINT); //kill server
                    exit(0);
                }

                printf("Eliminated player %d.\n\nPlayer %d goes next (%d remaining players).\n", skipped_client - PLR_OFFSET, get_front(plr_queue) - PLR_OFFSET, plr_queue->size);

                // char *buff = malloc(BUFFER_SIZE);
                // strcpy(buff, "Your time ran out.");
                // int wbytes = write(skipped_client, buff, BUFFER_SIZE);
                // err(wbytes, "sigalrm write to client error");

                serialize(plr_queue, data);
                shmdt(data);


                timer_flag[0] = 0;
            }
            shmdt(timer_flag);
            usleep(0.1 * 1000000);
        }
    }
    else {
        child_processes[no_processes] = sigf;
        no_processes++;
    }

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
            // printf("Recieved from terminal: '%s'\n",buff);
            if (strcmp(buff, "show") == 0) {
                int shmid = shmget(SHM_KEY, sizeof(struct queue), 0);
                int *data = shmat(shmid, 0, 0); //attach
                struct queue *plr_queue = deserialize(data);
                
                printf("\nPlayers in queue:\n");
                print_queue(plr_queue);
                printf("\n");
                
                serialize(plr_queue, data);
                shmdt(data);
            }
        }

        // if socket
        if (FD_ISSET(listen_socket, &read_fds)) {
            //accept the connection
            int client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);

            int plr_num = client_socket - PLR_OFFSET;
            write(client_socket, &plr_num, sizeof(int));

            if (send(client_socket, letter, sizeof(char), 0) < 0) {
                perror("Server send error");
                exit(EXIT_FAILURE);
            }
            // printf("Connected, waiting for data.\n");
            err(client_socket, "server accept error");

            {
                int shmid = shmget(SHM_KEY, sizeof(struct queue), 0);
                int *data = shmat(shmid, 0, 0); //attach
                struct queue *player_queue = deserialize(data);

                if (player_queue->size == MAX_CAPACITY) {
                    char *rejection = malloc(BUFFER_SIZE);
                    sprintf(rejection, "Game is full! (%d/%d players)", player_queue->size, MAX_CAPACITY);
                    write(client_socket, rejection, BUFFER_SIZE);
                    close(client_socket);
                    continue;
                }

                // printf("New player %d\n", client_socket);

                //debug_print(plr_queue);
                enqueue(player_queue, client_socket);
                // printf("CLIENT %d\n", client_socket);
                //print_queue(plr_queue);
                // playerList[players] = client_socket;
                // players++;
                printf("Player %d has joined the game(%d current players)\n", client_socket - PLR_OFFSET, player_queue->size);

                int shmid_start = shmget(STARTSHM_KEY, sizeof(int), IPC_CREAT | 0666);
                int *game_started = shmat(shmid_start, 0, 0);

                if (*game_started == 0) {
                    if (player_queue->size >= MIN_PLAYERS) {
                        set_timer(TIME_LIMIT);
                        *game_started = 1;
                        printf("Game starting! You will have %d seconds to enter a valid word.\n", TIME_LIMIT);
                        printf("\nPlayer %d's turn(%d remaining players)\n", get_front(player_queue) - PLR_OFFSET, player_queue->size);
                    }
                    else {
                        printf("Waiting on %d more players\n", MIN_PLAYERS - player_queue->size);
                    }
                }
                else {
                    printf("\nPlayer %d's turn(%d remaining players)\n", get_front(player_queue) - PLR_OFFSET, player_queue->size);
                }

                shmdt(game_started);

                serialize(player_queue, data);
                shmdt(data);
            }

            // printf("Parent pid: %d\n", getpid());

            int f = fork();
            if (f == 0) {
                // printf("child pid: %d\n", getpid());
                while (1) {
                    //read the whole buff
                    int rbytes = read(client_socket,buff, BUFFER_SIZE);
                    if (rbytes == 0) {
                        printf("Player %d disconnected\n", client_socket - PLR_OFFSET);

                        int shmid = shmget(SHM_KEY, sizeof(struct queue), 0);
                        int *data = shmat(shmid, 0, 0); //attach
                        struct queue *plr_queue = deserialize(data);

                        int front = get_front(plr_queue);
                        remove_plr(plr_queue, client_socket); //remove player from the game

                        if (front == client_socket) {
                            //if it was their turn move to next player
                            if (plr_queue->size > 1) {
                                printf("\nPlayer %d's turn(%d remaining players)\n", get_front(plr_queue) - PLR_OFFSET, plr_queue->size);
                            }
                            reset_timer();
                        }

                        if (plr_queue->size == 1) {
                            printf("\nGame over. The winner is player %d\n", get_front(plr_queue) - PLR_OFFSET);
                            serialize(plr_queue, data);
                            shmdt(data);
                            kill(sigf, SIGINT); //kill timer
                            kill(getppid(), SIGINT); //kill server
                            exit(0);
                        }

                        serialize(plr_queue, data);
                        shmdt(data);
                        
                        exit(0);
                    }

                    int shmid_start = shmget(STARTSHM_KEY, sizeof(int), IPC_CREAT | 0666);
                    int *game_started = shmat(shmid_start, 0, 0);

                    int shmid = shmget(SHM_KEY, sizeof(struct queue), 0);
                    int *data = shmat(shmid, 0, 0); //attach
                    struct queue *plr_queue = deserialize(data);
                    // debug_print(plr_queue);

                    //print_queue(plr_queue);
                    // printf("CLIENT: %d\n", client_socket);
                    if (*game_started == 0) {
                        char msg[BUFFER_SIZE] = "Game has not started";
                        write(client_socket, msg, BUFFER_SIZE);
                    }
                    else if (find_word(buff)) {
                        char msg[BUFFER_SIZE] = "Word already used";
                        write(client_socket, msg, BUFFER_SIZE);
                    }
                    else if (get_front(plr_queue) == client_socket) {
                        // reset_timer();
                        // set_timer(5);
                        dequeue(plr_queue);
                        enqueue(plr_queue, client_socket);
                        //trim the string
                        buff[strlen(buff)]=0; //clear newline
                        if(buff[strlen(buff)]==13){
                            //clear windows line ending
                            buff[strlen(buff)]=0;
                        }
                        printf("Player %d answered '%s'\n", client_socket - PLR_OFFSET, buff);
                        // debug_print(plr_queue);

                        add_word(buff); //add word to file

                        printf("\nPlayer %d's turn(%d remaining players)\n", get_front(plr_queue) - PLR_OFFSET, plr_queue->size);
                        reset_timer();
                    } else {
                        char msg[BUFFER_SIZE] = "Not your turn";
                        write(client_socket, msg, BUFFER_SIZE);
                    }

                    shmdt(game_started);

                    serialize(plr_queue, data);
                    shmdt(data);

                    // if(timer_flag) {
                    //     break;
                    // }
                }
            }
            else {
                child_processes[no_processes] = f;
                no_processes++;
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