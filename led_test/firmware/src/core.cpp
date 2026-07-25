#include "core.hpp"

#include "devices/as5047.hpp"

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
AS5047 sensor = AS5047(&hspi1, GPIOD, GPIO_PIN_2);


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

            double data = sensor.get_angle();

            Serial.info(data);

            return StatusedValue<bool>(false, StatusCode::OK);
        }
    );

    ActionManager::add(blink);

    HAL_ADC_Start(&hadc1);
} // end of "init()"


void update()
{
    // ActionManager::update();

    double counts = sensor.get_raw_counts();

    int CPR = 16384;

    double data = (counts / CPR) * 2 * M_PI;

    Serial.info(counts);

    HAL_Delay(10);


} // end of "update()"