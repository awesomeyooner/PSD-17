#include "devices/l298n.hpp"


using namespace status_utils;


L298N::L298N(TIM_HandleTypeDef* timer, int in1, int in2) 
    :   m_in1(timer, in1),
        m_in2(timer, in2)
{} // end of "L298N(TIM_HandleTypeDef*, int, int)"


StatusCode L298N::init()
{
    StatusCode in1_init = m_in1.init();
    StatusCode in2_init = m_in2.init();

    return combine_statuses({in1_init, in2_init});

} // end of "init()"


void L298N::set_percent(double percent)
{
    if(percent > 1)
        percent = 1;
    else if(percent < -1)
        percent = -1;

    // Keep track of the commanded percent out
    m_percent = percent;

    if(percent > 0)
    {
        m_in1.set_duty(fabs(percent));
        m_in2.set_duty(0);
    }
    else if(percent < 0)
    {
        m_in1.set_duty(0);
        m_in2.set_duty(fabs(percent));
    }
    else // percent == 0
    {
        m_in1.set_duty(0);
        m_in2.set_duty(0);
    }

} // end of "set_percent(double)"


double L298N::get_percent()
{
    return m_percent;

} // end of "get_percent()"


void L298N::stop()
{
    set_percent(0);

} // end of "stop()"


TimerDevice& L298N::get_IN1()
{
    return m_in1;

} // end of "get_IN1()"


TimerDevice& L298N::get_IN2()
{
    return m_in2;

} // end of "get_IN2()"