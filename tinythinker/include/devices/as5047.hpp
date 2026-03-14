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
#include <stdint.h>

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

        AS5047(SPI_HandleTypeDef* spi, GPIO_TypeDef* GPIO_family, int pin_num, int timeout = 100);

        void init();

        int get_compensated_counts();
        
        int get_uncompensated_counts();

        int get_raw_counts(bool compensated = false);

        double get_raw_rotations(bool compensated = false);

        double get_raw_angle(bool compensated = false);

        double get_angle(bool compensated = false);

        int get_magnetic_magnitude();

        void NOP();

        void send_command(uint16_t address, uint8_t read_or_write);

        uint16_t read_data(uint16_t address);

        void transmit(uint16_t data);

        uint16_t recieve();

    private:

        static constexpr int CPR = 16384; // 14 bits
        static constexpr uint16_t UNCOMPENSATED_ANGLE_REGISTER = 0x3FFE;
        static constexpr uint16_t COMPENSATED_ANGLE_REGISTER = 0x3FFF;
        static constexpr uint16_t MAGNETIC_MAGNITUDE_REGISTER = 0x3FFD;
        static constexpr uint8_t WRITE = (uint8_t)0;
        static constexpr uint8_t READ = (uint8_t)1;
        static constexpr uint16_t RESULT_MASK = 0x3FFF;
        static constexpr uint16_t NOP_REGISTER = 0x0000;

        SPI_HandleTypeDef* m_spi = nullptr;
        GPIO_TypeDef* m_cs_family = nullptr; 
        int m_cs_pin;
        int m_timeout;

        void select();

        void deselect();

        uint8_t is_parity_even(uint16_t data);


}; // class AS5047

#endif // AS5047_HPP