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
/*
//doc xung encoder cua xe
static long pluse_Encoder1_Val = 0;
static float pend_Angle = 0.0;
static float pend_Angle_Array[PEND_ANGLE_LENGTH];

//doc xung encoder cua con lac
static long pluse_Encoder2_Val = 0;

static float pend_Setpoint = 180.0;
static float pend_Kp = 60;
static float pend_Ki = 5;
static float pend_Kd = 5;
static double PID_pend = 0;
//static float pend_Kp = 10;
//static float pend_Ki = 0;
//static float pend_Kd = 0;

//He so chuan
//static float cart_Kp = 3.7f;
//static float cart_Ki = 1.0f;
//static float cart_Kd = 0.9f;

//#define cart_Kp  3.7;
//#define cart_Ki  1.0;
//#define cart_Kd  0.9;

static int sampleTime = 100;
static float dt = 0;
static double PWM_Value = 0;

static float cart_Setpoint = 0;
static double cart_Position = 0;
static double PID_cart = 0;

static PID pid_pend;
static PID pid_cart;

static PID_Controller positionPID = {3.7, 1.0, 0.9, 0, 0};		//Bo PID vong trong (cho Cart)	
static PID_Controller anglePID = {60, 5, 0, 0, 0};										//Bo PID vong ngoai (cho Pend)
int main(void){
	
	
	dt = (float)sampleTime/1000;
	
	//Cac ham khoi tao
	systick_init();
	
	timer_irq_milli_start(T1, 100);
	UART_init(2, 115200);
	Init_DC_Control();
	Init_Encoder(1, PA, 4, PA, 5);
	Init_Encoder(2, PA, 10, PA, 11);
	//khoi tao bo PID cho con lac
//	PID_init(&pid_pend, pend_Setpoint, pend_Kp, pend_Ki, pend_Kd, PWM_MAX, PWM_MIN);
//	
//	//khoi tao bo PID cho xe
//	PID_init(&pid_cart, cart_Setpoint, cart_Kp, cart_Ki, cart_Kd, 50, -50);
	
	while(1){
		pluse_Encoder1_Val = Pluse_ENC1_Val();				//doc xung cua con lac
		pend_Angle = Read_Angle(pluse_Encoder1_Val);	//goc hien tai cua con lac
		pend_Angle_Array[PEND_ANGLE_LENGTH-1] = pend_Angle;
		
		pluse_Encoder2_Val = Pluse_ENC2_Val();
		cart_Position = (float)(pluse_Encoder2_Val*20/30);
			
		Cart_Move((int)(PWM_Value));
		
		UART_SEND(2, "PID 1: ");
		basic_msg(2, (int)(PID_pend));
		UART_SEND(2, "PID 2: ");
		basic_msg(2, (int)(PID_cart));
		UART_SEND(2, "PWM Value ");
		basic_msg(2, (int)(PWM_Value));
//		DelayMs(100);
	}
}

float Read_Angle(long pluse){
	float deg = ((float)pluse/PLUSE_PER_CYCLE_PEND*360);	//pulseCount*360/1000
	
	if(deg < 0)	deg = 0;
	else if(deg >= 360) deg = 360;
	return deg;
}
void TIM1_UP_IRQHandler(void){
	timer_irq_Rflag(T1);
	if(pend_Angle > 160 && pend_Angle <= 270){
//			PID_pend = PID_Compute(&pid_pend, pend_Angle, dt, 500, -500);
//			PID_pend = PID_Calculate(180, pend_Angle, pend_Kp, pend_Ki, pend_Kd, 500, -500, dt);
//			PID_cart = PID_Calculate(0, cart_Position, cart_Kp, cart_Ki, cart_Kd, 500, -500, dt);
		PID_cart = PID_Calculate(&anglePID, 0, 180, 0.1);
		PID_pend = PID_Calculate(&positionPID, PID_cart, PID_pend, 0.1);
	}
	else{
		PID_cart = 0;
		PID_pend = 0;
		PWM_Value = 0;
	}
	
	PWM_Value = PID_pend - PID_cart;
}
*/
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




