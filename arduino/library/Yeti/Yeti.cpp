#include <avr/io.h>
#include <avr/interrupt.h>
#include "Arduino.h"
#include <Wire.h>
#include <Servo.h>
#include <EEPROM.h>
#include "Yeti.h"

/*
 * Yeti.cpp: Library to control the walking yeti robot with Arduino
 * Copyright (c) 2015 Thom Kouwen. All rights reserved.
*/

#define USE_TIMER1

volatile unsigned long count80kHz;	// internal variable for the ultrasound sensor

static inline void increaseCount()
{
	count80kHz++;
}

ISR(TIMER2_COMPA_vect)				// interrupt to increase count on count80kHz variable
{
	increaseCount();
}

ISR(ANALOG_COMP_vect)
{
	//PORTB &= ~(1 << PORTB1);
}


yeti::yeti()
{
	Wire.begin();					// start the I2C bus

	count80kHz = 0;					// Initialize ultrasound sensor count

	// Variable initialization:
	beepNegative = 4;				// pin attached to the negative side of the beeper
	beepPositive = 11;				// pin attached to the positive side of the beeper
	leftLED = 2;					// Left LED (1) is attached to pin 2 on the Arduino
	rightLED = 8;					// Right LED (2) is attached to pin 8 on the Arduino
	pinMode(leftLED, OUTPUT);		// Set LED1 pin to output
	pinMode(rightLED, OUTPUT);		// Set LED2 pin to output

	displayAdress = 0x70 >> 1;		// I2C address of the 4* 7SEG driver chip, bit shifted because arduino uses 7 bit adresses

	// Digits 0 to 9
	digitsForDisplay[0] = 63;
	digitsForDisplay[1] = 6;
	digitsForDisplay[2] = 91;
	digitsForDisplay[3] = 79;
	digitsForDisplay[4] = 102;
	digitsForDisplay[5] = 109;
	digitsForDisplay[6] = 125;
	digitsForDisplay[7] = 7;
	digitsForDisplay[8] = 127;
	digitsForDisplay[9] = 111;

	// (Default) Variables to control servo movements
	if(EEPROM.read(0) == 255)
	{
		posLean = 62;				// 62
		posFwd = 65;				// 65
		servoMid = 87;				// 87
		negLean = 122;				// 122
		negFwd = 109;				// 109
	}
	else
	{
		posLean = EEPROM.read(0);
		posFwd = EEPROM.read(1);
		servoMid = EEPROM.read(2);
		negLean = EEPROM.read(3);
		negFwd = EEPROM.read(4);
	}

	// Constants to keep track of the position of the legs/body
	bodyPos = 0;
	bodyLeftFwd = 1;
	bodyRightFwd = 2;

	leanRight = 3;
	leanLeft = 4;
	leanUp = 5;
	prevLean = 0;

	leftForward = 6;
	rightForward = 7;
	bodyLevel = 8;
	prevMove = 0;

	// Keep track of what Yeti is supposed to do
	goForward = 0;
	goLeft = 0;
	goRight = 0;
	goBackwards = 0;
	prevMove = 0;
}


/* Iniitialize the Yeti */
void yeti::initYeti()
{
	// Attach both servos to the two servo objects
	servo1.attach(10);
	servo2.attach(9);
	servo1.write(87);
	servo2.write(87);

	// Set LED Pin modes
	pinMode(rightLED, OUTPUT);
	pinMode(leftLED, OUTPUT);
}


