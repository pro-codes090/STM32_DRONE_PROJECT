/*
 * fc.h
 *
 *  Created on: Mar 8, 2024
 *      Author: pratham
 */

#ifndef INC_FC_H_
#define INC_FC_H_

#define _esc_setup_(mode)  		LL_TIM_EnableCounter(TIM1) ;\
								TIM1->BDTR |= (1 << 15);\
								LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1) ;\
								LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2) ;\
								LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3) ;\
								LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH4) ;\
								LL_TIM_OC_SetCompareCH1(TIM1, 90) ;\
								LL_TIM_OC_SetCompareCH2(TIM1, 90) ;\
								LL_TIM_OC_SetCompareCH3(TIM1, 90) ;\
								LL_TIM_OC_SetCompareCH4(TIM1, 90) ;



#define _wireless_setup(mode)	LL_GPIO_SetOutputPin(GYRO_CS_GPIO_Port , GYRO_CS_Pin ) ;\
								LL_GPIO_SetOutputPin(NRF_CSN_GPIO_Port , NRF_CSN_Pin) ;\
								LL_SPI_Enable(NRF24L01_SPI);\
  	  	  	  	  	  	  	  	uint8_t RxAddress0[5] = {0x78, 0x78, 0x78, 0x78, 0x78};\
								uint8_t RxAddress1[5] = {0xB3, 0xB4, 0xB5, 0xB6, 0xCD};\
								uint8_t RxAddress2 = 0xF3;\
								uint8_t RxAddress3 = 0xF2;\
								uint8_t RxAddress4 = 0xF4;\
								uint8_t RxAddress5 = 0xF1;\
								NRF24L01_RxInit(100, NRF24L01_DATA_RATE_1MBPS, 2000);\
								NRF24L01_SetRxAddress(NRF24L01_RX_ADDRESS_P0, RxAddress0, 2000);\
								NRF24L01_SetRxAddress(NRF24L01_RX_ADDRESS_P1, RxAddress1, 2000);\
								NRF24L01_SetRxAddress(NRF24L01_RX_ADDRESS_P2, &RxAddress2, 2000);\
								NRF24L01_SetRxAddress(NRF24L01_RX_ADDRESS_P3, &RxAddress3, 2000);\
								NRF24L01_SetRxAddress(NRF24L01_RX_ADDRESS_P4, &RxAddress4, 2000);\
								NRF24L01_SetRxAddress(NRF24L01_RX_ADDRESS_P5, &RxAddress5, 2000);

#define _blink_lights(mode)   LL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin | LED_GREEN_Pin);\
							  HAL_Delay(1000) ;\
							  LL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin | LED_GREEN_Pin);\
							  HAL_Delay(1000) ;



#define BUZZER_POWER_UP(mode)	LL_TIM_EnableCounter(TIM3);\
								LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);\
								HAL_Delay(100) ;\
								LL_TIM_SetAutoReload(TIM3, 40) ;\
								HAL_Delay(100) ;\
								LL_TIM_SetAutoReload(TIM3, 45) ;\
								HAL_Delay(100) ;\
								LL_TIM_SetAutoReload(TIM3, 50) ;\
								HAL_Delay(100) ;\
								LL_TIM_SetAutoReload(TIM3, 21) ;\
								HAL_Delay(100) ;\
								LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH1) ;







#endif /* INC_FC_H_ */
