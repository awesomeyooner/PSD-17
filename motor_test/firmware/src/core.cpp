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
// AS5047 sensor = AS5047(&hspi1, GPIOD, GPIO_PIN_2);
// TimerDevice timer_device = TimerDevice(&htim3, TIM_CHANNEL_4);
// L298N phase_a = L298N(&htim2, TIM_CHANNEL_1, TIM_CHANNEL_4);
// L298N phase_b = L298N(&htim2, TIM_CHANNEL_2, TIM_CHANNEL_3);

void init()
{
    ActionManager::init();

    Serial.set_parse_type(ParseType::RAW);
    WireManager::attach(Serial);

    // Action blink = Action(0.5);

    // blink.link_callback(
    //     [](double, double) -> StatusedValue<bool>
    //     {
    //         led.toggle();

    //         double data = sensor.get_angle();

    //         Serial.info(data);

    //         return StatusedValue<bool>(false, StatusCode::OK);
    //     }
    // );

    // ActionManager::add(blink);

    // phase_a.init();
    // phase_b.init();

    led.set_high();

    // HAL_TIM_Base_Init(&htim2);
    // HAL_TIM_Base_Start(&htim2);

    HAL_TIM_PWM_Init(&htim2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    // timer_device.init();

    // HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    // __HAL_RCC_TIM3_CLK_ENABLE(); 
    // TIM2->CCR1 = 4000;

} // end of "init()"


void update()
{
    led.set_high();

    // ActionManager::update();

    // double counts = sensor.get_raw_counts();

    // int CPR = 16384;

    // double data = (counts / CPR) * 2 * M_PI;

    // Serial.info(counts);

    // HAL_Delay(10);

    // phase_a.set_percent(1);
    // phase_b.set_percent(0.5);

    // timer_device.set_duty(0);
    // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 9999);
    // Serial.info(__HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1));

} // end of "update()"


// // Define DMA Callback
// void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
// {
//     leds.dma_callback();

// }