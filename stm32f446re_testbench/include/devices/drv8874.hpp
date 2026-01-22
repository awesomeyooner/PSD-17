#ifndef DRV8874_HPP
#define DRV8874_HPP

#include <Arduino.h>

#include "EmbeddedLib/util/math/math_util.hpp"

class DRV8874
{

    public:

        DRV8874(int in1, int in2, TIM_TypeDef* timer, int Hz = 20000);

        DRV8874(int in1, int in2, HardwareTimer* hw_timer, int Hz = 20000);

        DRV8874(int in1, int in2, int Hz = 20000);

        void init();

        void init(HardwareTimer* hw_timer);
        
        void set_PWM(double duty1, double duty2);

        void set_percent(double percent);

    private:

        int m_in1;
        int m_in2;

        int m_ch1;
        int m_ch2;

        int m_frequency_Hz;

        HardwareTimer* m_hal_timer;


}; // class DRV8874


#endif // DRV8874_HPP