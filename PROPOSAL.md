# Final Project Proposal

## Group Members:

Arslan Ayushin, Dean Kaushal
       
# Intentions:

We are making a multiplayer text-based game where a letter is generated randomly. Players take turns entering a word that must contain the letter provided. If they do not enter a unique word, enter a word without the letter, or run out of time they are out of the game. Last person standing wins the game.

# Intended usage:

The game will be played on one computer with multiple terminals open. The server will setup shared memory, etc. and generate the letter. The clients will connect to the server, and display the letter of the game. Once at least 2 clients have connected, the first client will prompt the user for a word containing that letter, and will send the answer to the server. Then the next client will have a turn to enter a word. Every word used gets added to a list of already used words. If they fail to produce a unique word before the timer runs out, then they are out and will not have a turn in the next round. Once there is only one person that has a turn remaining, they win the game and the program ends. 
  
# Technical Details:

Topics covered:  
Our client-server connection will use pipes. The clients will connect to a well-known pipe which is the server. The clients will send their answers to the server when it is their turn.
The server will allocate shared memory to have a queue of players and their pid in the order of whose turn is next. New clients get added to the back of the queue. When a client responds they are moved to the back of the queue.
The server will send a signal to the client process when it is their turn. The client will handle it by prompting for an answer.
We will update a file that the clients can read from to see which words have already been used and can't be used again. Another file will have the letter(s) generated at the start of the game.

Algorithms/data structures:  
The project will use the 3 way handshake we learned in class to safely connect the client and server. We will use an array queue of the clients for the turn based aspect. We will use string parsing to verify that the answers contain the right letter/s and are unique.

Arslan:  
Forking/persistent server
Shared memory/queue
File for words already used  

Dean:  
Client/server pipes
String methods
Signal/Client signal handling  

We will distribute all features evenly.
    
# Intended pacing:

Implement a server that can handle multiples clients - 1/5/23  
Implement a client application that connects to the server, simple version of game with one player - 1/8/23  
Implement the core game logic server side, timeout system - 1/10/23  
Implement extra features i.e. networking- 1/12/23 
Thorough testing and bug-fixing of existing features - 1/16/23  
