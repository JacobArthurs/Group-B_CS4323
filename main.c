//Group B
//Jacob Arthurs
//jacob.arthurs@okstate.edu
//2-27-2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "player.c"

int main(){//for all instantces for printf, we need to change them to sendto,
//and all instances of scanf to recvfrom

//Why? Can't we just print to users screen and get information submited from user within main.c and player.c? 
//We can just add said information to the struct to be sent back and forth between the server.
//Also, please talk on groupme if you think there needs to be changes. I didn't see this for a long time. - Jacob
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
        exit(0);//need to replace exit with a call to terminate the
        //specific socket that this client is addressed to, and only that one.

        //This main.c file is a local players instance of the game. Exiting this instance of the game will not effect
        //the other players instance of the game. Whenever the local player is in this main menu a socket shouldn't even 
        //be opened yet because the game hasn't been started yet. - Jacob
    }
}