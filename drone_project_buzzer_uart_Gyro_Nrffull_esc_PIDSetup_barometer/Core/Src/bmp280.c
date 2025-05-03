/*
 * bmp280.c
 *
 *  Created on: Apr 2, 2024
 *      Author: pratham
 */

#include "bmp280.h"
#include <stdio.h>

uint16_t dig_T1;
int16_t  dig_T2;
int16_t  dig_T3;
uint16_t dig_P1;
int16_t  dig_P2;
int16_t  dig_P3;
int16_t  dig_P4;
int16_t  dig_P5;
int16_t  dig_P6;
int16_t  dig_P7;
int16_t  dig_P8;
int16_t  dig_P9;

extern double calib_alt_baro ;

#define I2C_PERIPH BMP_I2C

// I2C send function
uint32_t I2C_Send(uint8_t devAddr, uint8_t* pData, uint16_t size) {
    uint32_t bytes_sent = 0;

    // Start condition
    LL_I2C_GenerateStartCondition(I2C1);
    while (!LL_I2C_IsActiveFlag_SB(I2C1)) {

    }

    // Send device address with write
    LL_I2C_TransmitData8(I2C1, devAddr << 1);
    while (!LL_I2C_IsActiveFlag_ADDR(I2C1)) {
    }
    LL_I2C_ClearFlag_ADDR(I2C1);

    // Send data
    for (uint16_t i = 0; i < size; ++i) {
        LL_I2C_TransmitData8(I2C1, pData[i]);
        while (!LL_I2C_IsActiveFlag_TXE(I2C1)) {
        }
        bytes_sent++;
    }

    // Stop condition
    LL_I2C_GenerateStopCondition(I2C1);

    return bytes_sent;
}

// I2C receive function
uint32_t I2C_Receive(uint8_t devAddr, uint8_t* pData, uint16_t size) {
    uint32_t timeout = 10000; // Timeout value, adjust as needed
    uint32_t bytes_received = 0;

    // Start condition
    LL_I2C_GenerateStartCondition(I2C_PERIPH);
    while (!LL_I2C_IsActiveFlag_SB(I2C_PERIPH)) {
        if (--timeout == 0) return 0xFFFFFFFF; // Timeout
    }

    // Send device address with read
    LL_I2C_TransmitData8(I2C_PERIPH, (devAddr << 1) | 0x01); // Last bit 1 indicates read
    while (!LL_I2C_IsActiveFlag_ADDR(I2C_PERIPH)) {
        if (--timeout == 0) return 0xFFFFFFFF; // Timeout
    }
    LL_I2C_ClearFlag_ADDR(I2C_PERIPH);

    // Receive data
    for (uint16_t i = 0; i < size; ++i) {

    	if (i == size - 1) {
			LL_I2C_AcknowledgeNextData(I2C_PERIPH, LL_I2C_NACK) ;
		}

        while (!LL_I2C_IsActiveFlag_RXNE(I2C_PERIPH)) {

        }
        pData[i] = LL_I2C_ReceiveData8(I2C_PERIPH);
        bytes_received++;
    }

    I2C_PERIPH->CR1 |= (1 << I2C_CR1_ACK_Pos) ;
    // Stop condition
    LL_I2C_GenerateStopCondition(I2C_PERIPH);
    return bytes_received;
}

void read_calib(){

	uint8_t buff [24] ={0 } ;
	uint8_t temp = 0x88 ;
	I2C_Send(BMP_ADDR, &temp, 1) ;
	I2C_Receive(BMP_ADDR, buff,24 );
	dig_T1 = (buff[1]<<8) | buff[0];
	dig_T2 = (buff[3]<<8) | buff[2];
	dig_T3 = (buff[5]<<8) | buff[4];
	dig_P1 = (buff[7]<<8) | buff[5];
	dig_P2 = (buff[9]<<8) | buff[6];
	dig_P3 = (buff[11]<<8) | buff[10];
	dig_P4 = (buff[13]<<8) | buff[12];
	dig_P5 = (buff[15]<<8) | buff[14];
	dig_P6 = (buff[17]<<8) | buff[16];
	dig_P7 = (buff[19]<<8) | buff[18];
	dig_P8 = (buff[21]<<8) | buff[20];
	dig_P9 = (buff[23]<<8) | buff[22];

}

