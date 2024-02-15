#ifndef PARAMETERS_H
#define PARAMETERS_H


///////////////////////  Board type  ///////////////////////

// #define ARDUINO_NANO_33_BLE
#define ARDUINO_NANO_33_BLE_SENSE_REV2

#ifdef ARDUINO_NANO_33_BLE
#include <Arduino_LSM9DS1.h> // For Arduino Nano BLE 33
#endif

#ifdef ARDUINO_NANO_33_BLE_SENSE_REV2
#include <Arduino_BMI270_BMM150.h> // for Arduino Nano BLE 33 Sense Rev 2
#endif

//////////////////////////////////////////////

#define LED_RED 22
#define LED_GREEN 23
#define LED_BLUE 24

#define TXU0104_ENABLE 5 // Enable for TXU0104 voltage translator for encoders

#define MOTOR_ENCODER_1A A6 // 3V3 Motor encoder signals
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

#define PWM_FREQUENCY 15000

// Total bytes required for serial send buffer

#define SERIAL_MESSAGE_BUFFER_SIZE_0 28
#define SERIAL_SEND_BUFFER_SIZE_0 (2 * SERIAL_MESSAGE_BUFFER_SIZE_0 + 2 + 2)

#define SERIAL_MESSAGE_BUFFER_SIZE_1 25
#define SERIAL_SEND_BUFFER_SIZE_1 (2 * SERIAL_MESSAGE_BUFFER_SIZE_1 + 2 + 2)

#define SERIAL_MESSAGE_BUFFER_SIZE_2 25
#define SERIAL_SEND_BUFFER_SIZE_2 (2 * SERIAL_MESSAGE_BUFFER_SIZE_2 + 2 + 2)

#define SERIAL_HEADER 0x3E // decimal 62 '>'
#define SERIAL_PAD 0x5F    // decimal 95 '_'

////////////// IMU

//#define FILTER_UPDATE_RATE_HZ 10

#define IMU_MAHONY 0

#define IMU_MADGWICK 0

#define IMU_NXP 0

#define IMU_FUSION 1

// Timing

#define SERIAL_ASCII_INTERVAL 100
#define SERIAL_BINARY_INTERVAL 100
#define ENCODER_INTERVAL 100
#define IMU_INTERVAL 10
#define IMU_RATE 100


//////////////

#define SERIAL_BINARY 0

#define SERIAL_IMU_MAG_CAL 0

#define SERIAL_IMU_RAW 0

#define SERIAL_IMU_CALIBRATED 0

#define SERIAL_IMU_RAW_VS_CAL 0

#define SERIAL_IMU_RPY 0

#define SERIAL_IMU_RPY_SERIALPLOT 0

#define SERIAL_IMU_RPY_ADAFRUIT_WEBSERIAL 1

#define SERIAL_IMU_CALIBRATED_SERIALPLOT 0

#define CAL_GYR_X -0.1991395698f
#define CAL_GYR_Y 0.1889294647f
#define CAL_GYR_Z -0.1565932966f

#define CAL_MAG_OFFSET_X -2.623f
#define CAL_MAG_OFFSET_Y -0.034f
#define CAL_MAG_OFFSET_Z 1.72f

// rOW 1
#define CAL_MAG_11 0.999f
#define CAL_MAG_12 -0.011f
#define CAL_MAG_13 -0.013f

#define CAL_MAG_21 -0.011f
#define CAL_MAG_22 0.990f
#define CAL_MAG_23 -0.003f

#define CAL_MAG_31 -0.013f
#define CAL_MAG_32 -0.003f
#define CAL_MAG_33 1.012f

#endif