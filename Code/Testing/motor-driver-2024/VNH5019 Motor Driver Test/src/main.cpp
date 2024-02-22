#include <parameters.h>
#include <function_declarations_and_globals.h>

///////////////////////////////////////////////////////
//////////////// GLOBALS: MOTORS //////////////////////

mbed::PwmOut pwmPin1(digitalPinToPinName(MOTOR_DRIVER_PIN_PWM1)); // Initialize pwmPin1 with PWM1 pin.
mbed::PwmOut pwmPin2(digitalPinToPinName(MOTOR_DRIVER_PIN_PWM2)); // Initialize pwmPin1 with PWM1 pin.

volatile int32_t encoderCount1 = 0;     // Encoder count for motor 1
volatile bool encoderDirection1 = true; // Direction flag for motor 1
int32_t m1_enc_count_last = 0;

volatile int32_t encoderCount2 = 0;     // Encoder count for motor 2
volatile bool encoderDirection2 = true; // Direction flag for motor 2
int32_t m2_enc_count_last = 0;

volatile bool enc_state_1A = 0;
volatile bool enc_state_1B = 0;
volatile bool enc_state_2A = 0;
volatile bool enc_state_2B = 0;

volatile bool enc_state_1A_prev = 0;
volatile bool enc_state_1B_prev = 0;
volatile bool enc_state_2A_prev = 0;
volatile bool enc_state_2B_prev = 0;

///////////////////////////////////////////////////////
//////////////// GLOBALS: SERIAL //////////////////////

byte messageBuffer[128];
byte sendBuffer[128];

/////////////////////////////////////////////////////////
// Remember to update SERIAL_SEND_BUFFER_SIZE if this is changed
/////////////////////////////////////////////////////////

// [Message 0]

// 1 byte / 2 / 2
bool status_motor1_ina = false;
bool status_motor1_inb = false;
bool status_motor1_endiag = false;
bool status_motor2_ina = false;
bool status_motor2_inb = false;
bool status_motor2_endiag = false;
volatile bool status_m1_rot_dir = 1;
volatile bool status_m2_rot_dir = 1;

// 1 byte / 2 / 4
bool status_txu_oe = false;
bool status_d3 = false;
bool status_d11 = false;
bool status_d13 = false;
bool status_rgb_led_r = false;
bool status_rgb_led_g = false;
bool status_rgb_led_b = false;
bool status_flag_unknown_message = false;

// 1 byte / 2 / 6
bool status_flag_1 = false;
bool status_flag_2 = false;
bool status_flag_3 = false;
bool status_flag_4 = false;
bool status_flag_5 = false;
bool status_flag_6 = false;
bool status_flag_7 = false;
bool status_flag_8 = false;

// 4 bytes / 8 /  14
uint16_t status_motor1_pwm = 0;
uint16_t status_motor1_cs = 0;

// 4 bytes / 8 / 22
uint16_t status_motor2_pwm = 0;
uint16_t status_motor2_cs = 0;

// 8 bytes / 16 / 38
int32_t status_m1_enc_count = 0;
int32_t status_m1_rot_rate = 0;

// 8 bytes / 16 / 54
int32_t status_m2_enc_count = 0;
int32_t status_m2_rot_rate = 0;

// [27 message bytes]

//------

// [Message 1]

// 12 bytes / 24 / 24
float status_acc_x = 0;
float status_acc_y = 0;
float status_acc_z = 0;

// 12 bytes / 24 / 48
float status_angle_x = 0;
float status_angle_y = 0;
float status_angle_z = 0;

// [24 message bytes]

//--------

// [Message 2]

// 12 bytes / 24 /
float status_gyr_x = 0;
float status_gyr_y = 0;
float status_gyr_z = 0;

// 12 bytes / 24 /
float status_mag_x = 0;
float status_mag_y = 0;
float status_mag_z = 0;

// [24 message bytes]

//// TO BE ADDED TO SERIAL BINARY

float status_cal_acc_x;
float status_cal_acc_y;
float status_cal_acc_z;

float status_cal_gyr_x;
float status_cal_gyr_y;
float status_cal_gyr_z;

float status_cal_mag_x;
float status_cal_mag_y;
float status_cal_mag_z;

float status_imu_roll;
float status_imu_pitch;
float status_imu_yaw;

