#include <cstdint>
#ifndef WIRE_TYPES_HPP
#define WIRE_TYPES_HPP

enum class CommandType : uint8_t{
    
    DISABLE_MOTOR = 0,
    ENABLE_MOTOR = 1,
    CLOSED_LOOP_VELOCITY = 2,
    CLOSED_LOOP_POSITION = 3,
    CLOSED_LOOP_TORQUE = 4,
    OPEN_LOOP_VELOCITY = 5
    // rest of bytes reserved

}; // enum class CommandType

enum class RequestType : uint8_t{

    VELOCITY = 255,
    POSITION = 254,
    CURRENT = 253,
    ERROR = 252
    // rest of bytes reserved

}; // enum class RequestType

#endif // WIRE_TYPES_HPP