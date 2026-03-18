#ifndef LED_H
#define LED_H

#include "mzapo.h"
#include "params.h"
#include <stdlib.h>

// Structure to hold the LED value and counter, used for when the LED should be turned off/
typedef struct {
    unsigned int value;
    int counter;
} gametime_led_t;


/**
 * Creates a new gametime_led_t instance.
 */
gametime_led_t* new_led();

/**
 * Updates the LED value and counter based on the game state and time.
 *
 * @param self Pointer to the gametime_led_t instance.
 * @param game_state Pointer to the game state variable.
 */
void gametime_led_update(gametime_led_t* self, int* game_state);

/**
 * Sets the LED state.
 *
 * @param self Pointer to the gametime_led_t instance.
 */
void led_set(gametime_led_t* self);

#endif /* LED_H */
