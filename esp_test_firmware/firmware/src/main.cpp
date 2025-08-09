#include <Arduino.h>
#include <Wire.h>
#include <SimpleFOC.h>
#include <SimpleFOCDrivers.h>
#include "encoders/as5047/MagneticSensorAS5047.h"

#include "devices/led/builtin_led.hpp"

#define IN1_A 27
#define IN1_B 14
#define IN2_A 12
#define IN2_B 13

#define CS_PIN 2

BuiltinLED led;

StepperMotor motor = StepperMotor(50);
StepperDriver4PWM driver = StepperDriver4PWM(IN1_A, IN1_B, IN2_A, IN2_B);
MagneticSensorAS5047 sensor = MagneticSensorAS5047(CS_PIN);

Commander command = Commander(Serial);

void do_target(char* cmd){
  command.scalar(&motor.target, cmd);
}

void do_debug(char* cmd){
  command.scalar(&motor.voltage_limit, cmd);
}

void do_kP(char* cmd){
  command.scalar(&motor.PID_velocity.P, cmd);
  command.scalar(&motor.P_angle.P, cmd);
}

void do_kI(char* cmd){
  command.scalar(&motor.PID_velocity.I, cmd);
  command.scalar(&motor.P_angle.I, cmd);
}

void do_kD(char* cmd){
  command.scalar(&motor.PID_velocity.D, cmd);
  command.scalar(&motor.P_angle.D, cmd);
}

void do_kF(char* cmd){
  command.scalar(&motor.feed_forward_velocity, cmd);
}

void setup() {
  Serial.begin(115200);
  delay(3000);
  led.initialize();

  motor.useMonitoring(Serial);
  SimpleFOCDebug::enable(&Serial);

  sensor.init();
  sensor.min_elapsed_time = 0.005;
  motor.linkSensor(&sensor);

  driver.voltage_limit = 12;
  driver.voltage_power_supply = 25;
  driver.init();
  motor.linkDriver(&driver);

  motor.controller = MotionControlType::torque;
  motor.torque_controller = TorqueControlType::voltage;
  // motor.phase_resistance = 3.6;
  // motor.current_limit = 1;
  motor.voltage_limit = 12;
  motor.LPF_angle.Tf = 0.01;
  motor.LPF_velocity.Tf = 0.05;
  motor.init();
  motor.initFOC();

  command.add('T', do_target, "Target Velocity");
  command.add('E', do_debug, "Debug");
  command.add('P', do_kP, "Controller kP");
  command.add('I', do_kI, "Controller kI");
  command.add('D', do_kD, "Controller kD");
  command.add('F', do_kF, "Controller kF");

  Serial.println("Motor Ready!");
  Serial.println("Set target velocity [rad/s]");

  delay(1000);
}

void loop() {
  motor.loopFOC();
  motor.move();
  command.run();

  // Serial.print(motor.shaft_angle, 7);
  // Serial.print(",");
  // Serial.print(motor.shaft_velocity, 7);
  // Serial.print(",");
  // Serial.println(sensor.readMagnitude(), 7);
}