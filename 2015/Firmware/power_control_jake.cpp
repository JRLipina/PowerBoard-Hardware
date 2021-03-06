/*
 * power_control.cpp
 *
 * Created: 4/23/2015 9:15:37 PM
 *  Author: Jake Armenta
 */

#include "Arduino/Arduino.h"
#include "Arduino/eeprom.h"

#include "power_control.h"

uint8_t motorSingalPorts[NUM_IO_PORTS] = {0x00, 0x00, 0x00, 0x00};
float motorVoltageValues[NUM_MOTORS];
uint8_t numOverCurrentReads[NUM_MOTORS];
uint16_t motorZeroValue[NUM_MOTORS];

void initializeMotorController()
{
/*
	uint8_t i;
	uint16_t temp16;
	extern uint8_t motorSingalPorts[NUM_IO_PORTS];

	for( i = 0; i < NUM_MOTORS; ++i )
	{
		if( MOTOR_SIGNAL_PORT[i] == &PORTB )
		{
			motorSingalPorts[0] |= MOTOR_SIGNAL_PIN[i];
		}
		else if( MOTOR_SIGNAL_PORT[i] == &PORTC )
		{
			motorSingalPorts[1] |= MOTOR_SIGNAL_PIN[i];
		}
		else if( MOTOR_SIGNAL_PORT[i] == &PORTD )
		{
			motorSingalPorts[2] |= MOTOR_SIGNAL_PIN[i];
		}
		else if( MOTOR_SIGNAL_PORT[i] == &PORTE )
		{4
			motorSingalPorts[3] |= MOTOR_SIGNAL_PIN[i];
		}
		numOverCurrentReads[i] = 0;
	}

	EEPROM.get(0, temp16);

	if(i != 0)
	{
		i += sizeof(uint16_t);
		readMotorConfiguratin(i);
	}
	else
	{
		for(i = 0; i < NUM_MOTORS; ++ i);
		{
			motorZeroValue[i] = MID_ANALOG_READ_VALUE;
		}
	}
*/
	DDRB |= 0b00000010;
	DDRC |= 0b11000000;
	DDRD |= 0b00100011;
	DDRE |= 0b00000100;
}

void readMotorCurrents()
{
	uint8_t i;
	int tempRead;

	for(i = 0; i < NUM_MOTORS; ++i)
	{
		tempRead = analogRead(MOTOR_READ_PIN[i]);
		/* transforming a range of 0 - 1023 to a range of -50 to 50 */
		motorVoltageValues[i] = (tempRead - 512) * 50/512;
		if( motorVoltageValues[i] > MAX_MOTOR_CURRENT )
		{
			numOverCurrentReads[i]++;
			if (numOverCurrentReads[i] == NUM_CURRENT_READS_TRIGGER_FAULT )
			{
				turnOffMotorController(i);
			}
		}
		else
		{
			numOverCurrentReads[i] = 0;
		}
	}
}

inline void turnOffMotorController(const uint8_t motorNumber)
{
	*MOTOR_SIGNAL_PORT[motorNumber] &= ~(MOTOR_SIGNAL_PIN[motorNumber]);
}

inline void turnOnMotorController(const uint8_t motorNumber)
{
	*MOTOR_SIGNAL_PORT[motorNumber] |= MOTOR_SIGNAL_PIN[motorNumber];
}

/*
void turnOnAllMotors()
{
	PORTB |= motorSingalPorts[0];
	PORTC |= motorSingalPorts[1];
	PORTD |= motorSingalPorts[2];
	PORTE |= motorSingalPorts[3];
}
*/

void turnOnAllMotors()
{
	PORTB |= 0b00000010;
	PORTC |= 0b11000000;
	PORTD |= 0b00100011;
	PORTE |= 0b00000100;
}



void turnoffAllMotors()
{
	PORTB &= 0b11111101;
	PORTC &= 0b00111111;
	PORTD &= 0b11011100;
	PORTE &= 0b11111011;
}

/*
void turnoffAllMotors()
{
	PORTB &= ~(motorSingalPorts[0]);
	PORTC &= ~(motorSingalPorts[1]);
	PORTD &= ~(motorSingalPorts[2]);
	PORTE &= ~(motorSingalPorts[3]);
}
*/