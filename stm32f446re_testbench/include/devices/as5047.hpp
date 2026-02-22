// https://look.ams-osram.com/m/d05ee39221f9857/original/AS5047P-DS000324.pdf

#ifndef AS5047_HPP
#define AS5047_HPP

#include "Arduino.h"
#include "SPI.h"


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

        AS5047();
        AS5047(int cs_pin);

        void init(SPIClass* spi = &SPI);

        int get_compensated_counts();
        
        int get_uncompensated_counts();

        int get_raw_counts(bool compensated = false);

        double get_raw_rotations(bool compensated = false);

        double get_raw_angle(bool compensated = false);

        double get_angle(bool compensated = false);

        int get_magnetic_magnitude();

    private:

        SPIClass* m_spi;
        int m_cs_pin = -1;

        uint16_t NOP();
        uint16_t spi_transfer16(uint16_t write_data);

        static constexpr int CPR = 16384; // 14 bits
        static constexpr uint16_t UNCOMPENSATED_ANGLE_REGISTER = 0x3FFE;
        static constexpr uint16_t COMPENSATED_ANGLE_REGISTER = 0x3FFF;
        static constexpr uint16_t MAGNETIC_MAGNITUDE_REGISTER = 0x3FFD;
        static constexpr uint16_t PARITY = 0x8000;
        static constexpr uint16_t READ_WRITE = 0x4000;
        static constexpr uint16_t RESULT_MASK = 0x3FFF;

        static constexpr SPISettings SETTINGS = SPISettings(8000000, BitOrder::MSBFIRST, SPIMode::SPI_MODE1);


}; // class AS5047

#endif // AS5047_HPP