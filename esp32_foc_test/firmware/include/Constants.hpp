#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <Arduino.h>

struct SensoredMotorID{
    int enA, in1, in2; // L298N Channels
    int chA, chB; // Encoder Channels
}; // struct SensoredMotorID

const int TIMEOUT_MS = 500;
const int PWM_HERTZ = 100000; // 10 kHz
const int PWM_RESOLUTION = 8; // bits
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1); 

namespace MotorConstants{
    const int ENCODER_CPR = 12 * 4; //12ppr, 4 per
    const int GEAR_RATIO = 45;
    const int OUTPUT_SHAFT_CPR = ENCODER_CPR * GEAR_RATIO;
    const int MAX_RPM = 130;

    //robot specific stuff

        // L298N Configuration
        const int INDEX_MOTOR = 0; // 0, 1
        const int PIN_MOTOR_ENA = 5;
        const int PIN_MOTOR_IN1 = 17;
        const int PIN_MOTOR_IN2 = 16;

        // Quadrature Encoder Configuration
        const int PIN_MOTOR_CHA = 19;
        const int PIN_MOTOR_CHB = 18;

        // ID Struct
        const SensoredMotorID MOTOR_ID = {
            .enA = PIN_MOTOR_ENA,
            .in1 = PIN_MOTOR_IN1,
            .in2 = PIN_MOTOR_IN2,
            
            .chA = PIN_MOTOR_CHA,
            .chB = PIN_MOTOR_CHB
        };
}

#endif // CONSTANTS_HPP
