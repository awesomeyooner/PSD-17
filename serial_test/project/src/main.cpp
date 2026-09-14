#include <iostream>
#include <thread>
#include <chrono>

#include "plib/util/system.hpp"
#include "plib/util/logger.hpp"

#include "plib/util/string_util.hpp"

#include "plib/math/vector2d.hpp"

#include "CommiFaceLib/protocols/serial.hpp"
#include "CommiFaceLib/interfaces/communication_interface.hpp"

#include "PlotLib/scrolling_buffer.hpp"
#include "PlotLib/implot_plotter.hpp"

#include "plib/util/util.hpp"
#include "plib/util/system.hpp"


using namespace status_utils;
using namespace std;


float target_vd = 0;
float target_vq = 0;

bool start_recording = false;

int main(int argc, char* argv[])
{
    ImPlotter::init();

    SerialInterface serial;

    serial.init_field("product", "STM32 Virtual ComPort");

    // auto input_voltage = serial.request_data<double>(103, 500);

    // if(input_voltage.is_OK())
    //     cout << "Input Voltage: " << input_voltage.value << " V" << endl;
    
    while(System::is_alive())
    {
        try
        {
            this_thread::sleep_for(chrono::milliseconds(20));

            auto iA_read = serial.request_data<double>(101, 500);
            auto iB_read = serial.request_data<double>(102, 500);
            // auto angle_read = serial.request_data<double>(103, 500);
            // this_thread::sleep_for(chrono::milliseconds(5));

            serial.write_data<double>(99, target_vd);
            // this_thread::sleep_for(chrono::milliseconds(5));
            serial.write_data<double>(100, target_vq);
            // this_thread::sleep_for(chrono::milliseconds(5));

            Vector2d currents = {iA_read.value, iB_read.value};

            ImPlotter::push_data(
                currents.get_magnitude(),
                "Current Magnitude (Amps)"
            );

            ImPlotter::push_data(
                iA_read.value,
                "Phase A Current (Amps)"
            );

            ImPlotter::push_data(
                iB_read.value,
                "Phase B Current (Amps)"
            );

            // ImPlotter::push_data(
            //     angle_read.value,
            //     "Angle (Radians)"
            // );

            function<void()> add_inputs = []()
            {
                ImGui::SliderFloat("D-Voltage", &target_vd, -24, 24, "%.3f V");
                ImGui::SliderFloat("Q-Voltage", &target_vq, -24, 24, "%.3f V");
            };

            if(ImPlotter::update(add_inputs) == StatusCode::FAILED)
                System::shutdown();
        }
        catch(const exception& e)
        {
            Logger::error(e.what());
            System::shutdown();
        }
    }

    // serial.close();
    ImPlotter::shutdown();
    Logger::close();

    return 0;

} // end of "main(int, char*)"