/*
 * pid.h
 *
 *  Created on: Apr 16, 2024
 *      Author: pratham
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "main.h"
#define PID_SAMPLING_TIME 0.001

float  pid_single_loop_update( float error, float *I_Ctrl_Prev , float D_measure , float * D_measure_Pev);
float  pid_single_loop_update_outer( float error, float *I_Ctrl_Prev , float D_measure , float * D_measure_Pev);


#endif /* INC_PID_H_ */
