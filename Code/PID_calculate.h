
typedef struct {
    double Kp, Ki, Kd; 
    double prev_error;
    double integral;  
} PID_Controller;

double PID_Calculate(PID_Controller *pid, double setpoint, double process_variable, double dt);

