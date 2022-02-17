
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "player.c"
//the purpose of this client function is to serve as a mediator between the pllayer and the server,
//so that the client and the server are seperated, and that no data is saved in the client, but
//passed from the server to the player, and then fromt he player back to the client.

char [] client(struct user crtPlr){//for all instantces for printf, we need to change them to sendto,
//and all instances of scanf to recvfrom
	printf("Usable Letters: %s\n",crtPlr.randomAlphabets);
	printf("Your Current Score: %d\n",crtPlr.score);
	printf("Your Opponents Current Score: %d\n",crtPlr.opponentScore);
	printf("Words Already Used: %s\n",crtPlr.currentWord);//needs to be rewritten to happen many times over to print
	//each word that has been used and accepted already. Maybe a if or for loop that goes for how
	//large the array is holding all of the used words
	printf("Starting Character of the next word: %s\n",crtPlr.startingChar);
	printf("Please input one appropraite word: \n");
	char input[];
	scanf("%s",input);
	return input;
}
