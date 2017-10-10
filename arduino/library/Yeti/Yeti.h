#ifndef YETI_h
#define YETI_h

#include <avr/interrupt.h>
#include "Arduino.h"
#include <Wire.h>
#include <Servo.h>
#include <EEPROM.h>

/*
 * Yeti.h: Library to control the walking yeti robot with Arduino
 * Copyright (c) 2015 Thom Kouwen. All rights reserved.
*/


/* Global defines */

// Pins
#define BEEP_NEGATIVE	4	// pin attached to the negative side of the beeper
#define BEEP_POSITIVE	11	// pin attached to the positive side of the beeper
#define LED_LEFT		2	// Left LED (1) is attached to pin 2 on the Arduino
#define LED_RIGHT		8	// Right LED (2) is attached to pin 8 on the Arduino
#define PING			7	// Pin Parallax Ping sensor is connected to (AIN1)

// Movement/Position Constants
#define BODY_LEFT_FWD	1	// Left body side is in forward position
#define BODY_RIGHT_FWD	2	// Right body side is in forward position

#define LEAN_RIGHT		3	// Lean to the right
#define LEAN_LEFT		4	// Lean to the left

#define LEFT_FORWARD	5	// Left leg is in forward position
#define	RIGHT_FORWARD	6	// Right leg is in forward position
#define BODY_LEVEL		7	// Both legs are parallel to the body

// Musical Notes
#define NOTE_C0			16
#define NOTE_C0_SH		17
#define NOTE_D0			18
#define NOTE_D0_SH		19
#define NOTE_E0			21
#define NOTE_F0			22
#define NOTE_F0_SH		23
#define NOTE_G0			25
#define NOTE_G0_SH		26
#define NOTE_A0			28
#define NOTE_A0_SH		29
#define NOTE_B0			31
#define NOTE_C1			33
#define NOTE_C1_SH		35
#define NOTE_D1			37
#define NOTE_D1_SH		39
#define NOTE_E1			41
#define NOTE_F1			44
#define NOTE_F1_SH		46
#define NOTE_G1			49
#define NOTE_G1_SH		52
#define NOTE_A1			55
#define NOTE_A1_SH		58
#define NOTE_B1			62
#define NOTE_C2			65
#define NOTE_C2_SH		69
#define NOTE_D2			73
#define NOTE_D2_SH		78
#define NOTE_E2			82
#define NOTE_F2			87
#define NOTE_F2_SH		93
#define NOTE_G2			98
#define NOTE_G2_SH		104
#define NOTE_A2			110
#define NOTE_A2_SH		117
#define NOTE_B2			123
#define NOTE_C3			131
#define NOTE_C3_SH		139
#define NOTE_D3			147
#define NOTE_D3_SH		156
#define NOTE_E3			165
#define NOTE_F3			175
#define NOTE_F3_SH		185
#define NOTE_G3			196
#define NOTE_G3_SH		208
#define NOTE_A3			220
#define NOTE_A3_SH		233
#define NOTE_B3			247
#define NOTE_C4			262
#define NOTE_C4_SH		277
#define NOTE_D4			293
#define NOTE_D4_SH		294
#define NOTE_E4			330
#define NOTE_F4			349
#define NOTE_F4_SH		370
#define NOTE_G4			392
#define NOTE_G4_SH		415
#define NOTE_A4			440
#define NOTE_A4_SH		466
#define NOTE_B4			494
#define NOTE_C5			523
#define NOTE_C5_SH		554
#define NOTE_D5			587
#define NOTE_D5_SH		622
#define NOTE_E5			659
#define NOTE_F5			698
#define NOTE_F5_SH		740
#define NOTE_G5			784
#define NOTE_G5_SH		831
#define NOTE_A5			880
#define NOTE_A5_SH		932
#define NOTE_B5			987
#define NOTE_C6			1047
#define NOTE_C6_SH		1109
#define NOTE_D6			1175
#define NOTE_D6_SH		1245
#define NOTE_E6			1319
#define NOTE_F6			1397
#define NOTE_F6_SH		1480
#define NOTE_G6			1568
#define NOTE_G6_SH		1661
#define NOTE_A6			1760
#define NOTE_A6_SH		1865
#define NOTE_B6			1976
#define NOTE_C7			2093
#define NOTE_C7_SH		2217
#define NOTE_D7			2349
#define NOTE_D7_SH		2489
#define NOTE_E7			2637
#define NOTE_F7			2794
#define NOTE_F7_SH		2960
#define NOTE_G7			3136
#define NOTE_G7_SH		3322
#define NOTE_A7			3520
#define NOTE_A7_SH		3729
#define NOTE_B7			3951
#define NOTE_C8			4186
#define NOTE_C8_SH		4435
#define NOTE_D8			4699
#define NOTE_D8_SH		4978
#define NOTE_E8			5274
#define NOTE_F8			5588
#define NOTE_F8_SH		5920
#define NOTE_G8			6272
#define NOTE_G8_SH		6645
#define NOTE_A8			7040
#define NOTE_A8_SH		7459
#define NOTE_B8			7902


