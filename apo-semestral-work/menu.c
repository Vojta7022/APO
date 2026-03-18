#include "menu.h"

menu_t *new_menu(menu_t *self)
{
    self = malloc(sizeof(menu_t));
    self->selected = 0;
    self->last_knob_pos = 0;
    return self;
}

// Menu items
const char *menu_items[] = {
    "Tilemania+",
    "Play",
    "Gametime ",
    "Exit"};

void update_menu(menu_t *self, uint8_t knob, _Bool button_pressed, int *game_state, gametime_led_t *led, score_t *score, player_t *player1, player_t *player2)
{
    // Compute knob rotation delta considering wrap-around at 255 -> 0
    int delta = knob - self->last_knob_pos;
    if (delta > 128)
    {
        delta -= 256;
    }
    else if (delta < -128)
    {
        delta += 256;
    }
    self->last_knob_pos = knob;
    if (*game_state == MENU)
    {
        // Navigate down in menu
        if (delta > 2 && delta != 0)
        {
            self->selected = (self->selected + 1) % 3;
        }
        // Navigate up the menu
        else if (delta < -2 && delta != 0)
        {
            self->selected = (self->selected + 2) % 3;
        }
        // Handle button press
        if (button_pressed)
        {
            if (self->selected == 0)
            {
                *game_state = GAME;
                audio_play(1760, 250); // Play sound for game start
            }
            else if (self->selected == 1)
            {
                *game_state = EDIT_GAMETIME;
            }
            else if (self->selected == 2)
            {
                *game_state = EXIT;
            }
        }
    }
    else if (*game_state == EDIT_GAMETIME)
    {
        // Increase gametime with right turn
        if (delta > 2 && delta != 0) {
            game_time++;
            if (game_time > 20) game_time = 1;
        }
        // Decrease gametime with left turn 
        else if (delta < -2 && delta != 0) {
            game_time--;
            if (game_time < 1) game_time = 20;
        }
        // Confirm gametime with button press
        if (button_pressed) {
            *game_state = MENU;
        }
    }
    else
    {
        // Switch between "Play Again" and "Return to Menu"
        if (delta != 0)
            self->selected = (self->selected + 1) % 2;
        if (button_pressed)
        {
            // Reset game variables
            score->player1_score = 0;
            score->player2_score = 0;
            led->value = UINT32_MAX;
            led->counter = 0;
            dump_tiles(); // Clear the tiles

            // Reinitialize players
            free(player1);
            player1 = new_player(100, 160, RGB(255, 0, 0), 128, 1, 1);
            for (int i = player1->x / SQUARE_SIZE - 1; i <= player1->x / SQUARE_SIZE + 1; i++)
            {
                for (int j = player1->y / SQUARE_SIZE - 1; j <= player1->y / SQUARE_SIZE + 1; j++)
                {
                    tiles[j][i] = 1;
                }
            }
            free(player2);
            player2 = new_player(380, 160, RGB(0, 0, 255), 128, 2, 3);
            for (int i = player2->x / SQUARE_SIZE - 1; i <= player2->x / SQUARE_SIZE + 1; i++)
            {
                for (int j = player2->y / SQUARE_SIZE - 1; j <= player2->y / SQUARE_SIZE + 1; j++)
                {
                    tiles[j][i] = 3;
                }
            }

            // Switch game state based on selection
            if (self->selected == 0)
            {
                *game_state = GAME; // Play again
            }
            else if (self->selected == 1)
            {
                *game_state = MENU; // Return to menu
            }
        }
    }
}

void draw_menu(menu_t *self, uint16_t color, int *game_state, score_t *score, player_t *player1, player_t *player2)
{
    font_descriptor_t *fdes;
    fdes = &font_winFreeSystem14x16;
    int scale = 4;
    if (*game_state != GAME_OVER)
    {
        // Title
        lcd_draw_string(100, 20, menu_items[0], color, scale, fdes);

        // First menu item: Play
        if (self->selected == 0)
            lcd_draw_string(5, 100, "->", color, scale, fdes);
        lcd_draw_string(60, 100, menu_items[1], color, scale, fdes);

        // Second menu item: Gametime
        if (self->selected == 1)
            lcd_draw_string(5, 150, "->", color, scale, fdes);
        if (*game_state == EDIT_GAMETIME)
        {
            char buffer[20];
            snprintf(buffer, sizeof(buffer), "Gametime: %d", game_time);
            lcd_draw_string(60, 150, buffer, color, scale, fdes);
        }
        else
        {
            lcd_draw_string(60, 150, menu_items[2], color, scale, fdes);
        }

        // Third menu item: Exit
        if (self->selected == 2)
            lcd_draw_string(5, 200, "->", color, scale, fdes);
        lcd_draw_string(60, 200, menu_items[3], color, scale, fdes);
    }
    else
    {
        // Game Over screen
        lcd_draw_string(105, 20, "Game Over", color, scale, fdes);

        // Show winner or tie
        if (score->player1_score > score->player2_score)
        {
            lcd_draw_string(80, 70, "Player 1 wins!", player1->color, scale, fdes);
        }
        else if (score->player1_score < score->player2_score)
        {
            lcd_draw_string(80, 70, "Player 2 wins!", player2->color, scale, fdes);
        }
        else
        {
            lcd_draw_string(105, 70, "It's a tie!", color, scale, fdes);
        }

        // Option 1: Play Again
        if (self->selected == 0)
            lcd_draw_string(5, 170, "->", color, scale, fdes);
        lcd_draw_string(60, 170, "Play Again", color, scale, fdes);

        // Option 2: Return to Menu
        if (self->selected == 1)
            lcd_draw_string(5, 220, "->", color, scale, fdes);
        lcd_draw_string(60, 220, "Return to menu", color, scale, fdes);
    }
}