float status_imu_quat_w;
float status_imu_quat_x;
float status_imu_quat_y;
float status_imu_quat_z;

/////////////////////////////////////////////////////////
// End of status data sent over serial
/////////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//////////////// GLOBALS: IMU /////////////////////////

#if IMU_MAHONY == 1
Adafruit_Mahony filter;
#endif

#if IMU_MADGWICK == 1
Adafruit_Madgwick filter;
#endif

#if IMU_NXP == 1
Adafruit_NXPSensorFusion filter;
#endif

#if IMU_FUSION

const FusionMatrix gyroscopeMisalignment = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
const FusionVector gyroscopeSensitivity = {1.0f, 1.0f, 1.0f};
const FusionVector gyroscopeOffset = {0.0f, 0.0f, 0.0f};
const FusionMatrix accelerometerMisalignment = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
const FusionVector accelerometerSensitivity = {1.0f, 1.0f, 1.0f};
const FusionVector accelerometerOffset = {0.0f, 0.0f, 0.0f};
const FusionMatrix softIronMatrix = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
const FusionVector hardIronOffset = {0.0f, 0.0f, 0.0f};

// Initialise algorithms
FusionOffset offset;
FusionAhrs ahrs;

// Set AHRS algorithm settings
const FusionAhrsSettings settings = {
    .convention = FusionConventionNwu,
    .gain = 0.5f,
    .gyroscopeRange = 2000.0f, /* replace this with actual gyroscope range in degrees/s */
    .accelerationRejection = 10.0f,
    .magneticRejection = 10.0f,
    .recoveryTriggerPeriod = 5 * FUSION_SAMPLE_RATE, /* 5 seconds */
};

#endif

///////////////////////////////////////////////////////
//////////////// GLOBALS: TIMING //////////////////////

uint32_t time_loop_now = 0;
uint32_t time_loop_last = 0;
uint32_t time_loop_interval_ms = 0;

uint32_t time_encoder_1_now = 0;
uint32_t time_encoder_1_last = 0;

uint32_t time_encoder_2_now = 0;
uint32_t time_encoder_2_last = 0;

uint32_t time_imu_now = 0;
uint32_t time_imu_last = 0;
uint32_t time_imu_interval_ms = 0;

uint32_t time_send_binary_now = 0;
uint32_t time_send_binary_last = 0;

uint32_t time_send_ascii_now = 0;
uint32_t time_send_ascii_last = 0;

bool first_run = false; // for serial raw cal

///////////////////////////////////////////////////////
//////////////// SETUP ///////////////////////////////

void setup()
{

  Serial.begin(115200);

  ////////////////////////  SETUP: TXU0104 /////////////////////

  pinMode(TXU0104_ENABLE, OUTPUT);
  digitalWrite(TXU0104_ENABLE, LOW);
  status_txu_oe = false;

  ////////////////////////  SETUP: LEDs ////////////////////////

  initialize_leds();

  ////////////////////////  SETUP: MOTOR DRIVER ////////////////
  motor_driver_init();

  ////////////////////////  SETUP: IMU /////////////////////////

  if (!imu_initialize())
  {
    Serial.println(F("IMU failed to initialize."));
    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, LOW);
  }

#if (IMU_MADGWICK || IMU_MAHONY || IMU_NXP)
  filter.begin(IMU_RATE);
#endif

#if IMU_FUSION
  FusionOffsetInitialise(&offset, FUSION_SAMPLE_RATE);
  FusionAhrsInitialise(&ahrs);

  FusionAhrsSetSettings(&ahrs, &settings);

#endif
}

///////////////////////////////////////////////////////
//////////////// LOOP: ///////////////////////////////

