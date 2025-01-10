#include <chrono>
#include <thread>

class ThermalController {
private:
    double kp, ki, kd;
    double setpoint;
    double last_error;
    double integral;
    
public:
    ThermalController(double p, double i, double d, double target) 
        : kp(p), ki(i), kd(d), setpoint(target), last_error(0), integral(0) {}
    
    double compute(double current_temp) {
        double error = setpoint - current_temp;
        integral += error;
        double derivative = error - last_error;
        
        double output = kp * error + ki * integral + kd * derivative;
        last_error = error;
        
        return output;
    }
    
    void set_target(double target) {
        setpoint = target;
    }
};

extern "C" {
    ThermalController* create_controller(double p, double i, double d, double target) {
        return new ThermalController(p, i, d, target);
    }
    
    double compute_control(ThermalController* controller, double current_temp) {
        return controller->compute(current_temp);
    }
} 