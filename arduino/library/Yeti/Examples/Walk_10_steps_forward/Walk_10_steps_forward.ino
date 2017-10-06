#include <Servo.h>
#include <Wire.h>
#include <Yeti.h>                //include the yeti library
#include <EEPROM.h>

/*
Sketch written to test the yeti library, not to be released
*/

yeti robot;                    //create a yeti object

int rightLED = 8;              //pin connected to the right led
int leftLED = 2;               //pin connected to the left led

//variables for heartbeat function
uint8_t hbval=128;
int8_t hbdelta=8;
int timeInterval = 20;
int timePrevious = 0;

void setup()
{
  robot.initYeti();              //initialize the yeti program
  robot.initPing();              //initialize and enable the display
  robot.initDisplay();           //initialize and enable the display
  
  pinMode(rightLED, OUTPUT);     //set the pin attached to right led to output
  pinMode(leftLED, OUTPUT);      //set the pin attached to left led to output
  digitalWrite(rightLED, HIGH);  //switch the led on
  digitalWrite(leftLED, HIGH);   //switch other LED on as well 

  robot.moveForwardX(10);        //make Yeti take 10 steps forward
}

void loop()
{
  //make Yeti's LEDs show a heartbeat
  if(millis() - timePrevious > timeInterval)
  {
    if (hbval > 192) 
      hbdelta = -hbdelta;
    if (hbval < 16) 
      hbdelta = -hbdelta;
    hbval += hbdelta;
    timePrevious = millis();
  }
  //no PWM available, this calls for a bit of creativity. This is normally not 
  //a very good way to do this, because it uses the delay function, but since
  //the heartbeat is the only thing being executed, it's not important it is blocking
  digitalWrite(rightLED, HIGH);
  digitalWrite(leftLED, LOW);
  delayMicroseconds(255-hbval);
  digitalWrite(rightLED, LOW);
  digitalWrite(leftLED, HIGH);
  delayMicroseconds(hbval);
}
