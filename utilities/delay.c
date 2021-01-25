/*
 * delay.c
 *
 *  Created on: 9 dic. 2020
 *      Author: Victor
 */

#include "delay.h"
TIM_HandleTypeDef HTIMx;
uint32_t gu32_ticks = 0;

void timer_delay_init(void){

	uint32_t gu32_ticks = (HAL_RCC_GetHCLKFreq() / 1000000);

	HTIMx.Instance = TIM6;
	HTIMx.Init.CounterMode = TIM_COUNTERMODE_UP;
	HTIMx.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HTIMx.Init.Prescaler = gu32_ticks-1;
	HTIMx.Init.Period = 0xFFFF;

	__HAL_RCC_TIM6_CLK_ENABLE();

	if (HAL_TIM_Base_Init(&HTIMx)!=HAL_OK){
		Error_Handler();
}
	HAL_TIM_Base_Start(&HTIMx);
}

void delay_us(volatile uint16_t u16){

	HTIMx.Instance->CNT = 0;
	while(HTIMx.Instance->CNT<=u16){
	}

}
