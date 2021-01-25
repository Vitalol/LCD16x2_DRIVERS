
#include "main.h"
#include "LCD16x2.h"
#include "delay.h"
#include "stdio.h"

void SystemClock_Config(void);
void LCD16x2_Config(void);

LCD16x2_CfgType LCD16x2_CfgParam;
GPIO_InitTypeDef LCD_GPIO;
void Error_Handler(void);

int main(void)
{

  HAL_Init();
  timer_delay_init();
  LCD16x2_Config();
  LCD_init(&LCD16x2_CfgParam);
  LCD_clear(&LCD16x2_CfgParam);



  LCD_write_string(&LCD16x2_CfgParam,"Testing");
  LCD_set_cursor(&LCD16x2_CfgParam, 6, 2);
  LCD_write_string(&LCD16x2_CfgParam,"Test");

  while (1)
  {


	  LCD_SR(&LCD16x2_CfgParam);
	  HAL_Delay(500);
	  LCD_SR(&LCD16x2_CfgParam);
	  HAL_Delay(500);
	  LCD_SR(&LCD16x2_CfgParam);
	  HAL_Delay(500);
	  LCD_SR(&LCD16x2_CfgParam);
	  HAL_Delay(500);

	  LCD_SL(&LCD16x2_CfgParam);
	  HAL_Delay(500);
	  LCD_SL(&LCD16x2_CfgParam);
	  HAL_Delay(500);
	  LCD_SL(&LCD16x2_CfgParam);
	  HAL_Delay(500);
	  LCD_SL(&LCD16x2_CfgParam);
	  HAL_Delay(500);

	  }

}



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
void led_config(void){
	GPIO_InitTypeDef LED;

	LED.Mode = GPIO_MODE_OUTPUT_PP;
	LED.Pin = GPIO_PIN_5;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &LED);
}

void Error_Handler(void){

}
