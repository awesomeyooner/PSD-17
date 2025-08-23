#include <Arduino.h>
#include <Wire.h>

#include <string>
#include <vector>

#include "devices/led/builtin_led.hpp"

#define I2C_ADDR 4

BuiltinLED led;

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

    Wire.begin();
    led.initialize();
    led.turn_on();
}

void loop() {
    if(Serial.available()){
        String input = Serial.readStringUntil('\n');

        Serial.println("You said: " + input);

        Wire.beginTransmission(4);
        
        for(char c : input){
            Wire.write(c);
        }

        uint8_t status = Wire.endTransmission();

        if(status == 0){
            Serial.println("Sent Successfully!");
        }
        else{
            Serial.print("Sent Failed! Error Code: ");
            Serial.println((int)status);
        }

        
    }
}


// master writes to slave asking WHAT to send
// slave recieves with on_recieve, logging the requested data
// master requests bytes
// slave on_request gets called and writes to master wahtever the WHAT is