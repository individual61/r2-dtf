#ifndef PROGRAMSCOMMON_H
#define PROGRAMSCOMMON_H

#include <parameters.h>

extern mbed::PwmOut pwmPin1;
extern mbed::PwmOut pwmPin2;

void motor_driver_init(void);

void motor_driver_setM1Speed(int speed);
void motor_driver_setM2Speed(int speed);
void motor_driver_setSpeeds(int m1Speed, int m2Speed);

void motor_driver_setM1Brake(int brake);
void motor_driver_setM2Brake(int brake);
void motor_driver_setBrakes(int m1Brake, int m2Brake);

float motor_driver_getM1CurrentMilliamps(void);
float motor_driver_getM2CurrentMilliamps(void);

unsigned char motor_driver_getM1Fault(void);
unsigned char motor_driver_getM2Fault(void);



#endif