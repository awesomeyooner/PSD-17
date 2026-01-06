#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <unistd.h>
#include <thread>
#include <ctime>
#include <fstream>
#include <mutex>

#include "plib/i2c/i2c.hpp"
#include "plib/i2c/wire_device.hpp"
#include "plib/i2c/drivers/psd17.hpp"

#include "plib/math/math_helper.hpp"

#include "plib/util/util.hpp"
#include "plib/util/system.hpp"
#include "plib/util/logger.hpp"
#include "plib/util/implot_plotter.hpp"

// i2cdetect -l

float target = 0;

int main(int argc, char *argv[])
{
    // Code to be executed
    Logger::initialize();
    ImPlotter::initialize();

    if (I2C::init_name("MCP2221", true) == status_utils::StatusCode::FAILED)
    {
        Logger::error("I2C Bus Failed to Initialize! Exiting...");
        return -1;
    }

    PSD17 motor(0x4);

    Logger::info("Initializing device with address: " + std::to_string(motor.get_address()));

    // Plotter Thread
    std::thread plot_thread(
        []()
        {
            while(System::is_alive())
            {
                if(ImPlotter::update() == status_utils::StatusCode::FAILED)
                    System::shutdown();
            }

            Logger::info("Shutting down Plotter...");
        }
    );
    plot_thread.detach();
    
    // User Input Thread
    std::thread input_thread(
        []()
        {
            while(System::is_alive()){
                auto input = util::get_user_input_float();

                if(input.status == status_utils::StatusCode::FAILED)
                    System::shutdown();
                else if(input.status != status_utils::StatusCode::OK)
                    continue;

                target = input.value;
            }

            Logger::info("Shutting down input...");
        }
    );
    input_thread.detach();


    Logger::info("Enabling Motor...");
    if(motor.enable() == status_utils::StatusCode::OK)
        Logger::info("Enabled Motor!");
    else
        Logger::info("Failed to Enabled Motor!");
    
    // I2C Communication Loop
    while (System::is_alive())
    {   
        status_utils::StatusCode send_status = motor.send_command(target, CommandType::TARGET);

        if(send_status != status_utils::StatusCode::OK)
            continue;

        status_utils::StatusedValue<float> request = motor.request(RequestType::CURRENT);

        if(request.is_OK())
            ImPlotter::push_data(request.value);
    }

    Logger::info("Shutting down i2c communication...");

    ImPlotter::shutdown();

    Logger::info("Exitting!");

    return 0;
}