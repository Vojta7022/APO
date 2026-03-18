#ifndef MZAPO_H
#define MZAPO_H

#define REG16(addr) *(unsigned short*)(addr)
#define LCD(x, y) REG16(0xffe00000 + (x) * 2 + (y) * 2 * 480)

/**
 * Setup function for the MicroZed board. 
 * It maps the SPILED and PARLCD registers to memory.
 */
void mzapo_setup();

/**
 * Updates the LCD screen with the given framebuffer.
 * 
 * @param fb Pointer to the framebuffer data.
 */
void mzapo_lcd_update(unsigned short *fb);

/**
 * Sets the LED line values.
 * 
 * @param values The LED values to set.
 */
void mzapo_led_set(unsigned int values);

#endif /* MZAPO_H */
