#ifndef WIRE_TYPES_HPP
#define WIRE_TYPES_HPP

#include <cstdint>
#include <functional>

#include "util/status.hpp"

enum class CommandType : uint8_t{
    
    DISABLE_MOTOR = 0,
    ENABLE_MOTOR = 1,
    RUN_FOC_INIT = 2,
    TARGET = 3,
    CLOSED_LOOP_VELOCITY = 4,
    CLOSED_LOOP_POSITION = 5,
    CLOSED_LOOP_TORQUE = 6,
    OPEN_LOOP_VELOCITY = 7,
    // rest of bytes reserved...

    CHANGE_CURRENT_LIMIT = 121,
    CHANGE_PHASE_RESISTANCE = 122,
    CHANGE_VOLTAGE_ALIGNMENT = 123,

    CHANGE_MOTOR_VOLTAGE_LIMIT = 124,
    CHANGE_DRIVER_VOLTAGE_LIMIT = 125,
    CHANGE_SUPPLY_VOLTAGE = 126,

    CHANGE_TORQUE_MODE = 127,

    CHANGE_VELOCITY_KP = 128,
    CHANGE_VELOCITY_KI = 129,
    CHANGE_VELOCITY_KD = 130,

    CHANGE_POSITION_KP = 131,
    CHANGE_POSITION_KI = 132,
    CHANGE_POSITION_KD = 133,
    CHANGE_POSITION_KF = 134,

    CHANGE_CURRENT_Q_KP = 135,
    CHANGE_CURRENT_Q_KI = 136,
    CHANGE_CURRENT_Q_KD = 137,

    CHANGE_CURRENT_D_KP = 138,
    CHANGE_CURRENT_D_KI = 139,
    CHANGE_CURRENT_D_KD = 140
    // rest of bytes reserved...

}; // enum class CommandType

enum class RequestType : uint8_t{

    TARGET = 255,
    VELOCITY = 254,
    POSITION = 253,
    CURRENT = 252,
    ERROR = 251
    // rest of bytes reserved

}; // enum class RequestType

// packet gets sent to slave with register and payload
// check register value and call the designated runnable with the payload as the parameter (vector)
struct Command{
    uint8_t reg; // register byte
    int length; // payload to recieve length
    std::function<StatusCode(std::vector<uint8_t>*)> runnable; // what to run when register is called

}; // struct Command

// packet gets sent to slave with only register
// check register value and call the designated runnable with the payload as the parameter (vector)
struct Request{
    uint8_t reg; // register byte
    int length; // payload to send length
    std::function<StatusCode()> runnable; // what to run when register is called
}; // struct Request

#endif // WIRE_TYPES_HPP