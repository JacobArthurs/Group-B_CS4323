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
        char startingChar;
        char currentWord[7];
        int score;
        int opponentScore;
        int numFoundWords;
        int numNewWords;
        bool won;
};

//1=singleplayer, 2=multiplayer
void player(int singlePlayer){
    //Ignore all initialization of struct, it is a placeholder. Struct data will be obtained from server.
    struct user user1;
    strcpy(user1.randomAlphabets, "HLEEDA");
    user1.startingChar = 'h';
    user1.numFoundWords = 7;
    user1.numNewWords = 10;
    user1.won = true;
    user1.score = 15;

    char firstName[20];
    char lastName[20];
    char country[20];
    if(singlePlayer == 1){
        //Communicate with server that this is singleplayer
    }
    else if(singlePlayer == 2){
        //Communicate with server that this is multiplayer. Wait for other players

        //Get user information
        //Get first name, max 2 mins wait time
        clock_t before = clock();
        int sec = 0;
        printf("Please enter your first name: ");
        do{
            if( kbhit() ){
                scanf("%s", firstName);
                break;
            }
            clock_t difference = clock() - before;
            sec = difference / CLOCKS_PER_SEC;
        }
        while((sec/60) < 2);
        //if user didn't enter word in 2 mins communicate with server that player is inactive
        if((sec/60) >= 2){
            
        }

        //Get last name, max 2 mins wait time
        before = clock();
        sec = 0;
        printf("Please enter your last name: ");
        do{
            if( kbhit() ){
                scanf("%s", lastName);
                break;
            }
            clock_t difference = clock() - before;
            sec = difference / CLOCKS_PER_SEC;
        }
        while((sec/60) < 2);
        //if user didn't enter word in 2 mins communicate with server that player is inactive
        if((sec/60) >= 2){
            
        }

        //Get country, max 2 mins wait time
        before = clock();
        sec = 0;
        printf("Please enter your country: ");
        do{
            if( kbhit() ){
                scanf("%s", country);
                break;
            }
            clock_t difference = clock() - before;
            sec = difference / CLOCKS_PER_SEC;
        }
        while((sec/60) < 2);
        //if user didn't enter word in 2 mins communicate with server that player is inactive
        if((sec/60) >= 2){
            
        }
    }

    //Some condition, communicate with server that game is still active
    while(0){
        //wait for other players input/server input
        //get user input word, wait only 4 mins
        clock_t before = clock();
        int sec = 0;
        do{
            if( kbhit() ){
                scanf("%s", user1.currentWord);
                break;
            }
            clock_t difference = clock() - before;
            sec = difference / CLOCKS_PER_SEC;
        }
        while((sec/60) < 4);
        //if user didn't enter word in 4 mins, assign empty string
        if((sec/60) >= 4){
            strcpy("", user1.currentWord);
        }

        //Send struct to server for processing
        //validate currentWord and assign points
        //Get the condition for the while loop that the game is still active
    }
    //Game is finished after exiting while loop
    //Communicate with server if player won

    //Get user information if player won, was playing singleplayer, and if they scored high enough to be added to scoreboard
    if(singlePlayer == 1){
        printf("Please enter your first name: ");
        scanf("%s",firstName);
        printf("Please enter your last name: ");
        scanf("%s", lastName);
        printf("Please enter your country: ");
        scanf("%s", country);
        //Open file, append
        FILE *fptr = fopen("singlePlayer.txt", "a");
        fprintf(fptr, "\n");
        //Add first name, last name, country, score, number of found words, and number of added words to dictionary to scoreboard
        fprintf(fptr, "%-30s%-30s%-30s%-30d%-40d%d", firstName, lastName, country, user1.score, user1.numFoundWords, user1.numNewWords);
        //Flush and close file
        fflush(fptr);
        fclose(fptr);
    }
    
    //Get user information if they scored high enough to be added to scoreboard
    else if(singlePlayer == 2){
        char didWin[4];
        if(user1.won){
            strcpy(didWin, "Win");
        }
        else{
            strcpy(didWin, "Lose");
        }
        //Open file, append
        FILE *fptr = fopen("multiPlayer.txt", "a");
        fprintf(fptr, "\n");
        //Add first name, last name, country, score, didWin, number of found words, and number of added words to dictionary to scoreboard
        fprintf(fptr, "%-30s%-30s%-30s%-30d%-30s%-40d%d", firstName, lastName, country, user1.score, didWin, user1.numFoundWords, user1.numNewWords);
        //Flush and close file
        fflush(fptr);
        fclose(fptr);
    }
}