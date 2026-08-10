#ifndef ADC_DEVICE_HPP
#define ADC_DEVICE_HPP


#include "adc.h"

#include "EmbeddedLib/status.hpp"

#include <vector>
#include <stdint.h>
#include <cmath>


/**
 * @brief Wrapper class for ADC channels. This represents an entire ADC block, like ADC1, ADC2, etc.
 * This supports DMA and also polling when using only one channel.
 * 
 * When using `DMA`, call `start_DMA()` and you can use any of the `get_*()` methods at any time.
 * 
 * When using one channel and polling, call `poll()` then `get_*()`
 * 
 * When setting up, note that `Number of Conversion` means how many channels you want to have. For instance,
 * if I want to have 3 pins read 3 different voltages, then that value would be 3. When using multiple channels, set
 * `Scan Conversion Mode = Enabled`, and `Disabled` when using only 1. 
 * 
 * Ranks determine the index of values in the DMA buffer. 1 is index 0, 2 is index 1, so on.
 * 
 * You can change the rank by clicking on each rank and changing the channel. A greater sampling time means higher accuracy but at the cost of slower reads.
 * 
 * For `DMA`, please have the following settings
 * 
 * - Resolution = 12 bits
 * 
 * - Scan Conversion Mode = Enabled (when using multiple channels), set to Disabled if using only 1
 * 
 * - Continuous Conversion Mode = Enabled
 * 
 * - Discontinuous Conversion Mode = Disabled
 * 
 * - End of Conversion Selected = EOC at end of single channel conversion
 * 
 * Then go to `DMA Settings` and add a new request. 
 * 
 * - Direction = Peripheral to Memory
 * 
 * - Mode = Circular
 * 
 * - Increment Address (Peripheral) = Disabled
 * 
 * - Increment Address (Memory) = Enabled
 * 
 * - Data Width (Peripheral and Memory) = Word
 * 
 */
class ADCDevice
{

    public:

        /**
         * @brief Creates a new ADC wrapper using the given ADC and the number of channels. When using 
         * single polling mode, set `num_channels = 1` or just don't pass in a second argument. 
         * 
         * @param adc `ADC_HandleTypeDef*` Pointer to which ADC to use. For example, `ADC1 -> &hadc1`
         * @param num_channels `int` The number of channels this ADC reads. This should only be set to a number
         * greater than 1 when using DMA
         */
        ADCDevice(ADC_HandleTypeDef* adc, int num_channels = 1);

        /**
         * @brief Start sampling using DMA
         * 
         * @return `status_utils::StatusCode` OK if HAL_OK, FAILED otherwise 
         */
        status_utils::StatusCode start_DMA();

        /**
         * @brief Read the ADC by polling (blocking) with the given timeout, defaults to 100ms. This should only be used
         * when the ADC has 1 channel and not with DMA.
         * 
         * @param timeout_ms `int = 100` The read timeout in milliseconds
         * @return `double` The raw ADC value 
         */
        double poll(int timeout_ms = 100);

        /**
         * @brief Get the raw ADC value [0, 4095). This does not poll but rather returns the stored value from either DMA 
         * or previously called `poll()`
         * 
         * @param index `int = 0` The index of which channel to get. Ignore when using single channel.
         * @return `double` 
         */
        double get_value(int index = 0);

        /**
         * @brief Get the percent [0, 1] input on the given ADC channel. This does not poll but rather returns the stored value from either DMA 
         * or previously called `poll()`. This is the same as `get_value() / ADC_RESOLUTION`
         * 
         * @param index `int = 0` The index of which channel to get. Ignore when using single channel.
         * @return `double` 
         */
        double get_percent(int index = 0);

        /**
         * @brief Get the voltage read on the ADC [0, 3.3]. This does not poll but rather returns the stored value from either DMA 
         * or previously called `poll()`. This is the same as `get_percent() * ADC_VREF`
         * 
         * @param index `int = 0` The index of which channel to get. Ignore when using single channel.
         * @return `double` 
         */
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