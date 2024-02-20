/*
* Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
*
* Copyright (C) 2015, Chinomso Nosiri (100846639), Ayush Patel (100823602), Bilal Khalil (100824635)
* All rights reserved.
*
*/
#ifndef PLAYERS_H_
#define PLAYERS_H_

#include <stdbool.h>

#define MAX_LEN 256

// Player struct for each player
typedef struct {
    char name[MAX_LEN];
    int score;
} player;

// Returns true if the player name matches one of the existing players
extern int player_exists(player *players, int num_players, char *name);

// Go through the list of players and update the score for the
// player given their name
extern void update_score(player *players, int num_players, char *name, int score);

// update the score for the player given their array index
extern void update_score_index(player* players, int index, int score);

#endif /* PLAYERS_H_ */
