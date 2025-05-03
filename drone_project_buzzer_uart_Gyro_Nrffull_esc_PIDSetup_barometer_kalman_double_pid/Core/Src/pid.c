/*
 * pid.c
 *
 *  Created on: Apr 16, 2024
 *      Author: pratham
 */

#include "main.h"
#include "pid.h"
extern uint8_t pid_1ms_flag ;
extern int16_t throttle ;

float kp_in =  10;
float ki_in =  0.05;
float kd_in =  0.01   ;

float kp_out = 5;
float ki_out = 0;
float kd_out = 0;

float  pid_single_loop_update( float error, float *I_Ctrl_Prev , float D_measure , float * D_measure_Pev){

	float P_Ctrl = error * kp_in ;
	float I_Ctrl = (((float)*I_Ctrl_Prev ) + ( error * ki_in * (float)PID_SAMPLING_TIME));
	if (I_Ctrl >= 1000) {
		I_Ctrl = 1000;
	}else if(I_Ctrl <= -1000){
		I_Ctrl = -1000;
	}
	if ( ( throttle <= 30 ) || (pid_1ms_flag == 0 )  ) {
		I_Ctrl = 0 ;
	}
	*I_Ctrl_Prev = I_Ctrl ;

	float D_Ctrl = (((D_measure - ((float)(*D_measure_Pev)) ) / (float)PID_SAMPLING_TIME  ) * kd_in);

	*D_measure_Pev = D_measure ;
	return ( P_Ctrl + I_Ctrl + D_Ctrl) ;

}

float  pid_single_loop_update_outer( float error, float *I_Ctrl_Prev , float D_measure , float * D_measure_Pev){

	float P_Ctrl = error * kp_out ;
	float I_Ctrl = (((float)*I_Ctrl_Prev ) + ( error * ki_out * (float)PID_SAMPLING_TIME));
	if (I_Ctrl >= 1000) {
		I_Ctrl = 1000;
	}else if(I_Ctrl <= -1000){
		I_Ctrl = -1000;
	}
	if ( ( throttle <= 30 ) || (pid_1ms_flag == 0 )  ) {
		I_Ctrl = 0 ;
	}
	*I_Ctrl_Prev = I_Ctrl ;

	float D_Ctrl = -((D_measure ) * kd_out);

	*D_measure_Pev = D_measure ;
	return ( P_Ctrl + I_Ctrl + D_Ctrl) ;

}


