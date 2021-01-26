/*
 * LCD16x2.c
 *
 *  Created on: Dec 1, 2020
 *      Author: Victor
 */

#include "main.h"
#include "LCD16x2.h"
#include "delay.h"

static void LCD_pin_set(LCD16x2_CfgType *LCD16x2_CfgParam, int8_t Instr_Code);


/**
  * @brief  Initiation sequence for LCD
  *
  * @note  Instruction macros can be modified in LCD16x2.h for any other ini configuration.
  *
  * @param  LCD16x2_CfgParam Configuration structure.
  */

void LCD_init(LCD16x2_CfgType *LCD16x2_CfgParam){

	//Power on
	// WAIT 15 ms to be sure the LCD has Power on correctly
	HAL_Delay(150);
	// Ini instruction as given in datasheet
	LCD_cmd(LCD16x2_CfgParam, LCD_fun_set_ini);
	HAL_Delay(5);
	LCD_cmd(LCD16x2_CfgParam, LCD_fun_set_ini);
	delay_us(150);
	LCD_cmd(LCD16x2_CfgParam, LCD_fun_set_ini);
	LCD_cmd(LCD16x2_CfgParam, LCD_fun_set_4bits);
	LCD_cmd(LCD16x2_CfgParam, LCD_fun_set_lines_font_1);
	LCD_cmd(LCD16x2_CfgParam, LCD_fun_set_lines_font_2);
	LCD_cmd(LCD16x2_CfgParam, LCD_dp_off_1);
	LCD_cmd(LCD16x2_CfgParam, LCD_dp_off_2);
	LCD_cmd(LCD16x2_CfgParam, LCD_dp_clr_1);
	LCD_cmd(LCD16x2_CfgParam, LCD_dp_clr_2);
	LCD_cmd(LCD16x2_CfgParam, LCD_mode_set_1);
	LCD_cmd(LCD16x2_CfgParam, LCD_mode_set_2);

}

/**
  * @brief Writes a char on LCD (where the cursor is)
  *
  * @note  Split 8 bits character to two 4 bits and add the RS and RW bit.
  *
  * @param LCD16x2_CfgParam Configuration structure.
  *
  * @param Char Char to write on the LCD
  */

void LCD_write_char(LCD16x2_CfgType *LCD16x2_CfgParam, char Data){

	// Divide the 8 bit caracters in two 2 bits
	uint8_t LowNibble, HighNibble;

	LowNibble = Data&0x0F;
	HighNibble = Data&0xF0;
	HighNibble = (HighNibble>>4);

	uint8_t address = 0b100000; //Used to complete instruction to 6bits

	LowNibble |= address;
	HighNibble |= address;

	LCD_cmd(LCD16x2_CfgParam, HighNibble);
	LCD_cmd(LCD16x2_CfgParam, LowNibble);

}


/**
  * @brief Send the instruction (6 bits) to LCD.
  *
  * @note  Each instruction must be build to 4 bits mode ( RS RW D8 D7 D6 D5 )
  *
  * @param LCD16x2_CfgParam Configuration structure.
  *
  * @param Inst Instruction, some example for initialization can be found on header file.
  */

void LCD_cmd(LCD16x2_CfgType *LCD16x2_CfgParam, uint8_t Inst){

	LCD_pin_set(LCD16x2_CfgParam, Inst);
	HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO, LCD16x2_CfgParam->EN_PIN, 1);
	delay_us(LCD16x2_CfgParam->LCD_EN_Delay);
	HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO, LCD16x2_CfgParam->EN_PIN,0);
	delay_us(LCD16x2_CfgParam->LCD_EN_Delay);
}
/**
  * @brief Clear de LCD
  * @note
  *
  * @param  LCD16x2_CfgParam Configuration structure.
  *
  */
void LCD_clear(LCD16x2_CfgType *LCD16x2_CfgParam){
	LCD_cmd(LCD16x2_CfgParam, LCD_dp_clr_1);
	LCD_cmd(LCD16x2_CfgParam, LCD_dp_clr_2);
	HAL_Delay(2);
}

/**
  * @brief Set the cursor in the given coordinates
  * @note
  *
  * @param  LCD16x2_CfgParam Configuration structure.
  *
  * @param  column, Column where you want the cursor.
  *
  * @param    row, Row where you want the cursor.
  *
  */
void LCD_set_cursor(LCD16x2_CfgType *LCD16x2_CfgParam, uint8_t column, uint8_t row){

	uint8_t LowNibble, HighNibble;
	int8_t addr =0 ; // Base address for DDRAM is 0b1000 0000

if (row ==1 ){

	addr =  0x80+column-1;          // Example: Column 5 addr is 0x04 -> 0b1000 0000+0101-0001 = 0b1000 0100

	LowNibble = addr&0x0F;			// would be 0000 ADR ADR ADR ADR (0000 0100 in the example)
	HighNibble = (addr>>4);			// would be  0000 1	 ADR ADR ADR (0000 1000 in the example)
	HighNibble &= 0x0F;
	/*No need for more changes because RS and RW stay at 0*/
	LCD_cmd(LCD16x2_CfgParam, HighNibble);
	LCD_cmd(LCD16x2_CfgParam, LowNibble);
}
if (row == 2){

	addr = 0x80+0x40+column-1;			// Example: Column 5 addr is 0x44 -> 0b1000 0000 + 0b0100+0101 -0001 = 0b1100 0100

	LowNibble = addr&0x0F;				// would be 0000 ADR ADR ADR ADR (0000 0100 in the example)
	HighNibble = (addr>>4);				// would be  0000 1	 ADR ADR ADR (0000 1100 in the example)
	HighNibble &= 0x0F;					// To be sure RS and RW are 0 even with signal extension

	/*No need for more changes because RS and RW stay at 0*/
	LCD_cmd(LCD16x2_CfgParam, HighNibble);
	LCD_cmd(LCD16x2_CfgParam, LowNibble);
}
delay_us(LCD16x2_CfgParam->LCD_EN_Delay);  //This operation has a delay given in the datasheet.
}

