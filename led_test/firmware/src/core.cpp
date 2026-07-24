#include "core.hpp"

#include "EmbeddedLib/system.hpp"
#include "EmbeddedLib/devices/led.hpp"
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

#include <stdio.h>
#include <functional>


using namespace status_utils;
using namespace math;
using namespace std;


GPIODevice led = GPIODevice(GPIOC, GPIO_PIN_1);


void init()
{
    ActionManager::init();

    Serial.set_parse_type(ParseType::RAW);
    WireManager::attach(Serial);

    Action blink = Action(0.5);

    blink.link_callback(
        [](double, double) -> StatusedValue<bool>
        {
            led.toggle();

            Serial.info("Hello World!");

            return StatusedValue<bool>(false, StatusCode::OK);
        }
    );

    ActionManager::add(blink);

} // end of "init()"


void update()
{
    ActionManager::update();

} // end of "update()"