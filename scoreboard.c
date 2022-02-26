//Group B
//Jacob Arthurs
//jacob.arthurs@okstate.edu
//2-27-2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void singlePlayerScoreboard(struct user currentUser){
    //Open file, append
    FILE *fptr = fopen("singlePlayer.txt", "a");
    if(currentUser.won /*&& currentUser.score >= (lowest score on scoreboard)*/){
        //need to add logic to add currentUser to the correct position of scoreboard
        fprintf(fptr, "\n");
        //Add first name, last name, country, score, number of found words, and number of added words to dictionary to scoreboard
        fprintf(fptr, "%-30s%-30s%-30s%-30d%-40d%d", currentUser.firstName, currentUser.lastName, currentUser.country, currentUser.score, currentUser.numFoundWords, currentUser.numNewWords);
    }
    //Flush and close file
    fflush(fptr);
    fclose(fptr);
}

void multiPlayerScoreboard(struct user currentUser){
    //Open file, append
    FILE *fptr = fopen("multiPlayer.txt", "a");
    /*int n;
    int prev = 0;
    while (fscanf(fptr, "%-120d", &n) < prev) {
        printf("%d\n", n);
        prev = n;
    }*/
    /*if(currentUser.score >= (lowest score on scoreboard)){*/
        //need to add logic to add currentUser to the correct position of scoreboard
        char didWin[4];
        if(currentUser.won){
            strcpy(didWin, "Win");
        }
        else{
            strcpy(didWin, "Lose");
        }
        fprintf(fptr, "\n");
        //Add first name, last name, country, score, didWin, number of found words, and number of added words to dictionary to scoreboard
        fprintf(fptr, "%-30s%-30s%-30s%-30d%-30s%-40d%d", currentUser.firstName, currentUser.lastName, currentUser.country, currentUser.score, didWin, currentUser.numFoundWords, currentUser.numNewWords);
    //}
    //Flush and close file
    fflush(fptr);
    fclose(fptr);
}