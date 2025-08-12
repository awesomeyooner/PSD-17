#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <Wire.h>
#include <string>
#include <vector>
#include <cstring>
#include <cstdint>
#include <map>
#include <functional>

struct Register{
    uint8_t register_byte;
    

}; // struct Register

class RegisterMap : public std::map<uint8_t, std::function<void()>>{

}; // class RegisterMap

#endif // REGISTER_HPP