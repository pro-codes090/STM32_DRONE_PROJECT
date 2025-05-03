/*
 * kalman.h
 *
 *  Created on: Apr 7, 2024
 *      Author: pratham
 */

#ifndef INC_KALMAN_H_
#define INC_KALMAN_H_

#include "main.h"
#include "math.h"

#define KALMAN_SAMPLING_TIME 0.001

void kalman_angle (float kalman_state , float kalman_uncertanity ,float state_input, float measuremnt , float *kalman_angle_out );
void kalman2D();

#endif /* INC_KALMAN_H_ */
