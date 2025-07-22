#include <Arduino.h>
#include <Wire.h>

#include "devices/led/builtin_led.hpp"

#define I2C_ADDR 4

BuiltinLED led;

void on_recieve(int bytes){
    while(Wire.available() > 0){
        char c = Wire.read();

        Serial.print(c);
    }
}

void setup() {
    Serial.begin(115200);

    Wire.begin(4);
    Wire.onReceive(on_recieve);

    led.initialize();
    led.turn_on();
}

void loop() {
    
}

void on_request(){

}