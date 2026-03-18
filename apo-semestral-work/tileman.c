/*******************************************************************
  Main function of Tilemania game for Summer2024/2025 B35APO semestral work, utilizing MicroZed MZ_APO board

  tileman.c      - main file

  (C) Copyright 2025 by Jiří Skála and Vojtěch Ponrt
      e-mail:   skalaji7@cvut.cz, ponrtvoj@cvut.cz
      license: GNU GPL v3
 *******************************************************************/

#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 500

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include <mzapo_phys.h>
#include "mzapo_regs.h"
#include "serialize_lock.h"

#include "player.h"
#include "lcd.h"
#include "mzapo.h"
#include "params.h"
#include "menu.h"
#include "led.h"
#include "score.h"
#include "audio.h"

/**
 * Setup function for the game.
 */
void setup();

/**
 * Input function to read the knob values.
 *
 * @param knobs Pointer to the variable where the knob values will be stored.
 */
void input(uint32_t *knobs);

/**
 * Update function to handle game logic and state changes.
 */
void update();

/**
 * Output function to render the game state on the LCD screen and update the LED, if necessary, play sound etc.
 *
 * @param knobs Pointer to the variable containing the knob values.
 */
void output(uint32_t *knobs);

// Global game variables
uint32_t knobs;
player_t *player1;
player_t *player2;
menu_t *menu;
int game_state;
gametime_led_t *led;
score_t *score;

unsigned char *mem_base = NULL;

void setup()
{
  mzapo_setup();
  audio_init();
  game_state = 0;

  mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  if (mem_base == NULL)
  {
    fprintf(stderr, "Failed to map SPILED memory\n");
    exit(1);
  }

  led = new_led();
  led_set(led);
  input(&knobs);
  score = new_score();

  // tiles: 1 for player1 territory, 2 for player1 path
  player1 = new_player(100, 160, RGB(255, 0, 0), KNOB_R(&knobs), 1, 1);

  // Set the initial territory for player1
  for (int i = player1->x / SQUARE_SIZE - 1; i <= player1->x / SQUARE_SIZE + 1; i++)
  {
    for (int j = player1->y / SQUARE_SIZE - 1; j <= player1->y / SQUARE_SIZE + 1; j++)
    {
      tiles[j][i] = 1;
    }
  }
  // tiles: 3 for player2 territory, 4 for player2 path
  player2 = new_player(380, 160, RGB(0, 0, 255), KNOB_G(&knobs), 2, 2);

  // Set the initial territory for player2
  for (int i = player2->x / SQUARE_SIZE - 1; i <= player2->x / SQUARE_SIZE + 1; i++)
  {
    for (int j = player2->y / SQUARE_SIZE - 1; j <= player2->y / SQUARE_SIZE + 1; j++)
    {
      tiles[j][i] = 3;
    }
  }
  menu = new_menu();
}

void input(uint32_t *knobs)
{
  *knobs = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
}

void update()
{
  switch (game_state)
  {
  case MENU:
    update_menu(menu, KNOB_G(&knobs), KNOB_BTN_G(&knobs), &game_state, led, score, player1, player2);
    break;
  case GAME:
    player_update(player1, KNOB_R(&knobs), player2);
    player_update(player2, KNOB_B(&knobs), player1);
    gametime_led_update(led, &game_state);
    break;
  case EXIT:
    exit(0);
  case GAME_OVER:
    update_menu(menu, KNOB_G(&knobs), KNOB_BTN_G(&knobs), &game_state, led, score, player1, player2);
    break;
  case EDIT_GAMETIME:
    update_menu(menu, KNOB_G(&knobs), KNOB_BTN_G(&knobs), &game_state, led, score, player1, player2);
    break;
  default:
    break;
  }
}

void output(uint32_t *knobs)
{
  lcd_clear();
  switch (game_state)
  {
  case MENU:
    draw_menu(menu, RGB(255, 255, 255), &game_state, score, player1, player2);
    break;
  case GAME:
    tiles_draw(score, player1, player2);
    player_draw(player1);
    player_draw(player2);
    led_set(led);
    score_draw(score);
    break;
  case EXIT:
    break;
  case GAME_OVER:
    draw_menu(menu, RGB(255, 255, 255), &game_state, score, player1, player2);
    score_draw(score);
    break;
  case EDIT_GAMETIME:
    draw_menu(menu, RGB(255, 255, 255), &game_state, score, player1, player2);
    break;
  default:
    break;
  }
  lcd_update();
}

/**
 * Main function of the game, runs the game loop and handles input, update, and output.
 */
int main(int argc, char *argv[])
{
  /* Serialize execution of applications */
  /* Try to acquire lock the first */
  if (serialize_lock(1) <= 0)
  {
    printf("System is occupied\n");
    if (1)
    {
      printf("Waiting\n");
      /* Wait till application holding lock releases it or exits */
      serialize_lock(0);
    }
  }

  setup();

  // Play a melody at the start
  audio_play(440, 250);
  audio_play(880, 250);
  audio_play(1760, 1000);

  // Main game loop
  while (1)
  {
    input(&knobs);
    update();
    output(&knobs);

    // Delay to control the game speed
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 250000000; // 250ms
    nanosleep(&ts, NULL);

    // If all knobs are pressed, exit the game
    if (KNOB_BTN_G(&knobs) && KNOB_BTN_R(&knobs) && KNOB_BTN_B(&knobs))
    {
      printf("Shutting down\n");
      break;
    }
  }
  /* Release the lock */
  serialize_unlock();
  return 0;
}
