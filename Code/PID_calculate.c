#include "PID_calculate.h"

double PID_Calculate(PID_Controller *pid, double setpoint, double process_variable, double dt) {
	double error = setpoint - process_variable;
	if(pid->integral > 200){
		pid->integral = 200;
	}
	else if(pid->integral < -200){
		pid->integral = -200;
	}
	else {
		pid->integral += error*dt;
	}
	double derivative = (error - pid->prev_error) / dt;
	double output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;
	pid->prev_error = error;
	
	return output;
}