/* Run the leg calibration procedure */
void yeti::calibrate()
{
	IRSerialprintln("Yeti calibration procedure started");
	IRSerialprintln("Are you sure you want to calibrate me? Type Y or N and press enter");
	// Center both servos, to give users the ability to mount the servo arms at servo center
	servo1.write(87);
	servo1.write(87);
	while(Serial.available() == 0)
		delay(100);						// Wait for response
	char doCalibrate = Serial.read();	// Get response
	// Check and see if the user wants to enter calibration procedure
	if(doCalibrate == 'Y' || doCalibrate == 'y')
	{
		// Explain how the calibration works
		IRSerialprintln("type + or - and press enter to edit value, type X to stop");
		IRSerialprintln("Type S to save value");
		IRSerialprintln("We will start at the front servo, first point is lean right");

		int tempServo1 = 87;			// Servo position from which we are going to start
		int servoPos = 0;				// Which position are we calibrating, 0 = lean right, 1 = center and 2 is lean left
		servo1.write(tempServo1);
		// Here we are going to calibrate the front servo, or servo1
		while(servoPos < 3)
		{
			while(Serial.available() == 0);	//wait for something to happen on the serial bus
			char servoTempValue = Serial.read();
			if(servoTempValue == '+')
				tempServo1++;

			else if(servoTempValue == '-')
				tempServo1--;
		
			else if(servoTempValue == 'X' || servoTempValue == 'x')
				break;

			else if(servoTempValue == 'S' || servoTempValue == 's')
			{
				if(servoPos == 2)
				{
					negLean = tempServo1;
					IRSerialprintln("Lean left saved, now on to bottom servo calibration");
				}
				else if(servoPos == 1)
				{
					leanUp = tempServo1;
					IRSerialprintln("Center saved, now onto lean left");
				}
				else
				{
					posLean = tempServo1;
					IRSerialprintln("Lean right saved, onto center");
				}
				servoPos++;
			}
			else
				IRSerialprintln("Error, invalid entry");
			servo1.write(tempServo1);
		}
		servo1.write(leanUp);

		IRSerialprintln("Bottom servo, first calibration point is right forward");
		IRSerialprintln("Type S to save value");
		int tempServo2 = 87;
		servoPos = 0;				// Which position are we calibrating, 0 = right forward, 1 = center and 2 is left forward
		servo2.write(tempServo2);

		// Here we are going to calibrate the bottom servo, or servo2
		while(servoPos < 3)
		{
			while(Serial.available() == 0);   // Wait for something to happen on the serial bus
			char servoTempValue = Serial.read();
			if(servoTempValue == '+')
				tempServo2++;

			else if(servoTempValue == '-')
				tempServo2--;

			else if(servoTempValue == 'X' || servoTempValue == 'x')
				break;

			else if(servoTempValue == 'S' || servoTempValue == 's')
			{
				if(servoPos == 2)
				{
					negFwd = tempServo2;
					IRSerialprintln("Left forward saved, we are done now");
				}
				else if(servoPos == 1)
				{
					servoMid = tempServo2;
					IRSerialprintln("Center saved, now onto left forward");
				}
				else
				{
					posFwd = tempServo2;
					IRSerialprintln("Right forward saved, onto center");
				}
				servoPos++;
			}
			else
				IRSerialprintln("Error, invalid entry");
			servo2.write(tempServo2);
		}
		servo2.write(servoMid);
		EEPROM.write(0, posLean);
		EEPROM.write(1, posFwd);
		EEPROM.write(2, servoMid);
		EEPROM.write(3, negLean);
		EEPROM.write(4, negFwd);
	}
	else
		IRSerialprintln("calibration cancelled");
}


/* Initialize the serial IR transceiver */
void yeti::wirelessSerialInit()
{
	Serial.begin(2400);
	// PWM setup for IR Serial at 38kHz
	pinMode(11, OUTPUT);
	// Setup timer 2 for PWM on PB3
	TCCR2A = _BV(WGM20);
	TCCR2B = _BV(WGM22) | _BV(CS20); 

	// setup timer2 top value - freq = FCPU / 4 / OCR2A
	OCR2A = F_CPU / 4 / 38000;		// 52 for 8MHz, 105 for 16MHz  
	//TCCR2A |= _BV(COM2A0);		// Enable pwm on pin 11 (the IR LED)
}


/* Send data to the serial IR transceiver */
void yeti::IRSerialprint(int TXData)
{
	while (!(UCSR0A & _BV(UDRE0)));	// Wait until input buffer is clear
	UCSR0B &= ~(1<<RXEN0);			// Disable the receiver
	TCCR2A |= (1<<COM2A0);
	delay(1);

	Serial.print(char(TXData));		// Send the data 
	while (!(UCSR0A & _BV(TXC0)));	// Wait until all data has been transmitted
	TCCR2A &= ~(1<<COM2A0);

	delay(5);						// Add small delay to prevent receiving of reflections
	UCSR0B |= (1<<RXEN0);			// Enable the receiver again
}


