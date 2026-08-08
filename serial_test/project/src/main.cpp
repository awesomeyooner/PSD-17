#include <iostream>

#include "plib/util/system.hpp"
#include "plib/util/logger.hpp"

#include "plib/util/string_util.hpp"

#include "CommiFaceLib/protocols/serial.hpp"
#include "CommiFaceLib/interfaces/communication_interface.hpp"

#include "PlotLib/gnu_plotter.hpp"
#include "PlotLib/implot_plotter.hpp"

#include "plib/util/util.hpp"
#include "plib/util/system.hpp"

int main(int argc, char* argv[])
{
    ImPlotter::initialize();

    SerialInterface serial;

    serial.init_field("product", "STM32 Virtual ComPort");
    
    while(System::is_alive())
    {
        auto angle_read = serial.request_data<double>(101, 500);

        if(!angle_read.is_OK())
        {
            Logger::error("Failed to read! Skipping iteration...");
            continue;
        }

        double voltage = 5;

        serial.write_data<double>(100, voltage);

        Logger::debug(voltage);

        ImPlotter::push_data(
            angle_read.value,
            "AS5047 Angle (Radians)"
        );

        if(ImPlotter::update() == StatusCode::FAILED)
            System::shutdown();

        // Logger::info(angle_read.value);
    }

    serial.close();
    ImPlotter::shutdown();

    return 0;

} // end of "main(int, char*)"