#include <Arduino.h>
#include <Wire.h>
// #include "driver/i2c_slave.h"

#include <string>
#include <vector>

#include "devices/led/builtin_led.hpp"

#define PIN_SDA 21
#define PIN_SCL 22
#define I2C_ADDR 81
#define I2C_FREQUENCY 100000L

BuiltinLED led;

std::vector<uint8_t> buffer;

void on_recieve(int num_bytes){

    Serial.println("Recieve Called!");

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
    Serial.println("Request Called!");

    for(uint8_t c : buffer){
        Wire.write(c);
    }
}

void setup() {
    Serial.begin(115200);
    // Wire.setPins(PIN_SDA, PIN_SCL);

    Wire.onReceive(on_recieve);
    Wire.onRequest(on_request);

    if(Wire.begin(I2C_ADDR, PIN_SDA, PIN_SCL, I2C_FREQUENCY))
        Serial.println("Starting I2C...");
    else
        Serial.println("I2C Failed to Initialize!");

    led.initialize();
    led.turn_on();
}

void loop() {

}


// master writes to slave asking WHAT to send
// slave recieves with on_recieve, logging the requested data
// master requests bytes
// slave on_request gets called and writes to master wahtever the WHAT is