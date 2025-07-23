#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <thread>
#include <mutex>

// #include "bno055.hpp"

#include "i2c/i2c.hpp"

#include "util.hpp"
#include "math/math_helper.hpp"

// i2cdetect -l

int main(int argc, char* argv[]) {
    // Code to be executed

    // if(I2C::init("/dev/i2c-9") == StatusCode::FAILED)
    //     return -1;

    // i2c_device device;
    //     device.addr = 4;
    //     device.bus = I2C::get_bus();
    //     device.iaddr_bytes = 1;
    //     device.page_bytes = 1;

    // while(true){

    //     std::string buffer;

    //     std::cout << "Say something!" << std::endl;

    //     std::getline(std::cin, buffer);

    //     if(buffer == "exit")
    //         break;

    //     uint8_t data[buffer.length()];

    //     for(size_t i = 0; i < buffer.length(); i++){
    //         data[i] = buffer.at(i);
    //     }

    //     StatusCode status = I2C::write_bus(&device, data);
    
    //     if(status == StatusCode::OK)
    //         std::cout << "Sent Successfully!" << std::endl;
    //     else if(status == StatusCode::FAILED)
    //         std::cout << "Failed to Send!" << std::endl;

    //     // std::cout << "You said: " << buffer << std::endl;
    // }

    
    
    return 0;
}