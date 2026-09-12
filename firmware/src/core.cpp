#include "core.hpp"

#include "EmbeddedLib/devices/ws2812b.hpp"
#include "EmbeddedLib/devices/gpio_device.hpp"
#include "EmbeddedLib/devices/adc_device.hpp"

#include "WireLib/communication/protocols/serial_interface.hpp"

#include "ActionLib/ActionManager.hpp"

#include "PolarFOC/devices/encoders/as5047.hpp"
#include "PolarFOC/devices/drivers/dual_pwm_driver.hpp"

#include "PolarFOC/devices/motors/stepper_motor.hpp"


using namespace std;
using namespace status_utils;


// Status LEDs
GPIODevice led = GPIODevice(GPIOC, GPIO_PIN_1);
WS2812B leds = WS2812B(4, &htim2, TIM_CHANNEL_2);

// Current and Voltage Sensors
ADCDevice v_sensor = ADCDevice(&hadc2);
ADCCurrentSensor i_sensor = ADCCurrentSensor(&hadc1, 2200, 0.00045);

// Angle Sensor
AS5047 as5047 = AS5047(&hspi1, GPIOD, GPIO_PIN_2);

// Phase Drivers
DualPWMDriver phaseA = DualPWMDriver(&htim8, TIM_CHANNEL_1, TIM_CHANNEL_2);
DualPWMDriver phaseB = DualPWMDriver(&htim8, TIM_CHANNEL_3, TIM_CHANNEL_4);

// Motor Abstraction Class
StepperMotor motor = StepperMotor(50);


void init()
{
    // Enable DWT for nanosecond precision
    System::init();

    // Set initial timestamp
    ActionManager::init();

    HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_1);

    leds.init();

    motor.link_drivers(&phaseA, &phaseB);
    motor.link_encoder(&as5047);

    motor.link_current_sensor(&i_sensor);
    motor.link_voltage_sensor(&v_sensor);
    
    motor.calibrate_input_voltage(100000, 10000);

    if(motor.init() != StatusCode::OK)
        led.set_high();

    motor.calibrate_angle_offset(16);

    ActionManager::add(
        Action(0.02).link_callback(
            [](double, double) -> StatusedValue<bool>
            {
                // auto currents = motor.get_dq_currents();

                // double iA = currents.at(0);
                // double iB = currents.at(1);

                double iA = i_sensor.get_adc()->get_value(0);
                double iB = i_sensor.get_adc()->get_value(1);

                string text = "";

                text += "A: " + string_formatter::to_string(iA);
                text += "\t";
                text += "B: " + string_formatter::to_string(iB);

                Serial.println(text);

                return StatusedValue<bool>(false, StatusCode::OK);
            }
        )
    );

} // end of "init()"


void update()
{
    ActionManager::update();

    motor.refresh();

    // motor.set_target_voltage(15);

    // motor.move();

    motor.set_voltages(12, 12);

    // motor.set_percents(0.5, 0.25);

} // end of "update()"


// Define DMA Callback
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    leds.dma_callback();
}