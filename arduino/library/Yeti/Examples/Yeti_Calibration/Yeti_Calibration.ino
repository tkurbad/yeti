#include <Wire.h>
#include <Yeti.h>
#include <Servo.h>
#include <EEPROM.h>

/*
This program has been designed to make use of the calibration function supplied in the 
Yeti library. This program executes the calibration procedure, after uploading you need 
to open the Serial Port viewer in the top right hand corner of the Arduino IDE. This will 
guide you through the calibration process. See the manual for extra explanation of how 
this works
*/

yeti robot;                             //make a Yeti object

void setup()
{
  robot.initYeti();                     //initialize basic Yeti functionality 
  robot.wirelessSerialInit();           //initialize the Serial port
  robot.calibrate();                    //execute the calibration procedure
  
  robot.moveForwardX(0);                //center Yeti
}

void loop()
{
  //nothing to do here
}
  
