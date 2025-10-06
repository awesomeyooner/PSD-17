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

bool keep_alive = true;
float target = 0;

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

    // Plotter Thread
    std::thread plot_thread(
        [](){
            while(keep_alive){
                Plotter::plot();
            }

            Logger::info("Shutting down Plotter...");
        }
    );
    plot_thread.detach();

    // User Input Thread
    std::thread input_thread(
        [](){
            while(keep_alive){
                auto input = util::get_user_input_float();

                if(input.status == StatusCode::ERROR)
                    keep_alive = false;
                else if(input.status != StatusCode::OK)
                    continue;

                target = input.value;
            }

            Logger::info("Shutting down input...");
        }
    );
    input_thread.detach();

    // I2C Communication Loop
    while (keep_alive){   
        StatusCode send_status = motor.send_command(target, CommandType::TARGET);

        if(send_status != StatusCode::OK)
            continue;

        StatusedValue<float> request = motor.request(RequestType::CURRENT);

        if(request.is_OK())
            Plotter::push_data(request.value);
    }

    Logger::info("Shutting down i2c communication...");

    Plotter::close();

    Logger::info("Exitting!");

    return 0;
}