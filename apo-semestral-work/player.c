#include "player.h"

player_t *new_player(int x, int y, uint16_t color, uint8_t last_knob_pos, int player_num, int direction)
{
    player_t *self = malloc(sizeof(player_t));
    self->x = x;
    self->y = y;
    self->height = SQUARE_SIZE;
    self->width = SQUARE_SIZE;
    self->last_knob_pos = last_knob_pos;
    self->color = color;
    self->dir = direction;
    self->leaving_trail = 1;
    self->player_num = player_num;
    return self;
}

void player_update(player_t *self, uint8_t knob_pos, player_t *enemy)
{
    int delta = knob_pos - self->last_knob_pos;

    // Handle wrap-around of the knob (assuming 8-bit range 0–255)
    if (delta > 128)
    {
        delta -= 256;
    }
    else if (delta < -128)
    {
        delta += 256;
    }
    self->last_knob_pos = knob_pos;

    // Update direction based on knob position
    if (delta > 2 && delta != 0)
    {
        // Clockwise turn: turn right
        self->dir = (self->dir + 1) % 4;
    }
    else if (delta < -2 && delta != 0)
    {
        // Counter-clockwise turn: turn left
        self->dir = (self->dir + 3) % 4;
    }

    // Compute the new position based on direction
    int new_x = self->x + directions[self->dir][0] * SQUARE_SIZE;
    int new_y = self->y + directions[self->dir][1] * SQUARE_SIZE;

    // Move the player if within bounds
    if (new_x >= 0 && new_x + self->width <= LCD_WIDTH)
    {
        self->x = new_x;
    }
    if (new_y >= 0 && new_y + self->height <= LCD_HEIGHT)
    {
        self->y = new_y;
    }

    // If the tile is not part of the player's solid zone, check for trail behavior
    if ((tiles[self->y / SQUARE_SIZE][self->x / SQUARE_SIZE] != 1 && self->player_num == 1) ||
        (tiles[self->y / SQUARE_SIZE][self->x / SQUARE_SIZE] != 3 && self->player_num == 2))
    {
        // If stepping on the enemy's trail, clear it and play sound
        if (tiles[self->y / SQUARE_SIZE][self->x / SQUARE_SIZE] == enemy->player_num * 2)
        {
            enemy->leaving_trail = 0;
            audio_play(440, 150);
            for (int i = 0; i < LCD_HEIGHT / SQUARE_SIZE; i++)
            {
                for (int j = 0; j < LCD_WIDTH / SQUARE_SIZE; j++)
                {
                    if (tiles[i][j] == enemy->player_num * 2)
                    {
                        tiles[i][j] = 0;
                    }
                }
            }
        }
        // Mark the trail if currently leaving one
        if (self->leaving_trail == 1)
        {
            tiles[self->y / SQUARE_SIZE][self->x / SQUARE_SIZE] = self->player_num * 2;
        }
    }
    else
    {
        // Player entered their own zone, finalize captured area
        fill_captured_area(self);
        self->leaving_trail = 1;
    }
}

void player_draw(player_t *self)
{
    // Draw white border around the player
    lcd_draw_rectangle(self->x, self->y, self->width, self->height, RGB(255, 255, 255));
    // Draw the player in its color
    lcd_draw_rectangle(self->x + 1, self->y + 1, self->width - 2, self->height - 2, self->color);
}
