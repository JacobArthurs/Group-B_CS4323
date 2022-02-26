// Miguelangel Pineda Soto

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "text_processing.h"

typedef struct wordList {
    char    words[50][100];                                     // store all the words used through the game
    int     index;                                            // store next available spot in the entire list
    int     bonusIndex;                                       // store point where input file words end and dictionary file words begin
} wordList;

typedef struct alphabet {
    char    letters[100];                                       // store alphabet for continuous printing
} alphabet;

typedef struct scoreCard {                                                     // keep track of how many times a player has passed
    int score_1;
    int passes_1;
    
    int score_2;
    int passes_2;
} scoreCard;

void set_alphabet(struct alphabet *alpha, char input[]) {                          // Uses first line of input file as recurring alphabet print out
    strcpy(alpha->letters, input);
}

void print_alphabet(struct alphabet *alpha) {                                     // Prints out the alphabet for player
    printf("Alphabet: %s\n", alpha->letters);
}

_Bool check_letters(char input[], char alphabet[]) {                            // Check if all letters in user input are present in the alphabet
    int i;
    for (i = 0 ; i < strlen(input) ; ++i) {                                // check all letters in input
        if (input[i] >= 'a' && input[i] <='z') {
            input[i] -= 32;                                             // convert all letters to upper case
        }
        if (strchr(alphabet, input[i]) == NULL) {
            return false;
        }
    }
    return true;
}

void register_word(char input[], struct wordList *list ) {                          // Adds a used word into list of previous words
    strcpy(list->words[list -> index], input);
    list -> index++;
    return;
}

_Bool has_been_used(char input[], struct wordList *list) {                           // check if current input is present in previously used words
        
    int i;
    for (i = 0; i < sizeof(list); ++i) {
        if (strncmp(input, list->words[i], strlen(input)) == 0) {
            return true;
        }
    }
    return false;
}

void find_valid_words(struct wordList *list, FILE *pointer, char alphabet[], struct alphabet *alpha, _Bool skipLines) {
    char buffer_in[100];                                                    // buffer to read in from the  file
    int currIndex = 0;                                                     // determines end of user input words / beginning of dictionary words in validWords
    
    if (skipLines) {                                                       // if readig from an input file
        fgets(buffer_in, 100, pointer);                                      // Ignore blank line
        fgets(buffer_in, 100, pointer);                                      // Ignore words line
        fgets(buffer_in, 100, pointer);                                      // get first word in the list
        
        while (!feof(pointer)) {
            register_word(buffer_in, list);                                    // add current word to the list
            fgets(buffer_in, 100, pointer);
            currIndex++;
        }
        
        list -> bonusIndex = currIndex;                                       // Set index where bonus points begin
    }
    else {                                                                  // if reading from the dictionary file
        fgets(buffer_in, 100, pointer);                                      // get first word in the dictionary
        while (!feof(pointer)) {
            char firstLetter = buffer_in[0] - 32;
            if (strchr(alphabet, firstLetter) != NULL) {                      // if first character is in the alphabet
                if (check_letters(buffer_in, alphabet) && !(has_been_used(buffer_in, list))) {
                    register_word(buffer_in, list);                            // add current word to the list
                }
            }
            fgets(buffer_in, 100, pointer);
        }
    }
}

void print_word_list(struct wordList *list) {                                        // print all previously used words to the console
    int i;
    printf("Previously used words:\n");
    for (i = 0; i < list->index; ++i) {
        printf("%s\n", (list->words[i]));
    }
}

_Bool hasPassed(char input[], struct scoreCard *passes, int player) {               // check if user is trying to pass on this turn
    int i;
    for (i = 0 ; i < strlen(input) ; ++i) {                                  // check all letters in input
        if (input[i] >= 'a' && input[i] <='z') {
            input[i] -= 32;                                                // convert all letters to upper case
        }
        if (strncmp(input, "PASS", strlen(input)) == 0) {
            if (player == 0) { passes->passes_1++; }
            else { passes->passes_2++; }
            return true;
        }
    }
    return false;
}

_Bool is_postfix(char input[], char prevWord[]) {                                  // checks for postfixes from previous word in new word
    char postFix[30];
    strcpy(postFix, "");
    
    for (int i = 0 ; i < strlen(prevWord); ++i) {
        memcpy(postFix, &prevWord[strlen(prevWord) - i - 1], i+1 );
        if (strncmp(postFix, input, i + 1) == 0) {
            return true;
        }
    }
    return false;
}

_Bool is_first_run(struct wordList *usedWords) {
    if (usedWords -> index == 0) {
        return true;
    }
    return false;
}

