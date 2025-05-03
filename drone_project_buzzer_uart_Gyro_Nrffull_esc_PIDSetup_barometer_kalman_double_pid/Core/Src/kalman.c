/*
 * kalman.c
 *
 *  Created on: Apr 7, 2024
 *      Author: pratham
 */

#include "kalman.h"

float kalman_angle_out_pitch[2] ={0} ;
float kalman_angle_out_roll[2] ={0} ;

void kalman_angle (float kalman_state , float kalman_uncertanity , float state_input, float measuremnt , float *kalman_angle_out ){

	// getting the state from the input state vector
	kalman_state = kalman_state*1 + KALMAN_SAMPLING_TIME * state_input ;
	// making the prediction  = G*G^t * sd
	kalman_uncertanity = 1*kalman_uncertanity*1 + 4.5*4.5*KALMAN_SAMPLING_TIME * KALMAN_SAMPLING_TIME ;
	// computing the Kalman gain
	float Kalman_Gain = 1*kalman_uncertanity/(1*kalman_uncertanity*1 + (3*3) ) ;
	// update the estimation using the Kalman gain and the measurement from the accelerometer angle prediction
	kalman_state = kalman_state + Kalman_Gain * ( measuremnt - (1*kalman_state) ) ;
	// update the kalman_uncertanity
	kalman_uncertanity = (1 - Kalman_Gain*1)*kalman_uncertanity ;

	// save the uncertainty and state for the next iteration
	kalman_angle_out[0] = kalman_state ;
	kalman_angle_out[1] = kalman_uncertanity ;


}



void kalman2D(){
return ;
}



