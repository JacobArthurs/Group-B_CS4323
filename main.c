//Group B
//Jacob Arthurs
//jacob.arthurs@okstate.edu
//2-27-2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "player.c"
#include "scoreboard.c"

int main(){
    int userSelection;
    printf("1. Single-Player\n");
    printf("2. Multi-Player\n");
    printf("3. Exit\n");
    printf("Please enter a selection: ");
    scanf("%d", &userSelection);
    if(userSelection == 1){
        //Create user struct and get name info with player() to be passed to game.c to play game and modify struct
        struct user currentUser = player(currentUser);
        //Insert chat_client.c call here, which should call game.c, then after game is complete display scoreboard
        singlePlayerScoreboard(currentUser);
    }
    else if(userSelection == 2){
        //Create void user struct to be passed to game.c to play game and modify struct
        struct user currentUser;
        //Insert chat_client.c call here, which should call game.c, then get name info with player(), then after game is complete display scoreboard
        currentUser = player(currentUser);
        multiPlayerScoreboard(currentUser);
    }
    else if(userSelection == 3){
        //No need to close socket at this point. After a round has been played, chat_client.c 
        //already closes the socket after the game is finished. If the user selects to play again, 
        //a new socket connection will be made with chat_client.c
        printf("Thank you for playing!");
        exit(0);
    }
}