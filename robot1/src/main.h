/*
 * main.h
 *
 *  Created on: 6 lut 2016
 *      Author: tykus
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define POTENTIOMETER_CH 4
#define POTENTIOMETER_PORT PORTA
#define POTENTIOMETER_DDR DDRA

#define PHOTOTRANSISTOR_DDR DDRA
#define PHOTOTRANSISTOR_PORT PORTA
#define PHOTOTRANSISTOR_LT_CH 3
#define PHOTOTRANSISTOR_PT_CH 2
#define PHOTOTRANSISTOR_LP_CH 1
#define PHOTOTRANSISTOR_PP_CH 0
#define NUM_OF_PHOTOTRANSISTORS 4

#define ENGINE_DDR DDRC
#define ENGINE_PORT PORTC
#define ENGINE_PH1_CH 0
#define ENGINE_PH2_CH 1
#define ENGINE_PL1_CH 2
#define ENGINE_PL2_CH 3
#define ENGINE_LH1_CH 4
#define ENGINE_LH2_CH 5
#define ENGINE_LL1_CH 6
#define ENGINE_LL2_CH 7

#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED1_CH 0
#define LED2_CH 1
#define LED3_CH 2

#define PRZ_DDR DDRD
#define PRZ_PORT PORTD
#define PRZ_CH 2

#define SET_BIT(ADDRESS,BIT) (ADDRESS |= (1 << BIT))
#define CLEAR_BIT(ADDRESS,BIT) (ADDRESS &= ~(1 << BIT))
#define FLIP_BIT(ADDRESS,BIT) (ADDRESS ^= (1 << BIT))
#define CHECK_BIT(ADDRESS,BIT) (ADDRESS & (1 << BIT))

#define SET_BITMASK(x,y) (x |= (y))
#define CLEAR_BITMASK(x,y) (x &= (~y))
#define FLIP_BITMASK(x,y) (x ^= (y))
#define CHECK_BITMASK(x,y) (x & (y))

#define DIRECTION_NONE 0
#define DIRECTION_FORWARD 1
#define DIRECTION_BACKWARD 2
#define DIRECTION_LEFT 3
#define DIRECTION_RIGHT 4

void adcConfig(char channel);
uint16_t adcMeasure(void);
char direction(uint16_t measures[], char currentState);

volatile uint8_t on;

void initialize(void);
char checkPhototransistors(char state);
void setDirection(char actualDirection, char measureResult);

#endif /* MAIN_H_ */
