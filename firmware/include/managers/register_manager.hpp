#ifndef REGISTER_MANAGER_HPP
#define REGISTER_MANAGER_HPP

#include "motor_manager.hpp"
#include "wire_manager.hpp"

class RegisterManager
{

public:

    Request req_position = {
        .reg = (uint8_t)RequestType::POSITION,
        .length = sizeof(float),
        .runnable = [this]() -> StatusCode
        {
            float position = motor_manager->get_motor()->shaft_angle;

            std::vector<uint8_t> buf = I2C::float_to_bytes(position);

            wire_manager->get_write_buffer()->assign(buf.begin(), buf.end());

            return StatusCode::OK;
        }};

    static RegisterManager instance;
    static RegisterManager *get_instance();

    void init()
    {
        wire_manager->add_command(
            Command{
                .reg = (uint8_t)CommandType::TARGET,
                .length = sizeof(float),
                .runnable = 
                [this](std::vector<uint8_t> *data) -> StatusCode{
                    float value = I2C::bytes_to_float(*data);

                    motor_manager->get_motor()->target = value;

                    return StatusCode::OK;
                }
            }
        );

        wire_manager->add_request(
            Request{
                .reg = (uint8_t)RequestType::POSITION,
                .length = sizeof(float),
                .runnable = 
                [this]() -> StatusCode{
                    float position = motor_manager->get_motor()->shaft_angle;

                    std::vector<uint8_t> buf = I2C::float_to_bytes(position);

                    wire_manager->get_write_buffer()->assign(buf.begin(), buf.end());

                    return StatusCode::OK;
                }
            }
        );
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