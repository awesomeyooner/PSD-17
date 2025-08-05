#ifndef WIRE_DEVICE_HPP
#define WIRE_DEVICE_HPP

#include "i2c/i2c.h"

#include <string>
#include <iostream>
#include <functional>
#include <vector>

#include "i2c.hpp"
#include "status.hpp"

class WireDevice{

    public:

        WireDevice(int bus, int address, int page_bytes = 8){
            device.addr = address;
            device.bus = bus;
            device.page_bytes = page_bytes; // max for MCP2221A
            device.iaddr_bytes = 0;
            device.tenbit = 0;
            device.flags = 0;
            device.delay = 10;
        }

        i2c_device* get(){
            return &device;
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