#include "RotaryEncoder.h"

//----------------LINK----------------------------
//http://bildr.org/2012/08/rotary-encoder-arduino/
//------------------------------------------------

namespace HENRY
{
unsigned char Pin_a = 2; //These have to be 2 and 3 for some reason.
unsigned char Pin_b = 3;
unsigned char SwitchPin;
int encoderValue = 0;
int lastEncoded = 0;

RotaryEncoder::RotaryEncoder()
	: m_pinA(0), m_pinB(0), m_position(0), m_flagA(0), m_flagB(0)
{
}

void updateEncoder()
{
int MSB = digitalRead(Pin_a); //MSB = most significant bit
int LSB = digitalRead(Pin_b); //LSB = least significant bit
int encoded = (MSB << 1) |LSB;
int sum = (lastEncoded << 2) | encoded;
if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011){ encoderValue ++;} 
if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000){ encoderValue --;}
lastEncoded = encoded;
}

bool RotaryEncoder::init(unsigned char pin_a, unsigned char pin_b)
{
	Pin_a = pin_a;
	Pin_b = pin_b;

	if (pin_a == pin_b)
		return false;
	m_pinA = pin_a;
	m_pinB = pin_b;
	pinMode(m_pinA, INPUT_PULLUP);
	pinMode(m_pinB, INPUT_PULLUP);
	pinMode(SwitchPin, INPUT_PULLUP);
	
	//This part may not be needed//
	digitalWrite(m_pinA, HIGH); //turn pullup resistor on
	digitalWrite(m_pinB, HIGH); //turn pullup resistor on
	digitalWrite(SwitchPin, HIGH);
	///////////////////////////////


	attachInterrupt(0, &updateEncoder, CHANGE);
	attachInterrupt(1, &updateEncoder, CHANGE);
	return true;
}

void RotaryEncoder::update()
{
	if(digitalRead(SwitchPin)){


	}else{
		

	}
	
	Serial.println(encoderValue);
	Serial.println(digitalRead(m_pinA));
	Serial.println(digitalRead(m_pinB));
	delay(1000);
	
}
 // namespace HENRY



}
