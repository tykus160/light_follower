/*
 * utils.c
 *
 *  Created on: 12 lut 2016
 *      Author: tykus
 */

#include "main.h"

/**
 * Configure ADC.
 * channel - channel number for ADC (0-7)
 */
void adcConfig(char channel)
{
    // napiecie odniesienia = Vcc
    // kanal w zakresie od 0 do 7
    ADMUX = (1 << REFS0) | (channel & 0x07);
    // preskaler = 8
    ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
}

/**
 * Get value from ADC (on channel specified earlier in adcConfig).
 * return result of measure
 */
uint16_t adcMeasure(void)
{
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
    {   // oczekiwanie na koniec konwersji
    }
    return ADC;
}

/**
 * Try to set actual
 */
char direction(uint16_t measures[], char currentState)
{
    uint8_t i;
    uint8_t maxVal1 = 0;
    uint8_t maxVal2 = 0;
    for (i = 1; i < NUM_OF_PHOTOTRANSISTORS; ++i)
    {
        if (measures[i] > measures[maxVal1])
        {
            maxVal2 = maxVal1;
            maxVal1 = i;
        }
        else if (measures[i] > measures[maxVal2])
        {
            maxVal2 = i;
        }
    }

    char result = currentState;
    if ((maxVal1 == PHOTOTRANSISTOR_LP_CH && maxVal2 == PHOTOTRANSISTOR_PP_CH)
            || (maxVal2 == PHOTOTRANSISTOR_LP_CH
                    && maxVal1 == PHOTOTRANSISTOR_PP_CH))
    {
        result = DIRECTION_FORWARD;
    }
    else if ((maxVal1 == PHOTOTRANSISTOR_LT_CH
            && maxVal2 == PHOTOTRANSISTOR_PT_CH)
            || (maxVal2 == PHOTOTRANSISTOR_LT_CH
                    && maxVal1 == PHOTOTRANSISTOR_PT_CH))
    {
        result = DIRECTION_BACKWARD;
    }
    else if ((maxVal1 == PHOTOTRANSISTOR_LP_CH
            && maxVal2 == PHOTOTRANSISTOR_LT_CH)
            || (maxVal2 == PHOTOTRANSISTOR_LP_CH
                    && maxVal1 == PHOTOTRANSISTOR_LT_CH))
    {
        result = DIRECTION_LEFT;
    }
    else if ((maxVal1 == PHOTOTRANSISTOR_PT_CH
            && maxVal2 == PHOTOTRANSISTOR_PP_CH)
            || (maxVal2 == PHOTOTRANSISTOR_PT_CH
                    && maxVal1 == PHOTOTRANSISTOR_PP_CH))
    {
        result = DIRECTION_RIGHT;
    }

    return result;
}

/**
 * First initialization.
 * By default:
 * - robot is turned off
 * - engines are turned off
 * - LEDs are turned off
 * Also interruption is set.
 */
void initialize(void)
{
    on = FALSE;
    SET_BITMASK(LED_DDR, 0x07);
    SET_BITMASK(LED_PORT, 0x00);     // Leds turn off
    SET_BITMASK(ENGINE_DDR, 0xFF);   // Engines turn off
    SET_BITMASK(ENGINE_PORT, 0x33); // (for NPN - ON is 1, for PNP - ON is 0)

    SET_BIT(GICR, INT0);
    SET_BIT(MCUCR, ISC00);
    SET_BIT(MCUCR, ISC01);  // ISC00 i ISC01 - reakcja na zbocze narastające
    sei();
}

/**
 * Check states of all phototransistors.
 */
char checkPhototransistors(char state)
{
    uint16_t measures[NUM_OF_PHOTOTRANSISTORS];
    adcConfig(PHOTOTRANSISTOR_LP_CH);
    measures[0] = adcMeasure();
    adcConfig(PHOTOTRANSISTOR_LT_CH);
    measures[1] = adcMeasure();
    adcConfig(PHOTOTRANSISTOR_PP_CH);
    measures[2] = adcMeasure();
    adcConfig(PHOTOTRANSISTOR_PT_CH);
    measures[3] = adcMeasure();

    return direction(measures, state);
}

/**
 * Change direction
 */
void setDirection(char actualDirection, char measureResult)
{
    if (actualDirection != measureResult)
    {
        ENGINE_PORT = 0x33;
        _delay_ms(10);
        switch (measureResult)
        {
        case DIRECTION_FORWARD:
            ENGINE_PORT = 0xAA;
            break;

        case DIRECTION_BACKWARD:
            ENGINE_PORT = 0x55;
            break;

        case DIRECTION_LEFT:
            ENGINE_PORT = 0xA5;
            break;

        case DIRECTION_RIGHT:
            ENGINE_PORT = 0x5A;
            break;

        default:
            ENGINE_PORT = 0x33;
            break;
        }
    }
}