void loop()
{

  ////////////////////////  LOOP: Loop Timing  ////////////////////////

  time_loop_now = millis();
  time_loop_interval_ms = time_loop_now - time_loop_last;
  time_loop_last = time_loop_now;

  ////////////////////////  LOOP: IMU  ////////////////////////

  time_imu_now = millis();
  time_imu_interval_ms = time_imu_now - time_imu_last;
  if (time_imu_interval_ms > IMU_INTERVAL)
  {
    time_imu_last = time_imu_now;
    imu_update_accel_values();
    imu_update_mag_values();
    imu_update_gyro_values();

#if (IMU_MADGWICK || IMU_MAHONY || IMU_NXP)
    filter.update(status_cal_gyr_x, status_cal_gyr_y, status_cal_gyr_z,
                  status_cal_acc_x, status_cal_acc_y, status_cal_acc_z,
                  status_cal_mag_x, -status_cal_mag_y, -status_cal_mag_z, (time_imu_now - time_imu_last) / 1000.0); // (time_imu_now - time_imu_last)/1000.0
                                                                                                                    // time_imu_last = time_imu_now;

    status_imu_roll = filter.getRoll();
    status_imu_pitch = filter.getPitch();
    status_imu_yaw = filter.getYaw();
#endif

#if IMU_FUSION

    // Acquire latest sensor data
    // const clock_t timestamp = clock(); // not needed, already have time_imu_interval_ms// replace this with actual gyroscope timestamp
    FusionVector gyroscope = {status_cal_gyr_x, status_cal_gyr_y, status_cal_gyr_z};      // replace this with actual gyroscope data in degrees/s
    FusionVector accelerometer = {status_cal_acc_x, status_cal_acc_y, status_cal_acc_z};  // replace this with actual accelerometer data in g
    FusionVector magnetometer = {status_cal_mag_x, -status_cal_mag_y, -status_cal_mag_z}; // replace this with actual magnetometer data in arbitrary units

    // Apply calibration
    gyroscope = FusionCalibrationInertial(gyroscope, gyroscopeMisalignment, gyroscopeSensitivity, gyroscopeOffset);
    accelerometer = FusionCalibrationInertial(accelerometer, accelerometerMisalignment, accelerometerSensitivity, accelerometerOffset);
    magnetometer = FusionCalibrationMagnetic(magnetometer, softIronMatrix, hardIronOffset);

    // Update gyroscope offset correction algorithm
    gyroscope = FusionOffsetUpdate(&offset, gyroscope);

    // Calculate delta time (in seconds) to account for gyroscope sample clock error
    // not needed, already have time_imu_interval_ms

    // Update gyroscope AHRS algorithm
    FusionAhrsUpdate(&ahrs, gyroscope, accelerometer, magnetometer, time_imu_interval_ms / 1000.0);

    // Print algorithm outputs
    const FusionEuler euler = FusionQuaternionToEuler(FusionAhrsGetQuaternion(&ahrs));
    // const FusionVector earth = FusionAhrsGetEarthAcceleration(&ahrs);
    const FusionQuaternion quaternion = FusionAhrsGetQuaternion(&ahrs);

    status_imu_roll = euler.angle.roll;
    status_imu_pitch = euler.angle.pitch;
    status_imu_yaw = euler.angle.yaw;

    status_imu_quat_w = quaternion.element.w;
    status_imu_quat_x = quaternion.element.x;
    status_imu_quat_y = quaternion.element.y;
    status_imu_quat_z = quaternion.element.z;

#endif

    ////////////////////////  LOOP: MOTORS  ////////////////////////

    // Get latest encoder counts from interrupt-updated count
    // status_m1_enc_count = encoderCount1;
    // status_m1_rot_dir = encoderDirection1;
    // status_m2_enc_count = encoderCount2;
    // status_m2_rot_dir = encoderDirection2;

    time_encoder_1_now = millis();
    if (time_encoder_1_now - time_encoder_1_last > ENCODER_INTERVAL)
    {
      motor_update_enc1_rot_rate();
    }

    time_encoder_2_now = millis();
    if (time_encoder_2_now - time_encoder_2_last > ENCODER_INTERVAL)
    {
      motor_update_enc2_rot_rate();
    }

    ////////////////////////  LOOP: SERIAL READ  ////////////////////////

    readSerialData();

    ////////////////////////  LOOP: SERIAL SEND  ////////////////////////

    time_send_ascii_now = millis();
    if (time_send_ascii_now - time_send_ascii_last > SERIAL_ASCII_INTERVAL)
    {
      time_send_ascii_last = time_send_ascii_now;
      sendSerial_ascii();
    }
  }

  time_send_binary_now = millis();
  if (time_send_binary_now - time_send_binary_last > SERIAL_BINARY_INTERVAL)
  {
    time_send_binary_last = time_send_binary_now;

    if (SERIAL_BINARY)
    {
      sendSerialData(0);
      sendSerialData(1);
      sendSerialData(2);
    }
  }
}
