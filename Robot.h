
#ifndef ROBOT_H
	#define ROBOT_H

	#include "Pin_Config.h"
	#include "Config.h"
	
	#if defined(WIRING) && WIRING >= 100
		#include <Wiring.h>
	#elif defined(ARDUINO) && ARDUINO >= 100
		#include <Arduino.h>
	#else
		#include <WProgram.h>
	#endif
	
	#define SHOWDEBUG false
	#define RobotSerial Serial

	#if SHOWDEBUG == true
		#define DBSerial Serial
		#define DBSerial_print(...)    DBSerial.print(__VA_ARGS__)
		#define DBSerial_println(...)  DBSerial.println(__VA_ARGS__)
	#else
		#define DBSerial_print(...)
		#define DBSerial_println(...)
	#endif
	
	// Robot Class
	class Robot
	{			
		private:
			/* ************************************************************************************
			 *
			 *	Init-Funktionen für die
			 *	einzelnen Teilbereiche
			 *
			 * ************************************************************************************ */
			void InitSerial(void);
			void InitLeds(void);
			void InitADC(void);
			void InitEngine(void);
			void InitEnginePWM(void);
			
			void InitSwitches(void);
			void InitSensors(void);
			
			/* ************************************************************************************
			 *
			 *	Switches
			 *
			 * ************************************************************************************ */	
			volatile unsigned char SwitchStateInterrupt = SwitchState_None;
			volatile unsigned char switchValue = 0;
			volatile unsigned char switchInterruptAktiv = 0;
			
			/* ************************************************************************************
			 *
			 *	Battery, ADC
			 *
			 * ************************************************************************************ */	
			void SetADMUX(char pin);
			
			void ADCStartNext(void);
			void ADCStart(char Channel);
			void ADCWaitForConversion(void);
			
			void DoCheckADCMode(char channel);
			
			volatile char ADCPos = 0;
			volatile bool ADCBlockPassed = false;
			
			/* ************************************************************************************
			 *
			 *	Motor
			 *
			 * ************************************************************************************ */	
			volatile unsigned long RPMSensorCountRight = 0;
			volatile unsigned long RPMSensorCountLeft = 0;
			
			/* ************************************************************************************
			 *
			 *	Timer
			 *
			 * ************************************************************************************ */	
			volatile unsigned long TimerCount = 0;
			volatile unsigned int TimerOverflowCount = 0;
			volatile unsigned int TimerTcnt2 = 0;
			unsigned long TimerWaitTime = 0;
			
			void (*TimerISRfunction)();

		public:	
			static Robot * MyRobot;
			/* ************************************************************************************
			 *
			 *	Variablen
			 *
			 * ************************************************************************************ */
			
			long SwitchFactor = STANDARD_SWITCH_FACTOR;
			long InternalReferenceVoltage = STANDARD_INTERNAL_REFERENCE_VOLTAGE;
			
			double ResistorOben = STANDARD_RESISTOR_OBEN;
			double ResistorUnten = STANDARD_RESISTOR_UNTEN;
			
			
			/* ************************************************************************************
			 *
			 *	Initialisiert die Hardware
			 *	des Asuros.
			 *	(GPIO, ADC, PWM, Serial)
			 *
			 * ************************************************************************************ */
			void Init(void);
			 
			 
			/* ************************************************************************************
			 *
			 *	ADC - Batterie, Referenz, Temperatur
			 *
			 * ************************************************************************************ */
			char ADCMode = ADCMode_None;
			volatile int mySensorValues[9];
			
			bool ADCWaitForBlock(void);
			void ADCInterrupt(void);
			static void ADCInterruptISR(void)
			{
				if (MyRobot != NULL)
					MyRobot->ADCInterrupt();
			}
			
			static void ADCInterruptBlockISR(void)
			{
				if (MyRobot != NULL)
				{
					if(MyRobot->ADCMode == ADCMode_Block)
						ADCSRA |= (1<<ADSC);
				}
			}
			
			
			/* ************************************************************************************
			 *
			 *	Sensors
			 *
			 * ************************************************************************************ */
			unsigned int readReferenz(void);
			float readBattery(void);		
			float readVref(void);
			
			
			void readLinesensor(int *data, unsigned char LightStatus);
			void readLinesensor(int *data);
			unsigned int readLinesensor(char Side, unsigned char LightStatus);
			unsigned int readLinesensor(char Side);
			
			void readACS(int *data, unsigned char LightStatus);
			void readACS(int *data);
			unsigned int readACS(char Side, unsigned char LightStatus);
			unsigned int readACS(char Side);
			
			void readLDR(int *data);
			unsigned int readLDR(char Side);
			
			/* ************************************************************************************
			 *
			 *	StatusLED
			 *
			 * ************************************************************************************ */
			void setStatusLED(unsigned char color);
			void shiftStatusLED(void);
			void setFrontLED(unsigned char status);
			void setIRLeds(unsigned char status);
			
			/* ************************************************************************************
			 *
			 *	Engine
			 *
			 * ************************************************************************************ */
			void RPMLeft(void);
			static void RPMLeftISR(void)
			{
				if (MyRobot != NULL)
					MyRobot->RPMLeft();
			}
			
			void RPMRight(void);
			static void RPMRightISR(void)
			{
				if (MyRobot != NULL)
					MyRobot->RPMRight();
			}

			void setMotorDirectionLeft(char dir);
			void setMotorDirectionRight(char dir);
			void setMotorDirection (char left, char right);
						
			void setMotorSpeedLeft(int pwm, bool ChangeDirection = false);
			void setMotorSpeedRight(int pwm, bool ChangeDirection = false);
			void setMotorSpeed (int left, int right, bool ChangeDirection = false);
			
			void GetRPMSensorCount(int *data);
			unsigned long GetRPMSensorCount(char Side);
			
			
			
			
			/* ************************************************************************************
			 *
			 *	Switches
			 *
			 * ************************************************************************************ */	
			 
			//
			// Read Out Switches
			//
			unsigned char readSwitches(void);
			
			//
			//
			//
			bool switchAvailable(void);
			void switchInterruptOn(void);
			void switchInterruptOff(void);
			void switchInterrupt(void);
			
			static void SwitchISR(void)
			{
				if (MyRobot != NULL)
					MyRobot->switchInterrupt();
			}
			/* ************************************************************************************
			 *
			 *	Timer
			 *
			 * ************************************************************************************ */	
			
			static void TimerOverflowISR(void)
			{
				if (MyRobot != NULL)
					MyRobot->TimerOverflow();
			}
			
			void TimerSet(unsigned long ms, void (*isrfunction)());
			void TimerStart();
			void TimerStop();
			void TimerOverflow();
			
			
			/* ************************************************************************************
			 *
			 *	Kompatibilität
			 *
			 * ************************************************************************************ */	
			 
			void readOdometry(int *data);
			unsigned long readOdometry(char Side);
			
	};
	
	
#endif