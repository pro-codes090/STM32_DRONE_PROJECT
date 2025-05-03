/*
 * I2C_LL_layer.c
 *
 *  Created on: Apr 12, 2024
 *      Author: pratham
 */


#include "I2C_LL_layer.h"

#define I2C_PERIP I2C1
#define I2C_ADDR 0x76

void I2C_addr_phase(){
	LL_I2C_Enable(I2C1) ;
	LL_I2C_GenerateStartCondition(I2C_PERIP) ;
	while(!LL_I2C_IsActiveFlag_SB(I2C_PERIP)) {}

	LL_I2C_TransmitData8(I2C_PERIP, I2C_ADDR<< 1);

	LL_I2C_ClearFlag_ADDR(I2C_PERIP) ;

}

void I2c_Stop_transfer()
{
	LL_I2C_GenerateStopCondition(I2C_PERIP) ;

}
void I2C_master_Send(uint8_t size , uint8_t * data){

	while(size > 0){
	while(LL_I2C_IsActiveFlag_TXE(I2C_PERIP) == 0) {

	}

	LL_I2C_TransmitData8(I2C_PERIP, *data);

	size --  ;
	data ++ ;

	}
	while(!(LL_I2C_IsActiveFlag_TXE(I2C_PERIP))) {}
	while(!(LL_I2C_IsActiveFlag_BTF(I2C_PERIP))) {}

}
