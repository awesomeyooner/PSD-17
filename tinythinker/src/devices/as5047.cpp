#include "devices/as5047.hpp"

#include "helpers/usbfs.hpp"

AS5047::AS5047(SPI_HandleTypeDef* spi, GPIO_TypeDef* GPIO_family, int pin_num)
{
    m_spi = spi;
    m_cs_family = GPIO_family;
    m_cs_pin = pin_num;

} // end of "AS5047"


void AS5047::init()
{

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
    return (counts / CPR) * 2 * M_PI;

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
    // // Convert 2 byte data to 2 individual bytes
    // uint8_t msb_TX = (uint8_t)(write_data >> 8);
    // uint8_t lsb_TX = (uint8_t)(write_data & 0x00FF);

    // std::vector<uint8_t> TX_BUFF = {msb_TX, lsb_TX};

    // Pull CS Pin LOW before sending data
    HAL_GPIO_WritePin(m_cs_family, m_cs_pin, GPIO_PIN_RESET);

    // Trasmit the data
    HAL_SPI_Transmit(m_spi, (uint8_t*)&write_data, 1, 200);
    // HAL_SPI_Transmit(m_spi, TX_BUFF.data(), TX_BUFF.size(), 200);

    // uint8_t RX_BUFF[2];

    // Read the data at the register
    // HAL_SPI_Receive(m_spi, RX_BUFF, sizeof(RX_BUFF), 200);

    uint16_t read_data = 0;

    HAL_SPI_TransmitReceive(m_spi, (uint8_t*)NOP_REGISTER, (uint8_t*)read_data, 1, 200);
    // uint8_t msb_RX = RX_BUFF[0];
    // uint8_t lsb_RX = RX_BUFF[1];

    // uint16_t read_data = static_cast<uint16_t>(msb_RX) << 8 | lsb_RX;

    // USBFS::print_header("AS5047", read_data);

    // Pull CS Pin HIGH since we're done
    HAL_GPIO_WritePin(m_cs_family, m_cs_pin, GPIO_PIN_SET);

    return read_data & RESULT_MASK;

} // end of "spi_transfer16"


uint16_t AS5047::test()
{
    uint16_t read_mag = 0x7FFE;
    uint16_t nop_command = 0x0000;
    uint16_t temp = 0;

    HAL_GPIO_WritePin(m_cs_family, m_cs_pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(m_spi,(uint8_t*)&read_mag, (uint8_t*)&temp, 1, 200);
    HAL_GPIO_WritePin(m_cs_family, m_cs_pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(m_cs_family, m_cs_pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(m_spi, (uint8_t*)&nop_command, (uint8_t*)&temp, 1, 200);
    HAL_GPIO_WritePin(m_cs_family, m_cs_pin, GPIO_PIN_SET);

    return temp & 0x3FFF;
}