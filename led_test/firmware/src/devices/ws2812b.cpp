#include "devices/ws2812b.hpp"


using namespace std;
using namespace status_utils;


WS2812B::WS2812B(int num_leds, TIM_HandleTypeDef* timer, int channel)
{
    m_num_leds = num_leds;
    m_timer = timer;
    m_channel = channel;

    m_dma_buf_len = (m_num_leds * BITS_PER_LED) + RESET_PERIODS;

    m_pixel_data.resize(m_num_leds);
    m_dma_buffer.resize(m_dma_buf_len);

} // end of "WS2812B(int, TIM_HandleTypeDef*, int)"


StatusCode WS2812B::init()
{
    // Init PWM
    StatusCode timer_init = HAL_TIM_PWM_Init(m_timer) == HAL_OK ? StatusCode::OK : StatusCode::FAILED;

    // Clear the DMA buffer
    for(int i = 0; i < m_dma_buf_len; i++)
    {
        m_dma_buffer[i] = 0;
    }

    // Set the DMA transfer flag ready
    m_is_dma_ready = true;

    return timer_init;

} // end of "init()"


void WS2812B::set_color(int index, uint8_t r, uint8_t g, uint8_t b)
{
    // Out-of-Bounds check
    if(index > m_pixel_data.size() - 1 || index < 0)
        return;

    m_pixel_data.at(index).color.r = r;
    m_pixel_data.at(index).color.g = g;
    m_pixel_data.at(index).color.b = b;

} // end of "set_color(int, uint8_t, uint8_t, uint8_T)"


StatusCode WS2812B::update()
{
    // If DMA isn't ready then fail
    if(!m_is_dma_ready)
        return StatusCode::FAILED;

    int dma_buffer_index = 0;

    for(int led_index = 0; led_index < m_num_leds; led_index++)
    {
        for(uint8_t bit_index = 0; bit_index < BITS_PER_LED; bit_index++)
        {
            // If the bit has been set
            // Then set it HIGH in DMA
            if(m_pixel_data.at(led_index).data >> bit_index & 0x01)
                m_dma_buffer.at(dma_buffer_index) = CODE_1;
            // Else set it LOW
            else
                m_dma_buffer.at(dma_buffer_index) = CODE_0;
            
            dma_buffer_index++;
        }
    }

    StatusCode dma_status = HAL_TIM_PWM_Start_DMA(m_timer, m_channel, m_dma_buffer.data(), m_dma_buffer.size()) == HAL_OK ? 
        StatusCode::OK : StatusCode::FAILED;

    if(dma_status == StatusCode::OK)
        m_is_dma_ready = false;

    return dma_status;

} // end of "update()"


void WS2812B::dma_callback()
{
    HAL_TIM_PWM_Stop_DMA(m_timer, m_channel);

    m_is_dma_ready = true;

} // end of "dma_callback()"