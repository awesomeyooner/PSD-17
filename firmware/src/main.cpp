#define STM32_NUCLEO64_F446RE

#include <Arduino.h>
#include <Wire.h>
#include <SimpleFOC.h>
#include <SimpleFOCDrivers.h>
#include "encoders/as5047/MagneticSensorAS5047.h"

#include <string>
#include <vector>

#include "util/i2c.hpp"
#include "devices/led/builtin_led.hpp"

#define I2C_ADDR 4

#define IN1_A PA8
#define IN1_B PB10
#define IN2_A PB4
#define IN2_B PB5

#define CS_PIN PA15

#define CURRENT_SENSE_A PC4
#define CURRENT_SENSE_B PB1

StepperMotor motor = StepperMotor(50);
StepperDriver4PWM driver = StepperDriver4PWM(IN1_A, IN1_B, IN2_A, IN2_B);
MagneticSensorAS5047 sensor = MagneticSensorAS5047(CS_PIN);
// LowsideCurrentSense current_sensor = LowsideCurrentSense(1100, CURRENT_SENSE_A, CURRENT_SENSE_B);

Commander command = Commander(Serial);

std::vector<uint8_t> buffer;

void on_wire_recieve(int num_bytes){

    if(num_bytes == 0)
        return;

    buffer.clear();
    
    while(Wire.available()){
        uint8_t c = Wire.read();

        buffer.push_back(c);
    }

    // float f = I2C::bytes_to_float(buffer);

    // Serial.print("Float Value: ");
    // Serial.println(f);

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

void on_wire_request(){
    for(uint8_t c : buffer){
        Wire.write(c);
    }
}

void do_target(char* cmd){
  command.scalar(&motor.target, cmd);
}

void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_ADDR);
  Wire.onReceive(on_wire_recieve);
  Wire.onRequest(on_wire_request);

  BuiltinLED::initialize();

  motor.useMonitoring(Serial);
  SimpleFOCDebug::enable(&Serial);

    // Init Magnetic Encoder
  sensor.init();
  sensor.min_elapsed_time = 0.005;
  motor.linkSensor(&sensor);

  // Init Driver
  driver.voltage_limit = 24;
  driver.voltage_power_supply = 24;
  driver.init();
  motor.linkDriver(&driver);

  // Init Current Sensor
//   current_sensor.linkDriver(&driver);
//   current_sensor.init();
//   motor.linkCurrentSense(&current_sensor);

  // Motor Settings
  motor.controller = MotionControlType::torque;
  motor.torque_controller = TorqueControlType::voltage;
  motor.voltage_sensor_align = 16;
  motor.phase_resistance = 3.6;
  motor.current_limit = 1;
  motor.voltage_limit = 24;
  motor.LPF_angle.Tf = 0.01;
  motor.LPF_velocity.Tf = 0.01;

  // Init Motor
  motor.init();
  motor.initFOC();

  // Add Commands
  command.add('T', do_target, "Target Velocity");

  Serial.println("Motor Ready!");
  Serial.println("Set target velocity [rad/s]");

  delay(1000);
}

void loop() {
  motor.loopFOC();
  motor.move();
  command.run();
}
