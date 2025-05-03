/*
 * mpu6500.c
 *
 *  Created on: Jan 28, 2024
 *      Author: pratham
 */

#include "mpu6500.h"


uint8_t  __IMU__BUFFER__ [14] ;
volatile float c_gx ,c_gy, c_gz ;
void inline Select_Gyro (void){
	while(LL_SPI_IsActiveFlag_BSY(SPI3) || (! (LL_SPI_IsActiveFlag_TXE(SPI3))) ||  (LL_SPI_IsActiveFlag_RXNE(SPI3)) );
	GYRO_CS_GPIO_Port->ODR &= ~(1 << 6);
}

void inline DeSelect_Gyro (void){
	while(LL_SPI_IsActiveFlag_BSY(SPI3) || (! (LL_SPI_IsActiveFlag_TXE(SPI3))) ||  (LL_SPI_IsActiveFlag_RXNE(SPI3)) );
	GYRO_CS_GPIO_Port->ODR |= (1 << 6);
}

uint8_t Read_Reg_Single(uint8_t Reg ){
uint8_t temp  = 0 ;
	Select_Gyro() ;
	while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (LL_SPI_IsActiveFlag_RXNE(SPI3)) ) {}
	LL_SPI_TransmitData8(SPI3, ( Reg | 0x80) ) ; // msb of the addr has to be 1 for read operation
    while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (!(LL_SPI_IsActiveFlag_RXNE(SPI3))) ) {}
	LL_SPI_ReceiveData8(SPI3) ; // dummy read
	while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) ||  (LL_SPI_IsActiveFlag_RXNE(SPI3)) ) {}
	LL_SPI_TransmitData8(SPI3, 0x00) ;
	while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (!(LL_SPI_IsActiveFlag_RXNE(SPI3)))) {}
	temp = LL_SPI_ReceiveData8(SPI3) ;
	DeSelect_Gyro() ;
	return temp ;
}

void Read_Reg_Multi(uint8_t Reg , uint8_t* value , uint8_t len ){

	Select_Gyro() ;
    while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (LL_SPI_IsActiveFlag_RXNE(SPI3)) ) {}
    LL_SPI_TransmitData8(SPI3, ( Reg | 0x80) ) ; // msb of the addr has to be zero for write  operation
    while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (!(LL_SPI_IsActiveFlag_RXNE(SPI3))) ) {}
	LL_SPI_ReceiveData8(SPI3) ; // dummy read
	for (uint32_t i = 0; i < len ; i++) {
	    while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (LL_SPI_IsActiveFlag_RXNE(SPI3)) ) {}
		LL_SPI_TransmitData8(SPI3, 0x00) ;
		while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (!(LL_SPI_IsActiveFlag_RXNE(SPI3)))) {}
		*(value + i) = LL_SPI_ReceiveData8(SPI3) ;
	}
	DeSelect_Gyro() ;

}

void Write_Reg_Single(uint8_t Reg , uint8_t value){
	Select_Gyro() ;
	while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (LL_SPI_IsActiveFlag_RXNE(SPI3)) ) {}
	LL_SPI_TransmitData8(SPI3, ( Reg & 0x7f) ) ; // msb of the addr has to be zero for write operation
    while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (!(LL_SPI_IsActiveFlag_RXNE(SPI3))) ) {}
	LL_SPI_ReceiveData8(SPI3) ; // dummy read
	while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (LL_SPI_IsActiveFlag_RXNE(SPI3)) ) {}
	LL_SPI_TransmitData8(SPI3, value) ;
    while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (!(LL_SPI_IsActiveFlag_RXNE(SPI3))) ) {}
	LL_SPI_ReceiveData8(SPI3) ; // dummy read
	DeSelect_Gyro() ;
}

void Write_Reg_Multi(uint8_t Reg , uint8_t* value , uint8_t len ){
	Select_Gyro() ;
    while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (LL_SPI_IsActiveFlag_RXNE(SPI3)) ) {}
    LL_SPI_TransmitData8(SPI3, ( Reg & 0x7f) ) ; // msb of the addr has to be zero for write operation
    while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (!(LL_SPI_IsActiveFlag_RXNE(SPI3))) ) {}
	LL_SPI_ReceiveData8(SPI3) ; // dummy read
	for (uint32_t i = 0; i < len ; i++) {
	    while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (LL_SPI_IsActiveFlag_RXNE(SPI3)) ) {}
		LL_SPI_TransmitData8(SPI3, *(value + i)) ;
	    while((! (LL_SPI_IsActiveFlag_TXE(SPI3))) || ( LL_SPI_IsActiveFlag_BSY(SPI3)) || (!(LL_SPI_IsActiveFlag_RXNE(SPI3))) ) {}
		LL_SPI_ReceiveData8(SPI3) ; // dummy read
	}
	DeSelect_Gyro() ;
}


