#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

class Logger{
    
    public:
        static std::ofstream log_file;

        static bool initialize(){
            std::string datetime = get_date_time("-", "_");
            std::string file_name = "log_" + datetime + ".log";
            log_file.open("../logs/" + file_name);

            if(is_file_ok()){
                info("Program Started!");
                return true;
            }
            else{
                error("Failed to create .log file!");
                return false;
            }
        }

        static void close(){
            if(!is_file_ok())
                return;

            info("Program Ended!");
            log_file.close();
        }

        static void verbose(std::string text, bool is_verbose){
            if(verbose)
                log("INFO", text, false);
        }

        static void info(std::string text, bool write_file = true){
            log("INFO", text, write_file);
        }

        static void error(std::string text, bool write_file = true){
            log("ERROR", text, write_file);
        }

        static void debug(std::string text, bool write_file = true){
            log("DEBUG", text, write_file);
        }

        static void log(std::string header, std::string text, bool write_file = true){
            std::string formatted = format_text(header, text);

            if(write_file)
                write_to_file(formatted);

            std::cout << formatted << std::endl;
        }

        static std::string format_text(std::string header, std::string text){
            std::string datetime = get_date_time(":", " ");
            std::string formatted = datetime + " [" + header + "] " + text;

            return formatted;
        }

        static void write_to_file(std::string text){
            if(!log_file && !is_file_ok())
                return;

            log_file << text << std::endl;
        }

        static bool is_file_ok(){
            return log_file && log_file.is_open();
        }

        static std::string get_date_time(std::string unit_seperator = ":", std::string gap = "_"){
            time_t time = std::time(NULL);

            tm* datetime = std::localtime(&time);

            std::string year = std::to_string(datetime->tm_year + 1900);
            std::string month = datetime->tm_mon < 10 ? "0" + std::to_string(datetime->tm_mon) : std::to_string(datetime->tm_mon);
            std::string day = datetime->tm_mday < 10 ? "0" + std::to_string(datetime->tm_mday) : std::to_string(datetime->tm_mday);

            std::string hour = datetime->tm_hour < 10 ? "0" + std::to_string(datetime->tm_hour) : std::to_string(datetime->tm_hour);
            std::string min = datetime->tm_min < 10 ? "0" + std::to_string(datetime->tm_min) : std::to_string(datetime->tm_min);
            std::string sec = datetime->tm_sec < 10 ? "0" + std::to_string(datetime->tm_sec) : std::to_string(datetime->tm_sec);

            std::string all = year + unit_seperator + month + unit_seperator + day + gap + hour + unit_seperator + min + unit_seperator + sec;

            return all;
        }

    private:
       
}; // class Logger

std::ofstream Logger::log_file;

#endif // LOGGER_HPP