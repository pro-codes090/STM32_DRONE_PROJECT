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
#include "stm32f4xx_hal.h"

#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"

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
#define RAD_TO_DEG 57.295779513082320876798154814105
#define DEG_TO_RAD  0.01745329251994329577
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin LL_GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define NRF_CE_Pin LL_GPIO_PIN_0
#define NRF_CE_GPIO_Port GPIOC
#define NRF_CSN_Pin LL_GPIO_PIN_1
#define NRF_CSN_GPIO_Port GPIOA
#define USART_TX_Pin LL_GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin LL_GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define NRF_IRQ_Pin LL_GPIO_PIN_4
#define NRF_IRQ_GPIO_Port GPIOA
#define NRF_IRQ_EXTI_IRQn EXTI4_IRQn
#define LED_RED_Pin LL_GPIO_PIN_5
#define LED_RED_GPIO_Port GPIOA
#define buzzer_pwm_Pin LL_GPIO_PIN_6
#define buzzer_pwm_GPIO_Port GPIOA
#define LED_GREEN_Pin LL_GPIO_PIN_7
#define LED_GREEN_GPIO_Port GPIOA
#define GYRO_IRQ_Pin LL_GPIO_PIN_7
#define GYRO_IRQ_GPIO_Port GPIOC
#define GYRO_IRQ_EXTI_IRQn EXTI9_5_IRQn
#define motor_4_Pin LL_GPIO_PIN_8
#define motor_4_GPIO_Port GPIOA
#define motor_3_Pin LL_GPIO_PIN_9
#define motor_3_GPIO_Port GPIOA
#define motor_2_Pin LL_GPIO_PIN_10
#define motor_2_GPIO_Port GPIOA
#define motor_1_Pin LL_GPIO_PIN_11
#define motor_1_GPIO_Port GPIOA
#define TMS_Pin LL_GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin LL_GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin LL_GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define GYRO_CS_Pin LL_GPIO_PIN_6
#define GYRO_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
