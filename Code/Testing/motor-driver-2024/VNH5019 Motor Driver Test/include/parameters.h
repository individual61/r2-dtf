#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <Arduino.h>
#include <mbed.h>



#define LED_RED 22
#define LED_GREEN 23
#define LED_BLUE 24

#define TXU0104_ENABLE 5            // Enable for TXU0104 voltage translator for encoders

#define MOTOR_ENCODER_1A A6         // 3V3 Motor encoder signals
#define MOTOR_ENCODER_1B A7

#define MOTOR_ENCODER_2A A2
#define MOTOR_ENCODER_2B A3



#define MOTOR_DRIVER_PIN_INA1 2     // M1INA	    Motor 1 direction input A
#define MOTOR_DRIVER_PIN_INB1 4     // M1INB        Motor 1 direction input B
#define MOTOR_DRIVER_PIN_PWM1 9     // M1PWM	    Motor 1 speed input
#define MOTOR_DRIVER_PIN_EN1DIAG1 6 // M1EN/DIAG	Motor 1 enable input/fault output
#define MOTOR_DRIVER_PIN_CS1 A1     // M1CS	        Motor 1 current sense output

#define MOTOR_DRIVER_PIN_INA2 7      // M1INA	    Motor 2 direction input A
#define MOTOR_DRIVER_PIN_INB2 8      // M1INB       Motor 2 direction input B
#define MOTOR_DRIVER_PIN_PWM2 10     // M1PWM	    Motor 2 speed input
#define MOTOR_DRIVER_PIN_EN2DIAG2 12 // M1EN/DIAG	Motor 2 enable input/fault output
#define MOTOR_DRIVER_PIN_CS2 A0      // M1CS	    Motor 2 current sense output

#define PWM_FREQUENCY 5000

// Total bytes required for serial send buffer
#define SERIAL_MESSAGE_BUFFER_SIZE 16
#define SERIAL_HEADER 0x3E // decimal 62 '>'
#define SERIAL_PAD 0x5F // decimal 95 '_'
#define SERIAL_SEND_BUFFER_SIZE (2*SERIAL_MESSAGE_BUFFER_SIZE + 2)

#define SERIAL_BINARY 1

#endif