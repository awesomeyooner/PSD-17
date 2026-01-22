#include "foc/stepper_driver.hpp"


StepperDriver::StepperDriver(int in1_A, int in2_A, int in1_B, int in2_B, TIM_TypeDef* timer, int Hz) :
    m_driverA(in1_A, in2_A, Hz),
    m_driverB(in1_B, in2_B, Hz)
{
     // Create a new instance of hardware timer
     // associated with the given timer
     m_hw_timer = new HardwareTimer(timer);

} // end of constructor


// go to stm32_mcu.cpp
void StepperDriver::init()
{
    // Initialize the drivers with the given hardware timer
    m_driverA.init(m_hw_timer);
    m_driverB.init(m_hw_timer);

    auto handle = m_hw_timer->getHandle();

    // handle->Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED2;

    // HAL_TIM_Base_Init(handle);
    
} // end of "init"


void StepperDriver::set_supply_voltage(double voltage)
{
    m_supply_voltage = voltage;

} // end of "set_supply_voltage"


void StepperDriver::set_max_voltage(double voltage)
{
    m_max_voltage = voltage;

} // end of "set_max_voltage"


DRV8874* StepperDriver::get_driver_A()
{
    return &m_driverA;

} // end of "get_driver_A"


DRV8874* StepperDriver::get_driver_B()
{
    return &m_driverB;

} // end of "get_driver_B"


void StepperDriver::set_percent(double percent_A, double percent_B)
{
    m_driverA.set_percent(percent_A);
    m_driverB.set_percent(percent_B);

} // end of "set_percent"


void StepperDriver::set_voltage(double volts_A, double volts_B)
{
    // Clamp between the max voltage
    double volts_A_clamped = math::clamp(volts_A, -m_max_voltage, m_max_voltage);
    double volts_B_clamped = math::clamp(volts_B, -m_max_voltage, m_max_voltage);

    // % = requested / supply
    double duty_A = volts_A_clamped / m_supply_voltage;
    double duty_B = volts_B_clamped / m_supply_voltage;

    set_percent(duty_A, duty_B);

} // end of "set_voltage"