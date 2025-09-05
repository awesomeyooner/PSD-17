#ifndef MOTOR_MANAGER_HPP
#define MOTOR_MANAGER_HPP

#include <Arduino.h>
#include <SimpleFOC.h>
#include <SimpleFOCDrivers.h>
#include "encoders/as5047/MagneticSensorAS5047.h"

#define IN1_A PA8
#define IN1_B PB10
#define IN2_A PB4
#define IN2_B PB5

#define CS_PIN PA15

#define CURRENT_SENSE_A PC4
#define CURRENT_SENSE_B PB1

class MotorManager{

    public:
        static MotorManager instance;
        static MotorManager* get_instance();

        void init(){
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
            current_sensor.linkDriver(&driver);
            current_sensor.init();
            motor.linkCurrentSense(&current_sensor);

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
        }

        void update(){
            motor.loopFOC();
            motor.move();
        }

        StepperMotor* get_motor(){
            return &motor;
        }

        StepperDriver4PWM* get_driver(){
            return &driver;
        }

        MagneticSensorAS5047* get_sensor(){
            return &sensor;
        }

        LowsideCurrentSense* get_current_sensor(){
            return &current_sensor;
        }

    private:
        StepperMotor motor = StepperMotor(50);
        StepperDriver4PWM driver = StepperDriver4PWM(IN1_A, IN1_B, IN2_A, IN2_B);
        MagneticSensorAS5047 sensor = MagneticSensorAS5047(CS_PIN);
        LowsideCurrentSense current_sensor = LowsideCurrentSense(1100, CURRENT_SENSE_A, CURRENT_SENSE_B);

}; // class MotorManager

MotorManager MotorManager::instance;

MotorManager* MotorManager::get_instance(){
    return &instance;
}


#endif // MOTOR_MANAGER_HPP