#ifndef _SENSOR_H_
#define _SENSOR_H_

//sensor class
class Sensor{
  public:
    Sensor(const int pin);
    //initilaze sensor instance
    void init();
    //Read the sensors state
    bool read();
    //Return True if sensor is active
    bool isOn();

  private:
    const int m_pin;
    
};

#endif //_SENSOR_H_
