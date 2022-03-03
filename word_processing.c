// Miguelangel Pineda Soto

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
//#include "text_processing.h"
#include "player.c"

typedef struct wordList {
    char    words[50][100];                                     // store all the words used through the game
    int     index;                                            // store next available spot in the entire list
    int     bonusIndex;                                       // store point where input file words end and dictionary file words begin
} wordList;

typedef struct alphabet {
    char    letters[100];                                       // store alphabet for continuous printing
} alphabet;

typedef struct scoreCard {
    int score_1;                                              // keep track of player scores -- scores will not go below zero based on register_points()
    int passes_1;                                             // keep track of how many times a player has passed
    int badInputs_1;                                          // keep track of how many times a player has input an invalid word, 3 = 1 pass
    
    int score_2;
    int passes_2;
    int badInputs_2;
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

void register_word(char input[], struct user *list ) {                          // Adds a used word into list of previous words
    strcpy(list->wordList[list -> index], input);
    list -> index++;
    return;
}

void register_word2(char input[], struct wordList *list){
    strcpy(list->wordList[list -> index], input);
    list -> index++;
    return;
}

_Bool has_been_used(char input[], struct user plr) {                           // check if current input is present in previously used words
        
    int i;
    for (i = 0; i < sizeof(plr.index - 1); ++i) {
        if (strncmp(input, plr.wordList[i], strlen(input)) == 0) {
            return true;
        }
    }
    return false;
}

void find_valid_words(struct wordList *list, FILE *pointer, char *letters, _Bool skipLines) {
    char buffer_in[100];                                                    // buffer to read in from the  file
    int currIndex = 0;                                                     // determines end of user input words / beginning of dictionary words in validWords
    
    if (skipLines) {                                                       // if readig from an input file
        fgets(buffer_in, 100, pointer);                                      // Ignore blank line
        fgets(buffer_in, 100, pointer);                                      // Ignore words line
        fgets(buffer_in, 100, pointer);                                      // get first word in the list
        
        while (!feof(pointer)) {
            register_word2(buffer_in, list);                                    // add current word to the list
            fgets(buffer_in, 100, pointer);
            currIndex++;
        }
        list -> bonusIndex = currIndex;                                       // Set index where bonus points begin
    }
    else {                                                                  // if reading from the dictionary file
        fgets(buffer_in, 100, pointer);                                      // get first word in the dictionary
        while (!feof(pointer)) {
            char firstLetter = buffer_in[0] - 32;
            if (strchr(letters, firstLetter) != NULL) {                      // if first character is in the alphabet
                if (check_letters(buffer_in, letters) && !(has_been_used(buffer_in, list))) {
                    register_word2(buffer_in, list);                            // add current word to the list
                }
            }
            fgets(buffer_in, 100, pointer);
        }
    }
}

_Bool is_bonus_word(char input[], struct wordList *validWords) {                        // returns true if the word was found in the dictionary for bonus points
    char currWord[100];
    for (int i = validWords->bonusIndex ; i < sizeof(validWords) ; ++i) {
        strcpy(currWord, validWords->words[i]);
        if (strncmp(currWord, input, strlen(currWord)) == 0) {
            return true;
        }
    }
    return false;
}

void print_word_list(struct user player) {                                        // print all previously used words to the console
    int i;
    printf("Previously used words:\n");
    for (i = 0; i < player -> index; ++i) {
        printf("%s\n", wordList[i]);
    }
}

_Bool hasPassed(char input[], struct scoreCard *passes, _Bool gameTurn) {               // check if user is trying to pass on this turn
    int i;
    for (i = 0 ; i < strlen(input) ; ++i) {                                   // check all letters in input
        if (input[i] >= 'a' && input[i] <='z') {
            input[i] -= 32;                                                 // convert all letters to upper case
        }
    }
    if (strncmp(input, "PASS", strlen(input)) == 0) {
        if (!gameTurn) { passes->passes_1++; }
        else { passes->passes_2++; }
        return true;
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

_Bool is_first_run(struct user plr) {                                         // confirms if this is the first run of the game
    if (plr -> index == 0) {
        return true;
    }
    return false;
}

_Bool is_valid_start(char input[], char randChar) {                                     // checks the first word in the game for valid starting letter
    if (input[0] == randChar) { return true; }
    return false;
}

// Cofirms whether a word is a valid next word for the game
_Bool is_word_valid(char input[], char alphabet[], struct user plr, struct wordList *validWords, _Bool userCharPick, char randChar) {
    char prevWord[100];
    strcpy(prevWord, plr->wordList[plr->index - 1]);

    // Return true if:
    //                 The word contains the right kind of letters
    //                 The word has been not used before
    //                 The word is a postfix in some way of the previously used word
    //                 Account for whether this turn allows player to choose random characters
    
    if (check_letters(input, alphabet) && !has_been_used(input, plr) && is_first_run(plr) && !userCharPick
        && is_valid_start(input, randChar)) {
        return true;
    }
    else if (check_letters(input, alphabet) && !has_been_used(input, usedWords)
           && is_postfix(input, prevWord) && !userCharPick) {
        return true;
    }
    else if (check_letters(input, alphabet) && !has_been_used(input, usedWords) && userCharPick) {              // if both players passed
        return true;
    }
    return false;
}

// DO NOT DELETE
//void register_points(struct scoreCard *card, int playerNum, char input[], struct wordList *validWords) {                // register points in score card
//    int wordLen = (int) strlen(input);
//
//    if (playerNum == 1) {
//        if (strncmp(input, "invalid", strlen(input)) == 0) {
//            card->score_1 = (card->score_1 - 1 < 0) ? 0: card->score_1 - 1; }
//        else if (strncmp(input, "used", strlen(input)) == 0) {
//            card->score_1 = (card->score_1 - 2 < 0 || card->score_1 - 1 <= 0) ? 0: card->score_1 - 2; }
//        else if (wordLen == 3 || wordLen == 4) {
//            card->score_1 +=  (is_bonus_word(input, validWords)) ? 6: 1;}
//        else if (wordLen == 5) {
//            card->score_1 +=  (is_bonus_word(input, validWords)) ? 7: 2;}
//        else if (wordLen == 6) {
//            card->score_1 +=  (is_bonus_word(input, validWords)) ? 8: 3;}
//        else if (wordLen == 7) {
//            card->score_1 +=  (is_bonus_word(input, validWords)) ? 10: 5;}
//        else if (wordLen >= 8) {
//            card->score_1 +=  (is_bonus_word(input, validWords)) ? 16: 11;}
//    }
//    else if (playerNum == 2) {
//        if (strncmp(input, "invalid", strlen(input)) == 0) {
//            card->score_2 = (card->score_2 - 1 < 0) ? 0: card->score_2 - 1; }
//        else if (strncmp(input, "used", strlen(input)) == 0) {
//            card->score_2 = (card->score_2 - 2 < 0 || card->score_2 - 1 <= 0) ? 0: card->score_2 - 2; }
//        else if (wordLen == 3 || wordLen == 4) {
//            card->score_2 +=  (is_bonus_word(input, validWords)) ? 6: 1;}
//        else if (wordLen == 5) {
//            card->score_2 +=  (is_bonus_word(input, validWords)) ? 7: 2;}
//        else if (wordLen == 6) {
//            card->score_2 +=  (is_bonus_word(input, validWords)) ? 8: 3;}
//        else if (wordLen == 7) {
//            card->score_2 +=  (is_bonus_word(input, validWords)) ? 10: 5;}
//        else if (wordLen >= 8) {
//            card->score_2 +=  (is_bonus_word(input, validWords)) ? 16: 11;}
//    }
//}

void register_points(struct user card, char input[], struct wordList *validWords) {                // register points in score card
    int wordLen = (int) strlen(input);
    
    if (playerNum == 1) {
        if (strncmp(input, "invalid", strlen(input)) == 0) {
            card->score = (card->score - 1 < 0) ? 0: card->score - 1; }
        else if (strncmp(input, "used", strlen(input)) == 0) {
            card->score = (card->score - 2 < 0 || card->score - 1 <= 0) ? 0: card->score - 2; }
        else if (wordLen == 3 || wordLen == 4) {
            card->score +=  (is_bonus_word(input, validWords)) ? 6: 1;}
        else if (wordLen == 5) {
            card->score +=  (is_bonus_word(input, validWords)) ? 7: 2;}
        else if (wordLen == 6) {
            card->score +=  (is_bonus_word(input, validWords)) ? 8: 3;}
        else if (wordLen == 7) {
            card->score +=  (is_bonus_word(input, validWords)) ? 10: 5;}
        else if (wordLen >= 8) {
            card->score +=  (is_bonus_word(input, validWords)) ? 16: 11;}
    }
}

// DO NOT DELETE
//void print_game_status(struct wordList *usedWords, _Bool gameTurn, _Bool allowCharPick,                     // prints the current game status based on hw guidlines
//                struct scoreCard scoreCard, char firstLine[]) {
//    char nextChar;
//    char prevWord[100];
//
//    print_word_list(usedWords);
//    printf("Player 1 Score: %d\n", scoreCard.score_1);
//    printf("Player 2 Score: %d\n", scoreCard.score_2);
//
//    if (!gameTurn && !allowCharPick && scoreCard.badInputs_1 != 3) {
//        strcpy(prevWord, usedWords->words[usedWords->index-1]);
//        nextChar = prevWord[strlen(prevWord)-1];
//        printf("Player 1 - Enter a word using char %c:   Alphabet: %s", nextChar, firstLine);
//    }
//    else if (gameTurn && !allowCharPick && scoreCard.badInputs_2 != 3) {
//        strcpy(prevWord, usedWords->words[usedWords->index-1]);
//        nextChar = prevWord[strlen(prevWord)-1];
//        printf("Player 2 - Enter a word using char %c:   Alphabet: %s", nextChar, firstLine);
//    }
//    else if (!gameTurn && allowCharPick) {                                      // if both players have passed consecutively
//        printf("Player 1 - Enter a word using any char in the alphabet: %s", firstLine);
//        gameTurn = true;
//    }
//    else if (gameTurn && allowCharPick) {                                       // if both players have passed consecutively
//        printf("Player 2 - Enter a word using any char in the alphabet: %s", firstLine);
//        gameTurn = false;
//    }
//    else if (scoreCard.badInputs_1 == 3) {                                          // if first player has three bad inputs
//        hasPassed("PASS", &scoreCard, gameTurn);                                   // Register as a pass for respective player
//        printf("Player 2 - Enter a word using any char in the alphabet: %s", firstLine);
//        gameTurn = true;                                                       // change the current turn
//        allowCharPick = true;                                                   // allow next player to pick a character randomly
//    }
//    else if (scoreCard.badInputs_2 == 3) {                                          // if second player has three bad inputs
//        hasPassed("PASS", &scoreCard, gameTurn);                                   // Register as a pass for respective player
//        printf("Player 1 - Enter a word using any char in the alphabet: %s", firstLine);
//        gameTurn = false;                                                       // change  the current turn
//        allowCharPick = true;                                                   // allow next player to pick a character randomly
//    }
//}

void print_game_status(struct user player , char firstLine[]) {
    char nextChar;
    char prevWord[100];

    print_word_list(player.wordList);
    
    printf("Usable Letters: %s\n",player.randomAlphabets);
    printf("Your Current Score: %d\n",player.score);
    printf("Your Opponents Current Score: %d\n",player.opponentScore);
    printf("Starting Character of the next word: %s\n",player.currentWord[strlen(currentWord)-1]);
    printf("Please input one appropraite word: \n");

    // TODO:
    // Combine with game logic to print out differet last set of statements when the next player gets to choose the char
}

// NOTE: in order to return a string from this method it will need to be passed a string pointer from the caller
void generate_oppponent_word(struct user player, char prevWord[], struct wordList validWords) {
    _Bool passing = true;
    
    // Cycle through valid words' input file portion only for opponent moves
    for (int i = 0 ; i < player -> index; ++i) {
        // if the word has not been used and is valid based on the previous word, copy the string to the external oppWord char array (String)
        if (!has_been_used(player->wordList[i][0]) && is_word_valid(player->wordList[i][0], player.randomAlphabets, player.wordList, validWords, prevword, prevWord[strlen(prevWord - 1)])) {
            strcpy(player.currentWord, player->usedWords[i][0])
            passing = false;
            break;
        }
    }
    // If no valid word was found within the threshold, submit a pass for this round
    if (passing) {
        strcpy(oppWord, "Pass");
    }
}

void start_game(void) {
    
    char  user_input[100];                                 // store user's words in
    char  firstLine[100];                                  // store the alphabet for every file
    int   randInt;
    _Bool  gameTurn = false;                                // used to switch from one player to another
    _Bool  isGameOver = false;
    char  nextChar;
    _Bool consecPasses = false;
    _Bool allowCharPick = false;                                  // denotes whether players get to choose a random character
    
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
    
    // print_word_list(&validWords);                                                                            // use to confirm all possible  words registered in validWords
    
// -------------------------------------------------- Obtain inputs from the user ---------------------------------------------------------------------------------------------
    
    randInt = rand() % (strlen(firstLine) - 3);
    nextChar = firstLine[randInt];
    printf("Player 1 - Enter a word using char: %c   Alphabet: %s", nextChar, alpha.letters);
    scanf("%s", user_input);
    
    while (!isGameOver) {
        
        if (hasPassed(user_input, &scoreCard, gameTurn)) {                            // check if user is passing this turn
            
            if (scoreCard.passes_1 == 2 && scoreCard.passes_2 == 2) {                    // if we have had two consecutive passes from both players
                printf("The game is over. \n");                                             // end the game
                isGameOver = true;
                break;
            }
            if (scoreCard.passes_1 == 1 && scoreCard.passes_2 == 1) {
                allowCharPick = true;                                               // if both players have passed, allow next player to pick a character
            }
            
            consecPasses = true;                                                    // note the fact that one player has started passing
            gameTurn = !gameTurn;
        }
        else if (is_word_valid(user_input, firstLine, &usedWords, &validWords, allowCharPick, nextChar)) {
            if (!gameTurn) {
                register_points(&scoreCard, 1, user_input, &validWords);                             // register scores for player one
                gameTurn = true;
                consecPasses = false;
                allowCharPick = false;
                scoreCard.passes_1 = 0;                                                        // reset the pass count
                scoreCard.badInputs_1 += 0;                                                     // reset the invalid input count
            }
            else {
                register_points(&scoreCard, 2, user_input, &validWords);                             // register scores for player two
                gameTurn = false;
                consecPasses = false;
                allowCharPick = false;
                scoreCard.passes_1 = 0;                                              // reset the pass count
                scoreCard.badInputs_2 += 0;                                           // reset the invalid input count
            }
            register_word(user_input, &usedWords);                                     // register word in usedWords for this game
        }
        else if (!has_been_used(user_input, &usedWords)) {                               // deduct points for invalid words
            printf("Invalid word. \n");
            if (!gameTurn) {
                register_points(&scoreCard, 1, "invalid", &validWords);
                scoreCard.badInputs_1 += 1;
            }
            else {
                register_points(&scoreCard, 2, "invalid", &validWords);
                scoreCard.badInputs_2 += 1;
            }
        }
        else if (has_been_used(user_input, &usedWords)) {                                // deduct points for words already used
            printf("This word has already been used. \n");
            if (!gameTurn) {
                register_points(&scoreCard, 1, "used", &validWords);
                scoreCard.badInputs_1 += 1;                                            // register the bad input from user one
            }
            else {
                register_points(&scoreCard, 2, "used", &validWords);
                scoreCard.badInputs_2 += 1;                                            // register bad input from user two
            }
        }
        
        print_game_status(&usedWords, gameTurn, allowCharPick, scoreCard, firstLine);    // print the current status of the game
        
        scanf("%s", user_input);
        
    }

}

