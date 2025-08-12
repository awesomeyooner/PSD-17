#ifndef L298N_HPP
#define L298N_HPP

#include "base/driver.hpp"
#include "Constants.hpp"
#include <Arduino.h>
namespace hardware_component{
    
    class L298N : public Driver{

        public:

            /**
             * @brief Creates a new driver object for the L298N motor driver
             * 
             * @param _enA Enable Pin
             * @param _in1 Input 1 pin
             * @param _in2 Input 2 pin
             */
            L298N(int _enA, int _in1, int _in2) : Driver(){
                enA = _enA;
                in1 = _in1;
                in2 = _in2;
            }

            void initialize() override{
                //setup pins for pwm control
                pinMode(enA, OUTPUT);
                pinMode(in1, OUTPUT);
                pinMode(in2, OUTPUT);
            }

            void set_percent(double request) override{

                //out of bounds protection
                if(request > 1)
                    request = 1;
                else if(request < -1)
                    request = -1;

                if(request > 0){
                    digitalWrite(enA, HIGH);
                    
                    analogWrite(in1, map(abs(request) * 1000, 0, 1000, 0, MAX_DUTY_CYCLE));
                    analogWrite(in2, 0);
                }     
                else if(request < 0){
                    digitalWrite(enA, HIGH);
                    analogWrite(in1, 0);
                    analogWrite(in2, map(abs(request) * 1000, 0, 1000, 0, MAX_DUTY_CYCLE));
                }
                else{
                    digitalWrite(enA, LOW);
                    analogWrite(in1, 0);
                    analogWrite(in2, 0);
                }
            }

            void stop() override{
                set_percent(0);
            }

            void brake(){
                digitalWrite(enA, HIGH);
                analogWrite(in1, MAX_DUTY_CYCLE);
                analogWrite(in2, MAX_DUTY_CYCLE);
            }
            

        private:

            int enA, in1, in2;

    }; // class L298N

} // namespace hardware_component

#endif