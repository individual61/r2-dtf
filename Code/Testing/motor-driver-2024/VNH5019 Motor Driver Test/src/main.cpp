#include <parameters.h>
#include <function_declarations_and_globals.h>

mbed::PwmOut pwmPin1(digitalPinToPinName(MOTOR_DRIVER_PIN_PWM1)); // Initialize pwmPin1 with PWM1 pin.
mbed::PwmOut pwmPin2(digitalPinToPinName(MOTOR_DRIVER_PIN_PWM2)); // Initialize pwmPin1 with PWM1 pin.

char serialInputchar;

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

/////////////////////////////////////////////////////////
// Remember to update SERIAL_SEND_BUFFER_SIZE if this is changed
/////////////////////////////////////////////////////////

byte messageBuffer[128];
byte sendBuffer[128];

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
float status_ang_rate_x = 0;
float status_ang_rate_y = 0;
float status_ang_rate_z = 0;

// [12 message bytes]

/////////////////////////////////////////////////////////
// End of status data sent over serial
/////////////////////////////////////////////////////////

void setup()
{

  Serial.begin(115200);

  ////// TXU0104
  pinMode(TXU0104_ENABLE, OUTPUT);
  digitalWrite(TXU0104_ENABLE, LOW);
  status_txu_oe = false;

  ////// LEDs
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, HIGH);

  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, HIGH);

  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, HIGH);

  ////// Motor Driver
  motor_driver_init();
  // pinMode(MOTOR_ENCODER_1A, INPUT);
  // pinMode(MOTOR_ENCODER_1B, INPUT);
  // pinMode(MOTOR_ENCODER_2A, INPUT);
  // pinMode(MOTOR_ENCODER_2B, INPUT);
}

//============

uint32_t ii = 0;

uint32_t timenow_1 = 0;
uint32_t timelast_1 = 0;

uint32_t timenow_2 = 0;
uint32_t timelast_2 = 0;

uint32_t time_send_now = 0;
uint32_t time_send_last = 0;
uint32_t time_send_interval = 100;

#define ENCODER_INTERVAL 100

// int xx_serial_message_buffer_size = 10;

volatile bool interrupt_called = 0;

float float_rate1 = 0.0;
float float_rate2 = 0.0;

void loop()
{
  // Get latest encoder counts from interrupt-populated count
  status_m1_enc_count = encoderCount1;
  status_m1_rot_dir = encoderDirection1;
  status_m2_enc_count = encoderCount2;
  status_m2_rot_dir = encoderDirection2;

  //  bool astate = enc_state_2A;
  //  bool bstate = enc_state_2B;

  /*Serial.print(status_m1_enc_count);
  Serial.print(" ");
  Serial.print(status_m1_rot_dir);
  Serial.print(" ");
  Serial.print(status_m2_enc_count);
  Serial.print(" ");
  Serial.println(status_m2_rot_dir);*/

  timenow_1 = millis();

  if (timenow_1 - timelast_1 > ENCODER_INTERVAL)
  {
    uint32_t interval_1 = timenow_1 - timelast_1;
    timelast_1 = timenow_1;

    int32_t traveled = status_m1_enc_count - m1_enc_count_last;
    m1_enc_count_last = status_m1_enc_count;

    status_m1_rot_rate = 1000 * (((float)traveled) / ((float)interval_1));
  }

  timenow_2 = millis();

  if (timenow_2 - timelast_2 > ENCODER_INTERVAL)
  {
    uint32_t interval_2 = timenow_2 - timelast_2;
    timelast_2 = timenow_2;

    int32_t traveled = status_m2_enc_count - m2_enc_count_last;
    m2_enc_count_last = status_m2_enc_count;

    status_m2_rot_rate = 1000 * (((float)traveled) / ((float)interval_2));
  }

  readSerialData();

  time_send_now = millis();

  if (time_send_now - time_send_last > time_send_interval)
  {
    time_send_last = time_send_now;

    if (SERIAL_BINARY)
    {
      sendSerialData(0);
      sendSerialData(1);
      sendSerialData(2);
    }
  }
}
