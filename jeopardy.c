/*
* Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
*
* Copyright (C) 2015, Chinomso Nosiri (100846639), Ayush Patel (100823602), Bilal Khalil (100824635)
* All rights reserved.
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "questions.h"
#include "players.h"
#include "jeopardy.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256
#define NUM_PLAYERS 4

typedef enum {
    G_STATE_SELECT_TURN,
    G_STATE_PLAY_TURN,
    G_STATE_ANSWER_QUESTION,
    G_STATE_DISPLAY_RESULTS,
} gamestate;

// Put global environment variables here
int num_players = 0;
int questions_answered = 0;
player players[NUM_PLAYERS];
char buffer[BUFFER_LEN] = { 0 };

// Processes the answer from the user containing what is or who is and tokenizes it to retrieve the answer.
void tokenize(char *input, char **tokens){
    char* str = strtok(input, " ");

    if(str != NULL){
        if(strcmp(str, "who") != 0 && strcmp(str, "what") != 0)return;
    }

    str = strtok(NULL, " ");
    if(str != NULL){
        if(strcmp(str, "is") != 0)return;
    }

    *tokens = strtok(NULL, "\n");
}

int show_results_comp_func(const void* p1, const void* p2){
    int f = players[*((int*)p1)].score;
    int s = players[*((int*)p2)].score;
    if (f > s) return -1;
    if (f < s) return 1;
    return 0;
}

// Displays the game results for each player, their name and final score, ranked from first to last place
void show_results(player *players, int num_players){
    printf("These are the results...\n");

    int indices[NUM_PLAYERS];
    for (int i=0; i<num_players; indices[i]=i, ++i);

    qsort(indices, num_players, sizeof(int), show_results_comp_func);

    printf("The winner is %s! With a score of $%d\n", players[indices[0]].name, players[indices[0]].score);
    for (int i=1; i<num_players; ++i){
        player* p = players+indices[i];
        printf("%d. %s, score: $%d\n", (i+1), p->name, p->score);
    }
}


void get_question(char* input, char** category, int* value){
    char c;
    int i = 0;
    int val_start = -1, val_end = -1;
    do {
        c = input[i];
        if (c>='0' && c<='9'){
            if (val_start==-1){
                val_start = i;
            }
        }else{
            if (val_start!=-1 && val_end==-1){
                input[i] = '\0';
                val_end = i;
            }
        }
        i++;
    } while(c != '\0');
    *value = atoi(input+val_start);

    i -= 2;
    val_start = -1; val_end = -1;
    do {
        c = input[i];
        if ((c>='A' && c<='Z') || (c>='a' && c<='z')){
            if (val_start==-1){
                val_start = i;
                input[i+1] = '\0';
            }
        }else{
            if (val_start!=-1 && val_end==-1){
                val_end = i+1;
                break;
            }
        }
        --i;
    } while(c != '\0');
    *category = input+val_end;
}

void clear_screen(){
    printf("\e[1;1H\e[2J");
}

bool get_input(char* buffer){
    printf(">> ");
    fflush(stdin);

    // memset(buffer, 0, sizeof(buffer));
    if (fgets(buffer, BUFFER_LEN, stdin) == NULL)return false;
    size_t len = strlen(buffer);
    if (len <= 1)return false;

    if (buffer[len - 1] == '\n'){
        buffer[len - 1] = '\0';
    }
    return true;
}


int main()
{
    memset(players, 0, sizeof(players));

    // Display the game introduction and initialize the questions
    printf("Welcome to a game a Jeopardy!\n");
    initialize_game();

    // Prompt for players names
    clear_screen();
    printf("How many people are playing? There can only be up to %d players: ", NUM_PLAYERS);
    scanf("%d", &num_players);
    num_players = (num_players > NUM_PLAYERS) ? NUM_PLAYERS : num_players;
    printf("\nEnter your names...\n");

    for (int i=0; i<num_players; ++i){
        printf("%d. ", (i+1));
        scanf("%s", players[i].name);
    }

    printf("\nThe game is starting...\n");
    sleep(1);


    int player_index = -1;
    int question_index = -1;
    gamestate state = G_STATE_SELECT_TURN;
    bool running = true;

    // Perform an infinite loop getting command input from users until game ends
    while (running)
    {
        clear_screen();

        switch (state){
            case G_STATE_SELECT_TURN: {
                if (questions_answered >= NUM_QUESTIONS){
                    state = G_STATE_DISPLAY_RESULTS;
                    continue;
                }

                printf("Who's turn is it?\n");
                if (!get_input(buffer))continue;
                player_index = player_exists(players, num_players, buffer);
                if (player_index == -1){
                    printf("That player doesn't exist...\n");
                    sleep(2);
                    continue;
                }
                state = G_STATE_PLAY_TURN;
            } break;

            case G_STATE_PLAY_TURN: {
                display_categories();
                printf(
                    "\nIts %s's turn!\n"
                    "Please enter the dollar amount and category of the question you want to answer, "
                    "but please let the category be the last word in the sentence\n"
                    "Example: Let me go for a 100 dollars in programming\n"
                    "Example: $100 programming\n",
                    players[player_index].name
                );
                if (!get_input(buffer))continue;

                char* category = NULL;
                int value = 0;
                get_question(buffer, &category, &value);

                question_index = get_question_index(category, value);
                if (question_index == -1){
                    printf("That question doesn't exist...\n");
                    sleep(2);
                    continue;
                }

                if (already_answered(category, value)){
                    printf("That question has already been answered...\n");
                    sleep(2);
                    continue;
                }
                state = G_STATE_ANSWER_QUESTION;
            } break;

            case G_STATE_ANSWER_QUESTION: {
                display_question_index(question_index);
                if (question_index < 0)continue;
                question* q = questions+question_index;

                printf("\nWhat's %s's answer?\n", players[player_index].name);
                if (!get_input(buffer))continue;
                char* answer = NULL;
                tokenize(buffer, &answer);
                if (!answer){
                    printf("Invalid Input...\n");
                    sleep(1);
                    continue;
                }

                bool valid = valid_answer_index(question_index, answer);
                printf("\nYour answer is... (suspenseful music)\n");

                if (valid){
                    printf("Correct!!\nYou've just earned $%d\n", q->value);
                    update_score_index(players, player_index, q->value);
                    sleep(2);
                }else{
                    sleep(2);
                    printf("Incorrect.\nThe correct answer was: %s\n", q->answer);
                    sleep(2);
                }
                q->answered = true;
                questions_answered++;
                state = G_STATE_SELECT_TURN;
            } break;

            case G_STATE_DISPLAY_RESULTS: {
                printf("There are no more questions left to answer.\n\n");
                show_results(players, num_players);
                running = false;
            } break;
        }

        // Call functions from the questions and players source files

        // Execute the game until all questions are answered

        // Display the final results and exit
    }
    return EXIT_SUCCESS;
}
