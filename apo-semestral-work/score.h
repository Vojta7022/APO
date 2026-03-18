#ifndef SCORE_H
#define SCORE_H
#include "params.h"
#include "player.h"
#include <stdio.h>

typedef struct player player_t;

typedef struct score {
    int player1_score;
    int player2_score;
} score_t;

score_t* new_score();

void score_update(score_t* self, player_t* player);

void score_draw(score_t* self);

#endif /* SCORE_H */
