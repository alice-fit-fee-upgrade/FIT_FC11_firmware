/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
extern TIM_HandleTypeDef htim4;

struct button sw_buttons[SW_PINS_NUM] = {
  {.pin = GPIO_PIN_8, .b_debounced = true},
  {.pin = GPIO_PIN_9, .b_debounced = true},
  {.pin = GPIO_PIN_10, .b_debounced = true},
  {.pin = GPIO_PIN_11, .b_debounced = true},
  {.pin = GPIO_PIN_12, .b_debounced = true},
};
/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
        * Free pins are configured automatically as Analog (this feature is enabled through
        * the Code Generation settings)
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, EN1_Pin|EN2_Pin|EN3_Pin|EN4_Pin
                          |EN5_Pin|EN6_Pin|EN7_Pin|EN8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RS_DE_GPIO_Port, RS_DE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = EN1_Pin|EN2_Pin|EN3_Pin|EN4_Pin
                          |EN5_Pin|EN6_Pin|EN7_Pin|EN8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1 PB2 PB12 PB13
                           PB14 PB15 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 PA10 PA11
                           PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = RS_DE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RS_DE_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  for (uint8_t sw_idx = 0; sw_idx < SW_PINS_NUM; ++sw_idx)
  {
    if (sw_buttons[sw_idx].pin == GPIO_Pin)
    {
      if(sw_buttons[sw_idx].b_debounced == true)
      {
  		  HAL_TIM_Base_Start_IT(&htim4);
		    sw_buttons[sw_idx].b_debounced = false;
	    }
	    else
      {
		    __NOP();
	    }
    }
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  if (htim == &htim4)
  {
    for (uint8_t sw_idx = 0; sw_idx < SW_PINS_NUM; ++sw_idx)
    {
      sw_buttons[sw_idx].state = HAL_GPIO_ReadPin(GPIOA, sw_buttons[sw_idx].pin);
      sw_buttons[sw_idx].b_debounced = true;
    }
    HAL_TIM_Base_Stop_IT(htim);
  }
}

void gpio_sw_state_read()
{
  for (uint8_t sw_idx = 0; sw_idx < SW_PINS_NUM; ++sw_idx)
  {
    sw_buttons[sw_idx].state = HAL_GPIO_ReadPin(GPIOA, sw_buttons[sw_idx].pin);
  }

  return;
}

uint8_t gpio_sw_addr_get()
{
  uint8_t addr = 0;
  for (uint8_t sw_idx = 0; sw_idx < SW_PINS_NUM; ++sw_idx)
  {
    if (sw_buttons[sw_idx].state == GPIO_PIN_SET)
    {
      addr |= 0x01 << sw_idx;
    }
  }

  return addr;
}
/* USER CODE END 2 */
