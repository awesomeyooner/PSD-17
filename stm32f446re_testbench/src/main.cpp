#include <Arduino.h>

#include "WireLib/communication/wire_manager.hpp"
#include "WireLib/registers/register_manager.hpp"

#include "EmbeddedLib/devices/builtin_led.hpp"

#include "devices/as5047.hpp"
#include "foc/stepper_driver.hpp"

#define I2C_ADDR 4

#define IN1_A PA8
#define IN2_A PA9
#define IN1_B PA10
#define IN2_B PA11

StepperDriver driver(IN1_A, IN2_A, IN1_B, IN2_B, TIM1, 20000);
AS5047 sensor(PB6);

float target = 0;

void setup()
{
  Serial.begin(115200);

  // Serial.println("Start of Setup");

  // WireManager::initialize(I2C_ADDR);

  // driver.set_max_voltage(24.5);
  // driver.set_supply_voltage(24.5);

  // sensor.init();
  // driver.init();

  // RegisterManager::add_request(
  //   Request::make_float(25, 
  //     []() -> float
  //     {
  //       return (float)sensor.get_angle();
  //     }
  //   )
  // );

  // RegisterManager::add_request(
  //   Request::make_float(26, 
  //     []() -> float
  //     {
  //       return (float)sensor.get_magnetic_magnitude();
  //     }
  //   )
  // );

  // RegisterManager::add_command(
  //   Command::make_float(3,
  //     &target
  //   )
  // );

  Serial.println("End of Setup");
}

void loop() 
{
  // put your main code here, to run repeatedly:
  // driver.set_percent(target, target);
  // driver.get_driver_A()->set_PWM(1, 1);
  // driver.get_driver_B()->set_PWM(1, 1);
}