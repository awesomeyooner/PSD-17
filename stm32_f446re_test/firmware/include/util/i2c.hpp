#ifndef I2C_HPP
#define I2C_HPP

#include <Wire.h>
#include <string>
#include <vector>
#include <cstring>
#include <cstdint>

union FloatsBytesConverter{
    float f_value;
    uint8_t bytes[4];
} f_to_b;

class I2C{

    public:

        static float bytes_to_float(std::vector<uint8_t> v_bytes){
            f_to_b.bytes[0] = v_bytes.at(0);
            f_to_b.bytes[1] = v_bytes.at(1);
            f_to_b.bytes[2] = v_bytes.at(2);
            f_to_b.bytes[3] = v_bytes.at(3);

            return f_to_b.f_value;
        }

        static std::vector<uint8_t> float_to_bytes(float value){
            f_to_b.f_value = value;

            std::vector<uint8_t> bytes(
                {
                    f_to_b.bytes[0],
                    f_to_b.bytes[1],
                    f_to_b.bytes[2],
                    f_to_b.bytes[3]
                }
            );

            return bytes;
        }
        
}; // class I2C

#endif // I2C_HPP