int32_t t_fine;
int32_t BME280_compensate_T_int32(int32_t adc_T)
{
	int32_t var1_t, var2_t, T;
	var1_t = ((((adc_T>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
	var2_t = (((((adc_T>>4) - ((int32_t)dig_T1)) * ((adc_T>>4) - ((int32_t)dig_T1)))>> 12) *((int32_t)dig_T3)) >> 14;
	t_fine = var1_t + var2_t;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}
uint32_t BME280_compensate_P_int64(int32_t adc_P)
{
	int64_t var1, var2, p;
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)dig_P6;
	var2 = var2 + ((var1*(int64_t)dig_P5)<<17);
	var2 = var2 + (((int64_t)dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)dig_P3)>>8) + ((var1 * (int64_t)dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)dig_P1)>>33;
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576-adc_P;
	p = (((p<<31)-var2)*3125)/var1;
	var1 = (((int64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7)<<4);
	return (uint32_t)( p  ) ;
}

double get_alt(){

	int32_t adc_T, adc_P ;
	uint8_t RawData[6] = {0} , data;
	data = 0xF7 ;
	I2C_Send(BMP_ADDR, &data , 1) ;
	I2C_Receive(BMP_ADDR, RawData, 6) ;
	adc_T = (RawData[3]<<12)|(RawData[4]<<4)|(RawData[5]>>4);
	adc_P = (RawData[0]<<12)|(RawData[1]<<4)|(RawData[2]>>4);

	int32_t var1_t, var2_t ;
	var1_t = ((((adc_T>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
	var2_t = (((((adc_T>>4) - ((int32_t)dig_T1)) * ((adc_T>>4) - ((int32_t)dig_T1)))>> 12) *((int32_t)dig_T3)) >> 14;
	t_fine = var1_t + var2_t;

	int64_t var1, var2, p;
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)dig_P6;
	var2 = var2 + ((var1*(int64_t)dig_P5)<<17);
	var2 = var2 + (((int64_t)dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)dig_P3)>>8) + ((var1 * (int64_t)dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)dig_P1)>>33;
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576-adc_P;
	p = (((p<<31)-var2)*3125)/var1;
	var1 = (((int64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7)<<4);
	return  (double) ( 44330.0*(1.0 - pow(((double)( p >>8 )*0.01)*0.0009869232667 , 0.1903)));

}

void calib_baro_alt (void){

	for (uint16_t i = 0;  i < 2000; i++) {
		calib_alt_baro += get_alt() ;
		HAL_Delay(1) ;
	}
	calib_alt_baro = (double)calib_alt_baro / (double)2000.00 ;
	printf("calib alt %f \n" , calib_alt_baro) ;

}

void baro_init(void){

uint8_t data[2] = {0xD0 , 0} ;
I2C_Send(BMP_ADDR, data, 1) ;
I2C_Receive(BMP_ADDR,data , 1) ;

if (data[0] == 0x58) {
	printf("device found \n") ;
	data[0] = 0xE0 ;
	data[1] = 0xB6 ;
	I2C_Send(BMP_ADDR, data, 2) ;


	data[0] =  0xF4 ; // put in sleep mode
	data[1] = 0x00 ;
	I2C_Send(BMP_ADDR, data, 2) ;

	data[0] = 0xF5;// filter and t stand by setting
	data[1] = 0x14;
	I2C_Send(BMP_ADDR, data, 2);

	data[0] = 0xF4 ;  // mode and p and t oversampling setting
	data[1] = 0x57 ;
	I2C_Send(BMP_ADDR, data, 2);

	}else{
		printf("Device not found \n") ;
	}

	read_calib() ;

}
