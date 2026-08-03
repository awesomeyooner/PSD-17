#ifndef L298N_HPP
#define L298N_HPP

#include <cmath>

#include "tim.h"

#include "EmbeddedLib/status.hpp"
#include "EmbeddedLib/devices/timer_device.hpp"


class L298N
{

    public:

        /**
         * @brief Create a new L298N motor driver with the given pins
         * 
         * @param timer `TIM_HandleTypeDef*` Timer family
         * @param in1 `int` IN1 Channel
         * @param in2 `int` IN2 Channel
         */
        L298N(TIM_HandleTypeDef* timer, int in1, int in2);

        /**
         * @brief Initialize the timers
         * 
         * @return `status_utils::StatusCode` 
         */
        status_utils::StatusCode init();

        /**
         * @brief Sets the percent output of the motor, from `[-1, 1]`. 
         * 
         * Example: `0.5` is 50% output in the forward direction. `-0.5` will be reverse
         * 
         * This will clamp input to [-1, 1]
         * 
         * @param percent `double` The percent speed from `[-1, 1]`
         */
        void set_percent(double percent);

        /**
         * @brief Gets the percent output command sent to the motor, from `[-1, 1]
         * 
         * @return `double`
         */
        double get_percent();

        /**
         * @brief Stops the motor (sets percent to 0)
         * 
         */
        void stop();

        TimerDevice& get_IN1();

        TimerDevice& get_IN2();

    private:

        // IN1 Channel
        TimerDevice m_in1;

        // IN2 Channel
        TimerDevice m_in2;

        // Bookkeeping for the percent output applied [-1, 1]
        double m_percent = 0;

}; // class L298N 


#endif // L298N_HPP