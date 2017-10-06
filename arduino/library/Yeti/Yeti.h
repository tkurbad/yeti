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


class yeti
{
	private:
		Servo servo1;					// Servo object 1 (Front Servo)
		Servo servo2;					// Servo object 2 (Bottom Servo)

		unsigned int distancePos;		// Ultrasound distance in cm
		
		int beepNegative;				// pin attached to the negative side of the beeper
		int beepPositive;				// pin attached to the positive side of the beeper
		int leftLED;					// Left LED (1) pin
		int rightLED;					// Right LED (2) pin

		int displayAdress;				// I2C address of the 4* 7SEG driver chip, bit shifted because arduino uses 7 bit adresses
		int digitsForDisplay[10];		// Digits 0 to 9

		// Variables to control servo movements
		int posLean, negLean, posFwd, negFwd;
		int servoMid;

		// Variables / Constants to keep track of the position of the legs/body
		int bodyPos;
		int bodyLeftFwd, bodyRightFwd;

		int leanRight, leanLeft, leanUp;
		int prevLean;

		int leftForward, rightForward, bodyLevel;
		int leanCurrent, bodyCurrent;
		
		int goForward, goLeft, goRight, goBackwards, prevMove;
		
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

		void beep(int frequency, int duration);	// Sound the buzzer
		void initDisplay();						// Initialize the display
		void displayDigit(int displayNumber = 0000);	// Display a 4 digit number 
		void displayDigit(int firstDigit, int secondDigit, 
			int thirdDigit, int forthDigit);	// Display single digits

		void usleep(unsigned long us);			// Sleep function
		void increaseCount();
		void initPing();						// Initialise the ultrasound sensor
		int  ping();							// Get the current distance measurement from ultrasound sensor

		void initRF(int baudrate);				// Initialize for the wireless module
		void getRFData();						// Get data from the wireless module
		void respondRF();						// Move Yeti according to wireless command, send data back (servo position, etc.)
		void updateCommand();					// Return data about the current state of Yeti to the wireless module
		void RFTest();							// Test wireless connection
};

#endif
