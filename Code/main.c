/*
	PWM A		 	<-> 	PA1
	PWM B 		<-> 	PA9
	
	ENC_1_A		<-> 	PA4
	ENC_1_B		<-> 	PA5
*/

#include <stdlib.h>

#include "msg_drive.h"
#include "uart_drive.h"
#include "systick_time.h"

#include "cart_move.h"
#include "read_encoder.h"
#include "PID_calculate.h"


double Read_Angle(long pluse);
void TIM1_UP_IRQHandler(void);

#define PLUSE_PER_CYCLE_PEND				1000
#define R_PWM_port 			PA	
#define R_PWM_pin 			1
#define L_PWM_port 			PA	
#define L_PWM_pin 			9
#define PWM_FREQ_kHz		2

#define 	MAX_PWM				500
#define 	MIN_PWM				(-500)
#define 	SAMPLE_TIME		100		//ms

//Khoi tao cac thong so cho bo PID
//Kp=60, Ki=5, Kd=5, preError=0, integral=0
static PID_Controller anglePID = {60, 8, 5, 0, 0};						

static double theta_setpoint = 180.0;			//goc mong muon cua con lac 
static double theta_current = 0.0;  			//goc thuc te cua con lac

static double dt = (double)SAMPLE_TIME / 1000;		//doi thanh 0.1s
static double PID_Pend = 0;								//gia tri PID cua con lac

int main(void){
	PWM_BTS7960_Controller pwm;
	ENCODER encoder;
	
	//su dung Timer1 de tao ngat moi 100ms
	timer_irq_milli_start(T1, (int)SAMPLE_TIME);
	
	UART_init(2, 115200);
	
	Init_DC_Control(&pwm, R_PWM_port, R_PWM_pin, L_PWM_port, L_PWM_pin, PWM_FREQ_kHz);
	Init_Encoder(&encoder, ENCODER_A_port, ENCODER_A_pin, ENCODER_B_port, ENCODER_B_pin);
	
	while(1){
		//doc gia tri cua goc tra ve
		double read_angle = Read_Angle(Pluse_Return());
		theta_current = read_angle;
		
		//dieu khien Motor theo gia tri bo PID
		Cart_Move(&pwm, (int)(PID_Pend));
		
		
		UART_SEND(2, "Theta: ");
		basic_msg(2, (int)(Pluse_Return()));
		
		UART_SEND(2, "PWM Value: ");
		basic_msg(2, (int)(theta_current));
	}
}

//lay gia tri xung doi thanh goc
double Read_Angle(long pluse){
	double deg = ((double)pluse/PLUSE_PER_CYCLE_PEND*360);	//pulseCount*360/1000
	
//	if(deg < 0)	deg = 0;
//	else if(deg >= 360) deg = 360;
	return deg;
}

//Ham ngat moi 100ms su dung Timer1
void TIM1_UP_IRQHandler(void){
	
	timer_irq_Rflag(T1);
	
	if(theta_current > 160 && theta_current <= 270){
		//tinh toan PID
		PID_Pend = PID_Calculate(&anglePID, theta_setpoint, theta_current, dt);
		
		//gioi han gia tri PWM 
		if(PID_Pend >= MAX_PWM){
			PID_Pend = MAX_PWM;
		}
		else if(PID_Pend <= MIN_PWM){
			PID_Pend = MIN_PWM;
		}
	}
	else{
		PID_Pend = 0;
	}
	
}




