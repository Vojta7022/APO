#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include <stdint.h>
#include "lcd.h"
#include "params.h"
#include "audio.h"

// Structure to hold the player's position, size, direction, color, and other attributes
// This structure is used to represent a player in the game
typedef struct player
{
    int x, y;
    int width, height;
    int dir;
    uint8_t last_knob_pos;
    uint16_t color;
    int player_num;
    _Bool leaving_trail;
} player_t;

/**
 * Creates a new player instance.
 *
 * @param x The x-coordinate of the player.
 * @param y The y-coordinate of the player.
 * @param color The color of the player.
 * @param last_knob_pos The last position of the knob or encoder.
 * @param player_num The player number (1 or 2).
 * @param direction The initial direction of the player (0-3).
 *
 * @return Pointer to the newly created player_t instance.
 */
player_t *new_player(int x, int y, uint16_t color, uint8_t last_knob_pos, int player_num, int direction);

/**
 * Updates the player's position based on the knob position and the enemy player.
 *
 * @param self Pointer to the player_t instance.
 * @param knob_pos The current position of the knob or encoder.
 * @param enemy Pointer to the enemy player_t instance.
 */
void player_update(player_t *self, uint8_t knob_pos, player_t *enemy);

/**
 * Draws the player on the LCD screen.
 *
 * @param self Pointer to the player_t instance.
 */
void player_draw(player_t *self);

#endif /* PLAYER_H */