/* Send data and a newline to the serial IR transceiver */
void yeti::IRSerialprintln(int TXData)
{
	while (!(UCSR0A & _BV(UDRE0)));	// Wait until input buffer is clear
	UCSR0B &= ~(1<<RXEN0);			// Disable the receiver
	TCCR2A |= (1<<COM2A0);
	delay(1);

	Serial.print(char(TXData));		// Send the data 
	Serial.println();				// Send newline
	while (!(UCSR0A & _BV(TXC0)));	// Wait until all data has been transmitted
	TCCR2A &= ~(1<<COM2A0);

	delay(5);						// Add small delay to prevent receiving of reflections
	UCSR0B |= (1<<RXEN0);			// Enable the receiver again
}


/* Send a string to the serial IR transceiver */
void yeti::IRSerialprint(String TXData)
{
	while (!(UCSR0A & _BV(UDRE0)));		// Wait until input buffer is clear
	UCSR0B &= ~(1<<RXEN0);				// Disable the receiver
	TCCR2A |= (1<<COM2A0);
	delay(1);

	Serial.print(TXData);    			// Send the string
	while (!(UCSR0A & _BV(TXC0)));		// Wait until all data has been transmitted
	TCCR2A &= ~(1<<COM2A0);

	delay(5);							// Add small delay to prevent receiving of reflections
	UCSR0B |= (1<<RXEN0);				// Enable the receiver again
}


/* Send a line of text (string + newline) to the serial IR transceiver */
void yeti::IRSerialprintln(String TXData)
{
	while (!(UCSR0A & _BV(UDRE0)));		// Wait until input buffer is clear
	UCSR0B &= ~(1<<RXEN0);				// Disable the receiver
	TCCR2A |= (1<<COM2A0);
	delay(1);

	Serial.print(TXData);				// Send the string
	Serial.println();					// Send newline
	while (!(UCSR0A & _BV(TXC0)));		// Wait until all data has been transmitted
	TCCR2A &= ~(1<<COM2A0);

	delay(5);							// Add small delay to prevent receiving of reflections
	UCSR0B |= (1<<RXEN0);				// Enable the receiver again
}


/* Read data from the serial IR transceiver */
int yeti::IRSerialread()
{
	return Serial.read();				// Just return serial data 
}


/* Make Yeti move nrSteps steps forward */
void yeti::moveForwardX(int nrSteps)
{
	if(nrSteps != 0)
	{
		for(int currentNumberSteps = 0; currentNumberSteps < nrSteps; currentNumberSteps++)
		{
			moveForwardXNC();
		}
	}

	moveBody(leanUp);
	moveLegs(bodyLevel);
}


/* Make Yeti move one step forward, don't center legs afterwards */
void yeti::moveForwardXNC()
{
	if(bodyPos == bodyLeftFwd)		// If left is currently in front, go here
	{
		moveBody(leanLeft, 16);
		moveLegs(rightForward, 13);
		moveBody(leanUp, 16);
		bodyPos = bodyRightFwd;
	}
	else
	{
		moveBody(leanRight, 16);
		moveLegs(leftForward, 13);
		moveBody(leanUp, 16);
		bodyPos = bodyLeftFwd;
	}
}


/* Make Yeti move one step backward, don't center legs afterwards */
void yeti::moveBackwardXNC()
{
	if(bodyPos == bodyLeftFwd)
	{
		moveBody(leanRight, 16);
		moveLegs(rightForward, 16);
		moveBody(leanUp, 16);
		bodyPos = bodyRightFwd;
	}
	else
	{
		moveBody(leanLeft, 16);
		moveLegs(leftForward, 16);
		moveBody(leanUp, 16);
		bodyPos = bodyLeftFwd;
	}
}


/* Make Yeti turn right by the specified angle */
void yeti::turnRight(int angle)
{
	moveBody(leanLeft, 15);
	moveLegs(leftForward, 15);
	moveBody(leanUp, 16);
	delay(150);
	if(angle == 0)
		moveLegs(bodyLevel, 16);
	else
		moveLegs(rightForward, 16);
	moveBody(leanRight, 17);
	moveLegs(bodyLevel, 17);
	moveBody(leanUp, 17);
}


