#include "core.hpp"

#include "EmbeddedLib/system.hpp"
#include "EmbeddedLib/devices/led.hpp"
#include "EmbeddedLib/devices/ws2812b.hpp"
#include "EmbeddedLib/devices/gpio_device.hpp"
#include "EmbeddedLib/devices/adc_device.hpp"
#include "EmbeddedLib/math/math_util.hpp"
#include "EmbeddedLib/math/vector2d.hpp"

#include "WireLib/communication/wire_manager.hpp"
#include "WireLib/communication/protocols/serial_interface.hpp"
#include "WireLib/util/byte_converter.hpp"

#include "ActionLib/ActionManager.hpp"

#include "PolarFOC/foc_math.hpp"
#include "PolarFOC/devices/encoders/as5047.hpp"
#include "PolarFOC/devices/drivers/l298n.hpp"
#include "PolarFOC/devices/motors/stepper_motor.hpp"
#include "PolarFOC/filters/low_pass_filter.hpp"

// #include "devices/as5047.hpp"
// #include "devices/l298n.hpp"
// #include "devices/adc_device.hpp"

#include "spi.h"
#include "tim.h"
#include "adc.h"
#include "usb_device.h"
#include "gpio.h"

#include "stm32f4xx_hal_tim.h"

#include <stdio.h>
#include <functional>


using namespace status_utils;
using namespace math;
using namespace std;


GPIODevice led = GPIODevice(GPIOC, GPIO_PIN_1);

StepperMotor motor = StepperMotor(50);

L298N phase_A = L298N(&htim3, TIM_CHANNEL_3, TIM_CHANNEL_4);
L298N phase_B = L298N(&htim3, TIM_CHANNEL_1, TIM_CHANNEL_2);

AS5047 as5047 = AS5047(&hspi1, GPIOD, GPIO_PIN_2);
LowPassFilter angle_filter = LowPassFilter(0.9);
LowPassFilter velocity_filter = LowPassFilter(0.9);

ADCDevice adc = ADCDevice(&hadc1);

Vector2d phase_voltages;

void stop();
void inverse_park(double el_angle, double vd, double vq = 0);
void step(double voltage = 12, double delay_ms = 20);
double get_electrical_angle();
double get_angle_offset(double voltage = 12);
double get_input_voltage();

double prev_angle = 0;
double prev_timestamp = 0;
double velocity = 0;

double target = 0;


void stop()
{
    phase_A.stop();
    phase_B.stop();
}


double get_pole_pairs()
{
    HAL_Delay(500);

    as5047.refresh();
    double starting = as5047.get_angle();

    int steps = 20;

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

    // pole_pairs = (double)steps / rotations;
    return std::round((double)steps / rotations);
}


double get_input_voltage()
{
    adc.poll();

    double V_adc = adc.get_voltage();

    double R_1 = 100000;
    double R_2 = 10000;

    return V_adc * (R_1 + R_2) / R_2;

} // end of "get_input_voltage()"


void init()
{
    System::init();

    Serial.set_parse_type(ParseType::PACKET);

    WireManager::attach(Serial);

    motor.link_drivers(&phase_A, &phase_B);
    motor.link_encoder(&as5047);

    motor.set_input_voltage(
        get_input_voltage()
    );

    motor.init();

    motor.calibrate_angle_offset(12);

    // get_pole_pairs();

    RegisterManager::add_command(
        Command<double>(
            100,
            [](double data) -> StatusCode
            {
                System::feed();

                target = data;
                // motor.set_target_voltage(data);
                
                return StatusCode::OK;
            }
        )
    );

    RegisterManager::add_request(
        Request<double>(
            101,
            []() -> double
            {
                return angle_filter.get();
                // return as5047.get_angle();
            }
        )
    );

    RegisterManager::add_request(
        Request<double>(
            102,
            []() -> double
            {
                return velocity_filter.get();
                // return velocity;
                // return as5047.get_velocity();
            }
        )
    );

    RegisterManager::add_request(
        Request<double>(
            103,
            []() -> double
            {
                return motor.get_input_voltage();
            }
        )
    );

    RegisterManager::add_request(
        Request<double>(
            104,
            []() -> double
            {
                return motor.m_openloop_angle;
            }
        )
    );

} // end of "init()"


void update()
{   
    System::update();
    ActionManager::update();

    motor.refresh();
    angle_filter.update(motor.get_encoder()->get_angle());

    // as5047.refresh();
    // angle_filter.update(as5047.get_angle());

    double timestamp = System::get_seconds(true);
    double angle = angle_filter.get();

    double dTheta = angle - prev_angle;
    double dt = timestamp - prev_timestamp;

    if(dt != 0)
        velocity = dTheta / dt;

    prev_timestamp = timestamp;
    prev_angle = angle;

    velocity_filter.update(velocity);


    if(!System::is_OK())
    {
        led.set_high();
        // stop();
        motor.stop();

        return;
    }

    led.set_low();

    motor.inverse_park_openloop(12, target);
    // motor.move();

} // end of "update()"


void inverse_park(double el_angle, double vd, double vq)
{
    phase_voltages = {vd, vq};

    phase_voltages = phase_voltages.rotate(el_angle);
    // double vA = vd * cos(el_angle) - vq * sin(el_angle);
    // double vB = vd * sin(el_angle) + vq * cos(el_angle);

    phase_A.set_voltage(phase_voltages.at(0));
    phase_B.set_voltage(phase_voltages.at(1));
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

    // 315 Degrees
    phase_A.set_voltage(voltage);
    phase_B.set_voltage(-voltage);
    HAL_Delay(delay_ms);
}


double get_electrical_angle()
{
    return (as5047.get_angle()) * 50;
}


double get_angle_offset(double voltage)
{
    // for(int i = 0; i < 4; i++)
    // {
    //     step(12, 100);
    //     HAL_Delay(500);
    // }

    HAL_Delay(500);

    phase_A.set_voltage(voltage);
    phase_B.set_voltage(0);

    HAL_Delay(1000);

    as5047.refresh();

    HAL_Delay(500);

    phase_A.stop();
    phase_B.stop();

    return as5047.get_angle();
}