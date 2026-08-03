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

TimerDevice t3c4 = TimerDevice(&htim3, TIM_CHANNEL_4);
TimerDevice t3c3 = TimerDevice(&htim3, TIM_CHANNEL_3);
TimerDevice t3c2 = TimerDevice(&htim3, TIM_CHANNEL_2);
TimerDevice t3c1 = TimerDevice(&htim3, TIM_CHANNEL_1);

AS5047 as5047 = AS5047(&hspi1, GPIOD, GPIO_PIN_2);

void init()
{
    Serial.set_parse_type(ParseType::RAW);

    t3c4.init();
    t3c3.init();
    t3c2.init();
    t3c1.init();

    as5047.init();

} // end of "init()"


void update()
{   
    led.set_high();

    t3c4.set_duty(0.8);
    t3c3.set_duty(0.6);
    t3c2.set_duty(0.4);
    t3c1.set_duty(0.2);

} // end of "update()"