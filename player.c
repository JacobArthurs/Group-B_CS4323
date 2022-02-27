//Group B
//Jacob Arthurs
//jacob.arthurs@okstate.edu
//2-27-2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

struct user {
    char randomAlphabets[7];
    char wordList[20][100]; 
    char currentWord[7];
    int score;
    int opponentScore;
    int numFoundWords;
    int numNewWords;
    _Bool won;
    char firstName[20];
    char lastName[20];
    char country[20];
    int skipCount;
    int clientpid;
};

//1=singleplayer, 2=multiplayer
struct user player(struct user currentPlayer){
    //Ignore all initialization of struct, it is a placeholder. Struct data will be obtained from server.
    strcpy(currentPlayer.randomAlphabets, "HLEEDA");
    currentPlayer.numFoundWords = 7;
    currentPlayer.numNewWords = 10;
    currentPlayer.won = true;
    currentPlayer.score = 14;

    //Get user information
    //Get first name, max 2 mins wait time
    clock_t before = clock();
    int sec = 0;
    printf("Please enter your first name: ");
    do{
        if( kbhit() ){
            scanf("%s", currentPlayer.firstName);
            break;
        }
        clock_t difference = clock() - before;
        sec = difference / CLOCKS_PER_SEC;
    }
    while((sec/60) < 2);
    if((sec/60) >= 2){
        //if user didn't enter word in 2 mins communicate with server that player is inactive
    }

    //Get last name, max 2 mins wait time
    before = clock();
    sec = 0;
    printf("Please enter your last name: ");
    do{
        if( kbhit() ){
            scanf("%s", currentPlayer.lastName);
            break;
        }
        clock_t difference = clock() - before;
        sec = difference / CLOCKS_PER_SEC;
    }
    while((sec/60) < 2);
    if((sec/60) >= 2){
        //if user didn't enter word in 2 mins communicate with server that player is inactive
    }

    //Get country, max 2 mins wait time
    before = clock();
    sec = 0;
    printf("Please enter your country: ");
    do{
        if( kbhit() ){
            scanf("%s", currentPlayer.country);
            break;
        }
        clock_t difference = clock() - before;
        sec = difference / CLOCKS_PER_SEC;
    }
    while((sec/60) < 2);
    if((sec/60) >= 2){
        //if user didn't enter word in 2 mins communicate with server that player is inactive
    }
    return currentPlayer;
}