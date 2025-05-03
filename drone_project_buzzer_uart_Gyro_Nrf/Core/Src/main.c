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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "mpu6500.h"
#include "nrf24.h"

//#define TRANSMITTER or RECEIVER
#define TRANSMITTER

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
uint16_t usart_data_req = 0 ;
uint8_t usart_rx_data  = 0;
uint8_t mpu_update = 0;


float gx, gy, gz ;
float ax, ay, az ;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int __io_putchar(int ch){
	while( !(LL_USART_IsActiveFlag_TXE(USART2))) ;
    LL_USART_TransmitData8(USART2, ch) ;
return ch ;
}

void BUZZER_POWER_UP() { LL_TIM_EnableCounter(TIM3);
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);
	HAL_Delay(100) ;
	LL_TIM_SetAutoReload(TIM3, 40) ;
	HAL_Delay(100) ;
	LL_TIM_SetAutoReload(TIM3, 45) ;
	HAL_Delay(100) ;
	LL_TIM_SetAutoReload(TIM3, 50) ;
	HAL_Delay(100) ;
	LL_TIM_SetAutoReload(TIM3, 21) ;
	HAL_Delay(100) ;
	LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH1) ;
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
  MX_SPI2_Init();
  MX_I2C1_Init();
  MX_SPI3_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

//  BUZZER_POWER_UP() ;

  LL_USART_EnableIT_RXNE(USART2) ;
  LL_GPIO_SetOutputPin(GYRO_CS_GPIO_Port , GYRO_CS_Pin ) ;
  LL_GPIO_SetOutputPin(NRF_CSN_GPIO_Port , NRF_CSN_Pin) ;

  HAL_Delay(100) ;
  mpu_init(SPI3 , GFS_2000DPS ,AFS_16G) ;
  calib_mpu() ;

  LL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin | LED_GREEN_Pin);
  HAL_Delay(1000) ;
  LL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin | LED_GREEN_Pin);
  HAL_Delay(1000) ;



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if (usart_data_req == 1 ) {
		usart_data_req = 0  ;
		usart_rx_data = LL_USART_ReceiveData8(USART2);
        LL_USART_TransmitData8(USART2, usart_rx_data) ;


        switch (usart_rx_data) {
			case '0':
		         LL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin | LED_GREEN_Pin);
				break;
			case '1':
				 LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);
				break;
			case '2':
				 LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH1);
				break;
       }
	}

	if (mpu_update == 1) {
		mpu_update = 0 ;
		readSensor() ;
		getGyro(&gx, &gy, &gz) ;
		getAccel(&ax, &ay, &az) ;
	}


//		  HAL_Delay(100);



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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
