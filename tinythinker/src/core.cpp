#include "core.hpp"

#include "devices/as5047.hpp"
#include "helpers/usbfs.hpp"

#include <string>
#include <stdio.h> 

AS5047 sensor(&hspi1, GPIOD, GPIO_PIN_2);


void blink(int on_time)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
    HAL_Delay(on_time);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
}


void core_init()
{
    sensor.init();
} // end of "core_init"


void core_update()
{
    // for(int i = 0; i < 3; i++)
    // {
    //     blink(50);
    //     HAL_Delay(100);
    // }

    double data = sensor.get_angle(true);
    // double data = sensor.get_magnetic_magnitude();
    // double data = 3.14;
    // int data = 5;

    USBFS::println(data);
    // USBFS::print_header("My Header", "Hello World!");

    HAL_Delay(50);

} // end of "core_update"