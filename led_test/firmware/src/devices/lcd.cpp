#include "lcd.hpp"


using namespace std;
using namespace status_utils;


LCD::LCD(I2C_HandleTypeDef* i2c, int address)
{
    m_i2c = i2c;
    m_address = address;

} // end of "LCD(I2C_HandleTypeDef*, int)"


StatusCode LCD::init()
{
    // 4 Bit init
    HAL_Delay(50);
    send_command(0x30);
    HAL_Delay(5);
    send_command(0x30);
    HAL_Delay(1);
    send_command(0x30);
    HAL_Delay(10);
    send_command(0x20);
    HAL_Delay(10);

    // Init Display
    send_command(0x28);
    HAL_Delay(1);
    send_command(0x08);
    HAL_Delay(1);
    send_command(0x01);
    HAL_Delay(2);
    send_command(0x06);
    HAL_Delay(1);
    send_command(0x0C);

    return StatusCode::OK;

} // end of "init()"


StatusCode LCD::set_cursor(int x, int y)
{
    uint8_t x_command = x;
    uint8_t y_command;

    if(y == 0)
        y_command = 0x80;
    else if(y == 1)
        y_command = 0xC0;

    return combine_statuses({
        send_command(y_command),
        send_command(x_command)
    });

} // end of "set_cursor(int, int)"


StatusCode LCD::send_data(uint8_t data)
{
    uint8_t data_u = data & 0xf0;
    uint8_t data_1 = (data << 4) & 0xf0;

    vector<uint8_t> data_t(4);

    data_t[0] = data_u | 0x0D;
    data_t[1] = data_u | 0x09;
    data_t[2] = data_1 | 0x0D;
    data_t[3] = data_1 | 0x09;

    return transmit(data_t);

} // end of "send_data(uint8_t)"


StatusCode LCD::send_command(uint8_t command)
{
    uint8_t data_u = command & 0xf0;
    uint8_t data_1 = (command << 4) & 0xf0;

    vector<uint8_t> data_t(4);

    data_t[0] = data_u | 0x0C;
    data_t[1] = data_u | 0x08;
    data_t[2] = data_1 | 0x0C;
    data_t[3] = data_1 | 0x08;

    return transmit(data_t);

} // end of "send_command(uint8_t)"


StatusCode LCD::send_string(string str)
{
    StatusCode status = StatusCode::OK;

    for(char c : str)
    {
        if(send_data(c) != StatusCode::OK)
            status = StatusCode::FAILED;
    }

    return status;

} // end of "send_string(string)"


StatusCode LCD::transmit(const vector<uint8_t>& bytes)
{
    auto status = HAL_I2C_Master_Transmit(m_i2c, m_address, (uint8_t*)bytes.data(), bytes.size(), m_timeout_ms);

    return status == HAL_OK ? StatusCode::OK : StatusCode::FAILED;

} // end of "transmit(const vector<uint8_t>&)"
