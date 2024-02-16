#ifndef PROGRAMSCOMMON_H
#define PROGRAMSCOMMON_H

#include <parameters.h>

#include <Arduino.h>
#include <mbed.h>
#include <SimpleKalmanFilter.h>
#include <Adafruit_AHRS.h>
#include <Fusion.h>


///////////////////////////////////////////////////////
//////////////// IMU: /////////////////////////////////


extern float acc_filt_x, acc_filt_y, acc_filt_z;

//extern SimpleKalmanFilter acc_kalman_filter_x;
//extern SimpleKalmanFilter acc_kalman_filter_y;
//extern SimpleKalmanFilter acc_kalman_filter_z;

bool imu_initialize(void);
void imu_update_accel_values(void);
void imu_update_accel_values_filtered(void);
void imu_update_mag_values(void);
void imu_update_gyro_values(void);

float imu_get_acc_update_rate();
float imu_get_mag_update_rate();
float imu_get_gyro_update_rate();

///////////////////////////////////////////////

extern float status_imu_roll;
extern float status_imu_pitch;
extern float status_imu_yaw;

extern float status_imu_quat_w;
extern float status_imu_quat_x;
extern float status_imu_quat_y;
extern float status_imu_quat_z;


#if IMU_MAHONY == 1
extern Adafruit_Mahony filter;
#endif

#if IMU_MADGWICK == 1
extern Adafruit_Madgwick filter;
#endif

#if IMU_NXP == 1
extern Adafruit_NXPSensorFusion filter;
#endif

#if IMU_FUSION == 1
extern FusionOffset offset;
extern FusionAhrs ahrs;
#endif

///////////////////////////////////////////////////////
//////////////// SIMPLE: KALMAN: ////////////////////

// Using Arduino LSM9DS1 library: [-4, +4] g -/+0.122 mg

// For temperature measurements, the units of the standard
// deviation would be degrees Celsius or Fahrenheit.
#define ACC_KALMAN_MEASUREMENT_UNCERTAINTY 0.0148f

//  For example, if you're estimating the position and velocity
// of an object in meters and meters per second, respectively,
// the units of the covariance matrix would be meters squared
// and meters squared per second squared.
#define ACC_KALMAN_ESTIMATION_UNCERTAINTY 0.2f

// For example, if you're estimating the position and velocity
// of an object in meters and meters per second, respectively,
// and the time units used in the Kalman filter equations are seconds,
//  the units of the process noise covariance matrix would be meters
// squared per second squared and meters squared per second to the fourth power
#define ACC_KALMAN_PROCESS_NOISE_UNCERTAINTY 0.1f



///////////////////////////////////////////////////////
//////////////// MOTORS: ////////////////////

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

void motor_update_enc1_rot_rate();
void motor_update_enc2_rot_rate();

extern volatile bool enc_state_1A;
extern volatile bool enc_state_1B;
extern volatile bool enc_state_2A;
extern volatile bool enc_state_2B;

extern volatile bool enc_state_1A_prev;
extern volatile bool enc_state_1B_prev;
extern volatile bool enc_state_2A_prev;
extern volatile bool enc_state_2B_prev;


extern volatile int32_t encoderCount1;  // Encoder count for motor 1
extern volatile bool encoderDirection1; // Direction flag for motor 1
extern int32_t m1_enc_count_last;

extern volatile int32_t encoderCount2;  // Encoder count for motor 2
extern volatile bool encoderDirection2; // Direction flag for motor 2
extern int32_t m2_enc_count_last;


extern volatile bool interrupt_called;

float motor_driver_getM1CurrentMilliamps(void);
float motor_driver_getM2CurrentMilliamps(void);

unsigned char motor_driver_getM1Fault(void);
unsigned char motor_driver_getM2Fault(void);



///////////////////////////////////////////////////////
//////////////// SERIAL: READ: ////////////////////////

extern void readSerialData();

///////////////////////////////////////////////////////
//////////////// SERIAL: ASCII: ///////////////////////
extern void sendSerial_ascii();


///////////////////////////////////////////////////////
//////////////// SERIAL: BINARY: //////////////////////

extern void sendSerialData(byte message_type);
extern byte messageBuffer[128];
extern byte sendBuffer[128];

///////////-----------////////////

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
extern float status_gyr_x;
extern float status_gyr_y;
extern float status_gyr_z;

extern float status_mag_x;
extern float status_mag_y;
extern float status_mag_z;

////

extern float status_cal_acc_x;
extern float status_cal_acc_y;
extern float status_cal_acc_z;

extern float status_cal_gyr_x;
extern float status_cal_gyr_y;
extern float status_cal_gyr_z;

extern float status_cal_mag_x;
extern float status_cal_mag_y;
extern float status_cal_mag_z;

extern bool first_run;

///////////////////////////////////////////////////////
//////////////// TIMING: //////////////////////////////

extern uint32_t time_encoder_1_now;
extern uint32_t time_encoder_1_last;

extern uint32_t time_encoder_2_now;
extern uint32_t time_encoder_2_last;

extern uint32_t time_imu_now;
extern uint32_t time_imu_last;
extern uint32_t time_imu_interval_ms;

extern uint32_t time_send_binary_now;
extern uint32_t time_send_binary_last;

extern uint32_t time_send_ascii_now;
extern uint32_t time_send_ascii_last;

///////////////////////////////////////////////////////
//////////////// LEDS: //////////////////////////////

extern void initialize_leds();


#endif //#ifndef PROGRAMSCOMMON_H