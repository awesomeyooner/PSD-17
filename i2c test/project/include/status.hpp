#ifndef STATUS_HPP
#define STATUS_HPP

#include <iostream>
#include <string>
#include <cstdarg>
#include <vector>

enum class StatusCode{
    OK,
    FAILED
};

std::string status_to_string(StatusCode status){
    std::string output = "";

    switch(status){
        case StatusCode::OK:
            output = "OK";
            break;

        case StatusCode::FAILED:
            output = "FAILED";
            break;
    }

    return output;
}

bool statuses_OK(std::vector<StatusCode> statuses){
    
    for(StatusCode status : statuses){
        if(status == StatusCode::FAILED)
            return false;
    }

    return true;
}

void print_status(StatusCode status){

    switch(status){
        case StatusCode::OK: 
            std::cout << "OK!" << std::endl;
            break;
        
        case StatusCode::FAILED: 
            std::cout << "FAILED!" << std::endl;
            break;
    }
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

template<typename T>

struct StatusedArray{

    T array[0] = {};
    StatusCode status;

    StatusedArray(T _array[], StatusCode _status) : status(_status){
        array = _array;
    }

}; // struct StatusedArray

#endif // STATUS_HPP