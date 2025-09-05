#ifndef PSD17_HPP
#define PSD17_HPP

#include "i2c/i2c.hpp"
#include "i2c/wire_device.hpp"
#include "i2c/wire_types.hpp"

class PSD17 : public WireDevice{

    public:

        PSD17(int address) : WireDevice(address){}

        StatusCode disable(){
            return send_command(CommandType::DISABLE_MOTOR);
        }

        StatusCode enable(){
            return send_command(CommandType::ENABLE_MOTOR);
        }

        StatusedValue<bool> is_enabled(){
            StatusedValue<float> is_enabled = read_from_register((uint8_t)RequestType::IS_ENABLED);

            if(is_enabled.status != StatusCode::OK)
                return StatusedValue<bool>(false, StatusCode::FAILED);

            return StatusedValue<bool>((int)is_enabled.value == 0 ? false : true, StatusCode::OK);
        }

        StatusCode send_command(CommandType type){
            return write((uint8_t)type);
        }

        StatusCode send_command(float cmd, CommandType type){
            return write_to_register((uint8_t)type, cmd);
        }

        StatusedValue<float> request(RequestType type){
            return read_from_register((uint8_t)type);
        }

        

    private:

}; // class PSD17

#endif // PSD17_HPP