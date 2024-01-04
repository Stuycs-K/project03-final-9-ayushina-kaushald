# Final Project Proposal

## Group Members:

Arslan Ayushin, Dean Kaushal
       
# Intentions:

We are making a multiplayer text-based game inspired by Word Bomb. The server generates 1-3 letters. Players take turns entering a word that must contain the letters provided. If they do not enter a unique word within the time limit they are out of the game. Last person standing wins the game.

# Intended usage:

The game will be played on one computer with multiple terminals open. The server will generate 3 random letters that the clients can see. The 3 letters will stay the same for the whole game. There will be a timer that will incremently get slower each round. One round will include each client having a turn to type a word that contains the 3 letters in any order. If they fail to produce a unique word before the timer runs out, then they are out and will not have a turn in the next round. Once there is only one person that has a turn remaining, they win the game and the program ends. 
  
# Technical Details:

A description of your technical design. This should include:
   
How you will be using the topics covered in class in the project.
     
How you are breaking down the project and who is responsible for which parts.
  
What data structures you will be using and how.
     
What algorithms and /or data structures you will be using, and how.
    
# Intended pacing:

Implement a server that can handle multiples clients - 1/5/23
Implement a client application that connects to the server - 1/8/23
Implement the core game logic server side - 1/10/23
Implement the timer mechanism - 1/12/23
Thorough testing and bug-fixing/Maybe implement networking - 1/16/23