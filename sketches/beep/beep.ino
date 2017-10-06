#include <Servo.h>
#include <Wire.h>
#include <Yeti.h>                //include the yeti library
#include <EEPROM.h>

/*
This program has been written do demonstrate how great Yeti is.
Yeti does have to be calibrated before loading this program, or
it might fall over or not walk properly.
It starts by displaying 1234 on the display, if that is connected.
Immediately after that, it swings its body from right to left.
After that, it will make 6 steps forward and then make the front
LED's act as a heartbeat.
*/

yeti robot;                    //create a yeti object

int rightLED = 8;              //pin connected to the right led
int leftLED = 2;               //pin connected to the left led

void setup()
{
  robot.initYeti();                     //initialize the yeti program
  robot.beep(330, 10000);
}

void loop()
{
digitalWrite(rightLED, HIGH);
delay(1000);
digitalWrite(rightLED, LOW);
digitalWrite(leftLED, HIGH);
delay(1000);
digitalWrite(leftLED, LOW);
}
