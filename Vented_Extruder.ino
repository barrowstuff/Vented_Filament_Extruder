/*
Vented Filament Extruder Application

Language: Wiring/Arduino
 
*/

/********************************************************
 *
 * To connect the PID to the digital out use "time proportioning
 * control"  It's essentially a really slow version of PWM.
 * First we decide on a window size (30000mS) We then 
 * set the pid to adjust its output between 0 and that window
 * size.  Lastly, we add some logic that translates the PID
 * output into "Relay On Time" with the remainder of the 
 * window being "Relay Off Time"
 ********************************************************/

#include <PID_v1.h>

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

int WindowSize = 30000;
unsigned long windowStartTime;
byte directionPin = 8;
byte stepPin = 9;
int motorspeed = 100;
int motorhighlim = 500;
int motorlowlim = 50;
int tempcontrol = 0;
int heating = 0;
int tempsetpoint = 120;
byte HeaterPin = 13;
int pulseWidthMicros = 5;  // microseconds
int millisbetweenSteps = 5; // milliseconds
String tempstring = "Extruder Temperature = ";

float ExtruderTemp = 0;    // extruder temperature sensor
int ExtruderTempInt = 0;    // integer value of the extruder temperature sensor
int inByte = 0;         // incoming serial byte


void setup() 
{ 
  // start serial port at 9600 bps:
  Serial.begin(9600);
  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  digitalWrite(stepPin, LOW);
  pinMode(HeaterPin, OUTPUT);
  
  windowStartTime = millis();

  //initialize the variables we're linked to
  Setpoint = tempsetpoint;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop()
{
  // if we get a valid byte, branch to selected logic:
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
  }
  
  // report extruder temperature when a T is sent:
  if (inByte == 'T')  {
    ExtrudeTemp();
  } 
  
  // run the motor forward when a F is sent:
  if (inByte == 'F')  {
    ExtMotorFor();
  }
  
  // run the motor in reverse when a R is sent:
  if (inByte == 'R')  {
    ExtMotorRev();
  }
    
  // stop the motor when a S is sent:
  if (inByte == 'S')  {
    ExtMotorOff();
  }
  
   // Increase the motor speed when a Z is sent:
  if (inByte == 'Z')  {
    ExtMotorInc();
  }
   
   // Decrease the motor speed when a X is sent:
  if (inByte == 'X')  {
    ExtMotorDec();
  }
  
  // turn on the heater when a H is sent:
  if (inByte == 'H')  {
    HeaterOn();
  }
  // turn off the heater when a C is sent:
  if (inByte == 'C')  {
    HeaterOff();
  }
  
  // increase the temp setpoint:
  if (inByte == 'U')  {
    TempInc();
  }  
  
  // decrease the temp setpoint:
  if (inByte == 'D')  {
    TempDec();
  }
 
  // set temp control mode to auto or manual:
  if (inByte == 'A')  {
    TempMode();
  }
    if (inByte == 'M')  {
    TempMode();
  }
  
  // temp control in auto mode
  // read extruder temperature input:
  ExtruderTemp = (18.63637994*pow(1.002467337,analogRead(A0))) + 140;
  ExtruderTempInt = int(ExtruderTemp);
  // delay 10ms to let the ADC recover:
  delay(100);
  
  /*
  if (tempcontrol == 1)  {
    if (ExtruderTempInt > tempsetpoint)  {
      if (heating == 1){
        HeaterOff();
        delay(20);
      }
    }
  }
  
  if (tempcontrol == 1)  {
    if (ExtruderTempInt < tempsetpoint - 2)  {
      if (heating == 0){
        HeaterOn();
        delay(20);
      }
    }
  }
  */
  inByte = 0;
 
  Input = ExtruderTempInt;
 
  if (tempcontrol == 1)  {
    myPID.Compute();
  
    /************************************************
     * turn the output pin on/off based on pid output
     ************************************************/
    unsigned long now = millis();
    if(now - windowStartTime>WindowSize)
    { //time to shift the Relay Window
      windowStartTime += WindowSize;
    }
    if(Output > now - windowStartTime) HeaterOn();
    else HeaterOff();
  }

  if (tempsetpoint > 335)  {
    HeaterOff();
    delay(20);
  }
}
