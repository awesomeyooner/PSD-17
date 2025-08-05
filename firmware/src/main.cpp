#include <Arduino.h>
#include <Wire.h>

#include "devices/led/builtin_led.hpp"

#define I2C_ADDR 4

BuiltinLED led;

void on_recieve(int bytes){
    while(Wire.available() > 0){
        int8_t c = Wire.read();

        Serial.println(c);
    }
}

void setup() {
    Serial.begin(115200);

    Wire.begin(I2C_ADDR);
    Wire.onReceive(on_recieve);

    led.initialize();
    led.turn_on();
}

void loop() {
    
}

void on_request(){

}

// master writes to slave asking WHAT to send
// slave recieves with on_recieve, logging the requested data
// master requests bytes
// slave on_request gets called and writes to master wahtever the WHAT is