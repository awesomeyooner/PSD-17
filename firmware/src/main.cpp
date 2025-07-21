#include <Arduino.h>

#include "devices/led/builtin_led.hpp"

BuiltinLED led;

void setup() {
    Serial.begin(115200);

    led.initialize();
    led.turn_on();

    Serial.println("Say Something!");
}

void loop() {
    
    if(Serial.available()){
        String message = Serial.readStringUntil('\n');

        Serial.println("I heard: " + message);
    }
}
