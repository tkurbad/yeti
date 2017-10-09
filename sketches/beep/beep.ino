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

void setup()
{
  robot.initYeti();                     //initialize the yeti program
  robot.initPing();
  robot.playNote(NOTE_G1, 500, 600);
  robot.playNote(NOTE_E1, 500, 600);
  robot.playNote(NOTE_E1, 500, 1100);
  robot.playNote(NOTE_F1, 500, 600);
  robot.playNote(NOTE_D1, 500, 600);
  robot.playNote(NOTE_D1, 500, 1100);
  robot.playNote(NOTE_C1, 500, 600);
  robot.playNote(NOTE_D1, 500, 600);
  robot.playNote(NOTE_E1, 500, 600);
  robot.playNote(NOTE_F1, 500, 600);
  robot.playNote(NOTE_G1, 500, 600);
  robot.playNote(NOTE_G1, 500, 600);
  robot.playNote(NOTE_G1, 500, 1100);
  robot.playNote(NOTE_G1, 500, 600);
  robot.playNote(NOTE_E1, 500, 600);
  robot.playNote(NOTE_E1, 500, 1100);
  robot.playNote(NOTE_F1, 500, 600);
  robot.playNote(NOTE_D1, 500, 600);
  robot.playNote(NOTE_D1, 500, 1100);
  robot.playNote(NOTE_C1, 500, 600);
  robot.playNote(NOTE_E1, 500, 600);
  robot.playNote(NOTE_G1, 500, 600);
  robot.playNote(NOTE_G1, 500, 600);
  robot.playNote(NOTE_C1, 1500, 2100);
  robot.playNote(NOTE_D1, 500, 600);
  robot.playNote(NOTE_D1, 500, 600);
  robot.playNote(NOTE_D1, 500, 600);
  robot.playNote(NOTE_D1, 500, 600);
  robot.playNote(NOTE_D1, 500, 600);
  robot.playNote(NOTE_E1, 500, 600);
  robot.playNote(NOTE_F1, 500, 1100);
  robot.playNote(NOTE_E1, 500, 600);
  robot.playNote(NOTE_E1, 500, 600);
  robot.playNote(NOTE_E1, 500, 600);
  robot.playNote(NOTE_E1, 500, 600);
  robot.playNote(NOTE_E1, 500, 600);
  robot.playNote(NOTE_F1, 500, 600);
  robot.playNote(NOTE_G1, 500, 1100);
  robot.playNote(NOTE_G1, 500, 600);
  robot.playNote(NOTE_E1, 500, 600);
  robot.playNote(NOTE_E1, 500, 1100);
  robot.playNote(NOTE_F1, 500, 600);
  robot.playNote(NOTE_D1, 500, 600);
  robot.playNote(NOTE_D1, 500, 1100);
  robot.playNote(NOTE_C1, 500, 600);
  robot.playNote(NOTE_E1, 500, 600);
  robot.playNote(NOTE_G1, 500, 600);
  robot.playNote(NOTE_G1, 500, 600);
  robot.playNote(NOTE_C1, 1500, 2100);
}

void loop()
{
  robot.playNote(NOTE_B8, 50, 0);
  robot.rightLEDOn();
  delay(1000);
  robot.rightLEDOff();
  robot.leftLEDOn();
  delay(1000);
  robot.leftLEDOff();
  int distance=robot.ping();
}
