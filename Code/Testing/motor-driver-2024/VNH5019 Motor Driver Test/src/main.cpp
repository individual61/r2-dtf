#include <parameters.h>
#include <function_declarations_and_globals.h>

mbed::PwmOut pwmPin1(digitalPinToPinName(MOTOR_DRIVER_PIN_PWM1)); // Initialize pwmPin1 with PWM1 pin.
mbed::PwmOut pwmPin2(digitalPinToPinName(MOTOR_DRIVER_PIN_PWM2)); // Initialize pwmPin1 with PWM1 pin.

char serialInputchar;

// Remember to update SERIAL_SEND_BUFFER_SIZE if this is changed
/////////////////////////////////////////////////////////
// 1 byte
bool status_motor1_ina = false;
bool status_motor1_inb = true;
bool status_motor1_endiag = false;
bool status_motor2_ina = true;
bool status_motor2_inb = false;
bool status_motor2_endiag = true;
bool status_flag_1 = false;
bool status_flag_2 = true;

// 1 byte
bool status_txu_oe = true;
bool status_d3 = true;
bool status_d11 = false;
bool status_d13 = true;
bool status_rgb_led_r = true;
bool status_rgb_led_g = true;
bool status_rgb_led_b = true;
bool status_flag_3 = true;

// 2 bytes
uint8_t status_motor1_pwm = 4;
uint8_t status_motor1_cs = 5;

// 2 bytes
uint8_t status_motor2_pwm = 6;
uint8_t status_motor2_cs = 7;

// 2 bytes
uint8_t status_m1_enc_count = 8;
uint8_t status_m1_rot_rate = 9;

// 2 bytes
uint8_t status_m2_enc_count = 10;
uint8_t status_m2_rot_rate = 11;

// 3 bytes
int8_t status_acc_x = 12;
int8_t status_acc_y = 13;
int8_t status_acc_z = 14;

// 3 bytes
int8_t status_ang_rate_x = 15;
int8_t status_ang_rate_y = 16;
int8_t status_ang_rate_z = 17;
/////////////////////////////////////////////////////////


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  Serial.println("Dual VNH5019 Motor Shield");

  motor_driver_init();

  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);

  pinMode(23, OUTPUT);
  digitalWrite(23, HIGH);

  pinMode(24, OUTPUT);
  digitalWrite(24, HIGH);

  //   pwmPin1.period( 1.0 / PWM_FREQUENCY );
  // pwmPin1.write( 0.5 );

  // pwmPin1.write(500 / 1000.0);
}

// unsigned long time_start;
// unsigned long time_now;
// time_now = millis()


void loop()
{
  sendSerialData();
  delay(100); // Wait for 2 seconds before sending again

  if (Serial.available())
  {
    serialInputchar = Serial.read();

    Serial.print("received\t");
    Serial.println(serialInputchar);

    if (serialInputchar == 'A')
    {
      digitalWrite(22, LOW);
    }

    if (serialInputchar == 'B')
    {
      digitalWrite(23, LOW);
    }

    if (serialInputchar == 'C')
    {
      digitalWrite(24, LOW);
    }

//    delay(500);
    digitalWrite(22, HIGH);
    digitalWrite(23, HIGH);
    digitalWrite(24, HIGH);
  }

  Serial.println("M1/2 0 to 1000");
  for (int i = 0; i <= 200; i++)
  {
    motor_driver_setM1Speed(i);
    motor_driver_setM2Speed(i);
    delay(100);
  }
}
