#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void singlePlayerScoreboard(struct user currentUser){
    //need to validate that currentUser won, and that they scored high enough to be added to the scoreboard
    //need to add logic to add currentUser to the correct position of scoreboard
    //Open file, append
    FILE *fptr = fopen("singlePlayer.txt", "a");
    fprintf(fptr, "\n");
    //Add first name, last name, country, score, number of found words, and number of added words to dictionary to scoreboard
    fprintf(fptr, "%-30s%-30s%-30s%-30d%-40d%d", currentUser.firstName, currentUser.lastName, currentUser.country, currentUser.score, currentUser.numFoundWords, currentUser.numNewWords);
    //Flush and close file
    fflush(fptr);
    fclose(fptr);
}

void multiPlayerScoreboard(struct user currentUser){
    //need to validata that currentUser scored high enough to be added to the scoreboard
    //need to add logic to add currentUser to the correct position of scoreboard
    char didWin[4];
    if(currentUser.won){
        strcpy(didWin, "Win");
    }
    else{
        strcpy(didWin, "Lose");
    }
    //Open file, append
    FILE *fptr = fopen("multiPlayer.txt", "a");
    fprintf(fptr, "\n");
    //Add first name, last name, country, score, didWin, number of found words, and number of added words to dictionary to scoreboard
    fprintf(fptr, "%-30s%-30s%-30s%-30d%-30s%-40d%d", currentUser.firstName, currentUser.lastName, currentUser.country, currentUser.score, didWin, currentUser.numFoundWords, currentUser.numNewWords);
    //Flush and close file
    fflush(fptr);
    fclose(fptr);
}