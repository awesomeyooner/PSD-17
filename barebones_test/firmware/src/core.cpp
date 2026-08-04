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

GPIODevice led = GPIODevice(GPIOC, GPIO_PIN_1);

L298N phase_A = L298N(&htim3, TIM_CHANNEL_3, TIM_CHANNEL_4);
L298N phase_B = L298N(&htim3, TIM_CHANNEL_1, TIM_CHANNEL_2);

AS5047 as5047 = AS5047(&hspi1, GPIOD, GPIO_PIN_2);

void init()
{
    Serial.set_parse_type(ParseType::RAW);

    phase_A.init();
    phase_B.init();

    as5047.init();

} // end of "init()"


void update()
{   
    led.set_high();

    phase_A.set_percent(0.5);
    phase_B.set_percent(0.25);

    double data = as5047.get_angle();

    Serial.println(data, 9);

} // end of "update()"