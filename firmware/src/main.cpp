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

#define I2C_ADDR 4

ADC_HandleTypeDef hadc1;

WireManager* wire_manager = WireManager::get_instance();
MotorManager* motor_manager = MotorManager::get_instance();
RegisterManager* reg_manager = RegisterManager::get_instance();

void config_adc_sampling_time(uint32_t cycles){
  ADC_ChannelConfTypeDef config = {0};

  config.Channel = ADC_CHANNEL_0;
  config.Rank = 1;
  config.SamplingTime = cycles;

  HAL_ADC_ConfigChannel(&hadc1, &config);
}

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
