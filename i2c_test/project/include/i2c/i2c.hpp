#ifndef I2C_HPP
#define I2C_HPP

#include "i2c/i2c.h"

#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <cstring>
#include <cstdint>

#include "status.hpp"

class I2C{

    public:
        static int bus;

        static StatusCode init(const char* name = "/dev/i2c-9"){
            bus = i2c_open(name);

            if(bus == -1)
                return StatusCode::FAILED;
            else
                return StatusCode::OK;
        }

        static int get_bus(){
            return bus;
        }

        static float bytes_to_float(std::vector<uint8_t> v_bytes){
            uint8_t bytes[v_bytes.size()];
            std::copy(v_bytes.begin(), v_bytes.end(), bytes);

            return *reinterpret_cast<float*>(bytes);
        }

        static std::vector<uint8_t> float_to_bytes(float value){
            size_t float_size = sizeof(float);

            std::vector<uint8_t> bytes(float_size);

            memcpy(bytes.data(), &value, float_size);

            return bytes;
        }

        static StatusedValue<std::vector<uint8_t>> read_bus(i2c_device* device, size_t num_bytes){
            uint8_t buffer[num_bytes] = {};

            StatusCode status = i2c_read(device, 0, buffer, num_bytes) == num_bytes ? StatusCode::OK : StatusCode::FAILED;

            std::vector<uint8_t> vec(buffer, buffer + num_bytes);

            return StatusedValue<std::vector<uint8_t>>(vec, status);
        }

        static StatusedValue<float> read_bus(i2c_device* device){
            size_t float_size = sizeof(float);

            // Get the byte vector
            StatusedValue<std::vector<uint8_t>> read = read_bus(device, float_size);

            // Convert Byte Vector to Float
            float value = bytes_to_float(read.value);

            return StatusedValue<float>(value, read.status);
        }

        static StatusCode write_bus(i2c_device* device, uint8_t write){
            std::vector data = {write};

            write_bus(device, data);
        }

        static StatusCode write_bus(i2c_device* device, std::vector<uint8_t>& write){
            uint8_t buffer[write.size()];

            std::copy(write.begin(), write.end(), buffer);

            return i2c_write(device, 0, buffer, write.size()) == write.size() ? StatusCode::OK : StatusCode::FAILED;
        }

        static StatusCode write_bus(i2c_device* device, float data){
            
            // Convert Float to Byte Array
            std::vector<uint8_t> bytes = float_to_bytes(data);

            return write_bus(device, bytes);
        }

    private:


}; // class I2C

int I2C::bus = -1;

#endif // I2C_HPP