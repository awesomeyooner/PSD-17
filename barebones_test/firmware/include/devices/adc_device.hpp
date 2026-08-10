#ifndef ADC_DEVICE_HPP
#define ADC_DEVICE_HPP


#include "adc.h"

#include "EmbeddedLib/status.hpp"

#include <vector>
#include <stdint.h>
#include <cmath>


class ADCDevice
{

    public:

        ADCDevice(ADC_HandleTypeDef* adc, int num_channels = 1);

        status_utils::StatusCode init();

        double poll(int timeout_ms = 100);

        double get_value(int index = 0);

        double get_percent(int index = 0);

        double get_voltage(int index = 0);


    private:

        // The ADC Reference voltage. ADC readings will be from [0, 4096], but the actual
        // voltage is ADC_VREF * (reading / 4096)
        static constexpr double ADC_VREF = 3.3; // Volts

        // The max value the ADC will return
        static constexpr double ADC_RESOLUTION = 4096; // 12 bits

        // The internal ADC handle to use
        ADC_HandleTypeDef* m_adc = nullptr;

        // Buffer of the ADC readings
        std::vector<uint32_t> m_read_buffer;

        // The number of channels this object represents
        int m_num_channels = -1;

}; // class ADCDevice


#endif // ADC_DEVICE_HPP