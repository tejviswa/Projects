/*
 * 16x2_LCD.c
 *
 *  Created on: Dec 29, 2024
 *      Author: LENOVO
 */
#include "main.h"
#include "16x2_LCD.h"
#include "stm32f4xx_hal.h"


void KM_LCD_Init(void)
{
	HAL_Delay(20);
	KM_LCD_Write_Cmd(0x33);  // Initialize
	HAL_Delay(5);
	KM_LCD_Write_Cmd(0x32);  // 4-bit mode
	KM_LCD_Write_Cmd(0x0C);  // Display on, cursor off
	KM_LCD_Write_Cmd(0x01);  // Clear display

}


void KM_Write_High_Nibble(unsigned char data)
{
    // Shift the high nibble to the lower nibble position
    data >>= 4;

    // Write each bit of the high nibble to the corresponding LCD pin
    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    // Pulse the Enable pin to signal the LCD to read the data
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(10); // Short delay to ensure the LCD processes the data
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
}

void KM_Write_Low_Nibble(unsigned char data)
{
	//masking the higher nibble
	data &= 0x0F;

	 // Write each bit of the lower nibble to the corresponding LCD pin
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

	// Pulse the Enable pin to signal the LCD to read the data
	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
	HAL_Delay(10); // Short delay to ensure the LCD processes the data
	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);

}


void KM_LCD_Write_Cmd(unsigned char data)
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET);
	KM_Write_High_Nibble(data);
	KM_Write_Low_Nibble(data);
}

void KM_LCD_Write_Data(unsigned char data)
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET);
	KM_Write_High_Nibble(data);
	KM_Write_Low_Nibble(data);
}

void KM_LCD_Write_Str(char *data)
{
	while(*data)
	{
		KM_LCD_Write_Data(*data++);

	}
}

/*============CUSTOM CHARACTER============*/
void custom_degree(void)
{
	//char arr[] = {0x0E, 0x0A, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00};
	char arr[] = {0x00,0x00,0x0A,0x00,0x11,0x0E,0x00,0x00};
	KM_LCD_Write_Cmd(0x40);
	for(int i=0;i<8;i++)
	{
		KM_LCD_Write_Data(arr[i]);
	}
	KM_LCD_Write_Data(0);

}
