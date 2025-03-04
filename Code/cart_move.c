#include "cart_move.h"

void Init_DC_Control(PWM_BTS7960_Controller *pwm, char R_port, char R_pin, char L_port, char L_pin, int freq_kHz){
	//khai bao chan PWM - Right
	pwm->R_PWM_port = R_port;
	pwm->R_PWM_pin  = R_pin;
	
	//khai bao chan PWM - Left
	pwm->L_PWM_port = L_port;
	pwm->L_PWM_pin  = L_pin;
	
	//tan so dieu khien dong co (kHz), doi ra chu ky (us)
	float cycle_us = (1/(float)freq_kHz)*1000;
	pwm->PWM_Cycle = (int)cycle_us;	
}

void Cart_Move(PWM_BTS7960_Controller *pwm, signed int value_PWM){
	if(value_PWM > 0){
		timer_pwm_micro(pwm->R_PWM_port, pwm->R_PWM_pin, pwm->PWM_Cycle, value_PWM);
		timer_pwm_micro(pwm->L_PWM_port, pwm->L_PWM_pin, pwm->PWM_Cycle, 0);
	}
	if(value_PWM < 0){
		timer_pwm_micro(pwm->R_PWM_port, pwm->R_PWM_pin, pwm->PWM_Cycle, 0);
		timer_pwm_micro(pwm->L_PWM_port, pwm->L_PWM_pin, pwm->PWM_Cycle, abs(value_PWM));
	}
	if(value_PWM == 0){
		timer_pwm_micro(pwm->R_PWM_port, pwm->R_PWM_pin, pwm->PWM_Cycle, 0);
		timer_pwm_micro(pwm->L_PWM_port, pwm->L_PWM_pin, pwm->PWM_Cycle, 0);	
	}
}