/**
  * @brief Writes a string
  * @note
  *
  * @param  LCD16x2_CfgParam Configuration structure.
  *
  * @param  *string, address to the string.
  *
  *
  */

void LCD_write_string(LCD16x2_CfgType *LCD16x2_CfgParam, char *string){

	for (int i = 0; string[i]!= '\0'; i++){
		LCD_write_char(LCD16x2_CfgParam, string[i]);
	}

}

/**
  * @brief Shift LCD characters and cursort to left
  * @note
  *
  * @param  LCD16x2_CfgParam Configuration structure.
  *
  *
  */

void LCD_SL(LCD16x2_CfgType *LCD16x2_CfgParam){
	//    RS RW DB7 DB6 DB5 DB4 BD3 DB2 DB1 DB0
	// 0b 0   0  0   0   0   1   1   0   *    *
	// High Nibble -> 0001
	// Low Nibble  -> 1000

	LCD_cmd(LCD16x2_CfgParam, 0x01);
	LCD_cmd(LCD16x2_CfgParam, 0x08);
	delay_us(LCD16x2_CfgParam->LCD_EN_Delay);

}

/**
  * @brief Shift LCD characters and cursort to right
  * @note
  *
  * @param  LCD16x2_CfgParam Configuration structure.
  *
  *
  */

void LCD_SR(LCD16x2_CfgType *LCD16x2_CfgParam){
	//    RS RW DB7 DB6 DB5 DB4 BD3 DB2 DB1 DB0
	// 0b 0   0  0   0   0   1   1   1   *    *
	// High Nibble -> 0001
	// Low Nibble  -> 1100

	LCD_cmd(LCD16x2_CfgParam, 0x01);
	LCD_cmd(LCD16x2_CfgParam, 0x0C);
	delay_us(LCD16x2_CfgParam->LCD_EN_Delay);

}
/**
  * @brief Set the GPIO pin given a instruction code
  * @note
  *
  * @param  LCD16x2_CfgParam Configuration structure.
  *
  * @param  Instr, 6 bits instruction with 1 to indicate high and 0 to indicate low.
  *
  */


static void LCD_pin_set(LCD16x2_CfgType *LCD16x2_CfgParam, int8_t Instr){
	HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO,LCD16x2_CfgParam->RS_PIN, ((Instr)&(1<<5))>>5);
	HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO,LCD16x2_CfgParam->RW_PIN, ((Instr)&(1<<4))>>4);
	HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO,LCD16x2_CfgParam->D7_PIN, ((Instr)&(1<<3))>>3);
	HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO,LCD16x2_CfgParam->D6_PIN, ((Instr)&(1<<2))>>2);
	HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO,LCD16x2_CfgParam->D5_PIN, ((Instr)&(1<<1))>>1);
	HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO,LCD16x2_CfgParam->D4_PIN, (Instr)&(1));
}


/* @brief Configure the GPIO pins and clock given the desired configuration by the LCD16x2_CfgParam and a GPIO_InitTypeDef to implement it.
* @note
*
* @param  LCD16x2_CfgParam Configuration structure.
*
* @param  Instr, 6 bits instruction with 1 to indicate high and 0 to indicate low.
*
*/

void LCD_GPIO_cfg(LCD16x2_CfgType *LCD16x2_CfgParam){

	if(LCD16x2_CfgParam->LCD_GPIO == GPIOA)
		__HAL_RCC_GPIOA_CLK_ENABLE();
	if(LCD16x2_CfgParam->LCD_GPIO == GPIOB)
		__HAL_RCC_GPIOB_CLK_ENABLE();
	if(LCD16x2_CfgParam->LCD_GPIO == GPIOC)
		__HAL_RCC_GPIOC_CLK_ENABLE();
	if(LCD16x2_CfgParam->LCD_GPIO == GPIOD)
		__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitTypeDef LCD_GPIO;

	LCD_GPIO.Pin = LCD16x2_CfgParam->D7_PIN | LCD16x2_CfgParam->D6_PIN | LCD16x2_CfgParam->D5_PIN | LCD16x2_CfgParam->D4_PIN | LCD16x2_CfgParam->EN_PIN | LCD16x2_CfgParam->RS_PIN | LCD16x2_CfgParam->RW_PIN;
	LCD_GPIO.Mode = GPIO_MODE_OUTPUT_PP;
	LCD_GPIO.Pull = GPIO_NOPULL;
	LCD_GPIO.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(LCD16x2_CfgParam->LCD_GPIO, &LCD_GPIO);

}


