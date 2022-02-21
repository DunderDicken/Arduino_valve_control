

// Include Libraries
#include "Arduino.h"
#include "SolenoidValve.h"
#include "Button.h"
#include "Sensor.h"

// Pin Definitions
#define VALVE1_PIN2	2
#define VALVE2_PIN3 3
#define VALVE3_PIN4 4
#define VALVE4_PIN5 5
#define PUSHBUTTON_PIN11 11 //Start first
#define PUSHBUTTON_PIN12 12 //Start
#define PUSHBUTTON_PIN13 13 //Stop
#define SENSOR1_PIN7   7
#define SENSOR2_PIN8   8
#define SENSOR3_PIN9   9
#define SENSOR4_PIN10 10

// Global variables and defines

// object initialization
SolenoidValve solenoidValve1(VALVE1_PIN2);
SolenoidValve solenoidValve2(VALVE2_PIN3);
SolenoidValve solenoidValve3(VALVE3_PIN4);
SolenoidValve solenoidValve4(VALVE4_PIN5);

Button buttonStartFirst(PUSHBUTTON_PIN11);
Button buttonStart(PUSHBUTTON_PIN12);
Button buttonStop(PUSHBUTTON_PIN13);

Sensor sensor1(SENSOR1_PIN7);
Sensor sensor2(SENSOR2_PIN8);
Sensor sensor3(SENSOR3_PIN9);
Sensor sensor4(SENSOR4_PIN10);


// Define vars
const int timeout = 10000;       //define timeout of 10 sec
const int printTimeout = 2000;   //define timeout of 1 sec
uint32_t timer = millis();
uint32_t openTimer1 = millis();
uint32_t openTimer2 = millis();
uint32_t openTimer3 = millis();
uint32_t openTimer4 = millis();
int openTimeout = 50;   //define timeout for opening time
int serialCommand = 0;

bool oneStarted = false;
bool twoStarted = false;
bool threeStarted = false;
bool fourStarted = false;

enum States {
  START_FIRST,
  START,
  RUNNING,
  RUNNING_TIMERS,
  PRINT,
  STOPPED
};
States state = STOPPED;


// Setup
void setup()
{
  // Setup Serial
  Serial.begin(9600);
  while (!Serial) ; // wait for serial port to connect. Needed for native USB
  Serial.println("-Serial started-");

  // Init objects
  buttonStartFirst.init();
  buttonStart.init();
  buttonStop.init();

  sensor1.init();
  sensor2.init();
  sensor3.init();
  sensor4.init();

  state = STOPPED;
}

// Main logic
void loop()
{
  if (state == RUNNING)
  {
    // Sensor/valve1
    if (sensor1.isOn())
    {
      solenoidValve1.on();
    }
    else
    {
      solenoidValve1.off();
    }
    // Sensor/valve2
    if (sensor2.isOn())
    {
      solenoidValve2.on();
    }
    else
    {
      solenoidValve2.off();
    }
    // Sensor/valve3
    if (sensor3.isOn())
    {
      solenoidValve3.on();
    }
    else
    {
      solenoidValve3.off();
    }
    // Sensor/valve4
    if (sensor4.isOn())
    {
      solenoidValve4.on();
    }
    else
    {
      solenoidValve4.off();
    }

    // Check if stop button is pressed or released
    if (buttonStop.onPress() || buttonStop.onRelease())
    {
      state = STOPPED;
    }

  }// RUNNING

  if (state == RUNNING_TIMERS)
  {
    // Sensor/valve1
    if (sensor1.isOn() && !oneStarted)
    {
      //Start OnTimer
      openTimer1 = millis();
      solenoidValve1.on();
      oneStarted = true;
    }
    if (oneStarted && ((millis() - openTimer1) > openTimeout) )
    {
      solenoidValve1.off();
    }
    if (!solenoidValve1.getState() && sensor1.read() == LOW)
    {
      oneStarted = false;
    }

    // Sensor/valve2
    if (sensor2.isOn() && !twoStarted)
    {
      //Start OnTimer
      openTimer2 = millis();
      solenoidValve2.on();
      twoStarted = true;
    }
    if (twoStarted && ((millis() - openTimer2) > openTimeout) )
    {
      solenoidValve2.off();
    }
    if (!solenoidValve2.getState() && sensor2.read() == LOW)
    {
      twoStarted = false;
    }
    // Sensor/valve3
    if (sensor3.isOn() && !threeStarted)
    {
      //Start OnTimer
      openTimer3 = millis();
      solenoidValve3.on();
      threeStarted = true;
    }
    if (threeStarted && ((millis() - openTimer3) > openTimeout) )
    {
      solenoidValve3.off();
    }
    if (!solenoidValve3.getState() && sensor3.read() == LOW)
    {
      threeStarted = false;
    }

    // Sensor/valve4
    if (sensor4.isOn() && !fourStarted)
    {
      //Start OnTimer
      openTimer4 = millis();
      solenoidValve4.on();
      fourStarted = true;
    }
    if (fourStarted && ((millis() - openTimer4) > openTimeout) )
    {
      solenoidValve4.off();
    }
    if (!solenoidValve4.getState() && sensor4.read() == LOW)
    {
      fourStarted = false;
    }

    // Check if stop button is pressed or released
    if (buttonStop.onPress() || buttonStop.onRelease())
    {
      state = STOPPED;
    }

  }// RUNNING_TIMERS

  else if (state == START)
  {
    if (sensor1.isOn() ||
        sensor2.isOn() ||
        sensor3.isOn() ||
        sensor4.isOn() )
    {
      state = RUNNING;
    }
  }//START

  else if (state == START_FIRST)
  {
    // Always open only first valve
    allValvesOff();
    solenoidValve1.on();

    // Keep first On until next sensor is activated
    if (sensor2.isOn() ||
        sensor3.isOn() ||
        sensor4.isOn() )
    {
      solenoidValve1.off();
      state = RUNNING;
    }
  }//START_FIRST

  else if (state == STOPPED)
  {
    //Serial.println(F("========== STOPPED =========="));
    allValvesOff();

    if (buttonStart.onPress())
    {
      state = START;
      Serial.println(F("========== START =========="));

    }
    if (buttonStartFirst.onPress())
    {
      //state = START_FIRST;
      //Serial.println(F("========== START FIRST =========="));

      state = RUNNING_TIMERS;
      Serial.println(F("========== RUNNING_TIMERS =========="));

    }
  } // STOPPED
  else
  {
    //Error
    Serial.println(F("!!! ERROR !!! Unknown state"));
    state = STOPPED;
  }

  // Check if stop button is pressed or released
  if (buttonStop.onPress() || buttonStop.onRelease())
  {
    state = STOPPED;
  }

  // Read command from serial if available
  if (Serial.available() > 0)
  {
    serialCommand = Serial.read();
    Serial.print("command: ");
    Serial.println(serialCommand);
    if (serialCommand == 49)
      openTimeout += 5;

    if (serialCommand == 50)
      openTimeout -= 5;

    Serial.print("openTimeout: ");
    Serial.println(openTimeout);
    serialCommand = 0;

  }

  //Check printer timer
  if (millis() - timer > printTimeout)
  {
    printTimes();
    // printDebug();
  }
}

