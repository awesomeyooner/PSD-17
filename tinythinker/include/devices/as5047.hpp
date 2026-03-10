// https://look.ams-osram.com/m/d05ee39221f9857/original/AS5047P-DS000324.pdf

#ifndef AS5047_HPP
#define AS5047_HPP

#include "adc.h"
#include "can.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

#include "usbd_cdc_if.h"

#include <vector>
#include <cmath>

// #define CPR 16384 // 14 bits
// #define UNCOMPENSATED_ANGLE_REGISTER 0x3FFE
// #define COMPENSATED_ANGLE_REGISTER 0x3FFF
// #define MAGNETIC_MAGNITUDE_REGISTER 0x3FFD
// #define PARITY 0x8000
// #define READ_WRITE 0x4000
// #define RESULT_MASK 0x3FFF

class AS5047
{
    
    public:

        static constexpr int CPR = 16384; // 14 bits
        static constexpr uint16_t UNCOMPENSATED_ANGLE_REGISTER = 0x3FFE;
        static constexpr uint16_t COMPENSATED_ANGLE_REGISTER = 0x3FFF;
        static constexpr uint16_t MAGNETIC_MAGNITUDE_REGISTER = 0x3FFD;
        static constexpr uint16_t PARITY = 0x8000;
        static constexpr uint16_t READ_WRITE = 0x4000;
        static constexpr uint16_t RESULT_MASK = 0x3FFF;
        static constexpr uint16_t NOP_REGISTER = 0x0000; 
        static constexpr uint16_t NOP_COMMAND = 0xC000;

        AS5047();
        AS5047(SPI_HandleTypeDef* spi, GPIO_TypeDef* GPIO_family, int pin_num);

        void init();

        int get_compensated_counts();
        
        int get_uncompensated_counts();

        int get_raw_counts(bool compensated = false);

        double get_raw_rotations(bool compensated = false);

        double get_raw_angle(bool compensated = false);

        double get_angle(bool compensated = false);

        int get_magnetic_magnitude();

        uint16_t NOP();
        uint16_t spi_transfer16(uint16_t write_data);

        uint16_t test();

    private:

        SPI_HandleTypeDef* m_spi = nullptr;
        GPIO_TypeDef* m_cs_family = nullptr; 
        int m_cs_pin = -1;

        // uint16_t NOP();
        // uint16_t spi_transfer16(uint16_t write_data);


}; // class AS5047

#endif // AS5047_HPP