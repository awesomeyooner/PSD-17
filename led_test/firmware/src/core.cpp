#include "core.hpp"

#include "devices/as5047.hpp"
#include "devices/l298n.hpp"

#include "EmbeddedLib/system.hpp"
#include "EmbeddedLib/devices/led.hpp"
#include "EmbeddedLib/devices/ws2812b.hpp"
#include "EmbeddedLib/devices/gpio_device.hpp"
#include "EmbeddedLib/math/math_util.hpp"

#include "WireLib/communication/wire_manager.hpp"
#include "WireLib/communication/protocols/serial_interface.hpp"
#include "WireLib/util/byte_converter.hpp"

#include "ActionLib/ActionManager.hpp"

#include "adc.h"
#include "can.h"
#include "i2c.h"
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


GPIODevice led = GPIODevice(GPIOC, GPIO_PIN_1);
WS2812B rgb_leds = WS2812B(5, &htim2, TIM_CHANNEL_1);

void init()
{
    rgb_leds.init();
    
    ActionManager::init();

    Serial.set_parse_type(ParseType::RAW);
    WireManager::attach(Serial);

    Action blink = Action(0.5);

    blink.link_callback(
        [](double, double) -> StatusedValue<bool>
        {
            led.toggle();

            return StatusedValue<bool>(false, StatusCode::OK);
        }
    );

    ActionManager::add(blink);

} // end of "init()"


void update()
{
    ActionManager::update();

    rgb_leds.set_color(WS2812B::BLUE);


} // end of "update()"


// Define DMA Callback
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    rgb_leds.dma_callback();
}