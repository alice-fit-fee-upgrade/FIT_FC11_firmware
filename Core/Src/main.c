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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "cli.h"
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
uint8_t usart_buf[1];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
static cli_status_t cmd_read_status(int argc, char **argv);
static cli_status_t cmd_relay_l_off(int argc, char **argv);
static cli_status_t cmd_relay_l_on(int argc, char **argv);
static cli_status_t cmd_relay_m_off(int argc, char **argv);
static cli_status_t cmd_relay_m_on(int argc, char **argv);
static cli_status_t cmd_version(int argc, char **argv);
static cli_status_t cmd_diagnostic_mode(int argc, char **argv);
static cli_status_t cmd_reset(int argc, char **argv);
static cli_status_t cmd_upgrade_mode(int argc, char **argv);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
cmd_t cmd_tbl[] = {
  {.cmd = "R", .func = cmd_read_status}, 
  {.cmd = "L0", .func = cmd_relay_l_off},
  {.cmd = "L1", .func = cmd_relay_l_on},
  {.cmd = "M0", .func = cmd_relay_m_off},
  {.cmd = "M1", .func = cmd_relay_m_on},
  {.cmd = "version", .func = cmd_version},
  {.cmd = "diagnostic_mode_on", .func = cmd_diagnostic_mode},
  {.cmd = "board_reset", .func = cmd_reset},
  {.cmd = "remote_upgrade_mode", .func = cmd_upgrade_mode}
  };
cli_t cli;
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
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  uint8_t msg_buf[35] = {'\0'};
  uint8_t cnt = 0;
  cli.println = usart_print;
	cli.cmd_tbl = cmd_tbl;
	cli.cmd_cnt = sizeof(cmd_tbl) / sizeof(cmd_t);
	cli_init(&cli);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  gpio_sw_state_read();
  uint8_t addr = gpio_sw_addr_get();
  cli_set_address(&cli, addr);

  while (1)
  {
    /* Check address */
    uint8_t new_addr = gpio_sw_addr_get();
    if (new_addr != addr)
    {
      cli_set_address(&cli, new_addr);
    }

    /* Parse the message from console */
    cli_process(&cli);

    /* Some delay */
    HAL_Delay(250);

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART1_IRQn);
  /* EXTI15_10_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  /* EXTI9_5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

/* USER CODE BEGIN 4 */
cli_status_t cmd_read_status(int argc, char **argv)
{
	cli.println("R function executed");
	return CLI_OK;
}

cli_status_t cmd_relay_l_off(int argc, char **argv)
{
	cli.println("L0 function executed");
	return CLI_OK;
}

cli_status_t cmd_relay_l_on(int argc, char **argv)
{
	cli.println("L1 function executed");
	return CLI_OK;
}

cli_status_t cmd_relay_m_off(int argc, char **argv)
{
	cli.println("M0 function executed");
	return CLI_OK;
}

cli_status_t cmd_relay_m_on(int argc, char **argv)
{
	cli.println("M1 function executed");
	return CLI_OK;
}

cli_status_t cmd_version(int argc, char **argv)
{
	cli.println("version");
	return CLI_OK;
}

cli_status_t cmd_diagnostic_mode(int argc, char **argv)
{
	cli.println("diagnostic_mode_on");
	return CLI_OK;
}

cli_status_t cmd_reset(int argc, char **argv)
{
	cli.println("board_reset");
	return CLI_OK;
}

cli_status_t cmd_upgrade_mode(int argc, char **argv)
{
	cli.println("remote_upgrade_mode");
	return CLI_OK;
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
