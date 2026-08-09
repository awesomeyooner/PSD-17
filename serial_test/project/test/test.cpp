#include <iostream>
#include <cstdio>

#include "plib/util/system.hpp"

#include "PlotLib/scrolling_buffer.hpp"
#include "PlotLib/implot_plotter.hpp"


using namespace status_utils;


float input = 0;
float slider = 0;

float input_on_change = 0;


int main(int argc, char* argv[])
{
    // Initialize Plotter
    ImPlotter::init();

    // Loop until user requests to quit
    while (System::is_alive())
    {
        // Sample Data
        ImPlotter::push_data(sin(System::get_epoch()), "My First Line Plot");
        ImPlotter::push_data(cos(System::get_epoch()), "My Second Line Plot");

        // Add additional inputs
        std::function<void()> add_inputs = []()
        {
            ImGui::InputFloat("My Input", &input, 0.1f, 1.0f, "%.3f s");
            ImGui::SliderFloat("My Slider", &slider, 0, 100, "%.3f units");

            // Run code upon pressing enter
            if(ImGui::InputFloat("My Input Trigger", &input, 0.1f, 1.0f, "%.3f s", ImGuiInputTextFlags_EnterReturnsTrue))
            {
                std::cout << "Input Changed!" << std::endl;
            }
        };

        // If the user requests to quit, then shutdown
        if (ImPlotter::update(add_inputs) == status_utils::StatusCode::FAILED)
            System::shutdown();
    }

    // Shutdown Plotter
    ImPlotter::shutdown();

    return 0;
}