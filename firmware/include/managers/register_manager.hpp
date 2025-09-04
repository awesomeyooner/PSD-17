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
        std::vector<uint8_t>* w_buf = wire_manager->get_write_buffer();

        // Commands
        wire_manager->add_command(Command::make_float((uint8_t)CommandType::TARGET, &motor->target));

        // Requests
        wire_manager->add_request(Request::make_float((uint8_t)RequestType::POSITION, &motor->shaft_angle, w_buf));
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