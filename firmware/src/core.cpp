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


GPIODevice led = GPIODevice(GPIOC, GPIO_PIN_1);
WS2812B leds = WS2812B(4, &htim2, TIM_CHANNEL_2);
ADCDevice voltage_sensor = ADCDevice(&hadc2, 2);

AS5047 sensor = AS5047(&hspi1, GPIOD, GPIO_PIN_2);

DualPWMDriver phaseA = DualPWMDriver(&htim8, TIM_CHANNEL_1, TIM_CHANNEL_2);
DualPWMDriver phaseB = DualPWMDriver(&htim8, TIM_CHANNEL_3, TIM_CHANNEL_4);

ADCDevice i_sensor = ADCDevice(&hadc1, 2);

StepperMotor motor = StepperMotor(50);

void init()
{
    System::init();

    ActionManager::init();

    leds.init();

    voltage_sensor.start_DMA();
    i_sensor.start_DMA();

    motor.link_drivers(&phaseA, &phaseB);
    motor.link_encoder(&sensor);

    double V_in = 24.343;

    motor.set_input_voltage(V_in);

    motor.init();
    motor.calibrate_angle_offset(12);

    ActionManager::add(
        Action(0.02).link_callback(
            [](double, double) -> StatusedValue<bool>
            {
                double R1 = 100000;
                double R2 = 10000;

                double V_out = voltage_sensor.get_voltage(0);

                double V_in = V_out * (R1 + R2) / R2;

                phaseA.set_input_voltage(V_in);
                phaseB.set_input_voltage(V_in);

                // Serial.println(V_in);

                double iA = i_sensor.get_voltage(0);
                double iB = i_sensor.get_voltage(1);

                string text = "A: " + string_formatter::to_string(iA) + "\t";
                text += "B: " + string_formatter::to_string(iB);

                Serial.println(text);

                return StatusedValue<bool>(false, StatusCode::OK);
            }
        )
    );

} // end of "init()"


void update()
{
    sensor.refresh();

    ActionManager::update();

    motor.refresh();

    led.set_low();

    motor.inverse_park(12);

} // end of "update()"


// Define DMA Callback
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    leds.dma_callback();
}