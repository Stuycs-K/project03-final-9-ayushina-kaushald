# Dev Log:

## Arslan Ayushin

### 2024-01-03 - Proposal
Came up with ideas, started the intentions section of the proposal in class (30 mins)
Added the topics covered, algorithms/data structures part of the proposal (30 mins)

### 2024-01-04 - Server functions for socket/select/files
Worked with Dean in class on the server/client code (20 mins)
Wrote function to give server random letter (10 mins)
Added words.c and functions to write words to a file and check if a word is in file (30 mins)

### 2024-01-05 + Weekend - Player queue
Added makefile for server/client
Tested server/client, fixed reading socket cutting off last character and loop (30 mins)
Added queue.c and .h to start implementing queue for the players (15 min)
Create queue (15 min)

### 2024-01-08 - Server/client connections
Tried to fix client error with bytes received = 0, changed send/recv (30 mins)
Add fork to server, can now accept multiple clients(30 mins)

### 2024-01-09 - Logic for add to queue
Create player queue in the server, fix makefile and server.c (30 mins)
Finished enqueue/dequeue functions (10 mins)
Server adds clients to queue and prints queue(20 mins)

### 2024-01-10 - Game turns + check queue
Testing what happens when client disconnects, remove from player queue (30 mins)
Server checks if its the player turn before reading word from client (20 mins)
Start add shared memory/file (10 mins)

### 2024-01-11 - Shared memory setup
Server sets up and removes shared memory (15 mins)
Changed the buffer size of read/write but there is an issue (15 mins)
Fixed read/write buffer size bug (10 mins)
Tried to fix issue getting shared memory, added debug print function (20 mins)

### 2024-01-12 + Weekend - Finished up handle client disconnect + timer
Fixed shared memory and queue on the server so it only reads from the client when it is their turn (30 mins)
Added on to the timer code (had to move the SIGALRM handler into its own forked process because the sigalrm breaks select) (SIGINT now kills all child processes) (30 mins)
Finished the remove player from queue method, for when player disconnects or player runs out of time (30 mins)
Extra: server prints queue when you type "show" in the terminal (20 mins)


## Dean Kaushal

### 2024-01-03 - Brief description
Discussed project ideas and created project proposal(30 minutes)
Wrote the intended usage portion of the proposal(10 minutes)
Wrote the intended pacing portion of the proposal(10 minutes)

### 2024-01-04 - Brief description
Finalized project design with Arslan in class(10 minutes)
Copied server demo code with the select function and learned how to utilize its features(20 minutes)
Used the server demo code to create the client side. it is not completed and i need to figure out some things(30 minutes)

### 2024-01-05 - 2024-01-07 - Brief description
Continued to work on client/server communications, unable to get it working(1 hour)

### 2024-01-08 - Brief description
Worked with Arslan to finalize server/client communications working(30 min)
Started working on game logic functions(completed the checkWord method and started others)(30 min)

### 2024-01-09 - Brief description
Implemented game logic within the server/client communications(30 min)
Continued more game logic methods including dealing with the queue of players(30 min)

### 2024-01-10 - Brief description
Created a playersList arry and player counter in the server to handle multiple players(30 min)
Condensed game.c and words.c into one file, wrote the wordValid method to check if inputted word is acceptable(30 min)

### 2024-01-11 - Brief description
Tried to debug the way the queue works in class with Arslan(30 min)
Added else statements and more game logic to notify users when it is their turn or not their turn(30 min)

### 2024-01-12 - 2024-01-15 - Brief description
Learned how to use the setitimer function and added SIGALRM to sighandler to control turn timers(30 min)
Added timer methods and flags so that the server can individually control the timer for each client(30 min)
Used the methods in word.c in server.c and client.c to generate a random letter and check if the inputted word is valid(30 min)