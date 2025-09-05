#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <unistd.h>
#include <thread>
#include <ctime>
#include <fstream>
#include <mutex>

#include "i2c/i2c.hpp"
#include "i2c/wire_device.hpp"
#include "i2c/drivers/psd17.hpp"

#include "util/util.hpp"
#include "math/math_helper.hpp"
#include "util/logger.hpp"
#include "util/plotter.hpp"

// i2cdetect -l

int main(int argc, char *argv[])
{
    // Code to be executed

    Logger::initialize();
    Plotter::initialize();

    if (I2C::init_name("MCP2221", true) == StatusCode::FAILED)
    {
        Logger::error("I2C Bus Failed to Initialize! Exiting...");
        return -1;
    }

    PSD17 motor(0x4);

    Logger::info("Initializing device with address: " + std::to_string(motor.get_address()));

    while (true)
    {

        // std::string buffer;

        // std::cout << "\nSay something!" << std::endl;

        // std::getline(std::cin, buffer);

        // if (buffer == "exit")
        // {
        //     std::cout << "Exiting..." << std::endl;
        //     break;
        // }

        // float target;

        // try{
        //     target = std::stof(buffer);
        // }
        // catch(std::exception e){
        //     std::cout << "Not a proper number! Skipping..." << std::endl;
        //     continue;
        // }

        // StatusCode send_status = motor.send_command(target, CommandType::TARGET);

        // if(send_status == StatusCode::OK){
        //     std::cout << "Target sent successfully!" << std::endl;
        // }
        // else{
        //     std::cout << "Failed to send target! Skipping..." << std::endl;
        //     continue;
        // }

        StatusedValue<float> position = motor.request(RequestType::CURRENT);

        // if(position.status == StatusCode::OK){
        //     std::cout << "Read Position Successfully!" << std::endl;
        // }
        // else{
        //     std::cout << "Failed to read position! Skipping..." << std::endl;
        //     continue;
        // }

        Plotter::plot(position.value);
    }

    Plotter::close();

    return 0;
}