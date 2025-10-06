#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "status.hpp"

namespace util{

    void print(std::string text){
        std::cout << text << std::endl;
    }

    /**
     * @brief `std::to_string` but keeps precision of primitive data types
     * 
     * @param data Number to convert
     * @return std::string 
     */
    std::string to_string(double data){
        std::stringstream stream;

        stream << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10) << data;

        return stream.str();   
    }

    /**
     * @brief `std::to_string` but keeps precision of primitive data types
     * 
     * @param data Number to convert
     * @return std::string 
     */
    std::string to_string(float data){
        std::stringstream stream;

        stream << std::fixed << std::setprecision(std::numeric_limits<float>::max_digits10) << data;

        return stream.str();   
    }

    /**
     * @brief `std::to_string` but keeps precision of primitive data types
     * 
     * @param data Number to convert
     * @return std::string 
     */
    std::string to_string(int data){
        std::stringstream stream;

        stream << std::fixed << std::setprecision(std::numeric_limits<int>::max_digits10) << data;

        return stream.str();   
    }
    

    /**
     * @brief Gets user input as a string
     * 
     * @return `StatusedValue<std::string>` Returns `StatusCode::ERROR` when the message is `exit`, `StatusCode::OK` otherwise. 
     */
    StatusedValue<std::string> get_user_input_string(){
        std::string buffer;
        StatusCode status;

        std::cout << "\nSay something!" << std::endl;

        std::getline(std::cin, buffer);

        if(buffer == "exit")
            status = StatusCode::ERROR;
        else
            status = StatusCode::OK;

        return StatusedValue<std::string>(buffer, status);
    }

    /**
     * @brief Gets user input as a double
     * 
     * @return `StatusedValue<double>` Returns `StatusCode::ERROR` when the message is `exit`, `StatusCode::FAILED` if string failed to parse, `StatusCode::OK` if string properly parsed
     */
    StatusedValue<double> get_user_input_double(){
        std::string buffer;
        double value;
        StatusCode status = StatusCode::OK;

        std::cout << "\nSay something!" << std::endl;

        std::getline(std::cin, buffer);

        if (buffer == "exit"){
            status = StatusCode::ERROR;
            return StatusedValue<double>(value, status);
        }

        try{
            value = std::stod(buffer);
        }
        catch(std::exception e){
            status = StatusCode::FAILED;
        }

        return StatusedValue<double>(value, status);
    }

    /**
     * @brief Gets user input as a float
     * 
     * @return `StatusedValue<float>` Returns `StatusCode::ERROR` when the message is `exit`, `StatusCode::FAILED` if string failed to parse, `StatusCode::OK` if string properly parsed
     */
    StatusedValue<float> get_user_input_float(){
        std::string buffer;
        float value;
        StatusCode status = StatusCode::OK;

        std::cout << "\nSay something!" << std::endl;

        std::getline(std::cin, buffer);

        if (buffer == "exit"){
            status = StatusCode::ERROR;
            return StatusedValue<float>(value, status);
        }

        try{
            value = std::stof(buffer);
        }
        catch(std::exception e){
            status = StatusCode::FAILED;
        }

        return StatusedValue<float>(value, status);
    }

    /**
     * @brief Gets user input as an int
     * 
     * @return `StatusedValue<int>` Returns `StatusCode::ERROR` when the message is `exit`, `StatusCode::FAILED` if string failed to parse, `StatusCode::OK` if string properly parsed
     */
    StatusedValue<int> get_user_input_int(){
        std::string buffer;
        int value;
        StatusCode status = StatusCode::OK;

        std::cout << "\nSay something!" << std::endl;

        std::getline(std::cin, buffer);

        if (buffer == "exit"){
            status = StatusCode::ERROR;
            return StatusedValue<int>(value, status);
        }

        try{
            value = std::stoi(buffer);
        }
        catch(std::exception e){
            status = StatusCode::FAILED;
        }

        return StatusedValue<int>(value, status);
    }

} // namespace util

#endif // UTIL_HPP