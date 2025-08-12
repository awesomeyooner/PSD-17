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

class BuiltinLED{

    public:

        static void initialize(){
            super().initialize();
        }

        static void turn_on(){
            super().turn_on();
        }

        static void turn_off(){
            super().turn_off();
        }

        static void toggle(bool turn_on){
            super().toggle(turn_on);
        }
    
    private:
        
        static LED& super(){
            static LED instance(LED_PIN);

            return instance;
        }
};


#endif // BUILTIN_LED_HPP