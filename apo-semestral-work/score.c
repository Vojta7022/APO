#include "score.h"

score_t* new_score() {
    score_t* self = malloc(sizeof(score_t));
    self->player1_score = 0;
    self->player2_score = 0;
    return self;
}

void score_update(score_t* self, player_t* player) {
    if (player->player_num == 1) {
        self->player1_score++;
    } else {
        self->player2_score++;
    }
}

void score_draw(score_t* self) {
    char buffer[20];
    // Draw the scores on the LCD screen to the bottom left and right corners
    snprintf(buffer, sizeof(buffer), "P1: %d", self->player1_score);
    lcd_draw_string(0, 304, buffer, RGB(255, 255, 255), 1, &font_rom8x16);

    snprintf(buffer, sizeof(buffer), "P2: %d", self->player2_score);
    lcd_draw_string(400, 304, buffer, RGB(255, 255, 255), 1, &font_rom8x16);
}
