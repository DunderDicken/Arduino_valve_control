#include "Switchable.h"
#include <Arduino.h>
#include "Arduino.h"
Switchable::Switchable(const int pin)
  : m_pin(pin)
  , m_lastOnTime(0)
  , m_startTime(0)
{
  // Set pin as output
  pinMode(m_pin, OUTPUT);
  // Start state if off
  off();
}

//turn on:
void Switchable::on()
{
  digitalWrite(m_pin, 1); //high

  // If we actually switched state
  if (!m_state)
  {
    // set starttime
    m_startTime = millis();
  }
  m_state = true;
}

//turn off:
void Switchable::off()
{
  digitalWrite(m_pin, 0); //low
  
  // If we actually switched state
  if (m_state)
  {
    // save how long time output was On
    m_lastOnTime = millis() - m_startTime;
  }
  m_state = false;

}

void Switchable::toggle()
{
  digitalWrite(m_pin, !m_state); //low
  m_state = !m_state;
}

// dim pin
void Switchable::dim(int dimVal)
{
  analogWrite(m_pin, dimVal);
}

bool Switchable::getState()
{
  return m_state;
}

void Switchable::setState(bool state)
{
  digitalWrite(m_pin, state);
  m_state = state;
}

unsigned long Switchable::getLastOnTime()
{
  return m_lastOnTime;
}
