#include "core.hpp"

#include "devices/as5047.hpp"
#include "helpers/usbfs.hpp"

#include <string>
#include <stdio.h> 

AS5047 sensor(&hspi1, GPIOD, GPIO_PIN_2);

void core_init()
{
    sensor.init();
} // end of "core_init"


void core_update()
{
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
    HAL_Delay(100);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
    HAL_Delay(100);

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
    HAL_Delay(100);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
    HAL_Delay(100);

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
    HAL_Delay(100);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
    HAL_Delay(100);

    HAL_Delay(500);

    double data = sensor.test();
    // double data = sensor.get_magnetic_magnitude();
    // double data = 3.14;
    // int data = 5;

    USBFS::println(data);
    // USBFS::print_header("My Header", "Hello World!");

} // end of "core_update"