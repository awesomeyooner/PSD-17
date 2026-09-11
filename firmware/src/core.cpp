#include "core.hpp"

#include "EmbeddedLib/devices/ws2812b.hpp"
#include "EmbeddedLib/devices/gpio_device.hpp"

GPIODevice led = GPIODevice(GPIOC, GPIO_PIN_1);
WS2812B leds = WS2812B(4, &htim2, TIM_CHANNEL_2);


void init()
{
    if(leds.init() != status_utils::StatusCode::OK)
        led.set_high();

} // end of "init()"


void update()
{
    leds.set_color(WS2812B::BLUE);
    HAL_Delay(250);

    leds.set_color(WS2812B::RED);
    HAL_Delay(250);

} // end of "update()"


// Define DMA Callback
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    leds.dma_callback();
}