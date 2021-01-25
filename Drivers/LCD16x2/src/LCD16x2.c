/*
 * LCD16x2.c
 *
 *  Created on: Dec 1, 2020
 *      Author: Victor
 */
/*
 * TODO La función instruction set no envia la instrucción, activa las salidas por eso al iniciar no funciona.
 * TODO Cambiar nombre y poner delays necesarios para que el IC lo interprete.
 */
#include "main.h"
#include "LCD16x2.h"
#include "delay.h"

static void LCD_pin_set(LCD16x2_CfgType *LCD16x2_CfgParam, int8_t Instr_Code);
/*
typedef struct
{
	GPIO_TypeDef * LCD_GPIO;
	uint16_t D4_PIN;
	uint16_t D5_PIN;
	uint16_t D6_PIN;
	uint16_t D7_PIN;
	uint16_t EN_PIN;
	uint16_t RS_PIN;
	uint16_t LCD_EN_Delay;				// pulse enable delay, refer to datasheet
}LCD16x2_CfgType;
*/
/**
  * @brief  Initiation secuence for LCD
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
void LCD_write_char(LCD16x2_CfgType *LCD16x2_CfgParam, char Data){

	// Divide the 8 bit caracteres in two 2 bits
	uint8_t LowNibble, HighNibble;

	LowNibble = Data&0x0F;
	HighNibble = Data&0xF0;
	HighNibble = (HighNibble>>4);

	uint8_t address = 0b100000; //Used to complete instruction to 6bits

	LowNibble |= address;
	HighNibble |= address;

	LCD_pin_set(LCD16x2_CfgParam, HighNibble);
	HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO, LCD16x2_CfgParam->EN_PIN, 1);
	delay_us(60);
	HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO, LCD16x2_CfgParam->EN_PIN,0);
	delay_us(60);
	LCD_pin_set(LCD16x2_CfgParam, LowNibble);
	HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO, LCD16x2_CfgParam->EN_PIN, 1);
	delay_us(60);
	HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO, LCD16x2_CfgParam->EN_PIN, 0);
	delay_us(60);
}
void LCD_cmd(LCD16x2_CfgType *LCD16x2_CfgParam, uint8_t Inst){

	LCD_pin_set(LCD16x2_CfgParam, Inst);
	HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO, LCD16x2_CfgParam->EN_PIN, 1);
	delay_us(60);
	HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO, LCD16x2_CfgParam->EN_PIN,0);
	delay_us(60);
}
/**
  * @brief  Send given instruction to LCD
  * @note  Instruction macros can be modified in LCD16x2.h for any other ini configuration.
  *
  * @param  LCD16x2_CfgParam Configuration structure.
  *
  * @param Instr_Code instruction code for LCD.
  *
  */
void LCD_clear(LCD16x2_CfgType *LCD16x2_CfgParam){
	LCD_cmd(LCD16x2_CfgParam, LCD_dp_clr_1);
	LCD_cmd(LCD16x2_CfgParam, LCD_dp_clr_2);
	HAL_Delay(2);
}

void LCD_set_cursor(LCD16x2_CfgType *LCD16x2_CfgParam, uint8_t column, uint8_t row){
	// TODO idear la función para que el High nibble tenga la dirección y el low nibble tambien
	// Tengo que mandar 0 0 1   ADR ADR ADR High Nibble
	// y				0 0 ADR ADR ADR ADR Low Nibble
	// ADR empieza en va de 00 a 0F para la primera fila y de 40 a 4F para la segunda fila
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
delay_us(40);  //This operation has a delay given in the datasheet.
}
void LCD_write_string(LCD16x2_CfgType *LCD16x2_CfgParam, char *string){

	for (int i = 0; string[i]!= '\0'; i++){
		LCD_write_char(LCD16x2_CfgParam, string[i]);
	}

}

void LCD_SL(LCD16x2_CfgType *LCD16x2_CfgParam){
	//    RS RW DB7 DB6 DB5 DB4 BD3 DB2 DB1 DB0
	// 0b 0   0  0   0   0   1   1   0   *    *
	// High Nibble -> 0001
	// Low Nibble  -> 1000

	LCD_cmd(LCD16x2_CfgParam, 0x01);
	LCD_cmd(LCD16x2_CfgParam, 0x08);
	delay_us(60);

}

void LCD_SR(LCD16x2_CfgType *LCD16x2_CfgParam){
	//    RS RW DB7 DB6 DB5 DB4 BD3 DB2 DB1 DB0
	// 0b 0   0  0   0   0   1   1   1   *    *
	// High Nibble -> 0001
	// Low Nibble  -> 1100

	LCD_cmd(LCD16x2_CfgParam, 0x01);
	LCD_cmd(LCD16x2_CfgParam, 0x0C);
	delay_us(60);

}



static void LCD_pin_set(LCD16x2_CfgType *LCD16x2_CfgParam, int8_t Instr_Code){
		HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO,LCD16x2_CfgParam->RS_PIN, ((Instr_Code)&(1<<5))>>5);
		HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO,LCD16x2_CfgParam->RW_PIN, ((Instr_Code)&(1<<4))>>4);
		HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO,LCD16x2_CfgParam->D7_PIN, ((Instr_Code)&(1<<3))>>3);
		HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO,LCD16x2_CfgParam->D6_PIN, ((Instr_Code)&(1<<2))>>2);
		HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO,LCD16x2_CfgParam->D5_PIN, ((Instr_Code)&(1<<1))>>1);
		HAL_GPIO_WritePin(LCD16x2_CfgParam->LCD_GPIO,LCD16x2_CfgParam->D4_PIN, (Instr_Code)&(1));
}