/* Make Yeti turn left by the specified angle */
void yeti::turnLeft(int angle)
{
	moveBody(leanRight, 15);
	moveLegs(rightForward, 15);
	moveBody(leanUp, 16);
	delay(100);
	if(angle == 0)
		moveLegs(bodyLevel, 16);
	else
		moveLegs(leftForward, 16);
	moveBody(leanLeft, 17);
	moveLegs(bodyLevel, 17);
	moveBody(leanUp, 17);
}


/* Make Yeti lean his body (3 = right, 4 = left) */
void yeti::moveBody(int leanDirection, int delayFactor)
{
	if(leanDirection == leanRight)
	{
		for (int i = servo1.read(); i != posLean; i--)
		{
			delay(1 * delayFactor);
			servo1.write(i);
		}
		prevLean = leanRight;
	}
	else if(leanDirection == leanLeft)
	{
		for (int i = servo1.read(); i != negLean; i++)
		{
			delay(1 * delayFactor);
			servo1.write(i);
		}
		prevLean = leanLeft;
	}
	else
	{
		if(prevLean == leanRight)
		{
			for(int i = servo1.read(); i != servoMid; i++)
			{
				servo1.write(i);
				delay(1 * delayFactor);
			}
		}
		else if(prevLean == leanLeft)
		{
			for(int i = servo1.read(); i != servoMid; i--)
			{
				servo1.write(i);
				delay(1 * delayFactor);
			}
		}
	}
}


/* Make Yeti move his legs (left forward = 6, right forward = 7) */
void yeti::moveLegs(int walkDirection, int delayFactor)
{
	if(walkDirection == rightForward)
	{
		for (int i = servo2.read(); i != posFwd; i--)
		{
			delay(2 * delayFactor);
			servo2.write(i);
		}
   }
   else if(walkDirection == leftForward)
   {
		for (int i = servo2.read(); i != negFwd; i++)
		{
			delay(2 * delayFactor);
			servo2.write(i);
		}
   }
   else
   {   
		int servoUpDown;
		if(servo2.read() > servoMid)
			servoUpDown = 1;
		else
			servoUpDown = 0;
		for(int i = servo2.read(); i!= servoMid; i)
		{
			servo2.write(i);
			if(servoUpDown == 0)
				i++;
			else
				i--;
			delay(2*delayFactor);
		}
   }
}


/* Sound the buzzer (Serial communication is not possible during beeps!)
 */
void yeti::beep(int frequency, int duration)
{
	pinMode(beepNegative, OUTPUT);
	pinMode(beepPositive, OUTPUT);
	digitalWrite(beepNegative, HIGH);
	tone(11, frequency, duration);
}


/* Initialize the 7 segment 4 digit display module */
void yeti::initDisplay()
{
	Wire.beginTransmission(displayAdress);
	Wire.write(B00000000);			// This is the instruction byte. Zero means the next byte is the control byte
	Wire.write(B01000111);			// Control byte (dynamic mode on, digits 1+3 on, digits 2+4 on, 12mA segment current
	Wire.endTransmission();         // Send the buffered data
}


/* Display a 4 digit number */
void yeti::displayDigit(int displayNumber)
{
	int j = displayNumber;
	int displayBCD[4];
	for(int i = 0; i != 4; i++)
	{
		displayBCD[i] = j % 10;
		j /= 10;
	}
	Wire.beginTransmission(displayAdress);
	Wire.write(0x01);				// Instruction byte, start on the right-hand side
	Wire.write(digitsForDisplay[displayBCD[0]]);
	Wire.write(digitsForDisplay[displayBCD[2]]);
	Wire.write(digitsForDisplay[displayBCD[1]]);
	Wire.write(digitsForDisplay[displayBCD[3]]);
	Wire.endTransmission();
}


/* Display single digits */
void yeti::displayDigit(int firstDigit, int secondDigit, int thirdDigit,
	int forthDigit)
{
	Wire.beginTransmission(displayAdress);
	Wire.write(0x01);				// Write instruction byte, start on right-hand side
	// Write the 4 digits
	Wire.write(digitsForDisplay[forthDigit]);
	Wire.write(digitsForDisplay[secondDigit]);
	Wire.write(digitsForDisplay[thirdDigit]);
	Wire.write(digitsForDisplay[firstDigit]);

	Wire.endTransmission();
}


