#pragma once

#include <Adafruit_ADS1X15.h> //бібліотека 16-біт ацп

#define MQ7_LOAD_RESISTOR 10000
#define MQ7_CLEANAIR_RSRO 20.7
#define MQ7_REFERENCE_VOLTAGE 5
#define MQ7_LOAD_RESISTOR 10000
#define PWM_SIG 15
#define pin_voltageOntransistor 1 //напруга на транзисторі
#define pin_voltageOn_CO1 2 //напруга на першому датчику
#define pin_voltageOn_CO2 3 //напруга на другому датчику

extern int DutyCycle;
extern Adafruit_ADS1015 ads;

float volts_midArifm_pwmSig(int num_countdowns);
void dutyCycleOfPWM();
int calib_mq7_Ro(int pin_voltageOn);
float get_rawValue_mq7(int Ro, float reference_voltage_value, float pinCO);
