#ifndef PSD17_HPP
#define PSD17_HPP

#include "i2c/i2c.hpp"
#include "i2c/wire_device.hpp"
#include "i2c/wire_types.hpp"

class PSD17 : public WireDevice{

    public:

        PSD17(int address) : WireDevice(address){}

        StatusCode send_target(float target){
            std::vector<uint8_t> data = I2C::float_to_bytes(target);

            data.insert(data.begin(), (uint8_t)CommandType::TARGET);

            return write(data);
        }

        StatusedValue<float> get_position(){
            std::vector<uint8_t> data;

            data.push_back((uint8_t)RequestType::POSITION);

            if(write(data) != StatusCode::OK)
                return StatusedValue<float>(0, StatusCode::FAILED);
            
            StatusedValue<std::vector<uint8_t>> read_value = read(sizeof(float));

            if(read_value.status != StatusCode::OK)
                return StatusedValue<float>(0, StatusCode::FAILED);

            float value = I2C::bytes_to_float(read_value.value);

            return StatusedValue<float>(value, StatusCode::OK);
        }

    private:

}; // class PSD17

#endif // PSD17_HPP