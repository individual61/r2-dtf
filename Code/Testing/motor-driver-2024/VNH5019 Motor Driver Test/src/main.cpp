#include <parameters.h>
#include <function_declarations_and_globals.h>

mbed::PwmOut pwmPin1(digitalPinToPinName(MOTOR_DRIVER_PIN_PWM1)); // Initialize pwmPin1 with PWM1 pin.
mbed::PwmOut pwmPin2(digitalPinToPinName(MOTOR_DRIVER_PIN_PWM2)); // Initialize pwmPin1 with PWM1 pin.

char serialInputchar;

// Remember to update SERIAL_SEND_BUFFER_SIZE if this is changed
/////////////////////////////////////////////////////////
// 1 byte
bool status_motor1_ina = false;
bool status_motor1_inb = false;
bool status_motor1_endiag = false;
bool status_motor2_ina = false;
bool status_motor2_inb = false;
bool status_motor2_endiag = false;
bool status_flag_1 = false;
bool status_flag_2 = false;

// 1 byte
bool status_txu_oe = false;
bool status_d3 = false;
bool status_d11 = false;
bool status_d13 = false;
bool status_rgb_led_r = false;
bool status_rgb_led_g = false;
bool status_rgb_led_b = false;
bool status_flag_3 = false;

// 2 bytes
uint8_t status_motor1_pwm = 0;
uint8_t status_motor1_cs = 0;

// 2 bytes
uint8_t status_motor2_pwm = 0;
uint8_t status_motor2_cs = 0;

// 2 bytes
uint8_t status_m1_enc_count = 0;
uint8_t status_m1_rot_rate = 0;

// 2 bytes
uint8_t status_m2_enc_count = 0;
uint8_t status_m2_rot_rate = 0;

// 3 bytes
int8_t status_acc_x = 0;
int8_t status_acc_y = 0;
int8_t status_acc_z = 0;

// 3 bytes
int8_t status_ang_rate_x = 0;
int8_t status_ang_rate_y = 0;
int8_t status_ang_rate_z = 0;
/////////////////////////////////////////////////////////



void setup()
{
  // Disable TXU0104
  pinMode(TXU0104_ENABLE, OUTPUT);
  digitalWrite(TXU0104_ENABLE, LOW);
  status_txu_oe = false;

  motor_driver_init();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  // Serial.println("Dual VNH5019 Motor Shield");

  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, HIGH);

  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, HIGH);

  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, HIGH);
}



//============

int ii = 0;

void loop()
{

  readSerialData();
  if (SERIAL_BINARY)
  {
    sendSerialData();
  }
  delay(50);

  /*
  if (ii == 200)
  {
    ii = 0;
  }
  delay(100); // Wait for 2 seconds before sending again

  motor_driver_setM1Speed(ii);
  motor_driver_setM2Speed(ii);

  ii++;

  if (SERIAL_BINARY)
  {
    sendSerialData();
  }
  delay(50);

  */
}
