#ifndef BUILTIN_LED_HPP
#define BUILTIN_LED_HPP

#include <Arduino.h>
#include "led.hpp"

#ifdef ESP32_DEVKIT_V1
    #define LED_PIN 2
#endif

#ifdef SAMD21_SEEED_XIAO
    #define LED_PIN 13
#endif

class BuiltinLED : public LED{

    private:

    public:

        BuiltinLED() : LED(LED_PIN){}

        static void initialize(){
            initialize();
        }

        static void turn_on(){
            turn_on();
        }
};


#endif // BUILTIN_LED_HPP