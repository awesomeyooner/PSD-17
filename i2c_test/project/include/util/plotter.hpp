#ifndef PLOTTER_HPP
#define PLOTTER_HPP

#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <unistd.h>
#include <thread>
#include <ctime>
#include <mutex>
#include <fstream>
#include <chrono>

#include "status.hpp"

class Plotter{

    public:
        static std::chrono::_V2::system_clock::time_point start;
        static FILE* gnuplot;

        static StatusCode initialize(std::string window_name = "Window", std::string plot_name = "Plot", std::string x_label = "X Axis", std::string y_label = "Y Axis"){
            start = std::chrono::high_resolution_clock::now();
            gnuplot = popen("gnuplot", "w");

            if(!gnuplot)
                return StatusCode::FAILED;

            fprintf(gnuplot, "set terminal qt title '%s'\n", window_name.c_str());
            fprintf(gnuplot, "set title '%s'\n", plot_name.c_str());
            fprintf(gnuplot, "set xlabel '%s'\n", x_label.c_str());
            fprintf(gnuplot, "set ylabel '%s'\n", y_label.c_str());
            fprintf(gnuplot, "set autoscale xy\n");
            fprintf(gnuplot, "set grid\n");

            std::ofstream file("data.txt");

            return StatusCode::OK;
        }

        /**
         * @brief Returns the current time since epoch
         * 
         * @return time since epoch in `seconds`
         */
        static double get_time(){
            auto now = std::chrono::high_resolution_clock::now();
            double time = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

            return time;
        }

        /**
         * @brief Pushes new data to `data.txt`
         * 
         * @param time X-Axis default (`time`)
         * @param data Y-Axis data
         */
        static void push_data(std::string time, std::string data){
            std::ofstream file("data.txt", std::ios::app);
            file << time << " " << data << std::endl;
            file.close();
        }

        /**
         * @brief Pushes new data to `data.txt`
         * 
         * @param data Y-Axis data
         */
        static void push_data(std::string data){
            push_data(std::to_string(get_time()), data);
        }

        /**
         * @brief Pushes new data to `data.txt`
         * 
         * @param data Y-Axis data
         */
        static void push_data(double data){
            push_data(std::to_string(get_time()), std::to_string(data));
        }
        
        /**
         * @brief Pushes new data to `data.txt`
         * 
         * @param data Y-Axis data
         */
        static void push_data(float data){
            push_data(std::to_string(get_time()), std::to_string(data));
        }

        /**
         * @brief Pushes new data to `data.txt`
         * 
         * @param data Y-Axis data
         */
        static void push_data(int data){
            push_data(std::to_string(get_time()), std::to_string(data));
        }

        /**
         * @brief Plots `data.txt`
         * 
         */
        static void plot(){
            fprintf(gnuplot, "plot 'data.txt' using 1:2 with lines title 'Y Axis'\n");
            fflush(gnuplot);
        }

        /**
         * @brief Pushes new data and plots `data.txt`
         * 
         * @param time X-Axis default (`time`)
         * @param data Y-Axis data
         */
        static void plot(std::string time, std::string data){
            push_data(time, data);

            fprintf(gnuplot, "plot 'data.txt' using 1:2 with lines title 'Current'\n");
            fflush(gnuplot);
        }

        /**
         * @brief Pushes new data and plots `data.txt`
         * 
         * @param time X-Axis default (`time`)
         * @param data Y-Axis data
         */
        static void plot(std::string data){
            plot(std::to_string(get_time()), data);
        }

        /**
         * @brief Pushes new data and plots `data.txt`
         * 
         * @param time X-Axis default (`time`)
         * @param data Y-Axis data
         */
        static void plot(double data){
            plot(std::to_string(data));
        }

        /**
         * @brief Pushes new data and plots `data.txt`
         * 
         * @param time X-Axis default (`time`)
         * @param data Y-Axis data
         */
        static void plot(float data){
            plot(std::to_string(data));
        }

        /**
         * @brief Pushes new data and plots `data.txt`
         * 
         * @param time X-Axis default (`time`)
         * @param data Y-Axis data
         */
        static void plot(int data){
            plot(std::to_string(data));
        }
 
        /**
         * @brief Closes the GNUPlot pipe
         * 
         */
        static void close(){
            pclose(gnuplot);
        }

    private:

    /*
    to plot multiple lines, you have to have multiple columsn in data.txt
        file << time << " " << data_a << " " << data_b << std::endl;

    then use
        fprintf(gnuplot, "plot 'data.txt' using 1:2 with lines title 'Data A', \
                'data.txt' using 1:3 with lines title 'Data B'\n");

    the 'using 1:2' 'using 1:3' means it's plotting column 1 against column 2, and 1 to 3. Since column 1 is time you are plotting column_n against time.
    */
        

}; // class Plotter

std::chrono::_V2::system_clock::time_point Plotter::start;
FILE* Plotter::gnuplot;

#endif // PLOTTER_HPP