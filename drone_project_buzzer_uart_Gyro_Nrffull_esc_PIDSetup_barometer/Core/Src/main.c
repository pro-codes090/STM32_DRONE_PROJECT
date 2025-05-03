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
#include "nrf24l01.h"
#include "fc.h"
#include <math.h>
#define ARM_MATH_CM4
#include "arm_math.h"
#include "bmp280.h"

//#define TRANSMITTER or RECEIVER
#define RECEIVER


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
uint8_t Transmitter_Rx_flag = 0;
uint8_t RxNumber = 10;
uint16_t RxBuffer[NRF24L01_PAYLOAD_LENGTH /2] = { 0};

float gx, gy, gz ;
float ax, ay, az ;

int16_t throttle = 0, yaw = 0, pitch = 0, roll = 0;

uint8_t pid_1ms_flag = 0 ;
uint8_t esc_calib_flag = 0;
uint8_t arming_flag_2sec = 0 ;
uint8_t timer_counter_200ms = 0 ;
uint8_t timer_counter_flag_200ms = 0 ;
uint8_t mpu_update = 0;

uint32_t ccr1 = 0 ;
uint32_t ccr2 = 0 ;
uint32_t ccr3 = 0 ;
uint32_t ccr4 = 0 ;

float pitch_acc = 0 ;
float roll_acc = 0;
float acc_gravity  = 0 ;
double calib_alt_baro;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void Custom_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int __io_putchar(int ch){
	while( !(LL_USART_IsActiveFlag_TXE(USART2))) ;
    LL_USART_TransmitData8(USART2, ch) ;
    return ch ;
}

void Custom_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == NRF_IRQ_Pin)
	{
		Transmitter_Rx_flag =1;
		NRF24L01_RxReceive((uint8_t *)RxBuffer, &RxNumber, 2000);

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
  MX_SPI2_Init();
  MX_I2C1_Init();
  MX_SPI3_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  LL_USART_EnableIT_RXNE(USART2) ;
  BUZZER_POWER_UP(1)
  HAL_Delay(100) ;
  mpu_init(SPI3 , GFS_2000DPS ,AFS_16G) ;
  calib_mpu() ;
  _blink_lights(1)
  _wireless_setup(1)
  LL_I2C_Enable(BMP_I2C) ;
  baro_init() ;
  calib_baro_alt() ;

  LL_TIM_EnableIT_UPDATE(TIM2) ;
  LL_TIM_EnableCounter(TIM2) ;

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
			case '3' :
				esc_calib_flag = 1;
				BUZZER_CALIB_MODE(1) ;
				break ;

       }
	}

	if (mpu_update == 1) {
		mpu_update = 0 ;
		readSensor() ;
		getGyro(&gx, &gy, &gz) ;
		getAccel(&ax, &ay, &az) ;
	}

  if (Transmitter_Rx_flag == 1) {
	  throttle = RxBuffer[0] ;
	  throttle = throttle < 4 ? 0 : throttle ;
	  throttle = throttle >4001 ? 4000 : throttle ;

	  yaw      = RxBuffer[1] - (uint16_t)2050 ;
	  yaw  = (( 8 > yaw )&&( yaw > -6 ))? 0 : yaw ;
      yaw  = yaw > 2001? 2000 : yaw;
      yaw  = yaw < -2001? -2000 : yaw;

	  pitch    = RxBuffer[2] - (uint16_t)2065 ;
	  pitch  = (( 8 > pitch) &&( pitch > -4 ))? 0 : pitch ;
	  pitch  = pitch > 2001? 2000 : pitch;
	  pitch  = pitch < -2001? -2000 : pitch;

	  roll     = RxBuffer[3] - (uint16_t)2110 ;
	  roll  = (( 5 > roll )&&( roll > -5) )? 0 : roll;
	  roll  = roll> 2001? 2000 : roll;
	  roll  = roll< -2001? -2000 : roll;

	  Transmitter_Rx_flag = 0;
   }

	if (arming_flag_2sec == 0 && (( throttle <= 0 ) && (yaw >= 1950) && (pitch <= -1950) && (roll >= 1950))) {
	  // allow the motors to turn on
	  HAL_Delay(1000) ;
	  HAL_Delay(1000) ;
	  if (arming_flag_2sec == 0 && (( throttle <= 0 ) && (yaw >= 1950) && (pitch <= -1950) && (roll >= 1950))) {
	      BUZZER_ARMED(1) ;
	      arming_flag_2sec = 1;
	      LL_GPIO_SetOutputPin(LED_GREEN_GPIO_Port, LED_GREEN_Pin) ;
	      _esc_setup_(1) ;
	  }
}


  if (arming_flag_2sec == 1 && (( throttle <= 0 ) && (yaw <= -1950) && (pitch <= -1950) && (roll <= -1950))) {
	  // allow the motors to turn off

	  if (arming_flag_2sec == 1 && (( throttle <= 0 ) && (yaw  <= -1950 ) && (pitch <= -1950) && (roll  <= -1950))) {
	  arming_flag_2sec = 0 ;
	  BUZZER_UNARMED(1) ;
	  _esc_desetup_(1) ;
	  LL_GPIO_ResetOutputPin(LED_GREEN_GPIO_Port, LED_GREEN_Pin) ;
	  }
}

  if (pid_1ms_flag == 1) {
  	pid_1ms_flag = 0 ;
  	LL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin) ;

  	ccr1 = 4000 + (throttle )/1.6 - pitch/4 - roll/4 +yaw/4 ;
	ccr2 = 4000 + (throttle )/1.6+ pitch/4 + roll/4 +yaw/4 ;
	ccr3 = 4000 + (throttle )/1.6- pitch/4 + roll/4 -yaw/4 ;
	ccr4 = 4000 + (throttle )/1.6 + pitch/4 - roll/4 -yaw/4 ;

  }


  if (arming_flag_2sec == 1) {

	  TIM1->CCR1 = ccr1 > 8001 ? 8000 : ccr1 <= 4400 ? 4400 : ccr1 ;
	  TIM1->CCR2 = ccr2 > 8001 ? 8000 : ccr2 <= 4400 ? 4400 : ccr2 ;
	  TIM1->CCR3 = ccr3 > 8001 ? 8000 : ccr3 <= 4400 ? 4400 : ccr3 ;
	  TIM1->CCR4 = ccr4 > 8001 ? 8000 : ccr4 <= 4400 ? 4400 : ccr4 ;

  }else{

	  TIM1->CCR1 = 4000 ;
	  TIM1->CCR2 = 4000 ;
	  TIM1->CCR3 = 4000 ;
	  TIM1->CCR4 = 4000 ;
}

  roll_acc = atan2f(-ax , sqrtf(((ay*ay) + (az*az)))) ;
  pitch_acc = atan2f(ay , sqrtf(((ax *ax) + (az*az)))) ;
  acc_gravity = ( -cosf(pitch_acc)*ax + ay*(cosf(pitch_acc) * sinf(roll_acc)) + az*(cosf(pitch_acc) * cosf(roll_acc) ) ) ;

  printf("%f,%f\r\n" , roll_acc*RAD_TO_DEG , pitch_acc*RAD_TO_DEG);
  if (timer_counter_flag_200ms ) {


	  timer_counter_flag_200ms = 0;
  }

  if (esc_calib_flag == 1 ) {

  }


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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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
