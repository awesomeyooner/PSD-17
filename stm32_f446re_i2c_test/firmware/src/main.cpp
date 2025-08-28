#define STM32_NUCLEO64_F446RE

#include <Arduino.h>
#include <Wire.h>

#include <string>
#include <vector>

#include "devices/led/builtin_led.hpp"

#define I2C_ADDR 4

std::vector<uint8_t> buffer;

void on_recieve(int num_bytes){

    if(num_bytes == 0)
        return;

    buffer.clear();
    
    while(Wire.available()){
        uint8_t c = Wire.read();

        buffer.push_back(c);
    }

    Serial.println("\nDecimal Values: ");

    for(uint8_t c : buffer){
        Serial.print(c);
        Serial.print(" ");
    }

    Serial.println("\nASCII Values: ");
    
    for(uint8_t c : buffer){
        Serial.print((char)c);
    }

    Serial.println();
}

void on_request(){
    for(uint8_t c : buffer){
        Wire.write(c);
    }
}

void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_ADDR);
  Wire.onReceive(on_recieve);
  Wire.onRequest(on_request);

  BuiltinLED::initialize();
}

void loop() {
  
}
