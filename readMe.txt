Group B - CS 4323

To begin the execution of the game:

Please run main.c

Description: 

Our game begins its execution in main, where a server is connected to the cleint and is called. This server 
determines a random file for a game and finds all the possible words for that game after the user has determined 
what kind of game he is playing -- either single or multiplayer. The server also determines the initial character 
for the current game being played. From this point, each game is sent to their own game room where the chat 
server controls the natural flow of the game. Each player has a turn to input their next word, inputs obtained 
when it is not the players turn will be ignored. The word_processing file contains all of the functionality that 
is required to determine if the current word is in some way a postfix of the previous word, if a player has passed, 
it provides a method to register words, check whether a word is valid, and register both the word and the points 
associated with it. All requirements for the word processing of the game has been tested and found to be in working 
condition. Both the single and multiplayer Modes make use of the chat server and communicate with the scoreboard file 
that ultimately keeps track of The highest performers in each game mode, respectively. The scoreboard file has been 
tested and is found to be in working condition.

chat_client and chat_server were built using dummy sockets. The piped_client and piped_server were
The newer implementation we intended to use for the game to meet the requirements established by 
The instructor. 
