#include "devices/adc_device.hpp"


using namespace std;
using namespace status_utils;


ADCDevice::ADCDevice(ADC_HandleTypeDef* adc, int num_channels)
{
    m_adc = adc;
    m_num_channels = num_channels;

    m_read_buffer.resize(num_channels);

} // end of "ADCDevice(ADC_HandleTypeDef*, int)"


StatusCode ADCDevice::start_DMA()
{
    // Start DMA
    return HAL_ADC_Start_DMA(m_adc, m_read_buffer.data(), m_num_channels) == HAL_OK ?
        StatusCode::OK : StatusCode::FAILED;

} // end of "start_DMA()"


double ADCDevice::poll(int timeout_ms)
{
    HAL_ADC_Start(&hadc1);

    if(HAL_ADC_PollForConversion(m_adc, timeout_ms) == HAL_OK)
        m_read_buffer.at(0) = HAL_ADC_GetValue(m_adc);

    HAL_ADC_Stop(&hadc1);

    return m_read_buffer.at(0);

} // end of "poll(int)"


double ADCDevice::get_value(int index)
{
    return m_read_buffer.at(index);

} // end of "get_value()"


double ADCDevice::get_percent(int index)
{
    return get_value(index) / ADC_RESOLUTION;

} // end of "get_percent(int)"


double ADCDevice::get_voltage(int index)
{
    return get_percent() * ADC_VREF;

} // end of "get_voltage(int)"


