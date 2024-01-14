#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <Arduino.h>
#include <mbed.h>

////////////////////////////////////////////////////
// Library default pins
// Arduino Pin	  VNH5019 Driver Pin	Basic Function

// Outputs to motor board

// Digital 2   	M1INA	    Motor 1 direction input A
// Digital 4    M1INB	    Motor 1 direction input B
// Digital 7   	M2INA	    Motor 2 direction input A
// Digital 8    M2INB	    Motor 2 direction input B
// Digital 9    M1PWM	    Motor 1 speed input
// Digital 10   M2PWM	    Motor 2 speed input

// Inputs to arduino

// Digital 6   	M1EN/DIAG	Motor 1 enable input/fault output
// Digital 12  	M2EN/DIAG	Motor 2 enable input/fault output
// Analog 0  	  M1CS	    Motor 1 current sense output
// Analog 1  	  M2CS	    Motor 2 current sense output
///////////////////////////////////////////////////

#define MOTOR_DRIVER_PIN_INA1 2
#define MOTOR_DRIVER_PIN_INB1 4
#define MOTOR_DRIVER_PIN_PWM1 9
#define MOTOR_DRIVER_PIN_EN1DIAG1 6
#define MOTOR_DRIVER_PIN_CS1 A0

#define MOTOR_DRIVER_PIN_INA2 7
#define MOTOR_DRIVER_PIN_INB2 8
#define MOTOR_DRIVER_PIN_PWM2 10
#define MOTOR_DRIVER_PIN_EN2DIAG2 12
#define MOTOR_DRIVER_PIN_CS2 A1


#define PWM_FREQUENCY 19000

#endif