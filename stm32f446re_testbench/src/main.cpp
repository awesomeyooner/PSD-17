#include <Arduino.h>

#include "WireLib/communication/wire_manager.hpp"
#include "WireLib/registers/register_manager.hpp"

#include "EmbeddedLib/devices/builtin_led.hpp"

#include "devices/as5047.hpp"

#define I2C_ADDR 4


AS5047 sensor(PB6);

void setup()
{
  Serial.begin(115200);

  Serial.println("Start of Setup");

  WireManager::initialize(I2C_ADDR);

  sensor.init();

  RegisterManager::add_request(
    Request::make_float(25, 
      []() -> float
      {
        return (float)sensor.get_angle();
      }
    )
  );

  RegisterManager::add_request(
    Request::make_float(26, 
      []() -> float
      {
        return (float)sensor.get_magnetic_magnitude();
      }
    )
  );

  Serial.println("End of Setup");
}

void loop() 
{
  // put your main code here, to run repeatedly:
}