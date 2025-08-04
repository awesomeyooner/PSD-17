#include <Arduino.h>
#include <Wire.h>
#include <SimpleFOC.h>

#include "devices/led/builtin_led.hpp"

#define IN1_A 0
#define IN1_B 1
#define IN2_A 2
#define IN2_B 3

BuiltinLED led;

StepperMotor motor = StepperMotor(50);
StepperDriver4PWM driver = StepperDriver4PWM(IN1_A, IN1_B, IN2_A, IN2_B);

Commander command = Commander(Serial);

void do_target(char* cmd){
  command.scalar(&motor.target, cmd);
}

void setup() {
  led.initialize();

  driver.voltage_limit = 12;
  driver.voltage_power_supply = 25;
  driver.init();

  motor.linkDriver(&driver);

  motor.foc_modulation = FOCModulationType::SinePWM;
  motor.controller = MotionControlType::velocity_openloop;
  motor.phase_resistance = 3.6;
  motor.current_limit = 1;
  motor.voltage_limit = 12;

  motor.init();
  motor.initFOC();

  command.add('T', do_target, "Target Velocity");
  
  Serial.begin(115200);
  SimpleFOCDebug::enable(&Serial);

  Serial.println("Motor Ready!");
  Serial.println("Set target velocity [rad/s]");

  delay(1000);
}

void loop() {
  motor.loopFOC();
  motor.move();
  command.run();
}