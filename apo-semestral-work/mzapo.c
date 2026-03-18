#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include "mzapo.h"
#include "mzapo_regs.h"

void *mzapo_map(unsigned address)
{
    int fd = open("/dev/mem", O_RDWR);
    if (fd == -1)
    {
        fprintf(stderr, "Failed to open /dev/mem");
    }
    void *ptr = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, address);
    if (ptr == MAP_FAILED)
    {
        return NULL;
    }
    return ptr;
}

// Global pointers for SPILED and PARLCD registers
void *mzapo_spiled;
void *mzapo_parlcd;

void mzapo_setup()
{
    // Map SPI LED register block
    mzapo_spiled = mzapo_map(SPILED_REG_BASE_PHYS);
    // Map PARLCD register block
    mzapo_parlcd = mzapo_map(PARLCD_REG_BASE_PHYS);
}

void mzapo_lcd_update(unsigned short *fb)
{
    // Send command to start memory write to LCD
    *(volatile unsigned short *)(mzapo_parlcd + PARLCD_REG_CMD_o) = 0x2c;

    // Write 480x320 pixels (each 16-bit) to LCD data register
    for (unsigned i = 0; i < 480 * 320; i++)
    {
        *(volatile unsigned short *)(mzapo_parlcd + PARLCD_REG_DATA_o) = *fb++;
    }
}

void mzapo_led_set(unsigned int values)
{
    *(volatile unsigned *)(mzapo_spiled + SPILED_REG_LED_LINE_o) = values;
}
