//Group B
//Jacob Arthurs
//jacob.arthurs@okstate.edu
//2-27-2022

#include <stdio.h>
#include <stdlib.h>

#include "player.c"

int main(){
    int userSelection;
    printf("1. Single-Player\n");
    printf("2. Multi-Player\n");
    printf("3. Exit\n");
    printf("Please enter a selection: ");
    scanf("%d", &userSelection);
    if(userSelection == 1){
        player(1);
    }
    else if(userSelection == 2){
        player(2);
    }
    else if(userSelection == 3){
        printf("Thank you for playing!");
        exit(0);
    }
}