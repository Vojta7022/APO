#ifndef MENU_H
#define MENU_H

#include "lcd.h"
#include "mzapo.h"
#include "params.h"
#include "font_types.h"
#include "audio.h"
#include "score.h"
#include "led.h"

#define MENU_ITEMS sizeof(menu_items) / sizeof(menu_items[0])

// Structure to hold the selected menu item, and the last position of a knob which is used to navigate the menu
typedef struct {
    int selected;          // currently selected menu item
    uint8_t last_knob_pos;  // last position of a knob or encoder
} menu_t;

/**
 * Draws the menu on the LCD screen.
 * 
 * @param self Pointer to the menu_t instance.
 * @param color The color to use for the menu items.
 * @param game_state Pointer to the game state variable.
 * @param score Pointer to the score structure.
 * @param player1 Pointer to the first player structure.
 * @param player2 Pointer to the second player structure.
 */
void draw_menu(menu_t* self, uint16_t color, int* game_state, score_t* score, player_t* player1, player_t* player2);

/**
 * Creates a new menu_t instance.
 * 
 * @return Pointer to the newly created menu_t instance.
 */
menu_t* new_menu();

/**
 * Updates the menu based on the knob position and button press.
 * 
 * @param self Pointer to the menu_t instance.
 * @param knob The current position of the knob or encoder.
 * @param button_pressed Indicates if the button is pressed.
 * @param game_state Pointer to the game state variable.
 * @param led Pointer to the gametime_led_t instance.
 * @param score Pointer to the score structure.
 * @param player1 Pointer to the first player structure.
 * @param player2 Pointer to the second player structure.
 */
void update_menu(menu_t* self, uint8_t knob, _Bool button_pressed, int *game_state, gametime_led_t* led, score_t* score, player_t* player1, player_t* player2);
#endif /* MENU_H */
