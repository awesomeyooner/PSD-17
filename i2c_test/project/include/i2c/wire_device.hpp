#ifndef WIRE_DEVICE_HPP
#define WIRE_DEVICE_HPP

#include "i2c/i2c.h"

#include <string>
#include <iostream>
#include <functional>
#include <vector>

#include "i2c.hpp"
#include "util/status.hpp"

class WireDevice{

    public:

        WireDevice(int address, int bus = I2C::get_bus(), int page_bytes = 8){
            device.addr = address;
            device.bus = bus;
            device.page_bytes = page_bytes; // max for MCP2221A
            device.iaddr_bytes = 0;
            device.tenbit = 0;
            device.flags = 0;
            device.delay = 10;
        }

        int get_address(){
            return device.addr;
        }

        i2c_device* get(){
            return &device;
        }

        StatusedValue<float> read_from_register(uint8_t reg){
            std::vector<uint8_t> data;
            data.push_back(reg);

            if(write(data) != StatusCode::OK)
                return StatusedValue<float>(0, StatusCode::FAILED);
            
            StatusedValue<std::vector<uint8_t>> read_status = read(sizeof(float));

            if(read_status.status != StatusCode::OK)
                return StatusedValue<float>(0, StatusCode::FAILED);

            float val = I2C::bytes_to_float(read_status.value);

            return StatusedValue<float>(val, StatusCode::OK);
        }

        StatusCode write_to_register(uint8_t reg, float val){
            std::vector<uint8_t> data = I2C::float_to_bytes(val);
            data.insert(data.begin(), reg);

            return write(data);
        }

        StatusedValue<std::vector<uint8_t>> read(size_t num_bytes){
            return I2C::read_bus(&device, num_bytes);
        }

        StatusedValue<float> read(){
            return I2C::read_bus(&device);
        }

        StatusCode write(uint8_t write){
            return I2C::write_bus(&device, write);
        }

        StatusCode write(std::vector<uint8_t>& write){
            return I2C::write_bus(&device, write);
        }

        StatusCode write(float data){
            return I2C::write_bus(&device, data);
        }

    private:

        i2c_device device;

}; // class WireDevice

#endif // WIRE_DEVICE_HPP