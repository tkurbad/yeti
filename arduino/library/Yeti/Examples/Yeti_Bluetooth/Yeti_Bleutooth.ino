#include <Servo.h>
#include <Wire.h>
#include <Yeti.h>                //include the yeti library
#include <EEPROM.h>

/*
Sketch written to test the yeti library, not to be released
*/

yeti robot;                      //create a yeti object

int rightLED = 8;                //pin connected to the right led
int leftLED = 2;                 //pin connected to the left led

void setup()
{
  robot.initYeti();              //initialize the yeti program
  robot.initRF(9600);            //initialize the RF part of the library
  
  pinMode(rightLED, OUTPUT);     //set the pin attached to right led to output
  pinMode(leftLED, OUTPUT);      //set the pin attached to left led to output
  digitalWrite(rightLED, HIGH);  //switch the led on
  digitalWrite(leftLED, HIGH);   //switch other LED on as well 

  robot.moveForwardX(0);          //center Yeti
}

void loop()
{
  robot.getRFData();              //get data from the bluetooth module
  robot.respondRF();              //move Yeti according to the data
}
