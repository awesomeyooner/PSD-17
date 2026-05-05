#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <unistd.h>
#include <thread>
#include <ctime>
#include <fstream>
#include <mutex>

// #include "plib/i2c/i2c.hpp"
// #include "plib/i2c/wire_device.hpp"
// #include "plib/i2c/drivers/psd17.hpp"

#include "plib/math/math_helper.hpp"

#include "plib/util/util.hpp"
#include "plib/util/system.hpp"
#include "plib/util/logger.hpp"
#include "plib/util/implot_plotter.hpp"

#include "plib/util/byte_converter.hpp"

#include <libserial/SerialPort.h>

// i2cdetect -l

int main(int argc, char *argv[])
{
    // Code to be executed
    Logger::initialize();
    // ImPlotter::initialize();
    // ImPlotter::m_axis_flags = ImPlotAxisFlags_AutoFit;

    // if (I2C::init_name("MCP2221", true) == status_utils::StatusCode::FAILED)
    // {
    //     Logger::error("I2C Bus Failed to Initialize! Exiting...");
    //     return -1;
    // }

    // PSD17 motor(0x4);
    // float target = 0;

    // Logger::info("Initializing device with address: " + std::to_string(motor.get_address()));

    LibSerial::SerialPort connection;

    // ls -l /dev/serial/by-id/
    connection.Open("/dev/ttyACM0");
    connection.SetBaudRate(LibSerial::BaudRate::BAUD_115200);


    // User Input Thread
    std::thread input_thread(
        [&connection]()
        {
            Logger::info("Spinning up the User Input Thread...");

            while(System::is_alive())
            {
                auto reg = util::get_user_input_int("Please enter a register: ");
                auto input = util::get_user_input_string("Please Enter text: ");

                int length = input.value.length() + 2; // 1 for register, 1 for length

                std::vector<uint8_t> bytes;

                bytes.push_back((uint8_t)reg.value);
                bytes.push_back((uint8_t)length);
                bytes.insert(bytes.begin() + 2, input.value.begin(), input.value.end());

                if(input.status == status_utils::StatusCode::FAILED)
                {
                    System::shutdown();
                    break;
                }
                else if(input.status != status_utils::StatusCode::OK)
                    continue;

                std::cout << input.value.length() << " : " << input.value << std::endl;

                connection.Write(bytes);
            }

            Logger::info("Shutting down input...");
        }
    );
    input_thread.detach();

    // std::thread i2c_thread(
    //     []()
    //     {
    //         Logger::info("Spinning up the i2c Thread...");

    //         // Logger::info("Enabling Motor...");

    //         // if(motor.enable() == status_utils::StatusCode::OK)
    //         //     Logger::info("Enabled Motor!");
    //         // else
    //         // {
    //         //     Logger::info("Failed to Enabled Motor! Exiting...");
    //         //     System::shutdown();
    //         // }

    //         while(System::is_alive())
    //         {
    //             status_utils::StatusCode send_status = motor.send_command(target, CommandType::TARGET);

    //             if(send_status != status_utils::StatusCode::OK)
    //                 continue;

    //             // status_utils::StatusedValue<float> position = motor.request(RequestType::POSITION);

    //             // if(position.is_OK())
    //             //     ImPlotter::push_data(position.value, "Position");

    //             // status_utils::StatusedValue<float> velocity = motor.request(RequestType::VELOCITY);

    //             // if(velocity.is_OK())
    //             //     ImPlotter::push_data(velocity.value, "Velocity");

    //             // status_utils::StatusedValue<float> current = motor.request(RequestType::CURRENT);

    //             // if(current.is_OK())
    //             //     ImPlotter::push_data(current.value, "Current");

    //             // status_utils::StatusedValue<float> angle = motor.read_from_register(25);

    //             // if(angle.is_OK())
    //             //     ImPlotter::push_data(angle.value, "Angle (Rad)");

    //             // status_utils::StatusedValue<float> mag = motor.read_from_register(26);

    //             // if(mag.is_OK())
    //             //     ImPlotter::push_data(mag.value, "Magnet Strength");
    //         }

    //         Logger::info("Shutting down i2c communication...");
    //     }
    // );
    // i2c_thread.detach();

    // Plotter (has to be in main, not lambda function)
    while(System::is_alive())
    {

        if(connection.IsDataAvailable())
        {
            std::string read_data;

            try
            {
                std::cout << "Receieved data, reading..." << std::endl;
                connection.ReadLine(read_data, '\n', 500);
                // connection.Read(read_data, 8, 0);

                std::cout << read_data << std::endl;
                // std::vector<uint8_t> bytes(read_data.begin(), read_data.end());

                // double data = ByteConverter::bytes_to_double(bytes);

                // std::cout << "I heard: " << util::to_string(data) << std::endl;
                
            }
            catch(...)
            {
                std::cout << "failed" << std::endl;
                continue;
            }

            // std::cout << "I heard: " + read_data << std::endl;
        }
    }

    // ImPlotter::shutdown();

    connection.Close();

    Logger::info("Exitting!");

    return 0;
}