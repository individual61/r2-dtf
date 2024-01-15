#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <Arduino.h>
#include <mbed.h>


#define MOTOR_DRIVER_PIN_INA1 2     // M1INA	    Motor 1 direction input A
#define MOTOR_DRIVER_PIN_INB1 4     // M1INB        Motor 1 direction input B
#define MOTOR_DRIVER_PIN_PWM1 9     // M1PWM	    Motor 1 speed input
#define MOTOR_DRIVER_PIN_EN1DIAG1 6 // M1EN/DIAG	Motor 1 enable input/fault output
#define MOTOR_DRIVER_PIN_CS1 A0     // M1CS	        Motor 1 current sense output

#define MOTOR_DRIVER_PIN_INA2 7      // M1INA	    Motor 2 direction input A
#define MOTOR_DRIVER_PIN_INB2 8      // M1INB       Motor 2 direction input B
#define MOTOR_DRIVER_PIN_PWM2 10     // M1PWM	    Motor 2 speed input
#define MOTOR_DRIVER_PIN_EN2DIAG2 12 // M1EN/DIAG	Motor 2 enable input/fault output
#define MOTOR_DRIVER_PIN_CS2 A1      // M1CS	    Motor 2 current sense output


#define PWM_FREQUENCY 19000

#endif