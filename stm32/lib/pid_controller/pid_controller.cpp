#include <pid_controller.hpp>

class PIDController{
public:
    PIDController(float kp, float ki, float kd){
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
        this->accumulated_error = 0;
        this->control_output = 0;
    }
    ~PIDController(){

    }
private:
    float kp, ki, kd;
    double accumulated_error = 0;
    double control_output = 0;
};