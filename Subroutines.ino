
void ExtrudeTemp() 
{
// report extruder temperature:
    // send sensor values:
    String tempstring = "Extruder Temperature = ";
    tempstring = tempstring + ExtruderTempInt + " F [Setpoint is " + tempsetpoint + " F]";
    Serial.println(tempstring);
}

void ExtMotorFor() 
{
// run the motor forward:
  Serial.println("Running the Motor Forward");   // send text to the Serial Monitor  
  // pause motor before running forward
  digitalWrite(stepPin, LOW);
  delay(1000);
  digitalWrite(directionPin, HIGH);
  tone(stepPin, motorspeed);
}

void ExtMotorRev() 
{
// run the motor in reverse:
  Serial.println("Running the Motor in Reverse");  
  // pause motor before running in reverse
  digitalWrite(stepPin, LOW);
  delay(1000);
  digitalWrite(directionPin, LOW);
  tone(stepPin, motorspeed);
}

void ExtMotorInc() 
{
  // increase the motor speed:
  if (motorspeed < motorhighlim)  {
    motorspeed = motorspeed + 10;
    String tempstring = "Motor speed increased to ";
    tempstring = tempstring + motorspeed + " pps";
    Serial.println(tempstring);
  }
  else {
    String tempstring = "Motor speed upper limit is ";
    tempstring = tempstring + motorhighlim + " pps";
    Serial.println(tempstring);
  }
}

void ExtMotorDec() 
{
  // decrease the motor speed:
  if (motorspeed > motorlowlim)  {
    motorspeed = motorspeed - 10;
    String tempstring = "Motor speed decreased to ";
    tempstring = tempstring + motorspeed + " pps";
    Serial.println(tempstring);
  }
  else {
    String tempstring = "Motor speed lower limit is ";
    tempstring = tempstring + motorlowlim + " pps";
    Serial.println(tempstring);
  }
}

void ExtMotorOff() 
{
// Stop the motor:
  Serial.println("Stopping the Motor");
  noTone(stepPin);
  digitalWrite(stepPin, LOW);
}

void TempMode() 
{
// set temp control mode:
  if (inByte == 'A')  {
    tempcontrol = 1;
    Serial.println("Temp Mode Set to Auto");   // send text to the Serial Monitor
  }
  if (inByte == 'M')  {
    tempcontrol = 0;
    // turn heater off:
    digitalWrite(HeaterPin, LOW);
    heating = 0;
    Serial.println("Temp Mode Set to Manual");   // send text to the Serial Monitor
  }
}

void TempInc() 
{
// increase temp setpoint:
  if (tempcontrol == 1)  {
    if (tempsetpoint < 340)  {
      tempsetpoint = tempsetpoint + 5;
      Setpoint = tempsetpoint;
      String tempstring = "Extruder temp setpoint increased to ";
      tempstring = tempstring + tempsetpoint + " F";
      Serial.println(tempstring);
    }
  }
}

void TempDec()
{
// decrease temp setpoint:
  if (tempcontrol == 1)  {
    if (tempsetpoint > 30)  {
      tempsetpoint = tempsetpoint - 5;
      Setpoint = tempsetpoint;
      String tempstring = "Extruder temp setpoint decreased to ";
      tempstring = tempstring + tempsetpoint + " F";
      Serial.println(tempstring);
    }
  }
}

void HeaterOn() 
{
// turn heater on:
  if (tempsetpoint < 340)  {
    digitalWrite(HeaterPin, HIGH);
    heating = 1;
    Serial.println("Heater turned on");   // send text to the Serial Monitor
    ExtrudeTemp();
  }
}

void HeaterOff() 
{
// turn heater off:
  digitalWrite(HeaterPin, LOW);
  heating = 0;
  Serial.println("Heater turned off");   // send text to the Serial Monitor
  ExtrudeTemp();
}
