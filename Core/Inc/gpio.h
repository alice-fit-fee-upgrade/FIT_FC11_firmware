/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stdbool.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define SW_PINS_NUM   5

struct button
{
  uint16_t pin;
  GPIO_PinState state;
  bool b_debounced;
};
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
void gpio_sw_state_read(void);
uint8_t gpio_sw_addr_get(void);

static inline void gpio_en_set_state(uint8_t pin_idx, GPIO_PinState state)
{
  HAL_GPIO_WritePin(GPIOA, pin_idx, state);
  return;
}

static inline void gpio_fans_toggle_state()
{
  HAL_GPIO_TogglePin(GPIOA, EN1_Pin);
  HAL_GPIO_TogglePin(GPIOA, EN2_Pin);
  HAL_GPIO_TogglePin(GPIOA, EN3_Pin);
  HAL_GPIO_TogglePin(GPIOA, EN4_Pin);
  HAL_GPIO_TogglePin(GPIOA, EN5_Pin);
  return;
}
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

