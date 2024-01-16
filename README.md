[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/SQs7pKlr)
# WORD BOMB

### C CAPTAINS
Arslan Ayushin
Dean Kaushal
       
### Project Description:

We made a multiplayer text-based game where a letter is generated randomly at the start. Then, players take turns entering a word that must contain the letter provided. If they do not enter a unique word(one that hasn't been entered previously) that starts with the randomly generated letter within the time limit, they are eliminated from the game. The game will be played on one computer with multiple terminals open. The server will setup shared memory, etc. and generate the letter. The clients will connect to the server, and display the letter of the game. Once at least 3 clients have connected, the server will print which player must go next, and that client will send the answer that is typed to the server. Then the next client will have a turn to enter a word. Every word used gets added to a list of already used words. If they fail to produce a unique word before the timer runs out, then they are out and will not have a turn in the next round. Once there is only one person that has a turn remaining, they win the game and the program ends. 
  
### Instructions:

How does the user install/compile/run the program.
How does the user interact with this program?

To compile this program, type in "make compile". Then, type in "./server" which will initialize the game. Open up more terminals in the same directory and type in "./client" which will connect that terminal to the game.
Each client will have 10 seconds to come up with a word that starts with the "letter of the game". If the word is not unique or does not start with the correct letter, then the client will get the message "Invalid word".
Additionally, if the 10 seconds for a client's turn run out, the client will get the message "Time ran out" and they are removed from the game.
