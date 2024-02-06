#ifndef PROGRAMSCOMMON_H
#define PROGRAMSCOMMON_H

#include <parameters.h>

extern mbed::PwmOut pwmPin1;
extern mbed::PwmOut pwmPin2;

void motor_driver_init(void);

void motor_driver_setM1_INA(bool setting);
void motor_driver_setM1_INB(bool setting);
void motor_driver_setM2_INA(bool setting);
void motor_driver_setM2_INB(bool setting);
void motor_driver_setM1_PWM(int speed);
void motor_driver_setM2_PWM(int speed);

void motor_driver_setM1Speed(int speed);
void motor_driver_setM2Speed(int speed);
void motor_driver_setSpeeds(int m1Speed, int m2Speed);

void motor_driver_setM1Brake(int brake);
void motor_driver_setM2Brake(int brake);
void motor_driver_setBrakes(int m1Brake, int m2Brake);

void updateEncoder1();
void updateEncoder2();

extern volatile bool enc_state_1A;
extern volatile bool enc_state_1B;
extern volatile bool enc_state_2A;
extern volatile bool enc_state_2B;

extern volatile bool enc_state_1A_prev;
extern volatile bool enc_state_1B_prev;
extern volatile bool enc_state_2A_prev;
extern volatile bool enc_state_2B_prev;

extern volatile bool interrupt_called;

float motor_driver_getM1CurrentMilliamps(void);
float motor_driver_getM2CurrentMilliamps(void);

unsigned char motor_driver_getM1Fault(void);
unsigned char motor_driver_getM2Fault(void);

//extern void sendBool(bool value);
//extern void sendInt(int8_t value);
//extern void sendUnsignedInt(uint8_t value);
//extern void sendFloat(float value);

extern void sendSerialData(byte message_type);
extern byte messageBuffer[128]; 
extern byte sendBuffer[128];    


extern void readSerialData();

extern int xx_serial_message_buffer_size;
///////////////////////////////////////////////


// 1 byte
extern bool status_motor1_ina;
extern bool status_motor1_inb;
extern bool status_motor1_endiag;
extern bool status_motor2_ina;
extern bool status_motor2_inb;
extern bool status_motor2_endiag;
extern volatile bool status_m1_rot_dir;
extern volatile bool status_m2_rot_dir;

// 1 byte
extern bool status_txu_oe;
extern bool status_d3;
extern bool status_d11;
extern bool status_d13;
extern bool status_rgb_led_r;
extern bool status_rgb_led_g;
extern bool status_rgb_led_b;
extern bool status_flag_unknown_message;

// 1 byte
extern bool status_flag_1;
extern bool status_flag_2;
extern bool status_flag_3;
extern bool status_flag_4;
extern bool status_flag_5;
extern bool status_flag_6;
extern bool status_flag_7;
extern bool status_flag_8;

// 4 bytes
extern uint16_t status_motor1_pwm;
extern uint16_t status_motor1_cs;

// 4 bytes
extern uint16_t status_motor2_pwm;
extern uint16_t status_motor2_cs;

// 8 bytes
extern int32_t status_m1_enc_count;
extern int32_t status_m1_rot_rate;

// 8 bytes
extern int32_t status_m2_enc_count;
extern int32_t status_m2_rot_rate;

// 12 bytes
extern float status_acc_x;
extern float status_acc_y;
extern float status_acc_z;

// 12 bytes
extern float status_angle_x;
extern float status_angle_y;
extern float status_angle_z;

// 12 bytes
extern float status_ang_rate_x;
extern float status_ang_rate_y;
extern float status_ang_rate_z;

///////////////////////////////////////////////

extern volatile int32_t encoderCount1; // Encoder count for motor 1
extern volatile bool encoderDirection1; // Direction flag for motor 1
extern int32_t m1_enc_count_last;

extern volatile int32_t encoderCount2; // Encoder count for motor 2
extern volatile bool encoderDirection2; // Direction flag for motor 2
extern int32_t m2_enc_count_last;



#endif