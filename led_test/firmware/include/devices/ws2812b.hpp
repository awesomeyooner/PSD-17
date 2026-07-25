#ifndef WS2812B_HPP
#define WS2812B_HPP


#include "EmbeddedLib/status.hpp"

#include "stm32f4xx_hal.h"
#include "tim.h"

#include <cstdint>
#include <vector>

class WS2812B
{

    public:

        WS2812B(int num_leds, TIM_HandleTypeDef* timer, int channel);

        status_utils::StatusCode init();

        status_utils::StatusCode update();

        void set_color(int index, uint8_t r, uint8_t g, uint8_t b);

        void dma_callback();

    private:

        static constexpr int TIMER_FREQUENCY = 800000; // 800 kHz

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

        int m_dma_buf_len = 0;

        bool m_is_dma_ready = false;

        typedef union
        {
            struct
            {
                uint8_t g;
                uint8_t r;
                uint8_t b;
            } color;

            uint32_t data;

        } ColorData;

        std::vector<ColorData> m_pixel_data;
        std::vector<uint32_t> m_dma_buffer;


}; // class wS2812B


#endif // WS2812B_HPP