class yeti
{
	private:
		Servo servo1;					// Servo object 1 (Front Servo)
		Servo servo2;					// Servo object 2 (Bottom Servo)

		unsigned int distancePos;		// Ultrasound distance in cm
		
		int displayAdress;				// I2C address of the 4* 7SEG driver chip, bit shifted because arduino uses 7 bit adresses
		int digitsForDisplay[10];		// Digits 0 to 9

		// Variables to control servo movements
		int posLean;
		int negLean;
		int posFwd;
		int negFwd;
		int servoMid;
		int noLean;

		// Variables / Constants to keep track of the position of the legs/body
		int bodyPos;
		int prevLean;
		int leanCurrent;
		int bodyCurrent;

		int goForward;
		int goLeft;
		int goRight;
		int goBackwards;
		int prevMove;

		// Variables for the RF stuff
		byte inByte, inByte1, inByte2, inByte3, inByte4, 
			inByte5, inByte6, inByte7, inByte8;


	public:

		yeti();
		void initYeti();
		void calibrate();						// Calibrate Yeti's legs 

		void wirelessSerialInit();				// Initialize the wireless serial module
		void IRSerialprint(int TXData);			// Send a byte over the IR serial port
		void IRSerialprintln(int TXData);		// Send a byte and a newline over the IR serial port
		void IRSerialprint(String TXData);		// Send a string over the IR serial port
		void IRSerialprintln(String TXData);	// Send a string and newline over the IR serial port
		int  IRSerialread();					// Read data from the serial bus

		void moveForwardX(int nrSteps);			// Make yeti walk in a straight line
		void moveForwardXNC();					// Make yeti walk, no centering on exit
		void moveBackwardXNC();					// Make yeti walk backwards, no centering on exit
		void turnRight(int angle);				// Make yeti turn right
		void turnLeft(int angle);				// Make yeti turn left
		void moveBody(int leanDirection, int delayFactor = 13);	// Move yeti's body
		void moveLegs(int walkDirection, int delayFactor = 10);	// Move yeti's legs

		void leftLEDOn();						// Turn on left LED
		void leftLEDOff();						// Turn off left LED
		void rightLEDOn();						// Turn on right LED
		void rightLEDOff();						// Turn off right LED

		void beep(int frequency, int duration);	// Sound the buzzer
		void playNote(int note, int duration, int delayms = 0);
												// Play a note and (optionally) delay
		void initDisplay();						// Initialize the display
		void displayDigit(int displayNumber = 0000);	// Display a 4 digit number 
		void displayDigit(int firstDigit, int secondDigit, 
			int thirdDigit, int forthDigit);	// Display single digits

		/* No longer needed since we use a Parallax Ping distance sensor
		 * instead of the one supplied by Arexx
		 */
		//void usleep(unsigned long us);		// Sleep function
		//void initPing();						// Initialise the ultrasound sensor

		int  ping();							// Get the current distance measurement from ultrasound sensor

		void initRF(int baudrate);				// Initialize for the wireless module
		void getRFData();						// Get data from the wireless module
		void respondRF();						// Move Yeti according to wireless command, send data back (servo position, etc.)
		void updateCommand();					// Return data about the current state of Yeti to the wireless module
		void RFTest();							// Test wireless connection
};

#endif
