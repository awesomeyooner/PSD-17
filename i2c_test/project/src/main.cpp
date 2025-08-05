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

    if(I2C::init("/dev/i2c-4") == StatusCode::FAILED){
        std::cerr << "I2C Bus Failed to Initialize! Exiting..." << std::endl;
        return -1;
    }

    i2c_device device;
        device.addr = 4;
        device.bus = I2C::get_bus();
        device.iaddr_bytes = 0;
        device.page_bytes = 8; // max for MCP2221A
        device.tenbit = 0;
        device.flags = 0;
        device.delay = 10;

    while(true){

        std::string buffer;

        std::cout << "\nSay something!" << std::endl;

        std::getline(std::cin, buffer);

        if(buffer == "exit"){
            std::cout << "Exiting..." << std::endl;
            break;
        }

        uint8_t data[buffer.length()];

        std::cout << "You said: ";

        for(size_t i = 0; i < buffer.length(); i++){
            data[i] = buffer.at(i);
            
            std::cout << data[i];
        }

        std::cout << std::endl;

        if(sizeof(data) == 0){
            std::cout << "Nothing to send! Skipping..." << std::endl;
            continue;
        }

        StatusCode status = I2C::write_bus(&device, data, sizeof(data));
    
        if(status == StatusCode::OK)
            std::cout << "Sent Successfully!" << std::endl;
        else if(status == StatusCode::FAILED){
            std::cout << "Failed to Send!" << std::endl;
            continue;
        }

        StatusedValue<std::vector<uint8_t>> read = I2C::read_bus(&device, sizeof(data));

        if(read.is_OK())
            std::cout << "Read Successfully!" << std::endl;
        else{
            std::cout << "Failed to Read!" << std::endl;
            continue;
        }

        std::cout << "I heard: ";

        for(uint8_t r : read.value){
            std::cout << (int)r;
        }

        std::cout << std::endl;
    }
    
    return 0;
}