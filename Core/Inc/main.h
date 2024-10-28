/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"

#include "stm32l1xx_ll_usart.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_cortex.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_dma.h"

#include "stm32l1xx_ll_exti.h"

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
#define EN1_Pin GPIO_PIN_0
#define EN1_GPIO_Port GPIOA
#define EN2_Pin GPIO_PIN_1
#define EN2_GPIO_Port GPIOA
#define EN3_Pin GPIO_PIN_2
#define EN3_GPIO_Port GPIOA
#define EN4_Pin GPIO_PIN_3
#define EN4_GPIO_Port GPIOA
#define EN5_Pin GPIO_PIN_4
#define EN5_GPIO_Port GPIOA
#define EN6_Pin GPIO_PIN_5
#define EN6_GPIO_Port GPIOA
#define EN7_Pin GPIO_PIN_6
#define EN7_GPIO_Port GPIOA
#define EN8_Pin GPIO_PIN_7
#define EN8_GPIO_Port GPIOA
#define PWM_Pin GPIO_PIN_0
#define PWM_GPIO_Port GPIOB
#define SW1_Pin GPIO_PIN_8
#define SW1_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_9
#define SW2_GPIO_Port GPIOA
#define SW3_Pin GPIO_PIN_10
#define SW3_GPIO_Port GPIOA
#define SW4_Pin GPIO_PIN_11
#define SW4_GPIO_Port GPIOA
#define SW5_Pin GPIO_PIN_12
#define SW5_GPIO_Port GPIOA
#define TACH_Pin GPIO_PIN_3
#define TACH_GPIO_Port GPIOB
#define RS_DE_Pin GPIO_PIN_5
#define RS_DE_GPIO_Port GPIOB
#define RS_TX_Pin GPIO_PIN_6
#define RS_TX_GPIO_Port GPIOB
#define RS_RX_Pin GPIO_PIN_7
#define RS_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
