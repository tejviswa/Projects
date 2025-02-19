/* USER CODE BEGIN Header */
/*
 * Project Name: INDUSTRIAL CAN IOT GATEWAY
 *
 * Controller: STM32F401RBT6
 *
 * Peripherals

  		Switches--->	SW_UP: PC8 SW_DN: PC9

 		SPI2------->	SPI for MCP2515:
			 	 	 	SPI2 MOSI: PC3
                        SPI2 MISO: PC2
                        SPI2 CS  : PC4
                        SPI2 SCK : PB10

        UART6------->   UART6 for ESP8266:
                        UART6 TX: PC6
                        UART6 RX: PC7

        LCD Pins:--->   Data   : GPIOB PB0 TO PB6 AND PB8
                        Control: GPIOB PB3

        CAN Bus:--->	MCP2515 connected to CAN_H and CAN_L.
 *
 * Team Members: VISWANADHUNI TEJO PRAPULLA
 *               DUPAGUNTLA VENKATA SIVA BABA
 *               SHAPARTHI NIKHIL
 *               GADDE VIDHYADHARI
 *               KOMMINENI CHANDU MANI SAI
 *               SUNKU SRIDEVI
 *
 *
 *

  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CANSPI.h"
//#include "16x2_LCD.h"
#include "USART6.h"
#include "stdio.h"
#include "st7735.h"
//#include "img.h"
#include "fonts.h"
//#include "GFX.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
int DATA_RECIEVED;         //VARIABLE FOR CHECKING THE TASK 2
uCAN_MSG Msg;             //CAN MESSAGE STRUCTURE
unsigned int temp,node;   //VARIABLES FOR TEMPERATURE AND NODE_ID
char data[20];            //CHAR ARRAY FOR PRINTING IN LCD

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void TASK1_CAN_DataFrame(void);
void Task2_CAN_ResponseFrame(void);
void Task3__KMserver(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_USART6_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  CANSPI_Initialize();
  ST7735_Init();

 // testAll();
 // testFilledRoundRects();
 // while(1);

  ST7735_FillScreen(ST7735_BLACK);
  ST7735_WriteString(45, 30, "Welcome", Font_11x18, ST7735_WHITE, ST7735_BLACK);
  HAL_Delay(100);
  ST7735_WriteString(70, 50, "to", Font_11x18, ST7735_WHITE, ST7735_BLACK);
  HAL_Delay(100);
  ST7735_WriteString(5, 73, "Kernel Masters", Font_11x18, ST7735_WHITE, ST7735_BLACK);
  HAL_Delay(500);

  ST7735_WriteString(45, 30, "       ", Font_11x18, ST7735_WHITE, ST7735_BLACK);
   HAL_Delay(100);
   ST7735_WriteString(25, 50, "Presenting", Font_11x18, ST7735_WHITE, ST7735_BLACK);
   HAL_Delay(100);
   ST7735_WriteString(5, 73, "    you...    ", Font_11x18, ST7735_WHITE, ST7735_BLACK);
   HAL_Delay(1000);

  ST7735_FillScreenFast(ST7735_BLACK);
  //testFilledCircles(10, ST7735_RED);
 // testFastLines(ST7735_RED, ST7735_BLUE);

  ST7735_WriteString(30, 20, "Industial", Font_11x18, ST7735_WHITE, ST7735_BLACK);
  ST7735_WriteString(10, 50, "CAN - IOT", Font_16x26, ST7735_WHITE, ST7735_BLACK);
  ST7735_WriteString(43, 85, "GATEWAY", Font_11x18, ST7735_WHITE, ST7735_BLACK);
  //ST7735_WriteString(35, 30, "Industial CAN IOT GATEWAY ", Font_11x18, ST7735_WHITE, ST7735_BLACK);

 // while(1);
  HAL_Delay(200);

  ST7735_FillScreenFast(ST7735_BLACK);
  ST7735_WriteString(15, 45, "Initializing", Font_11x18, ST7735_WHITE, ST7735_BLACK);
  ST7735_WriteString(45, 75, "Wi-Fi", Font_11x18, ST7735_WHITE, ST7735_BLACK);
  Wifi_Init();
  for(int i=0;i<10;i++)
  {
	  ST7735_WriteString(100, 75, "   ", Font_11x18, ST7735_WHITE, ST7735_BLACK);
	  HAL_Delay(150);
	  ST7735_WriteString(100, 75, "...", Font_11x18, ST7735_WHITE, ST7735_BLACK);
  }

  ST7735_FillScreenFast(ST7735_BLACK);
  //ST7735_DrawImage(0, 0, 128, 160, (uint16_t *)logo);
  ST7735_WriteString(5, 5, "KERNEL MASTERS", Font_11x18, ST7735_WHITE, ST7735_BLACK);
  ST7735_WriteString(20, 25, "IOT GATEWAY", Font_11x18, ST7735_WHITE, ST7735_BLACK);
  ST7735_WriteString(5, 50, "Node1:", Font_11x18, ST7735_WHITE, ST7735_BLACK);
  ST7735_WriteString(5, 70, "Node2:", Font_11x18, ST7735_WHITE, ST7735_BLACK);




  //while(1);
  //HAL_Delay(1500);
 // Wifi_Init();
  //ST7735_FillScreen(ST7735_BLACK);
  //ST7735_WriteString(0, 30, "SELECT        NODE1:SW_UP   NODE2:SW_DN ", Font_11x18, ST7735_WHITE, ST7735_BLACK);




  /*
  KM_LCD_Init();

  KM_LCD_Write_Cmd(0x80);
  KM_LCD_Write_Str(" Industrial CAN");
  KM_LCD_Write_Cmd(0xc0);
  KM_LCD_Write_Str("  IOT GATEWAY");
  HAL_Delay(1500);
  KM_LCD_Write_Cmd(0x01);
  KM_LCD_Write_Cmd(0x80);
  KM_LCD_Write_Str("  Initializing");
  KM_LCD_Write_Cmd(0xc0);
  KM_LCD_Write_Str("     Wi-Fi");

  Wifi_Init();
  KM_LCD_Write_Cmd(0x01);
  KM_LCD_Write_Cmd(0x80);
  KM_LCD_Write_Str("SELECT :SW_UP");
  KM_LCD_Write_Cmd(0xc0);
  KM_LCD_Write_Str("        SW_DN");

*/



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	TASK1_CAN_DataFrame();     //SENDS DATA REQUEST TO NODE


	Task2_CAN_ResponseFrame();   //RECIEVES DATA FROM NODE

	if(DATA_RECIEVED)            //CHECKING IF THE DATA RECIEVED OR NOT
	{
		 Task3__KMserver();        //IF DATA RECIEVED SENDING TO KM SERVER
		 DATA_RECIEVED = 0;
	}


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SPI1_D_C_Pin|SPI1_RST_Pin|SPI1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_OUT_GPIO_Port, CS_OUT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|BUZZER_Pin
                          |RED_LED_Pin|GREEN_LED_Pin|LCD_D7_Pin|LCD_RS_Pin
                          |LCD_RW_Pin|LCD_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SPI1_D_C_Pin SPI1_RST_Pin SPI1_CS_Pin */
  GPIO_InitStruct.Pin = SPI1_D_C_Pin|SPI1_RST_Pin|SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : CS_OUT_Pin */
  GPIO_InitStruct.Pin = CS_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_D4_Pin LCD_D5_Pin LCD_D6_Pin BUZZER_Pin
                           RED_LED_Pin GREEN_LED_Pin LCD_D7_Pin LCD_RS_Pin
                           LCD_RW_Pin LCD_EN_Pin */
  GPIO_InitStruct.Pin = LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|BUZZER_Pin
                          |RED_LED_Pin|GREEN_LED_Pin|LCD_D7_Pin|LCD_RS_Pin
                          |LCD_RW_Pin|LCD_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : SW_UP_Pin SW_DN_Pin */
  GPIO_InitStruct.Pin = SW_UP_Pin|SW_DN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void TASK1_CAN_DataFrame(void)
{
	if(!(HAL_GPIO_ReadPin(SW_UP_GPIO_Port, SW_UP_Pin)))
	{
		HAL_Delay(200);
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin,1);
		HAL_Delay(100);
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin,0);


		Msg.frame.idType = 0; //for STANDARD_FRAME
		Msg.frame.id = 0X01;  //Message ID
		Msg.frame.dlc = 0;   //dlc
		Msg.frame.data0 = 0;
		CANSPI_Transmit(&Msg);
		//SW_UP = 0;
	}
	if(!(HAL_GPIO_ReadPin(SW_DN_GPIO_Port, SW_DN_Pin)))
		{
			HAL_Delay(200);
			HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin,1);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin,0);

			Msg.frame.idType = 0; //for STANDARD_FRAME
			Msg.frame.id = 0X02;  //Message ID
			Msg.frame.dlc = 0;   //dlc
			Msg.frame.data0 = 0;
			CANSPI_Transmit(&Msg);
			//SW_DN = 0;
		}

}

void Task2_CAN_ResponseFrame(void)
{
	if(CANSPI_Receive(&Msg))
	{
		temp = Msg.frame.data0;
		node = Msg.frame.id;


	//	KM_LCD_Write_Cmd(0x01);
		sprintf(data,"%d",temp);

		if(node==1)
		{
			ST7735_WriteString(70, 50,"  ", Font_11x18, ST7735_WHITE, ST7735_BLACK);
			ST7735_WriteString(70, 50,data, Font_11x18, ST7735_WHITE, ST7735_BLACK);
		}
		else if(node==2)
		{
			ST7735_WriteString(70, 70,"  ", Font_11x18, ST7735_WHITE, ST7735_BLACK);
			ST7735_WriteString(70, 70,data, Font_11x18, ST7735_WHITE, ST7735_BLACK);

		}

/*
		KM_LCD_Write_Str(data);
		KM_LCD_Write_Cmd(0x87);
		KM_LCD_Write_Data(0xdf);
*/
		DATA_RECIEVED = 1;

	//Task3__KMserver();
	}

}

void Task3__KMserver(void)
{
	Wifi_Transmit();

}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
