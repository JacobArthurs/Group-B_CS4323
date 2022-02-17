#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void server(){
    //Allocate array to hold 10 file names
    char **fileNames = malloc(sizeof(int) * 10);
    //Allocate each cell in filesnames array to hold 21 characters
    for(int i = 0; i < 10; i++){
        fileNames[i] = malloc( 21 * sizeof *fileNames[i] );
    }
    //Initialize all filenames
    strcpy(fileNames[0], "./input/input_01.txt");
    strcpy(fileNames[1], "./input/input_02.txt");
    strcpy(fileNames[2], "./input/input_03.txt");
    strcpy(fileNames[3], "./input/input_04.txt");
    strcpy(fileNames[4], "./input/input_05.txt");
    strcpy(fileNames[5], "./input/input_06.txt");
    strcpy(fileNames[6], "./input/input_07.txt");
    strcpy(fileNames[7], "./input/input_08.txt");
    strcpy(fileNames[8], "./input/input_09.txt");
    strcpy(fileNames[9], "./input/input_10.txt");
    //select a random index
    srand(time(NULL));
    int randomIndex = rand() % 9;
    //open random file
    FILE *fptr = fopen(fileNames[randomIndex], "r");
    //disolve filenames memory
    free(fileNames);

    //get num of players, server stuff
    int placeHolderNumPlayers = 2;
    int selectedPlayer = rand() % placeHolderNumPlayers + 1;

    //Communicate with selected player that he will start

    //print beginning letters of file
    char *letters = malloc(7);
    fgets(letters, 7, fptr);
    printf("Available letters: %s\n", letters);

    //print random beginning letter to slected player
    randomIndex = rand() % 8;
    printf("Starting letter: %s", letters[randomIndex]);
}