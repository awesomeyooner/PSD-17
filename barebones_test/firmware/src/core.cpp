#include "core.hpp"

#include "EmbeddedLib/system.hpp"
#include "EmbeddedLib/devices/led.hpp"
#include "EmbeddedLib/devices/ws2812b.hpp"
#include "EmbeddedLib/devices/gpio_device.hpp"
#include "EmbeddedLib/math/math_util.hpp"

#include "WireLib/communication/wire_manager.hpp"
#include "WireLib/communication/protocols/serial_interface.hpp"
#include "WireLib/util/byte_converter.hpp"

#include "ActionLib/ActionManager.hpp"

#include "devices/as5047.hpp"
#include "devices/l298n.hpp"

#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

#include "stm32f4xx_hal_tim.h"

#include <stdio.h>
#include <functional>


using namespace status_utils;
using namespace math;
using namespace std;


const double DRIVER_INPUT_VOLTAGE = 24; // V


GPIODevice led = GPIODevice(GPIOC, GPIO_PIN_1);

L298N phase_A = L298N(&htim3, TIM_CHANNEL_3, TIM_CHANNEL_4);
L298N phase_B = L298N(&htim3, TIM_CHANNEL_1, TIM_CHANNEL_2);

AS5047 as5047 = AS5047(&hspi1, GPIOD, GPIO_PIN_2);

double offset = 0;


void inverse_park(double el_angle, double vd, double vq = 0);
void step(double voltage = 12, double delay_ms = 20);
double get_electrical_angle();
double get_angle_offset(double voltage = 12);


double pole_pairs = 0;


void get_pole_pairs()
{
    HAL_Delay(500);

    as5047.refresh();
    double starting = as5047.get_angle();

    int steps = 10;

    for(int i = 0; i < steps; i++)
    {
        as5047.refresh();
        step();
        HAL_Delay(20);
    }

    as5047.refresh();
    double ending = as5047.get_angle();

    double diff = fabs(starting - ending);

    double rotations = diff / (2 * M_PI);

    pole_pairs = std::round((double)steps / rotations);
}


void init()
{
    Serial.set_parse_type(ParseType::RAW);

    phase_A.init();
    phase_B.init();

    phase_A.set_input_voltage(DRIVER_INPUT_VOLTAGE);
    phase_B.set_input_voltage(DRIVER_INPUT_VOLTAGE);

    as5047.init();

    // offset = get_angle_offset();

    get_pole_pairs();

} // end of "init()"


void update()
{   
    // led.set_high();

    // step(12, 3);

    // HAL_Delay(5);

    // double electrical_angle = get_electrical_angle();

    // inverse_park(
    //     get_electrical_angle(),
    //     0,
    //     5
    // );

    // Serial.println(pole_pairs);

    phase_A.stop();
    phase_B.stop();

    as5047.refresh();
    
    double angle = as5047.get_angle();
    // double angle = as5047.get_raw_angle();

    auto bytes = ByteConverter::double_to_bytes(angle);

    // HAL_Delay(50);
    // Serial.transmit_bytes(bytes);

    // auto status = Serial.println(angle);

    if(angle == 0.0)
    {
        led.set_high();
        // HAL_Delay(500);
    }
    else
        led.set_low();

    // Serial.println(angle, 9);
    // Serial.transmit_bytes(bytes);

    // HAL_Delay(5);

    // HAL_Delay(100);

    Serial.println(pole_pairs);
    // Serial.println(angle);

    // Serial.println(electrical_angle, 9);

} // end of "update()"


void inverse_park(double el_angle, double vd, double vq)
{
    double vA = vd * cos(el_angle) - vq * sin(el_angle);
    double vB = vd * sin(el_angle) + vq * cos(el_angle);

    phase_A.set_voltage(vA);
    phase_B.set_voltage(vB);
}


void step(double voltage, double delay_ms)
{
    // 0 Degrees
    phase_A.set_voltage(voltage);
    phase_B.set_voltage(0);
    HAL_Delay(delay_ms);

    // 45 Degrees
    phase_A.set_voltage(voltage);
    phase_B.set_voltage(voltage);
    HAL_Delay(delay_ms);

    // 90 Degrees
    phase_A.set_voltage(0);
    phase_B.set_voltage(voltage);
    HAL_Delay(delay_ms);

    // 135 Degrees
    phase_A.set_voltage(-voltage);
    phase_B.set_voltage(voltage);
    HAL_Delay(delay_ms);

    // 180 Degrees
    phase_A.set_voltage(-voltage);
    phase_B.set_voltage(0);
    HAL_Delay(delay_ms);

    // 235 Degrees
    phase_A.set_voltage(-voltage);
    phase_B.set_voltage(-voltage);
    HAL_Delay(delay_ms);

    // 270 Degrees
    phase_A.set_voltage(0);
    phase_B.set_voltage(-voltage);
    HAL_Delay(delay_ms);
}


double get_electrical_angle()
{
    return (as5047.get_angle() - offset) * 50;
}


double get_angle_offset(double voltage)
{
    HAL_Delay(500);

    phase_A.set_voltage(voltage);
    phase_B.set_voltage(0);

    HAL_Delay(1000);

    return as5047.get_angle();
}