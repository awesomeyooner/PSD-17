#include "devices/drv8874.hpp"


DRV8874::DRV8874(int in1, int in2, TIM_TypeDef* timer, int Hz)
{
    m_in1 = in1;
    m_in2 = in2;

    // Get the associated TIM Channels for each pin
    m_ch1 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(m_in1), PinMap_PWM));
    m_ch2 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(m_in2), PinMap_PWM));

    // Get the HW Timer object with the associated timer
    m_hal_timer = new HardwareTimer(timer);

    // Set the frequency
    m_frequency_Hz = Hz;

} // end of constructor


DRV8874::DRV8874(int in1, int in2, int Hz)
{
    m_in1 = in1;
    m_in2 = in2;

    // Get the associated TIM Channels for each pin
    m_ch1 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(m_in1), PinMap_PWM));
    m_ch2 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(m_in2), PinMap_PWM));

    // Set the frequency
    m_frequency_Hz = Hz;

} // end of constructor


DRV8874::DRV8874(int in1, int in2, HardwareTimer* hw_timer, int Hz) :
    DRV8874(in1, in2, Hz)
{
    m_hal_timer = hw_timer;

} // end of constructor



void DRV8874::init(HardwareTimer* hw_timer)
{
    m_hal_timer = hw_timer;

    init();
    
} // end of "init"


void DRV8874::init()
{
    // Pause before configuring
    m_hal_timer->pause();

    // Configure Input Pin 1
    m_hal_timer->setMode(
        m_ch1, // Channel
        TIMER_OUTPUT_COMPARE_PWM1, // Mode 
        m_in1 // Pin
    );

    // Configure Input Pin 2
    m_hal_timer->setMode(
        m_ch2, // Channel
        TIMER_OUTPUT_COMPARE_PWM1, // Mode 
        m_in2 // Pin
    );

    // Configure the PWM frequency
    m_hal_timer->setOverflow(m_frequency_Hz, HERTZ_FORMAT);

    // Resume since we're done
    m_hal_timer->refresh();
    m_hal_timer->resume();
    
} // end of "init"


void DRV8874::set_PWM(double duty1, double duty2)
{
    // duty1 and duty2 are [0, 1], but setPWM is [0, 100], so * 100
    // m_hal_timer->setPWM(m_ch1, m_in1, m_frequency_Hz, (uint32_t)(duty1 * 100.0));
    // m_hal_timer->setPWM(m_ch2, m_in2, m_frequency_Hz, (uint32_t)(duty2 * 100.0));

    m_hal_timer->setPWM(m_ch1, m_in1, m_frequency_Hz, (uint32_t)100);
    m_hal_timer->setPWM(m_ch2, m_in2, m_frequency_Hz, (uint32_t)100);

} // end of "setPWM"


void DRV8874::set_percent(double percent)
{
    // clamp from -1 to 1;
    double clamped = math::clamp(percent, -1, 1);

    // If positive, run DRV forward
    if(clamped > 0)
        set_PWM(clamped, 0);
    
    // If 0, explicity give 0 output
    else if(clamped == 0)
        set_PWM(0, 0);

    // If negative, run DRV reverse
    else if(clamped < 0)
        set_PWM(0, clamped);

} // end of "set_percent"