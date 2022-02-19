#include "Sensor.h"
#include <Arduino.h>

Sensor::Sensor(const int pin): m_pin(pin)
{
  
}
  
void Sensor::init()
{
  pinMode(m_pin, INPUT);
}
 
bool Sensor::read()
{
  return digitalRead(m_pin);
}
   
bool Sensor::isOn()
{
  return read();
}
