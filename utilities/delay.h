/*
 * delay.h
 *
 *  Created on: 9 dic. 2020
 *      Author: Victor
 */

#ifndef DELAY_H_
#define DELAY_H_
#include "stm32f4xx_hal.h"
#include "main.h"

void timer_delay_init(void);
void delay_us(volatile uint16_t u16);


#endif /* DELAY_H_ */
