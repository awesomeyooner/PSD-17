#include "core.hpp"

#include "EmbeddedLib/devices/ws2812b.hpp"
#include "EmbeddedLib/devices/gpio_device.hpp"
#include "EmbeddedLib/devices/adc_device.hpp"

#include "WireLib/communication/protocols/serial_interface.hpp"
#include "WireLib/communication/wire_manager.hpp"

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
ADCDevice v_sensor = ADCDevice(&hadc2, 2);
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

    // Enable Register commands via Serial
    Serial.set_parse_type(ParseType::PACKET);
    WireManager::attach(Serial);

    // Start Master Timer
    HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_1);

    leds.init();

    // Link all helper devices
    motor.link_drivers(&phaseA, &phaseB);
    motor.link_encoder(&as5047);

    motor.link_current_sensor(&i_sensor);
    motor.link_voltage_sensor(&v_sensor);
    
    motor.calibrate_input_voltage(100000, 10000);

    if(motor.init() != StatusCode::OK)
        led.set_high();

    motor.calibrate_angle_offset(16);

    RegisterManager::add_command(
        Command<double>(
            99, // Register Byte
            [](double data) -> StatusCode
            {
                System::feed();

                motor.set_target_vd(data);
                
                return StatusCode::OK;
            }
        )
    );

    RegisterManager::add_command(
        Command<double>(
            100, // Register Byte
            [](double data) -> StatusCode
            {
                System::feed();

                motor.set_target_vq(data);
                
                return StatusCode::OK;
            }
        )
    );

    RegisterManager::add_request(
        Request<double>(
            101, // Register Byte
            []() -> double
            {   
                return motor.get_dq_currents().at(0);
            }
        )
    );

    RegisterManager::add_request(
        Request<double>(
            102, // Register Byte
            []() -> double
            {   
                return motor.get_dq_currents().at(1);
            }
        )
    );

    RegisterManager::add_request(
        Request<double>(
            103, // Register Byte
            []() -> double
            {   
                return as5047.get_bounded_angle();
            }
        )
    );


    // ActionManager::add(
    //     Action(0.02).link_callback(
    //         [](double, double) -> StatusedValue<bool>
    //         {
    //             auto currents = motor.get_dq_currents();

    //             double iA = currents.at(0);
    //             double iB = currents.at(1);

    //             // double iA = i_sensor.get_adc()->get_value(0);
    //             // double iB = i_sensor.get_adc()->get_value(1);

    //             string text = "";

    //             text += "A: " + string_formatter::to_string(iA);
    //             text += "\t";
    //             text += "B: " + string_formatter::to_string(iB);

    //             Serial.println(text);

    //             return StatusedValue<bool>(false, StatusCode::OK);
    //         }
    //     )
    // );

} // end of "init()"


void update()
{
    System::update();

    ActionManager::update();

    if(!System::is_OK())
    {
        leds.set_color(WS2812B::RED);
        motor.stop();

        return;
    }

    leds.set_HSV(System::get_seconds(true) / 5, 1, 0.1);

    motor.refresh();

    // motor.set_target_voltage(15);

    motor.move();

    // motor.set_voltages(12, 12);

    // motor.set_percents(0.5, 0.25);

} // end of "update()"


// Define DMA Callback
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    leds.dma_callback();
}