void  mpu_init(SPI_TypeDef * SPIx , uint8_t gyro_rate , uint8_t accel_rate){
	LL_SPI_Enable(SPI3) ;
	Select_Gyro();

	// read who am i
	if (Read_Reg_Single(REG_WHO_AM_I) != 0x70 ) {
		// reset the chip
		printf("chip is not mpu6500 WHO_AMI failed ") ;
		LL_GPIO_SetOutputPin(LED_RED_GPIO_Port, LED_RED_Pin) ;
		return ;
	}

	Write_Reg_Single(PWR_MGMT_1, 0x80); // chip reset
	HAL_Delay(100) ;
	Write_Reg_Single(PWR_MGMT_1 , 0x00 ) ;
	HAL_Delay(100) ;
	Write_Reg_Single(PWR_MGMT_1, 0x00); // stable time source selection
	HAL_Delay(250) ;
	Write_Reg_Single(MPU_CONFIG, 0x00); // no dlpf and no accessories
	Write_Reg_Single(SMPLRT_DIV, 0x00); // no sample rate division

	Write_Reg_Single(GYRO_CONFIG, 0x00 ); // reset the gyro config
	Write_Reg_Single(ACCEL_CONFIG, 0x00); // reset the accel config
	Write_Reg_Single(ACCEL_CONFIG2, 0x00); // reset the accel config

	Write_Reg_Single(GYRO_CONFIG, (0x00 | (gyro_rate << 3))); // reset the gyro config
	Write_Reg_Single(ACCEL_CONFIG,(0x00 | (accel_rate << 3 ))); // reset the accel config
	Write_Reg_Single(ACCEL_CONFIG2, 0x00); // no dlpf

	Write_Reg_Single(SIGNAL_PATH_RESET , 0x07);
	HAL_Delay(100) ;
	Write_Reg_Single(REG_USER_CTRL , 0x10); // disable 12c use spi

	Write_Reg_Single(REG_INT_PIN_CFG    , 0x10); // config interrupt
	Write_Reg_Single(REG_INT_ENABLE     , 0x01); // enable interrupt

DeSelect_Gyro();
}


void readSensor()
{
Read_Reg_Multi(REG_ACCEL_XOUT_H, __IMU__BUFFER__, 14) ;

}

void getGyro(float * gx, float * gy, float * gz) {

  *gx =   ((float)((int16_t)(__IMU__BUFFER__[9]  | (__IMU__BUFFER__[8]  << 8) )) * 0.0609756) + c_gx ;
  *gy =   ((float)((int16_t)(__IMU__BUFFER__[11] | (__IMU__BUFFER__[10] << 8) )) * 0.0609756) + c_gy ;
  *gz =   ((float)((int16_t)(__IMU__BUFFER__[13] | (__IMU__BUFFER__[12] << 8) )) * 0.0609756) + c_gz ;

}

uint8_t getGyroTemp(){
	return ( ( __IMU__BUFFER__[7] | (__IMU__BUFFER__[6] << 8) )* 0.00299517 ) ;
}

void getAccel(float * ax, float * ay, float * az) {

  *ax = ((float)((int16_t)( __IMU__BUFFER__[1] | (__IMU__BUFFER__[0] << 8 ))) /2048);
  *ay = ((float)((int16_t)( __IMU__BUFFER__[3] | (__IMU__BUFFER__[2] << 8 ))) /2048);
  *az = ((float)((int16_t)( __IMU__BUFFER__[5] | (__IMU__BUFFER__[4] << 8 ))) /2048);
}

void calib_mpu(){
float gx ,gy, gz ;
float t_gx = 0,t_gy = 0, t_gz = 0;


	for (uint16_t i = 0; i < 2000 ; i++) {
		readSensor() ;
		getGyro(&gx, &gy, &gz);
		t_gx += gx ;
		t_gy += gy ;
		t_gz += gz ;

	}

	c_gx = t_gx/2000 ;
	c_gy = t_gy/2000 ;
	c_gz = t_gz/2000 ;


	if (c_gx > 0) {
		c_gx = (c_gx * -1 ) ;
	}else if (c_gx < 0){
		c_gx = (c_gx * -1 ) ;
	}

	if (c_gy > 0) {
		c_gy = (c_gy * -1 ) ;
	}else if (c_gy < 0){
		c_gy = (c_gy * -1 ) ;
	}

	if (c_gz > 0) {
		c_gz = (c_gz * -1 ) ;
	}else if (c_gz < 0){
		c_gz = (c_gz * -1 ) ;
	}



}














