#ifndef REGISTER_MANAGER_HPP
#define REGISTER_MANAGER_HPP

#include "motor_manager.hpp"
#include "wire_manager.hpp"

class RegisterManager
{

public:

    static RegisterManager instance;
    static RegisterManager *get_instance();

    void init()
    {
        // shorthand variables
        StepperMotor* motor = motor_manager->get_motor();
        StepperDriver4PWM* driver = motor_manager->get_driver();
        LowsideCurrentSense* current_sensor = motor_manager->get_current_sensor();
        std::vector<uint8_t>* w_buf = wire_manager->get_write_buffer();

        // Commands
        wire_manager->add_command(Command::make_runnable((uint8_t)CommandType::DISABLE_MOTOR, [motor](){motor->disable();}));
        wire_manager->add_command(Command::make_runnable((uint8_t)CommandType::ENABLE_MOTOR, [motor](){motor->enable();}));
        wire_manager->add_command(Command::make_runnable((uint8_t)CommandType::RUN_FOC_INIT, [motor](){motor->initFOC();}));

        wire_manager->add_command(Command::make_float((uint8_t)CommandType::TARGET, &motor->target));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CLOSED_LOOP_VELOCITY, &motor->target, [motor](){motor->controller = MotionControlType::velocity;}));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CLOSED_LOOP_POSITION, &motor->target, [motor](){motor->controller = MotionControlType::angle;}));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CLOSED_LOOP_TORQUE, &motor->target, [motor](){motor->controller = MotionControlType::torque;}));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::OPEN_LOOP_VELOCITY, &motor->target, [motor](){motor->controller = MotionControlType::velocity_openloop;}));
        
        // Settings
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_CURRENT_LIMIT, &motor->current_limit));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_PHASE_RESISTANCE, &motor->phase_resistance));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_VOLTAGE_ALIGNMENT, &motor->voltage_sensor_align));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_MOTOR_VOLTAGE_LIMIT, &motor->voltage_limit));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_DRIVER_VOLTAGE_LIMIT, &driver->voltage_limit));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_SUPPLY_VOLTAGE, &driver->voltage_power_supply));
        wire_manager->add_command(Command{
            .reg = (uint8_t)CommandType::CHANGE_TORQUE_MODE,
            .length = sizeof(float),
            .runnable = 
                [motor](std::vector<uint8_t>* data) -> StatusCode{
                    int type = (int)data->at(0);

                    switch(type){
                        case 0:
                            motor->torque_controller = TorqueControlType::voltage;
                            break;
                        case 1:
                            motor->torque_controller = TorqueControlType::dc_current;
                            break;
                        case 2:
                            motor->torque_controller = TorqueControlType::foc_current;
                            break;
                    }

                    return StatusCode::OK;
                }
            }
        );

        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_VELOCITY_KP, &motor->PID_velocity.P));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_VELOCITY_KI, &motor->PID_velocity.I));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_VELOCITY_KD, &motor->PID_velocity.D));

        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_POSITION_KP, &motor->P_angle.P));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_POSITION_KI, &motor->P_angle.I));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_POSITION_KD, &motor->P_angle.D));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_POSITION_KF, &motor->feed_forward_velocity));

        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_CURRENT_Q_KP, &motor->PID_current_q.P));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_CURRENT_Q_KI, &motor->PID_current_q.I));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_CURRENT_Q_KD, &motor->PID_current_q.D));

        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_CURRENT_D_KP, &motor->PID_current_d.P));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_CURRENT_D_KI, &motor->PID_current_d.I));
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::CHANGE_CURRENT_D_KD, &motor->PID_current_d.D));
        
        // Requests
        wire_manager->add_request(Request::make_float((uint8_t)RequestType::TARGET, &motor->target, w_buf));
        wire_manager->add_request(Request::make_float((uint8_t)RequestType::VELOCITY, &motor->shaft_velocity, w_buf));
        wire_manager->add_request(Request::make_float((uint8_t)RequestType::POSITION, &motor->shaft_angle, w_buf));
        wire_manager->add_request(Request::make_float((uint8_t)RequestType::IS_ENABLED, &motor->enabled, w_buf));
        
        wire_manager->add_request(Request{
            .reg = (uint8_t)RequestType::CURRENT,
            .length = sizeof(float),
            .runnable = [motor, current_sensor, w_buf]() -> StatusCode{

                std::vector<uint8_t> bytes = I2C::float_to_bytes(current_sensor->getFOCCurrents(motor->electricalAngle()).q);
                // std::vector<uint8_t> bytes = I2C::float_to_bytes(motor->LPF_current_q(current_sensor->getPhaseCurrents().a));

                w_buf->assign(bytes.begin(), bytes.end());
                
                return StatusCode::OK;
            }
        });

        // TODO: implement a detailed error system
        // wire_manager->add_request(Request::make_float((uint8_t)RequestType::ERROR, &motor->shaft_angle, w_buf));
    }

private:
    WireManager *wire_manager = WireManager::get_instance();
    MotorManager *motor_manager = MotorManager::get_instance();

}; // class RegisterManager

RegisterManager RegisterManager::instance;
RegisterManager *RegisterManager::get_instance()
{
    return &instance;
}

#endif // REGISTER_MANAGER_HPP