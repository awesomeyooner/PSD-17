#ifndef STATUS_HPP
#define STATUS_HPP

#include <iostream>
#include <string>
#include <cstdarg>
#include <vector>

enum class StatusCode{
    OK,
    ERROR, // use ERROR specifically for times that aren't necessarily fails
    FAILED // generally, use FAILED for times that don't call for OK
};

std::string status_to_string(StatusCode status){
    std::string output = "";

    switch(status){
        case StatusCode::OK:
            output = "OK";
            break;

        case StatusCode::ERROR:
            output = "ERROR";
            break;

        case StatusCode::FAILED:
            output = "FAILED";
            break;
    }

    return output;
}

bool statuses_OK(std::vector<StatusCode> statuses){
    
    for(StatusCode status : statuses){
        if(status != StatusCode::OK)
            return false;
    }

    return true;
}

void print_status(StatusCode status){
    std::string text = status_to_string(status);

    std::cout << text << std::endl;
}

template<typename T>

struct StatusedValue{

    T value;
    StatusCode status;
    
    StatusedValue(T _value, StatusCode _status) : value(_value), status(_status){}

    bool is_OK(){
        return status == StatusCode::OK;
    }

}; // struct StatusedValue

// template<typename T>

// struct StatusedArray{

//     T array[0] = {};
//     StatusCode status;

//     StatusedArray(T _array[], StatusCode _status) : status(_status){
//         array = _array;
//     }

// }; // struct StatusedArray

#endif // STATUS_HPP