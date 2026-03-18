#ifndef LCD_H
#define LCD_H

#include "params.h"
#include "mzapo_regs.h"
#include "mzapo_parlcd.h"
#include "mzapo.h"
#include <stdbool.h>
#include <stdlib.h>
#include "font_types.h"
#include "score.h"

#define CHAR_HEIGHT 16
#define CHAR_WIDTH 14

// Structure to hold the coordinates of a point
typedef struct
{
    int x, y;
} point_t;

// Player and score structures are defined in player.h and score.h
typedef struct player player_t;
typedef struct score score_t;

// Tiles array which represents the game area on the LCD screen
extern unsigned short tiles[LCD_HEIGHT / SQUARE_SIZE][LCD_WIDTH / SQUARE_SIZE];

/**
 * Draws a rectangle on the LCD screen.
 *
 * @param sx The x-coordinate of the top-left corner of the rectangle.
 * @param sy The y-coordinate of the top-left corner of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param color The color of the rectangle.
 *
 */
void lcd_draw_rectangle(unsigned sx, unsigned sy, unsigned w, unsigned h, unsigned short color);

/**
 * Updates the LCD screen with the current framebuffer.
 */
void lcd_update();

/**
 * Clears the LCD screen.
 */
void lcd_clear();

/**
 * Updates the tiles on the LCD screen based on the players' positions and game state.
 *
 * @param score Pointer to the score structure.
 * @param player1 Pointer to the first player structure.
 * @param player2 Pointer to the second player structure.
 */
void tiles_draw(score_t *score, player_t *player1, player_t *player2);

/**
 * Fills the captured area of a player on the LCD screen using a flood-fill algorithm.
 *
 * @param player Pointer to the player structure.
 */
void fill_captured_area(player_t *player);

/**
 * Draws a pixel on the LCD screen.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 */
void lcd_draw_pixel(int x, int y, unsigned short color);

/**
 * Draws a character on the LCD screen.
 *
 * @param x The x-coordinate of the character.
 * @param y The y-coordinate of the character.
 * @param ch The character to draw.
 * @param color The color of the character.
 * @param scale The scale factor for the character.
 * @param fdes Pointer to the font descriptor structure.
 */
void lcd_draw_char(int x, int y, char ch, unsigned short color, int scale, font_descriptor_t *fdes);

/**
 * Draws a square on the LCD screen.
 *
 * @param x The x-coordinate of the square.
 * @param y The y-coordinate of the square.
 * @param scale The scale factor for the square.
 * @param color The color of the square.
 */
void lcd_draw_pixel_big(int x, int y, int scale, unsigned short color);

/**
 * Returns the width of a character based on the font descriptor.
 *
 * @param ch The character to measure.
 * @param fdes Pointer to the font descriptor structure.
 * @return The width of the character.
 */
int char_width(int ch, font_descriptor_t *fdes);

/**
 * Draws a string on the LCD screen.
 *
 * @param x The x-coordinate of the string.
 * @param y The y-coordinate of the string.
 * @param str The string to draw.
 * @param color The color of the string.
 * @param scale The scale factor for the string.
 * @param fdes Pointer to the font descriptor structure.
 */
void lcd_draw_string(int x, int y, const char *str, unsigned short color, int scale, font_descriptor_t *fdes);

/**
 * Sets all the tiles in the game area to zero.
 */
void dump_tiles();

#endif /* LCD_H */
