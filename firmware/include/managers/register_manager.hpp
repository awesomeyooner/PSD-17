#ifndef REGISTER_MANAGER_HPP
#define REGISTER_MANAGER_HPP

#include "motor_manager.hpp"
#include "wire_manager.hpp"

class RegisterManager{

    public:

        Command cmd_target = {
            .reg = (uint8_t)CommandType::TARGET,
            .length = 4,
            .runnable = [this](std::vector<uint8_t>* data) -> StatusCode{
                return StatusCode::FAILED;
            }
        };

        static RegisterManager instance;
        static RegisterManager* get_instance();

        void init(){
            
        }

    private:
        WireManager* wire_manager = WireManager::get_instance();
        MotorManager* motor_manager = MotorManager::get_instance();


}; // class RegisterManager

RegisterManager RegisterManager::instance;
RegisterManager* RegisterManager::get_instance(){
    return &instance;
}

#endif // REGISTER_MANAGER_HPP