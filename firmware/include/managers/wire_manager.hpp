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

        std::vector<uint8_t> buffer;

        void init(int address){
            Wire.begin(address);
            Wire.onReceive(on_wire_recieve);
            Wire.onRequest(on_wire_request);
        }

        StatusCode update(uint8_t reg, std::vector<uint8_t>* data){
            if(request_map.empty() && command_map.empty())
                return StatusCode::FAILED;

            try{
                return request_map.at(reg).runnable();
            }
            catch(std::out_of_range e){
                try{
                    return command_map.at(reg).runnable(data);
                }
                catch(std::out_of_range e){
                    return StatusCode::FAILED;
                }
            }
        }

         static void on_wire_recieve(int num_bytes){

            if(num_bytes == 0)
                return;

            WireManager::get_instance()->buffer.clear();
            
            while(Wire.available()){
                uint8_t c = Wire.read();

                WireManager::get_instance()->buffer.push_back(c);
            }

            uint8_t reg = WireManager::get_instance()->buffer.at(0);

            WireManager::get_instance()->update(reg, &WireManager::get_instance()->buffer);
        }

        static void on_wire_request(){
            for(uint8_t c : WireManager::get_instance()->buffer){
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