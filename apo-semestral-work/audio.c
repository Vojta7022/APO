#include "audio.h"

volatile uint32_t *audiopwm_mem_base = NULL;

int audio_init(void)
{
    if (audiopwm_mem_base == NULL)
    {
        // Map physical address to virtual address space
        audiopwm_mem_base = (volatile uint32_t *)map_phys_address(AUDIOPWM_REG_BASE_PHYS, AUDIOPWM_REG_SIZE, 0);
        if (audiopwm_mem_base == NULL)
        {
            fprintf(stderr, "Failed to map audio PWM memory\n");
            return -1;
        }
    }

    // Set PWM period (controls volume and waveform resolution)
    audiopwm_mem_base[AUDIOPWM_REG_PWMPER_o / sizeof(uint32_t)] = 1024;

    // Enable the PWM by writing 1 to the control register
    audiopwm_mem_base[AUDIOPWM_REG_CR_o / sizeof(uint32_t)] = 1;

    return 0;
}

/**
 * Generates a sine wave of a given frequency and duration,
 * then plays it using PWM by writing samples to the hardware.
 */
void audio_play(unsigned int frequency, unsigned int duration_ms)
{
    // Calculate the number of samples needed for the given duration
    const unsigned int total_samples = (SAMPLE_RATE * duration_ms) / 1000;

    // Allocate memory for the audio samples
    int16_t *samples = malloc(total_samples * sizeof(int16_t));
    if (!samples)
    {
        fprintf(stderr, "Failed to allocate audio buffer\n");
        return;
    }

    // Generate sine wave samples
    for (unsigned int i = 0; i < total_samples; i++)
    {
        double t = (double)i / SAMPLE_RATE;
        samples[i] = (int16_t)(sin(2.0 * M_PI * frequency * t) * 32767);
    }

    // Record start time to manage playback timing
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // Interval between samples (in seconds)
    double interval = 1.0 / SAMPLE_RATE;

    // Playback loop: send samples at precise intervals
    for (unsigned int i = 0; i < total_samples; i++)
    {
        // Target time for this sample since start
        double target_time = i * interval;

        // Wait until the correct time to send this sample
        struct timespec now;
        double elapsed;
        do
        {
            clock_gettime(CLOCK_MONOTONIC, &now);
            elapsed = (now.tv_sec - start_time.tv_sec) +
                      (now.tv_nsec - start_time.tv_nsec) / 1e9;
        } while (elapsed < target_time);

        // Output the sample to the PWM register
        audiopwm_mem_base[AUDIOPWM_REG_PWM_o / sizeof(uint32_t)] = samples[i];
    }

    free(samples);
}
