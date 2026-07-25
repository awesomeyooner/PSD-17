#ifndef WS2812B_HPP
#define WS2812B_HPP


#include "EmbeddedLib/status.hpp"

#include "WireLib/communication/protocols/serial_interface.hpp"

#include "stm32f4xx_hal.h"
#include "tim.h"

#include <cstdint>
#include <vector>


/**
 * @brief Driver code for WS2812B Addressable LEDs.
 * After creating the object with the constructor,
 * call `init()` and `set_color()`, followed by `update()`
 * 
 * To set this up in CubeMX, ensure the following:
 * 
 * - Timer frequency is 800kHz
 * 
 * In `DMA Settings`
 * 
 * Add a new request in `Normal` mode
 * 
 * - Direction = `Memory to Peripheral`
 * 
 * - Use Fifo = `checked`
 * 
 * - Increment Address (Peripheral) = `Disabled`
 * 
 * - Increment Address (Memory) = `Enabled`
 * 
 * - Data Width (both) = `Word`
 * 
 * - Burst Size (both) = `Single`
 * 
 * In `NVIC Settings`
 * 
 * - `TIMx global interrupt` = `Enabled`
 */
class WS2812B
{

    public:

        // Union to convert each of the 3 color bytes
        // into one single word (32 bits)
        typedef union
        {
            struct
            {
                // Green Byte
                uint8_t g;

                // Red Byte
                uint8_t r;

                // Blue Byte
                uint8_t b;

            } color;

            // Data packet for DMA
            uint32_t data;

        } ColorData;

        /**
         * @brief Create a string of WS2812B LEDs with at the given Timer channel and
         * number if LEDs to control
         * 
         * @param num_leds `int`
         * @param timer `TIM_HandleTypeDef*` The Timer to use
         * @param channel `int` The channel to use
         */
        WS2812B(int num_leds, TIM_HandleTypeDef* timer, int channel);

        /**
         * @brief Initializes the timer and clears the DMA buffer
         * 
         * @return `status_utils::StatusCode` OK if PWM init was `HAL_OK` 
         */
        status_utils::StatusCode init();

        /**
         * @brief Populates the DMA buffer and sends it
         * 
         * @return `status_utils::StatusCode` OK if successfully sent / is not busy
         */
        status_utils::StatusCode update();

        /**
         * @brief Set the color of the given LED. You must call `update` to actually
         * update the strip
         * 
         * @param index `int` The index of the pixel to set, `[0, NUM_LEDS - 1]
         * @param r `uint8_t` Red
         * @param g `uint8_t` Green
         * @param b `uint8_t` Blue
         */
        void set_color(int index, uint8_t r, uint8_t g, uint8_t b);

        /**
         * @brief Set the color of the given LED. You must call `update` to actually
         * update the strip
         * 
         * @param index `int` The index of the pixel to set, `[0, NUM_LEDS - 1]
         * @param color_data `ColorData` Color struct to set
         */
        void set_color(int index, ColorData color_data);

        /**
         * @brief Set the color of the given range of LEDs. You must call `update` to actually
         * update the strip
         * 
         * @param start `int` The start index of the pixel to set, `[0, NUM_LEDS - 1]
         * @param end `int` The end of the pixel to set, `[start, NUM_LEDS - 1]
         * @param r `uint8_t` Red
         * @param g `uint8_t` Green
         * @param b `uint8_t` Blue
         */
        void set_color(int start, int end, uint8_t r, uint8_t g, uint8_t b);

        /**
         * @brief Set the color of the given range of LEDs. You must call `update` to actually
         * update the strip
         * 
         * @param start `int` The start index of the pixel to set, `[0, NUM_LEDS - 1]
         * @param end `int` The end of the pixel to set, `[start, NUM_LEDS - 1]
         * @param color_data `ColorData` Color struct to set
         */
        void set_color(int index, int end, ColorData color_data);

        /**
         * @brief Set the color of the entire LED strip. You must call `update` to actually
         * update the strip
         * 
         * @param r `uint8_t` Red
         * @param g `uint8_t` Green
         * @param b `uint8_t` Blue
         */
        void set_color(uint8_t r, uint8_t g, uint8_t b);

        /**
         * @brief Set the color of the entire LED strip. You must call `update` to actually
         * update the strip
         *  
         * @param color_data `ColorData` Color struct to set
         */
        void set_color(ColorData color_data);

        /**
         * @brief Stops the DMA and raises the ready flag to true upon DMA finish
         * 
         */
        void dma_callback();

        static constexpr ColorData RED = ColorData{{0, 255, 0}};
        static constexpr ColorData GREEN = ColorData{{255, 0, 0}};
        static constexpr ColorData BLUE = ColorData{{0, 0, 255}};

        static constexpr ColorData WHITE = ColorData{{255, 255, 255}};
        static constexpr ColorData OFF = ColorData{{0, 0, 0}};



    private:

        // The number of bits in a single data packet
        static constexpr int BITS_PER_LED = 24; // 8G, 8R, 8B in MSB

        // Duty cycle for High Time to send 1
        static constexpr int CODE_1 = 64; // 0.8 us

        // Duty cycle for High Time to send 0
        static constexpr int CODE_0 = 32; // 0.8

        // How many periods of low to send a reset signal
        static constexpr int RESET_PERIODS = 64; // 80 us

        // Number of LEDs to control
        int m_num_leds = 0;
        
        // The timer that controls the PWM
        TIM_HandleTypeDef* m_timer = nullptr;

        // The channel to use
        int m_channel = -1;

        // Length of the DMA buffer
        int m_dma_buf_len = 0;

        // Flag for if the DMA is ready to be set
        bool m_is_dma_ready = false;

        // Represents the colors to set for the string of pixels
        std::vector<ColorData> m_pixel_data;
        
        // DMA values
        std::vector<uint32_t> m_dma_buffer;


}; // class wS2812B


#endif // WS2812B_HPP