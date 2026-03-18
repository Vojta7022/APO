#include "led.h"

int game_time = 10;

gametime_led_t* new_led() {
    gametime_led_t* self = malloc(sizeof(gametime_led_t));
    self->value = UINT32_MAX; // All LEDs turned on
    self->counter = 0; // Initialize the counter to 0
    return self;
}

void gametime_led_update(gametime_led_t* self, int* game_state) {
    self->counter++;
    
    // If time for the next LED step and there are still LEDs on
    if (self->counter >= game_time && self->value != 0) {
        self->counter = 0; // Reset the counter
        self->value = self->value >> 1; // Shift the LED value to turn off one LED
    }
    // If all LEDs are off and a full tick passed 
    else if (self->counter >= 1 && self->value == 0) {
        *game_state = GAME_OVER; // Signal game over
    }
}

void led_set(gametime_led_t* self) {
    mzapo_led_set(self->value); // Apply current LED state to hardware
}
