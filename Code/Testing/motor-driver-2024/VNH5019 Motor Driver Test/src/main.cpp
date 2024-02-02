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

/////////////////////////////////////////////////////////
// Remember to update SERIAL_SEND_BUFFER_SIZE if this is changed
/////////////////////////////////////////////////////////

// 1 byte / 1
bool status_motor1_ina = false;
bool status_motor1_inb = false;
bool status_motor1_endiag = false;
bool status_motor2_ina = false;
bool status_motor2_inb = false;
bool status_motor2_endiag = false;
volatile bool status_m1_rot_dir = 1;
volatile bool status_m2_rot_dir = 1;

// 1 byte / 2
bool status_txu_oe = false;
bool status_d3 = false;
bool status_d11 = false;
bool status_d13 = false;
bool status_rgb_led_r = false;
bool status_rgb_led_g = false;
bool status_rgb_led_b = false;
bool status_flag_unknown_message = false;

// 1 byte / 3
bool status_flag_1 = false;
bool status_flag_2 = false;
bool status_flag_3 = false;
bool status_flag_4 = false;
bool status_flag_5 = false;
bool status_flag_6 = false;
bool status_flag_7 = false;
bool status_flag_8 = false;

// 4 bytes / 7
uint16_t status_motor1_pwm = 0;
uint16_t status_motor1_cs = 0;

// 4 bytes / 11
uint16_t status_motor2_pwm = 0;
uint16_t status_motor2_cs = 0;

// 8 bytes / 19
int32_t status_m1_enc_count = 0;
uint32_t status_m1_rot_rate = 0;

// 8 bytes / 27
int32_t status_m2_enc_count = 0;
uint32_t status_m2_rot_rate = 0;

// 12 bytes / 39
float status_acc_x = 0;
float status_acc_y = 0;
float status_acc_z = 0;

// 12 bytes / 51
float status_angle_x = 0;
float status_angle_y = 0;
float status_angle_z = 0;

// 12 bytes / 63
float status_ang_rate_x = 0;
float status_ang_rate_y = 0;
float status_ang_rate_z = 0;

/////////////////////////////////////////////////////////
// End of status data sent over serial
/////////////////////////////////////////////////////////

void setup()
{
  // Disable TXU0104
  pinMode(TXU0104_ENABLE, OUTPUT);
  digitalWrite(TXU0104_ENABLE, LOW);
  status_txu_oe = false;

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  motor_driver_init();

  Serial.begin(115200);
  // Serial.println("Dual VNH5019 Motor Shield");

  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, HIGH);

  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, HIGH);

  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, HIGH);

  pinMode(MOTOR_ENCODER_2A, OUTPUT);
  digitalWrite(TXU0104_ENABLE, LOW); //////////////////////////////////////////////
}

//============

uint32_t ii = 0;

uint32_t timenow_1 = 0;
uint32_t timelast_1 = 0;

uint32_t timenow_2 = 0;
uint32_t timelast_2 = 0;

uint32_t time_send_now = 0;
uint32_t time_send_last = 0;
uint32_t time_send_interval = 50;

#define ENCODER_INTERVAL 50

int xx_serial_message_buffer_size = 10;

void loop()
{
  // Get latest encoder counts from interrupt-populated count
  status_m1_enc_count = encoderCount1;
  status_m1_rot_dir = encoderDirection1;
  status_m2_enc_count = encoderCount2;
  status_m2_rot_dir = encoderDirection2;

  timenow_1 = millis();
  timenow_2 = timenow_1;

  if (timenow_1 - timelast_1 > ENCODER_INTERVAL)
  {
    uint32_t interval = timenow_1 - timelast_1;
    timelast_1 = timenow_1;
    uint32_t traveled = status_m1_enc_count - m1_enc_count_last;
    status_m1_rot_rate = traveled / ((float)interval);
  }

  if (timenow_2 - timelast_2 > ENCODER_INTERVAL)
  {
    uint32_t interval = timenow_2 - timelast_2;
    timelast_2 = timenow_2;
    uint32_t traveled = status_m2_enc_count - m2_enc_count_last;
    status_m2_rot_rate = traveled / ((float)interval);
  }

  readSerialData();

  time_send_now = millis();
  status_m1_enc_count = time_send_now - time_send_last;
  if (time_send_now - time_send_last > time_send_interval)
  {
    time_send_last = time_send_now;

    if (SERIAL_BINARY)
    {
      // sendSerialData();

      int xx_serial_send_buffer_size = (2 * xx_serial_message_buffer_size + 2);

      byte messageBuffer[xx_serial_message_buffer_size];
      byte sendBuffer[xx_serial_send_buffer_size]; // Adjust the buffer size as needed based on your data

      sendBuffer[0] = (byte)SERIAL_HEADER;
      sendBuffer[1] = (byte)SERIAL_HEADER;

      for (int i = 0; i < xx_serial_message_buffer_size; i++)
      {
        sendBuffer[2 + i * 2] = 0;//messageBuffer[i];
        sendBuffer[3 + i * 2] = (byte)SERIAL_PAD;
      }

      // Send the entire buffer with a single call to Serial.write()
      Serial.write(sendBuffer, xx_serial_message_buffer_size);
    }
  }

  // delay(500);
}


// >>È_._._ _Ð_._._ _._._._._À_œ_._ _._._._._._._._._._._._._û_â_._       with xx_serial_message_buffer_size = 63
/*
3E 3E C8 5F 08                                                                                                                                                                
5F 00                                                                                                                                                                         
5F 20                                                                                                                                                                         
5F D0                                                                                                                                                                         
5F 08                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 20                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F C0                                                                                                                                                                         
5F 9C                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 20                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F 00                                                                                                                                                                         
5F FB                                                                                                                                                                         
5F E2                                                                                                                                                                         
5F 01 
*/