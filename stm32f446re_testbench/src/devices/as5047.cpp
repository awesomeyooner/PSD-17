#include "devices/as5047.hpp"


AS5047::AS5047(int cs_pin)
{
    m_cs_pin = cs_pin;

} // end of "AS5047"


void AS5047::init(SPIClass* spi)
{
    m_spi = spi;

    // If the CS pin is set, configure it
    if(m_cs_pin != -1)
    {
        pinMode(m_cs_pin, OUTPUT);
        digitalWrite(m_cs_pin, HIGH);
    }

    // Start SPI
    m_spi->begin();
    
    // get_raw_angle();

} // end of "init"


int AS5047::get_uncompensated_counts()
{
    uint16_t command = UNCOMPENSATED_ANGLE_REGISTER | READ_WRITE;

    return spi_transfer16(command) & RESULT_MASK;

} // end of "get_uncompensated_counts"


int AS5047::get_compensated_counts()
{
    uint16_t command = COMPENSATED_ANGLE_REGISTER | PARITY | READ_WRITE;

    return spi_transfer16(command) & RESULT_MASK;

} // end of "get_compensated_counts"


int AS5047::get_raw_counts(bool compensated)
{
    return compensated ? get_compensated_counts() : get_uncompensated_counts();

} // end of "get_raw_counts"


double AS5047::get_raw_rotations(bool compensated)
{
    double counts = compensated ? get_compensated_counts() : get_uncompensated_counts();

    return counts / CPR;

} // end of "get_raw_rotations"


double AS5047::get_raw_angle(bool compensated)
{
    // Get the raw counts
    double counts = compensated ? get_compensated_counts() : get_uncompensated_counts();

    // Rotations = counts / CPR
    // Radians = Rotations * 2PI
    // Return Radians
    return (counts / CPR) * 2 * PI;

} // end of "get_raw_angle"


double AS5047::get_angle(bool compensated)
{
    double raw_angle = get_raw_angle();

    return raw_angle;
} // end of "get_angle"

int AS5047::get_magnetic_magnitude()
{
    uint16_t command = MAGNETIC_MAGNITUDE_REGISTER | READ_WRITE;

    spi_transfer16(command);

    return (int)NOP();

} // end of "get_magnetic_magnitude"


uint16_t AS5047::NOP()
{
    uint16_t read_data = spi_transfer16(0xFFFF);

    return read_data & RESULT_MASK;

} // end of "NOP"


uint16_t AS5047::spi_transfer16(uint16_t write_data)
{
    m_spi->beginTransaction(SETTINGS);

    // If the CS pin is present, pull it LOW
    if(m_cs_pin != -1)
        digitalWrite(m_cs_pin, LOW);

    // Read the data at the register
    uint16_t read_data = m_spi->transfer16(write_data);

    // If the CS pin is present, pull it HIGH
    if(m_cs_pin != -1)
        digitalWrite(m_cs_pin, HIGH);

    m_spi->endTransaction();

    return read_data;

} // end of "spi_transfer16"
