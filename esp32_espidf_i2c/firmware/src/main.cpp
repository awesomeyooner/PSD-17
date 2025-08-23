#define ESP32_DEVKIT_V1

#include <Arduino.h>
#include <Wire.h>
#include "esp32-hal-log.h"

#include "devices/led/builtin_led.hpp"
#include <esp_log.h>

#define PIN_SDA 21
#define PIN_SCL 22
#define I2C_ADDR 10
#define I2C_FREQUENCY 100000L

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

uint32_t i = 0;

void setup() {

  // Init System
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    delay(3000);
    BuiltinLED::initialize();

    Wire.onReceive(on_recieve);
    Wire.onRequest(on_request);

    Wire.begin(I2C_ADDR);
    // if(Wire.begin(I2C_ADDR, PIN_SDA, PIN_SCL, I2C_FREQUENCY))
    //     Serial.println("Starting I2C...");
    // else
    //     Serial.println("I2C Failed to Initialize!");

    #if CONFIG_IDF_TARGET_ESP32
        char message[64];
        snprintf(message, 64, "%lu Packets.", i++);
        Wire.slaveWrite((uint8_t *)message, strlen(message));
    #endif


  BuiltinLED::turn_on();
}

void loop() {

}