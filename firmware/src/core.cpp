#include "core.hpp"

#include "EmbeddedLib/devices/ws2812b.hpp"
#include "EmbeddedLib/devices/gpio_device.hpp"
#include "EmbeddedLib/devices/adc_device.hpp"

#include "WireLib/communication/protocols/serial_interface.hpp"

#include "ActionLib/ActionManager.hpp"

#include "PolarFOC/devices/encoders/as5047.hpp"

#include "devices/dual_pwm_driver.hpp"


using namespace std;
using namespace status_utils;


GPIODevice led = GPIODevice(GPIOC, GPIO_PIN_1);
WS2812B leds = WS2812B(4, &htim2, TIM_CHANNEL_2);
ADCDevice voltage_sensor = ADCDevice(&hadc2, 2);

AS5047 sensor = AS5047(&hspi1, GPIOD, GPIO_PIN_2);

DualPWMDriver phaseA = DualPWMDriver(&htim8, TIM_CHANNEL_1, TIM_CHANNEL_2);
DualPWMDriver phaseB = DualPWMDriver(&htim8, TIM_CHANNEL_3, TIM_CHANNEL_4);

ADCDevice i_sensor = ADCDevice(&hadc1, 2);

void init()
{
    ActionManager::init();

    leds.init();

    sensor.init();

    voltage_sensor.start_DMA();
    i_sensor.start_DMA();

    phaseA.init();
    phaseB.init();

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

                Serial.println(V_in);

                return StatusedValue<bool>(false, StatusCode::OK);
            }
        )
    );

} // end of "init()"


void update()
{
    sensor.refresh();

    ActionManager::update();

    phaseA.set_percent(0.1);
    phaseB.set_voltage(12);

} // end of "update()"


// Define DMA Callback
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    leds.dma_callback();
}