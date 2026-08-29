#include <iostream>

#include "plib/util/system.hpp"
#include "plib/util/logger.hpp"

#include "plib/util/string_util.hpp"

#include "CommiFaceLib/protocols/serial.hpp"
#include "CommiFaceLib/interfaces/communication_interface.hpp"

#include "PlotLib/scrolling_buffer.hpp"
#include "PlotLib/implot_plotter.hpp"

#include "plib/util/util.hpp"
#include "plib/util/system.hpp"


using namespace status_utils;
using namespace std;


float target_voltage = 0;

bool start_recording = false;

int main(int argc, char* argv[])
{
    Logger::init("csv");
    Logger::write_csv(
        {
            "Time (seconds)",
            "Encoder Angle (Radians)",
            "Fake Encoder Angle (Radians)",
            "Target Velocity (Radians)",
            "Encoder Velocity (Radians)"
        }
    );
    
    ImPlotter::init();

    SerialInterface serial;

    serial.init_field("product", "STM32 Virtual ComPort");

    auto input_voltage = serial.request_data<double>(103, 500);

    if(input_voltage.is_OK())
        cout << "Input Voltage: " << input_voltage.value << " V" << endl;
    
    while(System::is_alive())
    {

        auto angle_read = serial.request_data<double>(101, 500);
        auto vel_read = serial.request_data<double>(102, 500);

        auto fake_angle_read = serial.request_data<double>(104, 500);

        if(!angle_read.is_OK() || !vel_read.is_OK() || !fake_angle_read.is_OK())
        {
            Logger::error("Failed to read! Skipping iteration...");
            continue;
        }

        serial.write_data<double>(100, target_voltage);

        if(start_recording)
            Logger::write_csv(
                {
                    System::get_epoch(),
                    angle_read.value,
                    fake_angle_read.value,
                    target_voltage,
                    vel_read.value
                }
            );

        ImPlotter::push_data(
            angle_read.value,
            "AS5047 Angle (Radians)"
        );

        ImPlotter::push_data(
            vel_read.value,
            "AS5047 Velocity (Radians / sec)"
        );

        function<void()> add_inputs = []()
        {
            ImGui::SliderFloat("Voltage", &target_voltage, -24, 24, "%.3f V");

            ImGui::Checkbox("My Checkbox", &start_recording);
        };

        if(ImPlotter::update(add_inputs) == StatusCode::FAILED)
            System::shutdown();
    }

    serial.close();
    ImPlotter::shutdown();
    Logger::close();

    return 0;

} // end of "main(int, char*)"