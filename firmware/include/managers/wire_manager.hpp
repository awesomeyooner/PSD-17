#ifndef WIRE_MANAGER_HPP
#define WIRE_MANAGER_HPP

#include <Wire.h>
#include <map>
#include <unordered_map>

#include "i2c/i2c.hpp"
#include "i2c/wire_types.hpp"

class WireManager{

    public:
        static WireManager instance;

        static WireManager* get_instance();

        std::vector<uint8_t> read_buffer;
        std::vector<uint8_t> write_buffer;

        void init(int address){
            Wire.begin(address);
            Wire.onReceive(on_wire_recieve);
            Wire.onRequest(on_wire_request);
        }

        StatusCode update(uint8_t reg, std::vector<uint8_t>* data){
            if(request_map.empty() && command_map.empty())
                return StatusCode::FAILED;

            // if the register exists
            if(request_map.find(reg) != request_map.end()){
                // Serial.println("Request Map Called");
                return request_map.at(reg).runnable();
            }
            
            // if the register exists
            if(command_map.find(reg) != command_map.end()){
                // Serial.println("Command Map Called");
                return command_map.at(reg).runnable(data);
            }

            return StatusCode::FAILED;
        }

        std::vector<uint8_t>* get_read_buffer(){
            return &read_buffer;
        }
        
        std::vector<uint8_t>* get_write_buffer(){
            return &write_buffer;
        }

        static void on_wire_recieve(int num_bytes){

            if(num_bytes == 0)
                return;

            WireManager* wire_manager = WireManager::get_instance();

            wire_manager->read_buffer.clear();
            
            while(Wire.available()){
                uint8_t c = Wire.read();

                wire_manager->read_buffer.push_back(c);
            }

            uint8_t reg = wire_manager->read_buffer.at(0);

            // remove the register byte so that the vector is only the data
            wire_manager->read_buffer.erase(wire_manager->read_buffer.begin());

            // run the registers
            WireManager::get_instance()->update(reg, &WireManager::get_instance()->read_buffer);
        }

        static void on_wire_request(){
            for(uint8_t c : WireManager::get_instance()->write_buffer){
                Wire.write(c);
            }
        }

        void add_request(Request request){
            request_map.insert({request.reg, request});
        }

        void add_request(uint8_t reg, int length, std::function<StatusCode()> runnable){
            Request request = {
                .reg = reg,
                .length = length,
                .runnable = runnable
            };

            add_request(request);
        }

        void add_command(Command command){
            command_map.insert({command.reg, command});
        }

        void add_command(uint8_t reg, int length, std::function<StatusCode(std::vector<uint8_t>*)> runnable){
            Command command = {
                .reg = reg,
                .length = length,
                .runnable = runnable
            };

            add_command(command);
        }

    private:
        // register, runnable
        std::unordered_map<uint8_t, Request> request_map;
        std::unordered_map<uint8_t, Command> command_map;

}; // class WireManager

WireManager WireManager::instance;

WireManager* WireManager::get_instance(){
    return &instance;
}

#endif // WIRE_MANAGER_HPP