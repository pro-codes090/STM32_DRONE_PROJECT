/*
 * bmp280.h
 *
 *  Created on: Apr 2, 2024
 *      Author: pratham
 */

#ifndef SRC_BMP280_H_
#define SRC_BMP280_H_

#include "main.h"
#include <math.h>
#define BMP_I2C I2C1
#define BMP_ADDR 0x76


uint32_t I2C_Receive(uint8_t devAddr, uint8_t* pData, uint16_t size);

uint32_t I2C_Send(uint8_t devAddr, uint8_t* pData, uint16_t size);

void read_calib();

int32_t BME280_compensate_T_int32(int32_t adc_T);

uint32_t BME280_compensate_P_int64(int32_t adc_P);

double get_alt();

void baro_init(void );

void calib_baro_alt() ;

#endif /* SRC_BMP280_H_ */
