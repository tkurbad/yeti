#include <Servo.h>
#include <Wire.h>
#include <Yeti.h>             // Include the yeti library
#include <EEPROM.h>

/*
This is a small example program making Yeti sing, blink
his eyes, and probably more.
*/

yeti robot;                   // Create a yeti object
long distance = 0;             // Variable to store measured distance

void setup()
{
  robot.initYeti();           // Initialize the yeti
  robot.initDisplay();        // Initialize the 4 digit display
  robot.displayDigit(distance);
/*  robot.playNote(NOTE_G1, 500, 600);  // Play "Haenschen klein"
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
 */
}

void loop()
{
  robot.playNote(NOTE_B8, 50, 0); // Give a short beep
  distance = robot.ping();        // Measure distance
  robot.displayDigit(distance);   // Show distance on display
  robot.rightLEDOn();             // Blink right and left leds
  delay(1000);
  distance = robot.ping();        // Measure distance
  robot.displayDigit(distance);   // Show distance on display
  robot.rightLEDOff();
  robot.leftLEDOn();
  delay(1000);
  distance = robot.ping();        // Measure distance
  robot.displayDigit(distance);   // Show distance on display
  robot.leftLEDOff();
}
