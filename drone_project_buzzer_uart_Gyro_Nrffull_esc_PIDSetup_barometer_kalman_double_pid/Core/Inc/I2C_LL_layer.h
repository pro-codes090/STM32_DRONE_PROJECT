/*
 * I2C_LL_layer.h
 *
 *  Created on: Apr 12, 2024
 *      Author: pratham
 */

#ifndef INC_I2C_LL_LAYER_H_
#define INC_I2C_LL_LAYER_H_

#include "main.h"
#include "i2c.h"
void I2C_master_Send(uint8_t size , uint8_t * data) ;
void I2C_addr_phase();
void I2c_Stop_transfer() ;


#endif /* INC_I2C_LL_LAYER_H_ */