/* Initialize the ultrasound sensor */
void yeti::initPing()
{
	noInterrupts();					// Temporarily disable interrupts
	TCCR2A = (1<< WGM21);			// CTC, no IO toggling
	OCR2A = 0xC7;					// 80 kHz
	ADCSRA = 0;						// Disable ADC, leave settings intact
	ADMUX = 0x00;					// Multiplexer for comparator to ADC0
	ADCSRB = (1 << ACME);			// Enable the multiplexer
	//ACSR = 0x02;					// React on falling edge, no interrupt
	ACSR = B00010011;
	interrupts();					// Enable interrupts again  
}


/* Sleep function */
void yeti::usleep(unsigned long us)
{
	count80kHz = 0;
	while(count80kHz * 25/2 < (2 * us));
}


/* Get the current distance measurement from ultrasound sensor */
int yeti::ping()
{
	noInterrupts();					// Disable interrupts again
	pinMode(11, OUTPUT);			// Set pin B3 to output
	pinMode(9, OUTPUT);

	TCCR2A = (1 << COM2A0) | (1<< WGM21);	// Start toggling on OC2A
	TCCR2B = (1 << CS20);			// No pre-scale
	OCR2A = 0xC7;					// Set frequency to about 40kHz
	TIMSK2 = (1 << OCIE2A);			// Enable Compare Match A interrupt
	interrupts();					// Enable interrupts again
	count80kHz = 0;

	while(count80kHz < 20)			// Send 10 pulses
	{
		//OCR2A = 0xC7 + (int(10)-count80kHz); //slightly play with the frequency 0xC7
	}

	noInterrupts();
	TCCR2A = (1<< WGM21);			// CTC, no IO toggling
	interrupts();					// Enable interrupts again

	distancePos = 0;
 
	for(int pos = 0; pos < 150; pos++)	// If it takes longer then this the sensor is out of range
	{
		usleep(23);
		if((ACSR &(1 << ACI)) != 0)	// If ACI is set, we got a signal from the sensor
		{
			if(distancePos == 0)	// If distance is already set, don't set it again
				distancePos = pos;
		}

		ACSR |= (1 << ACI);
	}
	return distancePos;
}


/* Initialize the wireless module */
void yeti::initRF(int baudrate)
{
	Serial.begin(baudrate);
	inByte = 0;						// Initialize incoming serial bytes
	inByte1 = 0;
	inByte2 = 0;
	inByte3 = 0;
	inByte4 = 0;
	inByte5 = 0;
	inByte6 = 0;
	inByte7 = 0;
	inByte8 = 0;
}


/* Get data from the wireless module */
void yeti::getRFData()
{
	if(Serial.available() >= 8)
	{
		for(int i = Serial.available(); i >= 0; i -= 9)
		{
			// Get incoming bytes:
			Serial.print("Byte0: "); Serial.println(inByte  = Serial.read());
			Serial.print("Byte1: "); Serial.println(inByte1 = Serial.read());
			Serial.print("Byte2: "); Serial.println(inByte2 = Serial.read());
			Serial.print("Byte3: "); Serial.println(inByte3 = Serial.read()); // Control
			Serial.print("Byte4: "); Serial.println(inByte4 = Serial.read()); // Info1
			Serial.print("Byte5: "); Serial.println(inByte5 = Serial.read()); // Info2
			Serial.print("Byte6: "); Serial.println(inByte6 = Serial.read()); // Info3
			Serial.print("Byte7: "); Serial.println(inByte7 = Serial.read()); // Info4
			Serial.read();
			Serial.write(inByte4);

			switch (inByte5)
			{
				case 102: 
				{
					goForward = 0;
					goLeft = 0;
					goRight = 0;
					goBackwards = 0;
					prevMove = 1;
				} break;	// STOP

				case 53: 
				{
					goForward = 0;
					goLeft = 0;
					goRight = 0;
					goBackwards = 0;
					prevMove = 1;
				} break;	// STOP

				case 128: 
				{
					goForward = 1;
					goRight = 0;
					goLeft = 0;
					goBackwards = 0;
				} break;	// FORWARD

				case 56: 
				{
					goForward = 1;  
					goRight = 0;
					goLeft = 0;
					goBackwards = 0;
				} break;	// FORWARD

				case 96: 
				{
					if(inByte6 == 102)
					{
						// Turn on LEDs
						digitalWrite(leftLED, HIGH);
						digitalWrite(rightLED, HIGH);
					}
					else if(inByte6 == 6)
					{
						// Turn off LEDs
						digitalWrite(leftLED, LOW);
						digitalWrite(rightLED, LOW);
					}
					else if(((inByte6 & 0x18)== 0x18) || inByte5 == 52)
					{
						goLeft = 1;
						goForward = 0;
						goRight = 0;
						goBackwards = 0;
					}	// LEFT
				}   break;

				case 52: 
				{
					goLeft = 1;
					goForward = 0;
					goRight = 0;
					goBackwards = 0;
				}   break;	// LEFT

				case 120: 
				{
					goRight = 1; 
					goLeft = 0;
					goBackwards = 0;
					goForward = 0;
				} break;	// RIGHT

				case 54: 
				{
					goRight = 1; 
					goLeft = 0;
					goBackwards = 0;
					goForward = 0;
				} break;	// RIGHT

				case 24:
				{
					if(prevMove == 1);
					else
					{
						goBackwards = 1;
						goLeft = 0;
						goRight = 0;
						goForward = 0;
					}
				} break;	// BACKWARD

				case 50:
				{
					if(prevMove == 1);
					else
					{
						goBackwards = 1;
						goLeft = 0;
						goRight = 0;
						goForward = 0;
					}
				} break;	// BACKWARD

			}
			if(inByte3 == 3) 
				RFTest();
			else if(inByte3 == 2)
				respondRF();
		}

		while(Serial.available() > 0)
			char t = Serial.read();
	}
}


