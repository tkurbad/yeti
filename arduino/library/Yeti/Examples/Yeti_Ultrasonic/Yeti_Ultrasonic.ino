#include <Servo.h>
#include <Wire.h>
#include <Yeti.h>           
#include <EEPROM.h>

/*
This program has been designed to work with the ultrasonic expansion module
which is available for the Yeti, if you also have the display module, you can
uncomment the display setup and print functions and the distance will be 
printed on the display
*/

yeti robot;                    //create a yeti object

int rightLED = 8;              //pin connected to the right led


void setup()
{
  robot.initYeti();                //initialize the yeti program
  robot.initPing();                //initialize and enable the display
  robot.moveForwardX(0);           //center Yeti
  //robot.initDisplay();           //initialize and enable the display
  //robot.displayDigit(1, 2, 3, 4);      //display 5678 on the display
  
  pinMode(rightLED, OUTPUT);       //set the pin to output
  digitalWrite(rightLED, HIGH);    //switch the led on
  
  Serial.begin(57600);
  Serial.println("setup done");
}

void loop()
{
  delay(1500);
  int distanceToObject = robot.ping();
  Serial.print("distance is: ");
  Serial.println(distanceToObject);
  //robot.displayDigit(distanceToObject);  
}
