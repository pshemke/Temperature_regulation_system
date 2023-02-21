/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bmp2_config.h"
#include "pid2dof_config.h"
#include "bh1750_config.h"
#include "led_config.h"
#include "serial_api.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define GET_TIME_MS()  1000ul*((uint64_t)time_s)+\
                       (uint64_t)__HAL_TIM_GET_COUNTER(TIMESTAMP_TIM)/1000ul
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

float ref = 10.08f;
float temp = 0.0f;
int akcja = 0;
char wiadomosc[6];
uint8_t komunikat1[] = "Rref: 00000[degC] \r\n ";
uint16_t dl_kom;
uint8_t komunikat2[] = "Temp: 00000[degC] \r\n ";
uint16_t dl_kom2;

//<! Parameters
const float max_range = 60.00f;
const float min_range = 26.00f;
const float accuracy = 0.05f;
const float deviation_threshold = 0.2f;

//<! Values based on parameters
//float range = max_range - min_range;
float range = 34.00f;
//float deviation = range * accuracy;
float deviation = 1.7f;
//float reg_trigger = deviation * deviation_threshold;
float reg_trigger = 0.34f;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
//Test of UART and output pins
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	if(huart->Instance == USART3)
	{
		if(wiadomosc[2] == '.' && wiadomosc[5] == 'C')
		{
			sscanf (wiadomosc,"%fC", &ref);
		}
		HAL_UART_Receive_IT(&huart3, (uint8_t*)wiadomosc, 6);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == USER_Btn_Pin)
	{
		//"Temp: 00000[degC] \r\n "
		//"Yr: 00000 \r\n Y: 00000 \r\n RED: 00 %, Green: 00 %, Blue: 00 % \r\n";
		//"Yr: %f lx \r\n Y: %f lx \r\n RED: %d , Green: %d , Blue: %d \r\n Sygnal sterujacy: %f \r\n \r\n"
		dl_kom = sprintf((char *)komunikat1, "Ref: %2.2f degC \r\n ",
				ref);
	    HAL_UART_Transmit(&huart3, komunikat1, dl_kom, 100);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim5) {
	temp = BMP2_ReadTemperature_degC(&bmp2dev_1);

  }else if(htim == &htim2){
	  dl_kom2 = sprintf((char *)komunikat2, "Temp: %2.2f degC \r\n ",
	  				temp);
	  HAL_UART_Transmit(&huart3, komunikat2, dl_kom2, 100);
  }
}

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
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_TIM6_Init();
  MX_TIM5_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_SPI4_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart3, (uint8_t*)wiadomosc, 6);

  BMP2_Init(&bmp2dev_1);
  HAL_TIM_Base_Start_IT(&htim5);
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_GPIO_WritePin(HEATER_GPIO_Port,HEATER_Pin,0);
  HAL_GPIO_WritePin(COOLING_GPIO_Port,COOLING_Pin,1);
  while (1)
  {
    /* USER CODE END WHILE */

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
  RCC_OscInitStruct.PLL.PLLQ = 3;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

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
