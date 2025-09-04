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

#define I2C_ADDR 4

WireManager* wire_manager = WireManager::get_instance();
MotorManager* motor_manager = MotorManager::get_instance();
RegisterManager* reg_manager = RegisterManager::get_instance();

Commander command = Commander(Serial);

void do_target(char* cmd){
  command.scalar(&motor_manager->get_motor()->target, cmd);
}

void setup() {
  Serial.begin(115200);

  BuiltinLED::initialize();

  wire_manager->init(I2C_ADDR);
  motor_manager->init();
  reg_manager->init();
  
  // Add Commands
  command.add('T', do_target, "Target Velocity");

  Serial.println("Motor Ready!");
  Serial.println("Set target velocity [rad/s]");

  delay(1000);
}

void loop() {
  motor_manager->update();
  command.run();
}