void printTimes()
{
  String sol1 = "Solenoid1: "; sol1 += solenoidValve1.getLastOnTime();
  String sol2 = "Solenoid2: "; sol2 += solenoidValve2.getLastOnTime();
  String sol3 = "Solenoid3: "; sol3 += solenoidValve3.getLastOnTime();
  String sol4 = "Solenoid4: "; sol4 += solenoidValve4.getLastOnTime();

  Serial.println(F("=================="));
  Serial.println(sol1);
  Serial.println(sol2);
  Serial.println(sol3);
  Serial.println(sol4);
  Serial.println(F("==================\n"));

  // reset timer
  timer = millis();
}

void printDebug()
{
  Serial.println(F("======= DEBUG ========"));
  String stateStr = "State= ";

  switch (state) {
    case START_FIRST:
      stateStr += "START_FIRST";
      break;
    case START:
      stateStr += "START";
      break;
    case RUNNING:
      stateStr += "RUNNING";
      break;
    case STOPPED:
      stateStr += "STOPPED";
      break;
    default:
      stateStr += "UNKNOWN";
      break;
  }

  Serial.println(stateStr);

  String sol1 = "Solenoid1: "; sol1 += (solenoidValve1.getState()) ? "ON" : "OFF";
  String sol2 = "Solenoid2: "; sol2 += (solenoidValve2.getState()) ? "ON" : "OFF";
  String sol3 = "Solenoid3: "; sol3 += (solenoidValve3.getState()) ? "ON" : "OFF";
  String sol4 = "Solenoid4: "; sol4 += (solenoidValve4.getState()) ? "ON" : "OFF";

  Serial.println(sol1);
  Serial.println(sol2);
  Serial.println(sol3);
  Serial.println(sol4);

  String btnSTARTF = "Btn StartFirst: "; btnSTARTF += (buttonStartFirst.read()) ? "ON" : "OFF";
  String btnSTART  =      "Btn Start: "; btnSTART  += (buttonStart.read())      ? "ON" : "OFF";
  String btnSTOP   =       "Btn Stop: "; btnSTOP   += (buttonStop.read())       ? "ON" : "OFF";

  Serial.println(btnSTARTF);
  Serial.println(btnSTART);
  Serial.println(btnSTOP);

  String sens1 = "Sensor1: "; sens1 += (sensor1.isOn()) ? "ON" : "OFF";
  String sens2 = "Sensor2: "; sens2 += (sensor2.isOn()) ? "ON" : "OFF";
  String sens3 = "Sensor3: "; sens3 += (sensor3.isOn()) ? "ON" : "OFF";
  String sens4 = "Sensor4: "; sens4 += (sensor4.isOn()) ? "ON" : "OFF";

  Serial.println(sens1);
  Serial.println(sens2);
  Serial.println(sens3);
  Serial.println(sens4);

  Serial.println(F("==================\n"));
  // reset timer
  timer = millis();
}
void allValvesOff()
{
  solenoidValve1.off();
  solenoidValve2.off();
  solenoidValve3.off();
  solenoidValve4.off();
}

/*******************************************************


********************************************************/
