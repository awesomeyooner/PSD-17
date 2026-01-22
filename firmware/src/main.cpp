#define STM32_NUCLEO64_F446RE

#include <Arduino.h>
#include <Wire.h>
#include <SimpleFOC.h>
#include <SimpleFOCDrivers.h>
#include "encoders/as5047/MagneticSensorAS5047.h"

#include <string>
#include <vector>

#include "i2c/i2c.hpp"
#include "managers/wire_manager.hpp"
#include "managers/motor_manager.hpp"
#include "managers/register_manager.hpp"
#include "devices/led/builtin_led.hpp"

#include "stm32f4xx_hal.h"
#include "current_sense/hardware_specific/stm32/stm32f4/stm32f4_hal.h"

#define I2C_ADDR 4


WireManager* wire_manager = WireManager::get_instance();
MotorManager* motor_manager = MotorManager::get_instance();
RegisterManager* reg_manager = RegisterManager::get_instance();

void setup() {
  Serial.begin(115200);

  BuiltinLED::initialize();

  wire_manager->init(I2C_ADDR);
  motor_manager->init();
  reg_manager->init();
  
  Serial.println("Motor Ready!");

  delay(1000);
}

void loop() {
  motor_manager->update();
}
