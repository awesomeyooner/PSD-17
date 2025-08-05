#ifndef I2C_HPP
#define I2C_HPP

#include "i2c/i2c.h"

#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <cstring>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <system_error>

#include "status.hpp"

namespace fs = std::filesystem;

class I2C{

    public:
        static int bus;

        static StatusCode init(const char* name = "/dev/i2c-9"){
            bus = i2c_open(name);

            if(bus == -1)
                return StatusCode::FAILED;
            else
                return StatusCode::OK;
        }

        static StatusCode init(int adapter_number){
            std::string name = "/dev/i2c-" + std::to_string(adapter_number);

            return init(name.c_str());
        }

        static StatusCode init_name(std::string name, bool verbose = false){
            const fs::path i2c_path = "/sys/bus/i2c/devices";

            for(auto entry : fs::directory_iterator(i2c_path)){
                std::string directory_name = entry.path().filename().string();

                if(verbose)
                    std::cout << "Found Folder: " << directory_name << "...";


                // If it's not an i2c device then skip
                if(directory_name.compare(0, 4, "i2c-") != 0){

                    if(verbose)
                        std::cout << "Not an i2c Device! Skipping..." << std::endl;

                    continue;
                }

                int adapter_number;
                
                try{
                    adapter_number = std::stoi(directory_name.substr(4));
                }
                catch(std::exception& e){

                    if(verbose)
                        std::cout << "Not a proper adapter! Skipping..." << std::endl;

                    continue;
                }

                auto name_file = entry.path() / "name";

                if(!fs::is_regular_file(name_file))
                    continue;

                std::ifstream file(name_file);

                if(!file.is_open())
                    continue;

                std::string adapter_name;

                std::getline(file, adapter_name);

                if(adapter_name.empty())
                    continue;

                // do something with name and number

                // if the name is found
                if(adapter_name.find(name) != std::string::npos){

                    if(verbose)
                        std::cout << "Found adapter " << adapter_name << " in: " << directory_name << std::endl;

                    return init(adapter_number);
                }
                else{
                    if(verbose)
                        std::cout << "Not the right adapter! Skipping..." << std::endl;
                }
            }

            return StatusCode::FAILED;
        }

        static int get_bus(){
            return bus;
        }

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

        static StatusedValue<std::vector<uint8_t>> read_bus(i2c_device* device, size_t num_bytes){
            uint8_t buffer[num_bytes] = {};

            StatusCode status = i2c_read(device, 0, buffer, num_bytes) == num_bytes ? StatusCode::OK : StatusCode::FAILED;

            std::vector<uint8_t> vec(buffer, buffer + num_bytes);

            return StatusedValue<std::vector<uint8_t>>(vec, status);
        }

        static StatusedValue<float> read_bus(i2c_device* device){
            size_t float_size = sizeof(float);

            // Get the byte vector
            StatusedValue<std::vector<uint8_t>> read = read_bus(device, float_size);

            // Convert Byte Vector to Float
            float value = bytes_to_float(read.value);

            return StatusedValue<float>(value, read.status);
        }

        static StatusCode write_bus(i2c_device* device, uint8_t write){
            std::vector data = {write};

            write_bus(device, data);
        }

        static StatusCode write_bus(i2c_device* device, std::vector<uint8_t>& write){
            uint8_t buffer[write.size()];

            std::copy(write.begin(), write.end(), buffer);

            return i2c_write(device, 0, buffer, write.size()) == write.size() ? StatusCode::OK : StatusCode::FAILED;
        }

        static StatusCode write_bus(i2c_device* device, float data){
            
            // Convert Float to Byte Array
            std::vector<uint8_t> bytes = float_to_bytes(data);

            return write_bus(device, bytes);
        }

    private:


}; // class I2C

int I2C::bus = -1;

#endif // I2C_HPP