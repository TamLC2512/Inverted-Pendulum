#include "gp_drive.h"
#include "timer_drive.h"
#include <stdlib.h>

typedef struct{
	char 	R_PWM_port, R_PWM_pin;
	char 	L_PWM_port, L_PWM_pin;
	int PWM_Cycle;
}PWM_BTS7960_Controller;

void Init_DC_Control(PWM_BTS7960_Controller *pwm, char R_port, char R_pin, char L_port, char L_pin, int freq_kHz);
void Cart_Move(PWM_BTS7960_Controller *pwm, signed int value_PWM);


