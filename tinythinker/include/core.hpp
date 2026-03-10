#ifndef CORE_HPP
#define CORE_HPP

#include "adc.h"
#include "can.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

#include "usbd_cdc_if.h"


void core_init();

void core_update();


#endif // CORE_HPP