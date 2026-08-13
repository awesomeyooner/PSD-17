#ifndef LCD_HPP
#define LCD_HPP


#include "EmbeddedLib/status.hpp"

#include "i2c.h"
#include "stm32f4xx_hal.h"

#include <vector>
#include <cstdint>
#include <string>


class LCD
{

    public:

        /**
         * @brief Creates a new LCD object at the given address and I2C Bus
         * 
         * @param i2c `I2C_HandleTypeDef*` Pointer to the I2C bus to use
         * @param address `int` The I2C address this device is at
         */
        LCD(I2C_HandleTypeDef* i2c, int address);

        /**
         * @brief Initialize the display by sending commands per datasheet
         * 
         * @return `status_utils::StatusCode` OK if I2C transmitted data successfully 
         */
        status_utils::StatusCode init();

        /**
         * @brief Set the posiiton of the cursor, where `x` is the horizontal position
         * from `[0, X_MAX]` and `y` is the vertical position from `[0, Y_MAX]`. `(0, 0)`
         * is the top left corner
         * 
         * @param x `int` Horizontal coordinate from `[0, X_MAX]`, typically [0, 15]
         * @param y `int` Vertical coordinate from `[0, Y_MAX]`, typically [0, 1]
         * @return status_utils::StatusCode 
         */
        status_utils::StatusCode set_cursor(int x, int y);

        /**
         * @brief Send a byte for the LCD to display at the cursor
         * 
         * @param data `uint8_t`
         * @return `status_utils::StatusCode` OK if transmit was successful 
         */
        status_utils::StatusCode send_data(uint8_t data);

        /**
         * @brief Send an internal command to the LCD
         * 
         * @param command `uint8_t`
         * @return `status_utils::StatusCode` OK if transmit was successful 
         */
        status_utils::StatusCode send_command(uint8_t command);

        /**
         * @brief Send a string to the display starting at the cursor position
         * 
         * @param str `std::string`
         * @return `status_utils::StatusCode` OK if transmit was successful 
         */
        status_utils::StatusCode send_string(std::string str);

    private:

        // The i2c transaction timeout in milliseconds
        int m_timeout_ms = 100;

        // The i2c address
        int m_address = -1;

        // The i2c bus handle
        I2C_HandleTypeDef* m_i2c = nullptr;

        /**
         * @brief Transmit bytes to the LCD via I2C
         * 
         * @param bytes `const std::vector<uint8_t>&` 
         * @return `status_utils::StatusCode` OK if the bytes sent successfully 
         */
        status_utils::StatusCode transmit(const std::vector<uint8_t>& bytes);



}; // class LCD


#endif // LCD_HPP