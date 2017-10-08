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

// Notenwerte
int nC1=131;
int nC1_sharp=139;
int nD1=147;
int nD1_sharp=156;
int nE1=165;
int nE1_sharp=175;
int nF1=175;
int nF1_sharp=185;
int nG1=196;
int nG1_sharp=208;
int nA1=220;
int nA1_sharp=233;
int nB1=247;
int nB1_sharp=262;

/*C 261,63
#C  277,18
D 293,66
#D  311,13
E 329,63
#E  349,23
F 349,23
#F  369,99
G 391,995
#G  415,3
A 440
#A  466,16
B 493,88
#B  523,25
+C  523,25
+#C 554,37
+D  587,33
+#D 622,25
+E  659,26
+#E 698,46
+F  698,46
+#F 739,99
+G  783,99
+#G 830,61
+A  880
+#A 932,33
+B  987,77
+#B 1046,5
*/

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
  digitalWrite(rightLED, HIGH);
  delay(1000);
  digitalWrite(rightLED, LOW);
  digitalWrite(leftLED, HIGH);
  delay(1000);
  digitalWrite(leftLED, LOW);
  int distance=robot.ping();
}
