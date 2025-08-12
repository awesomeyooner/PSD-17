#ifndef I2C_HPP
#define I2C_HPP

#include <Wire.h>
#include <string>
#include <vector>
#include <cstring>
#include <cstdint>

class I2C{

    public:

        static float bytes_to_float(std::vector<uint8_t> v_bytes){
            uint8_t bytes[v_bytes.size()];
            std::copy(v_bytes.begin(), v_bytes.end(), bytes);

            return *reinterpret_cast<float*>(bytes);
        }

        static std::vector<uint8_t> float_to_bytes(float value){
            size_t float_size = sizeof(float);

            std::vector<uint8_t> bytes(float_size);

            memcpy(bytes.data(), &value, float_size);

            return bytes;
        }
        
}; // class I2C

#endif // I2C_HPP