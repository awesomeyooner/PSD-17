#include <Arduino.h>
#include <Wire.h>
#include <SimpleFOC.h>

#include "devices/led/builtin_led.hpp"

#define ENA 0
#define IN1 1
#define IN2 2
#define IN3 3
#define IN4 4
#define ENB 5

BuiltinLED led;

StepperMotor motor = StepperMotor(50);
StepperDriver4PWM driver = StepperDriver4PWM(IN1, IN2, IN3, IN4, ENA, ENB);

Commander command = Commander(Serial);

void do_target(char* cmd){
  command.scalar(&motor.target, cmd);
}

void setup() {
    Serial.begin(115200);
    SimpleFOCDebug::enable(&Serial);

    led.initialize();
    led.turn_on();

    driver.voltage_limit = 12;
    driver.voltage_power_supply = 24;
    driver.init();

    motor.linkDriver(&driver);

    motor.controller = MotionControlType::velocity_openloop;
    motor.phase_resistance = 3.6;
    motor.current_limit = 1;
    motor.voltage_limit = 12;

    motor.init();
    motor.initFOC();

    command.add('T', do_target, "Target Velocity");
    
    Serial.begin(115200);
    Serial.println("Motor Ready!");
    Serial.println("Set target velocity [rad/s]");

    led.initialize();
    led.turn_on();

    delay(1000);
}

void loop() {
    motor.move();
    command.run();
}