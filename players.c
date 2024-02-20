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
#include "players.h"


// Returns the index if the player name matches one of the existing players, and -1 if not
int player_exists(player *players, int num_players, char *name)
{
    for (int i=0; i<num_players; ++i){
        if (strcmp(name, players[i].name) == 0)return i;
    }
    return -1;
}

// update the score for the player given their array index
void update_score_index(player* players, int index, int score){
    players[index].score += score;
}

// Go through the list of players and update the score for the
// player given their name
void update_score(player *players, int num_players, char *name, int score)
{
    for (int i=0; i<num_players; ++i){
        if (strcmp(name, players[i].name) != 0)continue;
        update_score_index(players, i, score);
        return;
    }
}
