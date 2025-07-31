#include <Arduino.h>

#include <SimpleFOC.h>

#include "devices/led/builtin_led.hpp"

#define ENA 5
#define IN1 17
#define IN2 16 

#define IN3 13
#define IN4 12
#define ENB 14

BuiltinLED led;

StepperMotor motor = StepperMotor(50);
StepperDriver4PWM driver = StepperDriver4PWM(IN1, IN2, IN3, IN4, ENB, ENA);

// Commander command = Commander(Serial);

float vA = 0;
float vB = 0;

// void do_target(char* cmd){
//   command.scalar(&motor.target, cmd);
// }

// void change_vA(char* cmd){
//   command.scalar(&vA, cmd);
// }

// void change_vB(char* cmd){
//   command.scalar(&vB, cmd);
// }

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  SimpleFOCDebug::enable(&Serial);

  led.initialize();
  led.turn_on();

  driver.voltage_limit = 12;
  driver.voltage_power_supply = 25;
  driver.init();
  driver.enable();
  // motor.linkDriver(&driver);

  // motor.controller = MotionControlType::velocity_openloop;
  // motor.phase_resistance = 3.6;
  // motor.current_limit = 1;
  // motor.voltage_limit = 3;

  // motor.init();
  // motor.initFOC();

  // command.add('T', do_target, "Target Velocity");
  // command.add('A', change_vA, "Voltage for Phase A");
  // command.add('B', change_vB, "Voltage for Phase B");

  // Serial.begin(115200);
  // Serial.println("Motor Ready!");
  // Serial.println("Set target velocity [rad/s]");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

  // motor.loopFOC();
  // motor.move();
  // command.run();
  driver.setPwm(0, 0);
}
