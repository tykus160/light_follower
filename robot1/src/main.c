/*
 * main.c
 *
 *  Created on: 4 lut 2016
 *      Author: tykus
 */

#include "main.h"

const int DELAY = 50;

/**
 * Switch robot on or off
 */
ISR(INT0_vect)
{
    on = ~on;
}

/**
 * Main loop function
 */
int main(void)
{
    initialize();

    char oldDir = DIRECTION_NONE;
    char newDir = DIRECTION_NONE;

    while (TRUE)
    {
        if (on)
        {
            oldDir = newDir;
            newDir = checkPhototransistors(oldDir);
            LED_PORT = newDir & 0x07;
            setDirection(oldDir, newDir);
        }
        else
        {
            oldDir = DIRECTION_NONE;
            newDir = DIRECTION_NONE;
            CLEAR_BITMASK(LED_PORT, 0x07);     // Leds turn off
            ENGINE_PORT = 0x33;
        }
        _delay_ms(DELAY);
    }
}
