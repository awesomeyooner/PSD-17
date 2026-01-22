#ifndef STEPPER_DRIVER_HPP
#define STEPPER_DRIVE_HPP


#include <Arduino.h>

#include "devices/drv8874.hpp"

class StepperDriver
{

    public:

        StepperDriver(int in1_A, int in2_A, int in1_B, int in2_B, TIM_TypeDef* timer, int Hz = 20000);

        void init();

        DRV8874* get_driver_A();
        DRV8874* get_driver_B();

        void set_supply_voltage(double voltage);

        void set_max_voltage(double voltage);

        void set_percent(double percent_A, double percent_B);

        void set_voltage(double volts_A, double volts_B);

    private:

        double m_supply_voltage;
        double m_max_voltage;

        DRV8874 m_driverA;
        DRV8874 m_driverB;

        HardwareTimer* m_hw_timer;


}; // class StepperDriver

#endif // STEPPER_DRIVER_HPP