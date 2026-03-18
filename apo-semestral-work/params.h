#ifndef PARAMS_H
#define PARAMS_H

#include <stdint.h>

// Constants for the game
#define SQUARE_SIZE 10
#define LCD_WIDTH 480
#define LCD_HEIGHT 320
#define UINT4_MAX (UINT8_MAX >> 4)
// RGB color format for the LCD (5-6-5 format)
#define RGB(r, g, b) (((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3)
// Knob and button values
#define KNOB_B(val) ((u_int8_t)(*val & 0xff))
#define KNOB_G(val) ((u_int8_t)((*val >> 8) & 0xff))
#define KNOB_R(val) ((u_int8_t)((*val >> 16) & 0xff))
#define KNOB_BTN_B(val) (*val >> 24) & 0x01
#define KNOB_BTN_G(val) (*val >> 25) & 0x01
#define KNOB_BTN_R(val) (*val >> 26) & 0x01

// Directions for player movement
static const int directions[4][2] = {
    {0, -1}, // up
    {1, 0},  // right
    {0, 1},  // down
    {-1, 0}  // left
};

// Game states
// These states are used to control the flow of the game
typedef enum {
    MENU,
    GAME,
    EXIT, 
    GAME_OVER,
    EDIT_GAMETIME
} GAME_STATE;

// Game time variable
extern int game_time;

#endif /* PARAMS_H */
