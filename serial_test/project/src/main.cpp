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


int main(int argc, char* argv[])
{
    ImPlotter::init();

    SerialInterface serial;

    serial.init_field("product", "STM32 Virtual ComPort");
    
    while(System::is_alive())
    {
        auto angle_read = serial.request_data<double>(101, 500);
        auto vel_read = serial.request_data<double>(102, 500);

        if(!angle_read.is_OK() || !vel_read.is_OK())
        {
            Logger::error("Failed to read! Skipping iteration...");
            continue;
        }

        serial.write_data<double>(100, target_voltage);

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
            ImGui::SliderFloat("Voltage", &target_voltage, -12, 12, "%.3f V");
        };

        if(ImPlotter::update(add_inputs) == StatusCode::FAILED)
            System::shutdown();

        // Logger::info(angle_read.value);
    }

    serial.close();
    ImPlotter::shutdown();

    return 0;

} // end of "main(int, char*)"