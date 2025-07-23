#ifndef I2C_HPP
#define I2C_HPP

#include "i2c/i2c.h"

#include <string>
#include <iostream>
#include <functional>
#include <vector>

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

        static StatusedArray<uint8_t> read_bus(i2c_device* device, size_t num_bytes){
            uint8_t buffer[num_bytes] = {};

            StatusCode status = i2c_ioctl_read(device, 0, buffer, sizeof(buffer)) == sizeof(buffer) ? StatusCode::OK : StatusCode::FAILED;

            return StatusedArray<uint8_t>(buffer, status);
        }

        static StatusCode write_bus(i2c_device* device, uint8_t write){
            uint8_t data[] = {write};
            write_bus(device, data);
        }

        static StatusCode write_bus(i2c_device* device, uint8_t write[]){
            return i2c_ioctl_write(device, 0, write, sizeof(write)) == sizeof(write) ? StatusCode::OK : StatusCode::FAILED;
        }


    private:


}; // class I2C

int I2C::bus = -1;

#endif // I2C_HPP