#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <thread>
#include <mutex>

#include "i2c/i2c.hpp"
#include "i2c/wire_device.hpp"

#include "util/util.hpp"
#include "math/math_helper.hpp"
#include "util/logger.hpp"

// i2cdetect -l

int main(int argc, char *argv[])
{
    // Code to be executed

    Logger::initialize();

    if (I2C::init_name("MCP2221", true) == StatusCode::FAILED)
    {
        Logger::error("I2C Bus Failed to Initialize! Exiting...");
        return -1;
    }

    WireDevice device(I2C::get_bus(), 0x4);

    Logger::info("Initializing device with address: " + std::to_string(device.get_address()));

    while (true)
    {

        std::string buffer;

        std::cout << "\nSay something!" << std::endl;

        std::getline(std::cin, buffer);

        if (buffer == "exit")
        {
            std::cout << "Exiting..." << std::endl;
            break;
        }

        // float f = std::stof(buffer);

        // std::vector<uint8_t> data = I2C::float_to_bytes(f);
        // std::vector<uint8_t> data(buffer.begin(), buffer.end());
        std::vector<uint8_t> data;

        data.push_back(253);

        if (data.size() == 0)
        {
            std::cout << "Nothing to send! Skipping..." << std::endl;
            continue;
        }

        // Write Bus
        StatusCode status = device.write(data);

        if (status == StatusCode::OK)
            std::cout << "Sent Successfully!" << std::endl;
        else if (status == StatusCode::FAILED)
        {
            std::cout << "Failed to Send!" << std::endl;
            continue;
        }

        // Read Bus
        StatusedValue<std::vector<uint8_t>> read = device.read(sizeof(float));

        if (read.is_OK())
            std::cout << "Read Successfully!" << std::endl;
        else
        {
            std::cout << "Failed to Read!" << std::endl;
            continue;
        }

        // Decimal Print (write)
        std::cout << "Decimal Value: ";
        for (uint8_t c : data)
        {
            std::cout << (int)c << " ";
        }
        std::cout << std::endl;

        // Decimal Print (read)
        std::cout << "Decimal Value: ";
        for (uint8_t r : read.value)
        {
            std::cout << (int)r << " ";
        }
        std::cout << std::endl;

        // ASCII print (write)
        std::cout << "ASCII Value: ";
        for (uint8_t c : data)
        {
            std::cout << (char)c;
        }
        std::cout << std::endl;

        // ASCII Print (read)
        std::cout << "ASCII Value: ";
        for (uint8_t r : read.value)
        {
            std::cout << (char)r;
        }
        std::cout << std::endl;
    }

    return 0;
}