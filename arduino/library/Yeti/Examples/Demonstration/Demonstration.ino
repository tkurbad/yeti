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

//variables for heartbeat function, makes the front LEDs act like a heart :)
uint8_t hbval=128;
int8_t hbdelta=8;
int timeInterval = 20;
int timePrevious = 0;

//used for swinging Yeti from side to side
byte bodyRight = 3;
byte bodyLeft = 4;
byte bodyLevel = 5;


void setup()
{
  robot.initYeti();                     //initialize the yeti program
  robot.initPing();                     //initialize and enable the ultrasound
  robot.initDisplay();                  //initialize and enable the display
  
  pinMode(rightLED, OUTPUT);            //set the pin attached to right led to output
  pinMode(leftLED, OUTPUT);             //set the pin attached to left led to output
  digitalWrite(rightLED, HIGH);         //switch the led on
  robot.displayDigit(1, 2, 3, 4);       //display 1234 on the display
  
  robot.moveForwardX(0);
  delay(300);
  robot.moveBody(bodyRight);	        //swing Yeti's body to the right
  delay(300);
  robot.moveBody(bodyLeft);		        //swing to the left
  delay(150);
  robot.moveBody(bodyLevel);	        //level body
    
  robot.moveForwardX(6);                //make Yeti take 6 steps forward
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
  //no PWM on these pins so we have to get creative :)
  digitalWrite(rightLED, HIGH);
  digitalWrite(leftLED, LOW);
  delayMicroseconds(255-hbval);
  digitalWrite(rightLED, LOW);
  digitalWrite(leftLED, HIGH);
  delayMicroseconds(hbval);
}