/* Move Yeti according to wireless command, send data back
 * (servo position, etc.) */
void yeti::respondRF()
{
	if(goForward == 1)
		moveForwardXNC();
	else if(goRight == 1)
		turnRight(1);
	else if(goLeft == 1)
		turnLeft(1);
	else if(goBackwards == 1)
		moveBackwardXNC();
	else
		{
			if(prevMove)
			{
				moveLegs(leanUp, 25);
				moveBody(bodyLevel, 25);
				prevMove = 0;
			}
		}
}


/* Return data about the current state of Yeti to the wireless module */
void yeti::updateCommand()
{
	int servo1Pos, servo2Pos;
	servo1Pos = servo1.read();		// Get current position of the front servo
	servo2Pos = servo2.read();		// Get current position of the bottom servo

	// Disable the pull-ups on the I2C bus to get accurate values
	pinMode(5, INPUT);
	pinMode(19, INPUT);
	digitalWrite(5, LOW);
	digitalWrite(19, LOW);

	int batteryVoltage = analogRead(19);
	batteryVoltage /= 45;			// Scale battery voltage from 0-255

	// Enable the pull-up on the I2C bus again
	pinMode(5, OUTPUT);
	digitalWrite(5, HIGH);

	// Send all of the data
	Serial.write(1);					// Start Byte: 1
	Serial.write(255);					// Address PC: 255
	Serial.write(14);					// Length: 14
	Serial.write(254);					// Command: 254 "update data"
	Serial.write(batteryVoltage);		// Data 0: battery voltage
	Serial.write((byte)0x00);			// Data 1: current motor right, not available for Yeti
	Serial.write((byte)0x00);			// Data 2: current motor left, not available for Yeti
	Serial.write(servo1Pos);			// Data 3: front servo position
	Serial.write(servo2Pos);			// Data 4: bottom servo position
	Serial.write((byte)0x00);			// Data 5: errorbits
	Serial.write((byte)0x00);			// Data 6: linesensor left, not available
	Serial.write((byte)0x00);			// Data 7: linesensor right, not available
	Serial.write((byte)0x00);			// Data 8: Servo disabled
	Serial.write(4);					// Stopbit
}


/* Test wireless connection */
void yeti::RFTest()
{
	delay(100);
	for(int Test_Value_RF =0; Test_Value_RF<10; Test_Value_RF++)
	{
		Serial.write(1);				// Start Byte:		1
		Serial.write(255);				// Address PC:		255 
		Serial.write(6);				// Length: 			7 bytes 
		Serial.write(200);				// Control Byte:		200
		Serial.write((byte)0x10);		// Data byte 0:		10
		Serial.write(4);				// Stop Byte:		4
		delay(250);
	}
}