// Cofirms whether a word is a valid next word for the game
_Bool is_word_valid(char input[], char alphabet[], struct wordList *usedWords, struct wordList *validWords) {
    char prevWord[100];
    strcpy(prevWord, usedWords->words[usedWords->index - 1]);

    // Return true if:
    //                 The word contains the right kind of letters
    //                 The word has been used before
    //                 The word is a postfix in some way of the previously used word
    
    if (check_letters(input, alphabet) && !has_been_used(input, usedWords) && is_first_run(usedWords)) {
        return true;
    }
    else if (check_letters(input, alphabet) && !has_been_used(input, usedWords)
           && is_postfix(input, prevWord)) {
        return true;
    }
    return false;
}

void register_points(struct scoreCard *card, int playerNum, char input[]) {
    int wordLen = (int) strlen(input);
    
    if (playerNum == 1) {
        if (wordLen == 3 || wordLen == 4) { card->score_1 += 1; }
        else if (wordLen == 5) { card->score_1 += 2; }
        else if (wordLen == 6) { card->score_1 += 3; }
        else if (wordLen == 7) { card->score_1 += 5; }
        else if (wordLen >= 8) { card->score_1 += 11; }
    }
    else {
        if (wordLen == 3 || wordLen == 4) { card->score_2 += 1; }
        else if (wordLen == 5) { card->score_2 += 2; }
        else if (wordLen == 6) { card->score_2 += 3; }
        else if (wordLen == 7) { card->score_2 += 5; }
        else if (wordLen >= 8) { card->score_2 += 11; }
    }
}


void start_game(void) {
    
    char  user_input[100];                                 // store user's words in
    char  firstLine[100];                                  // store the alphabet for every file
    _Bool  gameTurn = false;                                // used to switch from one player to another
    _Bool  isGameOver = false;
    
    wordList usedWords = {.words = { {} } };                       // Instantiate a list of words struct to use in this game run
    wordList validWords = {.words = { {} } };                      // create list of all valid words in input file and dictionary
    alphabet alpha = {.letters = {}};                               // Instantiate alphabet struct
    scoreCard scoreCard = {0, 0, 0, 0};                            // Instantiate a score card for this game

// ---------------------------------------------------Create a list of all possible words-----------------------------------------------------------------------------------------
    FILE *inputFPtr;
    FILE *dictFPtr;
    
    inputFPtr = fopen("/Users/miguelsoto/Documents/OSU/04 Operating Systems/Scratch/scratch/scratch/input_01.txt", "r");
    dictFPtr = fopen("/Users/miguelsoto/Documents/OSU/04 Operating Systems/Scratch/scratch/scratch/dictionary.txt", "r");
    if (inputFPtr == NULL || dictFPtr == NULL) {                                // basic error check for file opening
        perror("Failed to open the file");
        return 1;
    }
    
    fgets(firstLine, 100, inputFPtr);
    set_alphabet(&alpha, firstLine);                                            // get alphabet from first line in input file
    
    find_valid_words(&validWords, inputFPtr, firstLine, &alpha, true);             // get all possible words from the input file
    fclose(inputFPtr);

    find_valid_words(&validWords, dictFPtr, firstLine, &alpha, false);             // get all possible words from the dictionary file
    fclose(dictFPtr);
    
    print_word_list(&validWords);
    
// -------------------------------------------------- Start the execution of the game ---------------------------------------------------------------------------------------------
    
    printf("Type a word in the following alphabet: %s\n", alpha.letters);
    scanf("%s", user_input);
    
    int iterations = 0;                                                            // TEMPORARY METHOD OF ENDING WHILE STATEMENT
    while (iterations < 8) {
        
        if (hasPassed(user_input, &scoreCard, 0)) {                                   // check if user is passing this turn
            printf("Passed. \n");
            
            if (scoreCard.passes_1 >= 3) {
                printf("The game is over. \n");
                break;
            }
            
            printf("Enter another word: ");
            scanf("%s", user_input);
            continue;
        }
        
        if (is_word_valid(user_input, firstLine, &usedWords, &validWords)) {
            // printf("This was a valid word\n");
            
            if (!gameTurn) {
                register_points(&scoreCard, 1, user_input);
                gameTurn = true;
            }
            
            else {
                register_points(&scoreCard, 2, user_input);
                gameTurn = false;
            }
            register_word(user_input, &usedWords);                                     // register word in usedWords
        }
        else {
            printf("Invalid word. \n");
        }
        
        print_word_list(&usedWords);
        printf("Player 1 Score: %d\n", scoreCard.score_1);
        printf("Player 2 Score: %d\n", scoreCard.score_2);
        
        if (!gameTurn) {
            printf("Player 1 - Enter another word: ");
        }
        else {
            printf("Player 2 - Enter another word: ");
        }
        
        scanf("%s", user_input);
        
        ++iterations;
    }
}


// TODO:
// add a way to check whether the recent input has been used in the past
// reference the point system for every successful word search
// find a way to use binary search through a file
// change format to while loop to keep game going
// incorporate player signaling from player.c
// add mechanism for passing a on a turn
