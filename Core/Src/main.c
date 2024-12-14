/* USER CODE BEGIN Header */
/**
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
#include "eth.h"
#include "spi.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"
#include "app_display.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TFTAgent.h"
#include "GeneralMath.h"
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

/* USER CODE BEGIN PV */
uint8_t pointsCount = 0;

uint16_t localX = 0;
uint16_t localY = 0;

uint16_t scaledXToScreen = 0;
uint16_t scaledYToScreen = 0;

uint16_t xArray[arraySize] = {0};
uint16_t yArray[arraySize] = {0};

uint32_t displayRefreshCycle = 0;// 50Hz
uint32_t preRead = 0;
uint32_t postRead = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_SPI2_Init();
  MX_DISPLAY_Init();
  /* USER CODE BEGIN 2 */
//  ILI9341_Object_t pObj = {0};
//  ILI9341_InitParams_t pInitParams = {0};
//  ILI9341_Init(&pObj, &pInitParams);  // Initialize the ILI9341 display
//
//  ILI9341_DisplayOn(&pObj);
  LCD_Init();
//  ILI9341_SetCursor(&pObj, 20, 20);
//  ILI9341_SetPixel(&pObj, 20, 20, BLACK);

  prepareNewBufferForDisplay();
//  LCD_SetCursor(0, 0);
  LCD_SetWindows(0, 0, 240, 320);
  writeNextBufferToDisplay();
  XPT2046_GetTouchPosition(&localX, &localY);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  preRead = HAL_GetTick();
	  XPT2046_GetTouchPosition(&localX, &localY);
	  postRead = HAL_GetTick();
	  if ((localX == 2047) && (localY == 0) )
	  {
		  pointsCount = 0;
	  }
	  else
	  {

		  scaledXToScreen = (uint16_t)( (float)localX * 320.0/(1950.0 -200.0) - 320.0 * 200.0/(1950.0 - 200.0) );  //200 - 1950
		  scaledYToScreen = (uint16_t)( (float)localY * 240.0/(1900.0 -130.0) - 240.0 * 130.0/(1900.0 - 130.0) );//130 - 1900
		  if (pointsCount >= 1)
		  {
			  if ((scaledXToScreen == xArray[pointsCount - 1]) && ( scaledYToScreen == yArray[pointsCount - 1]) )
			  {

			  }
			  else
			  {
				  updatePixelInBuffer(scaledXToScreen, scaledYToScreen, BLUE);
				  xArray[pointsCount] = scaledXToScreen;
				  yArray[pointsCount] = scaledYToScreen;
				  pointsCount++;
			  }
		  }
		  else if (pointsCount == 0)
		  {
			  updatePixelInBuffer(scaledXToScreen, scaledYToScreen, BLUE);
			  xArray[pointsCount] = scaledXToScreen;
			  yArray[pointsCount] = scaledYToScreen;
			  pointsCount++;
		  }
	  }
	  if (HAL_GetTick() - displayRefreshCycle >= 50)
	  {
		  displayRefreshCycle = HAL_GetTick();
		  if (pointsCount == arraySize)
		  {
			  for (uint8_t i = 0; i < arraySize - 1 ; i++)
			  {



				  LCD_DrawLine(xArray[i], yArray[i],xArray[i + 1], yArray[i + 1]);
				  if (i == 0)
				  {
					  updatePixelInBuffer(xArray[i], yArray[i], GREEN);
				  }
				  else if (i == arraySize - 2)
				  {
					  updatePixelInBuffer(xArray[i], yArray[i], RED);
				  }
				  else
				  {
					  updatePixelInBuffer(xArray[i], yArray[i], BLUE);
				  }

			  }
			  xArray[0] = xArray[arraySize - 1];
			  yArray[0] = yArray[arraySize - 1];
			  pointsCount = 1;
		  }
		  writeNextBufferToDisplay();

	  }
    /* USER CODE END WHILE */

//  MX_DISPLAY_Process();
    /* USER CODE BEGIN 3 */
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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
