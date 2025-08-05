#include <Arduino.h>
#include <Wire.h>

#include <string>

#include "devices/led/builtin_led.hpp"

#define I2C_ADDR 4

BuiltinLED led;

void on_recieve(int bytes){

    Serial.print("Recieved: ");

    while(Wire.available()){
        uint8_t c = Wire.read();

        Serial.print(c);
        Serial.print(" ");
    }

    Serial.println();
}

void on_request(){
    Serial.println("Request!");
    Wire.write(100);
    Wire.write(100);
    Wire.write(100);
}

void setup() {
    Serial.begin(115200);

    Wire.begin(I2C_ADDR);
    Wire.onReceive(on_recieve);
    Wire.onRequest(on_request);

    led.initialize();
    led.turn_on();
}

void loop() {
    
}


// master writes to slave asking WHAT to send
// slave recieves with on_recieve, logging the requested data
// master requests bytes
// slave on_request gets called and writes to master wahtever the WHAT is