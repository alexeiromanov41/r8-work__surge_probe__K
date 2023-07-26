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
#include "stm32f3xx_hal.h"

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
#define U_out_Pin GPIO_PIN_0
#define U_out_GPIO_Port GPIOA
#define I_out_Pin GPIO_PIN_1
#define I_out_GPIO_Port GPIOA
#define test_led_Pin GPIO_PIN_3
#define test_led_GPIO_Port GPIOA
#define DAC_I_out_Pin GPIO_PIN_4
#define DAC_I_out_GPIO_Port GPIOA
#define I_out_comp_Pin GPIO_PIN_7
#define I_out_comp_GPIO_Port GPIOA
#define soft_start_Pin GPIO_PIN_0
#define soft_start_GPIO_Port GPIOB
#define pwm_1_Pin GPIO_PIN_8
#define pwm_1_GPIO_Port GPIOA
#define pwm_2_Pin GPIO_PIN_9
#define pwm_2_GPIO_Port GPIOA
#define driver_error_Pin GPIO_PIN_12
#define driver_error_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
