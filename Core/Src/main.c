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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pid2dof_config.h"
#include "bh1750_config.h"
#include "led_config.h"
#include "serial_api.h"
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

//<! Time stamp
unsigned int time_s = 0;    // [s]

//<! Reference illuminance (reference signal)
float light_ref = 1000.0f;  // [lx]

//<! Measured illuminance (measurement signal)
float light_meas = 0.0f;    // [lx]

//<! LED PWM duty cycle (control signal)
float light_ctrl = 0.0f;    // [%]

//<! Serial port API Rx command buffer
Serial_API_Buffer rx_cmd;

//<! System variables (available via Serial API)
SysVars_TypeDef sys_vars[] = {
  { .val = &light_ref,       .var_id = {'R','E','F'}, .var_name = "Reference",      .var_unit = "\"lx\"" },
  { .val = &light_meas,      .var_id = {'R','D','O'}, .var_name = "Measurement",    .var_unit = "\"lx\"" },
  { .val = &light_ctrl,      .var_id = {'P','W','M'}, .var_name = "PWM Duty cycle", .var_unit = "\"\%\"" },
  { .val = &(hlight_pid.Kp), .var_id = {'C','K','P'}, .var_name = "PID Kp Gain",    .var_unit = "null" },
  { .val = &(hlight_pid.Ki), .var_id = {'C','K','I'}, .var_name = "PID Ki Gain",    .var_unit = "null" },
  { .val = &(hlight_pid.Kd), .var_id = {'C','K','D'}, .var_name = "PID Kd Gain",    .var_unit = "null" }
};

const unsigned int SYS_VARS_LEN = sizeof(sys_vars)/sizeof(SysVars_TypeDef);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  //<! Main measurement and control loop
  if(htim == CONTROL_TIM)
  {
    //<! Read measurement result
    light_meas = BH1750_ReadIlluminance_lux(&hlight_sensor);

    //<! Compute control signal
    light_ctrl = PID2DOF_GetOutput(&hlight_pid, light_ref, light_meas);

    //<! Write control output
    LED_PWM_WriteDuty(&hlight_soruce, light_ctrl);
  }

  else if(htim == TIMESTAMP_TIM)
  {
    time_s += 1000;
  }
}

/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  //<! Serial port API
  if(huart == CONTROL_UART)
  {
    //<! Parse control command
    if(Serial_API_IsCmdSet(&rx_cmd))
    {
      // Write system variable
      Serial_API_SetSysVar(&rx_cmd, sys_vars, SYS_VARS_LEN);
    }
    else if(Serial_API_IsCmdGet(&rx_cmd))
    {
      // Read system variable
      Serial_API_GetSysVar(&rx_cmd, sys_vars, SYS_VARS_LEN, GET_TIME_MS());
    }

    //<! Listen for control message
    HAL_UART_Receive_DMA(CONTROL_UART, rx_cmd.Buffer, sizeof(Serial_API_CmdStr));
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
  /* USER CODE BEGIN 2 */

  //<! Initialize LED light source
  LED_PWM_Init(&hlight_soruce);

  //<! Initialize BH1750 light sensor
  BH1750_Init(&hlight_sensor);

  //<! Start main measurement and control loop
  HAL_TIM_Base_Start_IT(CONTROL_TIM);

  //<! Start time stamp timer
  __HAL_TIM_CLEAR_FLAG(TIMESTAMP_TIM, TIM_FLAG_UPDATE);
  HAL_TIM_Base_Start_IT(TIMESTAMP_TIM);

  //<! Listen for control message
  HAL_UART_Receive_DMA(CONTROL_UART, rx_cmd.Buffer, sizeof(Serial_API_CmdStr));

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
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
