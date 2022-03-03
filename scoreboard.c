//Group B
//Jacob Arthurs
//jacob.arthurs@okstate.edu
//2-27-2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.c"

void singlePlayerScoreboard(struct user currentUser){
    //get index of insertion for currentuser
    int singlePlayerIndex = getLowestSingleplayerScore(currentUser.score);
    //Check to see if player scored high enough to be added to scoreboard, and if they won
    if(singlePlayerIndex != -1 && currentUser.won){
        //Open file, read
        FILE *fptr2 = fopen("singlePlayer.txt", "r");

        //Create string array of 12 strings, holding 300 chars each
        char **lines = malloc(sizeof(int) * 12);
        for( int i = 0; i < 12; i++)
            lines[i] = malloc( 300 * sizeof *lines[i] );

        //Loop through and read each line from file into the lines array
        for(int i = 0; i < 12; i++){
            char currentLine[300];
            //insert location
            if(i == singlePlayerIndex+2){
                sprintf(currentLine, "%-30s%-30s%-30s%-30d%-40d%-40d", currentUser.firstName, currentUser.lastName, currentUser.country, currentUser.score, currentUser.numFoundWords, currentUser.numNewWords);
                strcpy(lines[i], currentLine);
                //add trailing new line character to current user score
                strcat(lines[i], "\n");
            }
            //all other lines
            else{
                fgets(currentLine, sizeof(currentLine), fptr2);
                strcpy(lines[i], currentLine);
            }
        }
        //Flush and close file
        fflush(fptr2);
        fclose(fptr2);
        //open file, write
        FILE *fptr = fopen("singlePlayer.txt", "w");
        //loop through and write every string within the lines array to the file
        for(int i = 0; i < 12; i++){
            fprintf(fptr, lines[i]);
        }
        //Flush and close file
        fflush(fptr);
        fclose(fptr);
    }
}

void multiPlayerScoreboard(struct user currentUser){
    //get index of insertion for currentuser
    int multiplayerIndex = getLowestMultiplayerScore(currentUser.score);
    //Check to see if player scored high enough to be added to scoreboard
    if(multiplayerIndex != -1){
        //Open file, read
        FILE *fptr2 = fopen("multiPlayer.txt", "r");

        //Create string array of 12 strings, holding 300 chars each
        char **lines = malloc(sizeof(int) * 12);
        for( int i = 0; i < 12; i++)
            lines[i] = malloc( 300 * sizeof *lines[i] );

        //Get win/lose string
        char didWin[5];
        if(currentUser.won){
            strcpy(didWin, "Win");
        }
        else{
            strcpy(didWin, "Lose");
        }

        //Loop through and read each line from file into the lines array
        for(int i = 0; i < 12; i++){
            char currentLine[300];
            //insert location
            if(i == multiplayerIndex+2){
                sprintf(currentLine, "%-30s%-30s%-30s%-30d%-30s%-40d%d", currentUser.firstName, currentUser.lastName, currentUser.country, currentUser.score, didWin, currentUser.numFoundWords, currentUser.numNewWords);
                strcpy(lines[i], currentLine);
                //add trailing new line character to current user score
                strcat(lines[i], "\n");
            }
            //all other lines
            else{
                fgets(currentLine, sizeof(currentLine), fptr2);
                strcpy(lines[i], currentLine);
            }
        }
        //Flush and close file
        fflush(fptr2);
        fclose(fptr2);

        //open file, w
        FILE *fptr = fopen("multiPlayer.txt", "w");
        //loop through and write every string within the lines array to the file
        for(int i = 0; i < 12; i++){
            fprintf(fptr, lines[i]);
        }
        //Flush and close file
        fflush(fptr);
        fclose(fptr);
    }
}

//returns index of scoreboard position if player scored high enough, and returns -1 if player did not score high enough
int getLowestSingleplayerScore(int playerScore){
    //open file, read
    FILE *fptr = fopen("singlePlayer.txt", "r");
    //Create and allocate placeholder struct
    struct user *scoreboardPlayers;
    scoreboardPlayers = malloc ( sizeof ( struct user ) * 10);

    //Skip two lines
    char line[300];
    fgets(line, sizeof(line), fptr);
    fgets(line, sizeof(line), fptr);
    free(line);
    
    int lineNum = 0;
    int minScore = 100;
    int minIndex;
    int currentScore;
    int insertLocation = -1;
    //While file is active, loop through and find lowest score on scoreboard
    while(!feof(fptr))
    {
        fscanf(fptr, "%s%s%s%d%d%d", scoreboardPlayers[lineNum].firstName, scoreboardPlayers[lineNum].lastName, scoreboardPlayers[lineNum].country, 
               &scoreboardPlayers[lineNum].score, &scoreboardPlayers[lineNum].numFoundWords, &scoreboardPlayers[lineNum].numNewWords);
        currentScore = scoreboardPlayers[lineNum].score;
        if(currentScore <= minScore){
            minScore = currentScore;
            minIndex = lineNum;
        }
        //If player's score is higher than any score on scoreboard, set insertlocation equal to the first instance of this happening
        if(playerScore > currentScore && insertLocation == -1){
            insertLocation = lineNum;
        }
        lineNum++;
    }
    //free memory
    free(scoreboardPlayers);
    
    //Flush and close file
    fflush(fptr);
    fclose(fptr);

    //check if player score high enough to be added to scoreboard
    if(playerScore > minScore){
        return insertLocation;
    }
    else{
        return -1;
    }
}

//returns index of scoreboard position if player scored high enough, and returns -1 if player did not score high enough
int getLowestMultiplayerScore(int playerScore){
    //open file, read
    FILE *fptr = fopen("multiPlayer.txt", "r");

    //Create and allocate placeholder struct
    struct user *scoreboardPlayers;
    scoreboardPlayers = malloc ( sizeof ( struct user ) * 10);

    char didWin[4];
    
    //Skip two lines
    char line[300];
    fgets(line, sizeof(line), fptr);
    fgets(line, sizeof(line), fptr);
    free(line);
    
    int lineNum = 0;
    int minScore = 100;
    int minIndex;
    int currentScore;
    int insertLocation = -1;
    //While file is active, loop through and find lowest score on scoreboard
    while(!feof(fptr))
    {
        fscanf(fptr, "%s%s%s%d%s%d%d", scoreboardPlayers[lineNum].firstName, scoreboardPlayers[lineNum].lastName, scoreboardPlayers[lineNum].country, 
               &scoreboardPlayers[lineNum].score, didWin, &scoreboardPlayers[lineNum].numFoundWords, &scoreboardPlayers[lineNum].numNewWords);
        currentScore = scoreboardPlayers[lineNum].score;
        if(currentScore <= minScore){
            minScore = currentScore;
            minIndex = lineNum;
        }
        //If player's score is higher than any score on scoreboard, set insertlocation equal to the first instance of this happening
        if(playerScore > currentScore && insertLocation == -1){
            insertLocation = lineNum;
        }
        lineNum++;
    }
    //Free memory
    free(scoreboardPlayers);
    
    //Flush and close file
    fflush(fptr);
    fclose(fptr);

    //Check if player score high enough to get put on scoreboard
    if(playerScore > minScore){
        return insertLocation;
    }
    else{
        return -1;
    }
}