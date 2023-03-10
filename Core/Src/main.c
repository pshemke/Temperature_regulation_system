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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bmp2_config.h"
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

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//<! Global variables
float ref = 26.00f;
float temp = 0.0f;
char input_cmd[6];

//<! State of regulator
int is_cooling = 0;
int is_heating = 0;

//<! Parameters
#define MAX_RANGE 60.00f
#define MIN_RANGE 26.00f
#define HEATING_THRESHOLD 0.02f
#define COOLING_THRESHOLD 0.10f

//<! Values based on parameters
#define RANGE (MAX_RANGE - MIN_RANGE)
#define ONE_PERCENT_ACC (RANGE * 0.01f)
#define FIVE_PERCENT_ACC (RANGE * 0.05f)

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief  Regulate object based on measurement from sensor.
  * 		Heating is turned on with high state.
  * 		Cooling is turned on with low state.
  * @param  None
  * @retval None
  */
void toggle_regulation(){
	if(temp < (ref - HEATING_THRESHOLD)){
		HAL_GPIO_WritePin(HEATER_GPIO_Port,HEATER_Pin,1);
		HAL_GPIO_WritePin(COOLING_GPIO_Port,COOLING_Pin,1);
		}else if( temp > (ref + COOLING_THRESHOLD)){
		HAL_GPIO_WritePin(HEATER_GPIO_Port,HEATER_Pin,0);
		HAL_GPIO_WritePin(COOLING_GPIO_Port,COOLING_Pin,0);
		}else{
		HAL_GPIO_WritePin(HEATER_GPIO_Port,HEATER_Pin,0);
		HAL_GPIO_WritePin(COOLING_GPIO_Port,COOLING_Pin,1);
		}
	is_heating = HAL_GPIO_ReadPin(HEATER_GPIO_Port,HEATER_Pin);
	is_cooling = !(HAL_GPIO_ReadPin(COOLING_GPIO_Port,COOLING_Pin));
}

/**
  * @brief  Toggle LD2 if measurement is in 1 percent range from reference value.
  * @param  None
  * @retval None
  */
void check_one_percent_range(){
	if(temp < (ref + ONE_PERCENT_ACC) && temp > (ref - ONE_PERCENT_ACC)){
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,1);
		}else{
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,0);
		}
}

/**
  * @brief  Toggle LD3 if measurement is in 5 percent range from reference value.
  * @param  None
  * @retval None
  */
void check_five_percent_range(){
	if(temp < (ref + FIVE_PERCENT_ACC) && temp > (ref - FIVE_PERCENT_ACC)){
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin,1);
		}else{
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin,0);
		}
}

/**
  * @brief  Rx Transfer completed callback. Changes reference value through USART command.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	if(huart->Instance == USART3)
	{
		if(input_cmd[2] == '.' && input_cmd[5] == 'C')
		{
			sscanf (input_cmd,"%fC", &ref);
			if(ref < MIN_RANGE){ref = MIN_RANGE;}
			else if(ref > MAX_RANGE){ref = MAX_RANGE;}
		}
		HAL_UART_Receive_IT(&huart3, (uint8_t*)input_cmd, 6);
	}
}

/**
  * @brief  Display of reference value and regulator state on User button press.
  * @param  specified Pin.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == USER_Btn_Pin)
	{
		uint8_t message[] = "Ref: 00000[degC] Heating: 0 Cooling: 0 \r\n ";
		uint16_t mess_len;
		mess_len = sprintf((char *)message, "Ref: %2.2f degC Heating: %d Cooling: %d \r\n ",
				ref,is_heating,is_cooling);
	    HAL_UART_Transmit(&huart3, message, mess_len, 100);
	}
}

/**
  * @brief  TIM elapsed period callback.
  * 		TIM5 reads sensor value, changes state of regulator,
  * 		shows if current value is in set range of reference value.
  * 		TIM2 sends current measure value through USART.
  * @param  htim TIM handle.
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim5) {
	temp = BMP2_ReadTemperature_degC(&bmp2dev_1);
	toggle_regulation();
	check_one_percent_range();
	check_five_percent_range();

  }else if(htim == &htim2){
	  uint8_t message[] = "Temp: 00000[degC] \r\n ";
	  uint16_t mess_len;
	  mess_len = sprintf((char *)message, "Temp: %2.2f degC \r\n ",
	  				temp);
	  HAL_UART_Transmit(&huart3, message, mess_len, 100);
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
  MX_USART3_UART_Init();
  MX_TIM5_Init();
  MX_TIM2_Init();
  MX_SPI4_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart3, (uint8_t*)input_cmd, 6);

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
