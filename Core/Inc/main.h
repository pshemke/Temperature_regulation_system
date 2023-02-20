/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI4_CS_Pin GPIO_PIN_4
#define SPI4_CS_GPIO_Port GPIOE
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define USER_Btn_EXTI_IRQn EXTI15_10_IRQn
#define LAMP_TRIAC_Pin GPIO_PIN_3
#define LAMP_TRIAC_GPIO_Port GPIOF
#define LAMP_SYNC_Pin GPIO_PIN_5
#define LAMP_SYNC_GPIO_Port GPIOF
#define LAMP_SYNC_EXTI_IRQn EXTI9_5_IRQn
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define EXT1_Btn_Pin GPIO_PIN_0
#define EXT1_Btn_GPIO_Port GPIOC
#define EXT1_Btn_EXTI_IRQn EXTI0_IRQn
#define RMII_MDC_Pin GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define POTENTIOMETER2_Pin GPIO_PIN_2
#define POTENTIOMETER2_GPIO_Port GPIOC
#define EXT2_Btn_Pin GPIO_PIN_3
#define EXT2_Btn_GPIO_Port GPIOC
#define EXT2_Btn_EXTI_IRQn EXTI3_IRQn
#define RMII_REF_CLK_Pin GPIO_PIN_1
#define RMII_REF_CLK_GPIO_Port GPIOA
#define RMII_MDIO_Pin GPIO_PIN_2
#define RMII_MDIO_GPIO_Port GPIOA
#define LD4_Pin GPIO_PIN_3
#define LD4_GPIO_Port GPIOA
#define LED_RGB_R_Pin GPIO_PIN_5
#define LED_RGB_R_GPIO_Port GPIOA
#define ENC_CLK_Pin GPIO_PIN_6
#define ENC_CLK_GPIO_Port GPIOA
#define RMII_CRS_DV_Pin GPIO_PIN_7
#define RMII_CRS_DV_GPIO_Port GPIOA
#define RMII_RXD0_Pin GPIO_PIN_4
#define RMII_RXD0_GPIO_Port GPIOC
#define RMII_RXD1_Pin GPIO_PIN_5
#define RMII_RXD1_GPIO_Port GPIOC
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define POTENTIOMETER1_Pin GPIO_PIN_1
#define POTENTIOMETER1_GPIO_Port GPIOB
#define COOLING_Pin GPIO_PIN_13
#define COOLING_GPIO_Port GPIOF
#define DISP_2_Pin GPIO_PIN_14
#define DISP_2_GPIO_Port GPIOF
#define DISP_4_Pin GPIO_PIN_15
#define DISP_4_GPIO_Port GPIOF
#define DISP_E_Pin GPIO_PIN_7
#define DISP_E_GPIO_Port GPIOE
#define DISP_F_Pin GPIO_PIN_8
#define DISP_F_GPIO_Port GPIOE
#define HEATER_Pin GPIO_PIN_9
#define HEATER_GPIO_Port GPIOE
#define DISP_D_Pin GPIO_PIN_10
#define DISP_D_GPIO_Port GPIOE
#define DISP_1_Pin GPIO_PIN_11
#define DISP_1_GPIO_Port GPIOE
#define DISP_C_Pin GPIO_PIN_12
#define DISP_C_GPIO_Port GPIOE
#define DISP_3_Pin GPIO_PIN_13
#define DISP_3_GPIO_Port GPIOE
#define DISP_B_Pin GPIO_PIN_14
#define DISP_B_GPIO_Port GPIOE
#define DISP_A_Pin GPIO_PIN_15
#define DISP_A_GPIO_Port GPIOE
#define LED_RGB_G_Pin GPIO_PIN_10
#define LED_RGB_G_GPIO_Port GPIOB
#define LED_RGB_B_Pin GPIO_PIN_11
#define LED_RGB_B_GPIO_Port GPIOB
#define RMII_TXD1_Pin GPIO_PIN_13
#define RMII_TXD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define HEATERD13_Pin GPIO_PIN_13
#define HEATERD13_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define ENC_DT_Pin GPIO_PIN_7
#define ENC_DT_GPIO_Port GPIOC
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define DISP_G_Pin GPIO_PIN_9
#define DISP_G_GPIO_Port GPIOG
#define RMII_TX_EN_Pin GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOG
#define RMII_TXD0_Pin GPIO_PIN_13
#define RMII_TXD0_GPIO_Port GPIOG
#define DISP_H_Pin GPIO_PIN_14
#define DISP_H_GPIO_Port GPIOG
#define SW0_Pin GPIO_PIN_3
#define SW0_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
#define BH1750_SCL_Pin GPIO_PIN_8
#define BH1750_SCL_GPIO_Port GPIOB
#define BH1750_SDA_Pin GPIO_PIN_9
#define BH1750_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
