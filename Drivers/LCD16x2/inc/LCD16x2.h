/*
 * LCD16x2.h
 *
 * This driver is writen for 4bits mode, it doesn't work in 8bit mode.
 *  Created on: Dec 1, 2020
 *      Author: Victor
 */

#ifndef LCD16X2_INC_LCD16X2_H_
#define LCD16X2_INC_LCD16X2_H_
#include "stm32f4xx_hal.h"



/*
 *  PIN Configuration structure, refer to HAL_GPIO to get values
 */

typedef struct
{
	GPIO_TypeDef * LCD_GPIO;
	GPIO_InitTypeDef *LCD_GPIO_INI;
	uint16_t D4_PIN;
	uint16_t D5_PIN;
	uint16_t D6_PIN;
	uint16_t D7_PIN;
	uint16_t EN_PIN;
	uint16_t RW_PIN;
	uint16_t RS_PIN;
	uint16_t LCD_EN_Delay;				// pulse enable delay, refer to datasheet

}LCD16x2_CfgType;

/*
 * Function prototypes
 */
void LCD_init();
void LCD_write_char(LCD16x2_CfgType *LCD16x2_CfgParam, char Data);
void LCD_write_string(LCD16x2_CfgType *LCD16x2_CfgParam, char *string);
void LCD_cmd(LCD16x2_CfgType *LCD16x2_CfgParam, uint8_t Data);
void LCD_clear(LCD16x2_CfgType *LCD16x2_CfgParam);
void LCD_set_cursor(LCD16x2_CfgType *LCD16x2_CfgParam, uint8_t column, uint8_t row);
void LCD_SL(LCD16x2_CfgType *LCD16x2_CfgParam);
void LCD_SR(LCD16x2_CfgType *LCD16x2_CfgParam);

/*
 * Instruction codes for 4bits mode (6 bits) used for initialization
 * RS y R/W bit includes
 */

#define LCD_fun_set_ini				0b000011

#define LCD_fun_set_4bits			0b000010
#define LCD_fun_set_lines_font_1	0b000010
#define LCD_fun_set_lines_font_2	0b001000 // 0b00NF00 N-> row numbers F-> font type
#define LCD_dp_off_1				0b000000
#define LCD_dp_off_2				0b001100 //0b001DCB -> D display on/off // C cursor on/off // B // Cursor position character blink
#define LCD_dp_clr_1				0b000000
#define LCD_dp_clr_2				0b000001
#define LCD_mode_set_1				0b000000
#define LCD_mode_set_2				0b000011//0b0000IS I = I/D cursor moving direction. S: Shifts entire display

/*
 *  Example of Function to configure the pins and GPIO.

void LCD16x2_Config(void){

	// LCD Cfg struct

	LCD16x2_CfgParam.LCD_GPIO = GPIOB;

	LCD16x2_CfgParam.D7_PIN = GPIO_PIN_13;
	LCD16x2_CfgParam.D6_PIN = GPIO_PIN_5;
	LCD16x2_CfgParam.D5_PIN = GPIO_PIN_4;
	LCD16x2_CfgParam.D4_PIN = GPIO_PIN_10;

	LCD16x2_CfgParam.EN_PIN = GPIO_PIN_14;
	LCD16x2_CfgParam.RS_PIN = GPIO_PIN_1;
	LCD16x2_CfgParam.RW_PIN = GPIO_PIN_15;

	LCD16x2_CfgParam.LCD_EN_Delay = 60;


	// GPIO Config

	__HAL_RCC_GPIOB_CLK_ENABLE();


	LCD_GPIO.Pin = LCD16x2_CfgParam.D7_PIN | LCD16x2_CfgParam.D6_PIN | LCD16x2_CfgParam.D5_PIN | LCD16x2_CfgParam.D4_PIN | LCD16x2_CfgParam.EN_PIN | LCD16x2_CfgParam.RS_PIN | LCD16x2_CfgParam.RW_PIN;
	LCD_GPIO.Mode = GPIO_MODE_OUTPUT_PP;
	LCD_GPIO.Pull = GPIO_NOPULL;
	LCD_GPIO.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(LCD16x2_CfgParam.LCD_GPIO, &LCD_GPIO);


}
*/
#endif /* LCD16X2_INC_LCD16X2_H_ */
