// Miguelangel Pineda Soto

#include <stdio.h>
#include <string.h>             // used for comparisson of strings
#include <stdbool.h>            // used for boolean values
#include "text_processing.h"

typedef struct wordList {
    char    arr[20][100];   // store all the words used through the game
} wordList;

typedef struct alphabet {
    char    letters[100];   // store alphabet for continuous printing
} alphabet;

// Used to check whether all the letters in user input are present in the alphabet
// TODO: add a way to ensure the same character is not used twice or more than in alphabet
_Bool check_input_letters(char input[], char alphabet[]) {
    int i;
    for (i = 0 ; i < strlen(input) ; ++i) {         // check all letters in input
        if (strchr(alphabet, input[i]) == NULL) {
            return false;
        }
    }
    return true;
}

// Adds a used word into list of previous words
// TODO: add increasing index for additional words
void register_word(char input[], struct wordList *list ) {
    strcpy(list->arr[0], input);
    return;
}

// TODO: add a print previous words function

// Uses first line of input file as recurring alphabet print out
void set_alphabet(struct alphabet *alpha, char input[]) {
    strcpy(alpha->letters, input);
}

// Prints out the alphabet for player
void print_alphabet(struct alphabet *alpha) {
    printf("Alphabet: %s\n", alpha->letters);
}

// TODO: create function to determine a random letter to use for first run


void start_game(void) {
    char        buffer_in[100];             // buffer to read in from a file
    char        user_input[100];            // store user's words in
    char        firstLine[100];              // store the alphabet for every file

    FILE *fpointer;
    fpointer = fopen("/Users/miguelsoto/Documents/OSU/04 Operating Systems/Scratch/scratch/scratch/input_01.txt", "r");

    if (fpointer == NULL) {                     // basic error check for file opening
        perror("Failed to open the file");
        return 1;
    }

    // Instantiate a list of words struct to use in this game run
    wordList usedWords = {.arr = { {} } };
    // Instantiate alphabet struct
    alphabet alpha = {.letters = {}};

    int iterations = 0;      // temporary method of ending while statement
    while (iterations < 5) {

        if (!feof(fpointer)) {
            fgets(firstLine, 100, fpointer);        // read the first line of the file
            set_alphabet(&alpha, firstLine);
            printf("Alphabet: %s\n", alpha.letters);
        }

        printf("Type a word using the letters from this alphabet: ");
        scanf("%s", user_input);
        printf("You chose the following word: %s\n\n", user_input);

        // Check that all the letters used in the word are from the alphabet
        if (!check_input_letters(user_input, firstLine)) {
            printf("You used letters not included in the alphabet. Try again.\nS");
            return 1;
        }

        // Skip the next three lines in input file
        fgets(buffer_in, 100, fpointer);
        fgets(buffer_in, 100, fpointer);
        fgets(buffer_in, 100, fpointer);

        // Check words down the input file for a match
        while (!feof(fpointer)) {

            if ((strncmp(user_input, buffer_in, strlen(user_input)) == 0 && sizeof(user_input) == sizeof(buffer_in))) {
                printf("Your word matched!\n");
                // Add the matched word to a list of words already used
                register_word(user_input, &usedWords);
                break;
            }

            fclose(fpointer);                           // close file whe at EOF
        }

        if (feof(fpointer)) {
            printf("Your word did not match\n");
        }

        printf("Previously used words: %s\n", usedWords.arr[0]);
        rewind(fpointer);
        ++iterations;
    }
}


// TODO:
// add a way to check whether the recent input has been used in the past
// reference the point system for every successful word search
// find a way to use binary search through a file
// change format to while loop to keep game going
// incorporate player signaling from player.c

