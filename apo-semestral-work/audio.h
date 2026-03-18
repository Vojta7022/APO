#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include <mzapo_regs.h>
#include <mzapo_phys.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#define SAMPLE_RATE 8000 // 8 kHz sample rate

/**
 * Initializes the buzzer for playing tones by mapping the memory region.
 */
int audio_init(void);

/**
 * Plays a tone at the specified frequency for the specified duration.
 *
 * @param frequency The frequency of the tone in Hz.
 * @param duration_ms The duration to play the tone in milliseconds.
 */
void audio_play(unsigned int frequency, unsigned int duration_ms);

#endif /* AUDIO_H */
