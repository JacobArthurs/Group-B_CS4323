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
        //Create placeholder struct from player, this line will be replaced with a call to 
        //game.c which should modify or return a struct
        struct user currentUser = player(1);
        singlePlayerScoreboard(currentUser);
    }
    else if(userSelection == 2){
        //Create placeholder struct from player, this line will be replaced with a call to 
        //game.c which should modify or return a struct
        struct user currentUser = player(2);
        multiPlayerScoreboard(currentUser);
    }
    else if(userSelection == 3){
        //No need to close socket at this point. After a round has been played, chat_client.c 
        //already closes the socket after the game is finished. If the user selects to play again, 
        //a new socket will be made with chat_client.c
        printf("Thank you for playing!");
        exit(0);
    }
}