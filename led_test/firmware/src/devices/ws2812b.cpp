#include "devices/ws2812b.hpp"


using namespace std;
using namespace status_utils;


WS2812B::WS2812B(int num_leds, TIM_HandleTypeDef* timer, int channel)
    : m_timer(timer, channel)
{
    m_num_leds = num_leds;

    m_dma_buf_len = (m_num_leds * BITS_PER_LED) + RESET_PERIODS;

    m_pixel_data.resize(m_num_leds);
    m_dma_buffer.resize(m_dma_buf_len);

} // end of "WS2812B(int, TIM_HandleTypeDef*, int)"


StatusCode WS2812B::init()
{
    // Init PWM
    StatusCode timer_init = HAL_TIM_PWM_Init(m_timer.get_timer()) == HAL_OK ? StatusCode::OK : StatusCode::FAILED;

    // Calculate the CCRs for each 1 and 0 code as well as reset period
    int ARR = m_timer.get_ARR() + 1;
    
    CODE_1 = LED_FREQUENCY * T1H * ARR;
    CODE_0 = LED_FREQUENCY * T0H * ARR;

    RESET_PERIODS = (int)(LED_FREQUENCY * T_RESET);

    // Clear the DMA buffer
    for(int i = 0; i < m_dma_buf_len; i++)
    {
        m_dma_buffer.at(i) = 0;
    }

    // Set the DMA transfer flag ready
    m_is_dma_ready = true;

    return timer_init;

} // end of "init()"


StatusCode WS2812B::set_color(int index, uint8_t r, uint8_t g, uint8_t b)
{
    // Out-of-Bounds check
    if(index > m_pixel_data.size() - 1 || index < 0)
        return StatusCode::FAILED;

    m_pixel_data.at(index).color.r = r;
    m_pixel_data.at(index).color.g = g;
    m_pixel_data.at(index).color.b = b;

    return update();

} // end of "set_color(int, uint8_t, uint8_t, uint8_t)"


StatusCode WS2812B::set_color(int index, ColorData color_data)
{
    // Out-of-Bounds check
    if(index > m_pixel_data.size() - 1 || index < 0)
        return StatusCode::FAILED;

    m_pixel_data.at(index).color.r = color_data.color.r;
    m_pixel_data.at(index).color.g = color_data.color.g;
    m_pixel_data.at(index).color.b = color_data.color.b;

    return update();

} // end of "set_color(int, ColorData)"


StatusCode WS2812B::set_color(int start, int end, uint8_t r, uint8_t g, uint8_t b)
{
    for(int i = start; i < end; i++)
    {
        set_color(i, r, g, b);
    }

    return update();

} // end of "set_color(int, int, uint8_t, uint8_t, uint8_t)"


StatusCode WS2812B::set_color(int start, int end, ColorData color_data)
{
    for(int i = start; i < end; i++)
    {
        set_color(i, color_data);
    }

    return update();

} // end of "set_color(int, int, uint8_t, uint8_t, uint8_t)"


StatusCode WS2812B::set_color(uint8_t r, uint8_t g, uint8_t b)
{
    set_color(0, m_num_leds - 1, r, g, b);

    return update();

} // end of "set_color(uint8_t, uint8_t, uint8_t)"


StatusCode WS2812B::set_color(ColorData color_data)
{
    set_color(0, m_num_leds - 1, color_data);

    return update();

} // end of "set_color(int, int, uint8_t, uint8_t, uint8_t)"


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
            uint8_t byte = (bit_index / 8) * 8;
            uint8_t bit = 7 - (bit_index % 8);
            uint8_t bitIndex = byte + bit;

            // If the bit has been set
            // Then set it HIGH in DMA
            if((m_pixel_data.at(led_index).data >> bitIndex) & 0x01)
                m_dma_buffer.at(dma_buffer_index) = CODE_1;
            // Else set it LOW
            else
                m_dma_buffer.at(dma_buffer_index) = CODE_0;
            
            dma_buffer_index++;
        }
    }

    auto dma_status = HAL_TIM_PWM_Start_DMA(m_timer.get_timer(), m_timer.get_channel(), m_dma_buffer.data(), m_dma_buffer.size());

    if(dma_status == HAL_OK)
    {    
        m_is_dma_ready = false;
        return StatusCode::OK;
    }

    return StatusCode::FAILED;

} // end of "update()"


void WS2812B::dma_callback()
{
    HAL_TIM_PWM_Stop_DMA(m_timer.get_timer(), m_timer.get_channel());

    m_is_dma_ready = true;

} // end of "dma_callback()"