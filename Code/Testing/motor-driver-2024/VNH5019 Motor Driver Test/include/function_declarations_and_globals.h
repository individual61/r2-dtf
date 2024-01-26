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

extern void sendBool(bool value);
extern void sendInt(int8_t value);
extern void sendUnsignedInt(uint8_t value);
extern void sendFloat(float value);
extern void sendSerialData();

extern bool status_motor1_ina;
extern bool  status_motor1_inb;
extern bool  status_motor1_endiag;
extern bool  status_motor2_ina;
extern bool  status_motor2_inb;
extern bool  status_motor2_endiag;
extern bool  status_flag_1;
extern bool  status_flag_2;

extern bool  status_txu_oe;
extern bool  status_d3;
extern bool  status_d11;
extern bool  status_d13;
extern bool  status_rgb_led_r;
extern bool  status_rgb_led_g;
extern bool  status_rgb_led_b;
extern bool  status_flag_3;

extern uint8_t status_motor1_pwm;
extern uint8_t status_motor1_cs;

extern uint8_t status_motor2_pwm;
extern uint8_t status_motor2_cs;

extern uint8_t status_m1_enc_count;
extern uint8_t status_m1_rot_rate;

extern uint8_t status_m2_enc_count;
extern uint8_t status_m2_rot_rate;

extern int8_t status_acc_x;
extern int8_t status_acc_y;
extern int8_t status_acc_z;

extern int8_t status_ang_rate_x;
extern int8_t status_ang_rate_y;
extern int8_t status_ang_rate_z;

#endif