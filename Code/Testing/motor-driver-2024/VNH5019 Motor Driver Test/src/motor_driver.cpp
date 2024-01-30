#include <parameters.h>
#include <function_declarations_and_globals.h>

/* Modified from https://github.com/pololu/dual-vnh5019-motor-shield to work with Arduino Nano 33 BLE fast PWM */

void motor_driver_init(void)
{
    // Define pinMode for the pins and set the frequency for timer1.

    pinMode(MOTOR_DRIVER_PIN_INA1, OUTPUT);
    pinMode(MOTOR_DRIVER_PIN_INB1, OUTPUT);
    pinMode(MOTOR_DRIVER_PIN_PWM1, OUTPUT);
    pinMode(MOTOR_DRIVER_PIN_EN1DIAG1, INPUT);
    pinMode(MOTOR_DRIVER_PIN_CS1, INPUT);

    pinMode(MOTOR_DRIVER_PIN_INA2, OUTPUT);
    pinMode(MOTOR_DRIVER_PIN_INB2, OUTPUT);
    pinMode(MOTOR_DRIVER_PIN_PWM2, OUTPUT);
    pinMode(MOTOR_DRIVER_PIN_EN2DIAG2, INPUT);
    pinMode(MOTOR_DRIVER_PIN_CS2, INPUT);

    pwmPin1.period(1.0 / PWM_FREQUENCY);
    pwmPin2.period(1.0 / PWM_FREQUENCY);

    digitalWrite(MOTOR_DRIVER_PIN_INA1, LOW); // Make the motor coast no
    digitalWrite(MOTOR_DRIVER_PIN_INB1, LOW); // matter which direction it is spinning.
    pwmPin1.write(0.0);
    pwmPin2.write(0.0);
}

void motor_driver_setM1_INA(bool setting)
{
    digitalWrite(MOTOR_DRIVER_PIN_INA1, setting); // Make the motor coast no
    status_motor1_ina = setting;
}

void motor_driver_setM1_INB(bool setting)
{
    digitalWrite(MOTOR_DRIVER_PIN_INB1, setting); // Make the motor coast no
    status_motor1_inb = setting;
}

void motor_driver_setM2_INA(bool setting)
{
    digitalWrite(MOTOR_DRIVER_PIN_INA2, setting); // Make the motor coast no
    status_motor2_ina = setting;
}

void motor_driver_setM2_INB(bool setting)
{
    digitalWrite(MOTOR_DRIVER_PIN_INB2, setting); // Make the motor coast no
    status_motor2_inb = setting;
}

// Set PWM only, not INX
void motor_driver_setM1_PWM(int speed)
{
    if (speed < 0)
    {
        speed = -speed; // Make speed a positive quantity
    }

    if (speed > 255)
    { // Max PWM dutycycle
        speed = 255;
    }

    status_motor1_pwm = speed;

    pwmPin1.write(speed / 255.0);
}

// Set PWM only, not INX
void motor_driver_setM2_PWM(int speed)
{
    if (speed < 0)
    {
        speed = -speed; // Make speed a positive quantity
    }

    if (speed > 255)
    { // Max PWM dutycycle
        speed = 255;
    }

    status_motor2_pwm = speed;

    pwmPin2.write(speed / 255.0);
}



// Set speed for motor 1, speed is a number between /-255/ and /255/
void motor_driver_setM1Speed(int speed)
{

    unsigned char reverse = 0;

    if (speed < 0)
    {
        speed = -speed; // Make speed a positive quantity
        reverse = 1;    // Preserve the direction
    }

    if (speed > 255)
    { // Max PWM dutycycle
        speed = 255;
    }

    status_motor1_pwm = speed;

    if (speed == 0)
    {
        digitalWrite(MOTOR_DRIVER_PIN_INA1, LOW); // Make the motor coast no
        digitalWrite(MOTOR_DRIVER_PIN_INB1, LOW); // matter which direction it is spinning.
        status_motor1_ina = false;
        status_motor1_inb = false;
    }
    else if (reverse)
    {
        digitalWrite(MOTOR_DRIVER_PIN_INA1, LOW);
        digitalWrite(MOTOR_DRIVER_PIN_INB1, HIGH);
        status_motor1_ina = false;
        status_motor1_inb = true;
    }
    else
    {
        digitalWrite(MOTOR_DRIVER_PIN_INA1, HIGH);
        digitalWrite(MOTOR_DRIVER_PIN_INB1, LOW);
        status_motor1_ina = true;
        status_motor1_inb = false;
    }

    pwmPin1.write(speed / 255.0);
    // Serial.print(speed);
    // Serial.print("\t");
    // Serial.println(speed / 255.0);
}

// Set speed for motor 2, speed is a number between /-255/ and /255/
void motor_driver_setM2Speed(int speed)
{
    unsigned char reverse = 0;

    if (speed < 0)
    {
        speed = -speed; // Make speed a positive quantity
        reverse = 1;    // Preserve the direction
    }
    if (speed > 255) // Max PWM dutycycle
    {
        speed = 255;
    }

    status_motor2_pwm = speed;

    if (speed == 0)
    {
        digitalWrite(MOTOR_DRIVER_PIN_INA2, LOW); // Make the motor coast no
        digitalWrite(MOTOR_DRIVER_PIN_INB2, LOW); // matter which direction it is spinning.
        status_motor2_ina = false;
        status_motor2_inb = false;
    }
    else if (reverse)
    {
        digitalWrite(MOTOR_DRIVER_PIN_INA2, LOW);
        digitalWrite(MOTOR_DRIVER_PIN_INB2, HIGH);
        status_motor2_ina = false;
        status_motor2_inb = true;
    }
    else
    {
        digitalWrite(MOTOR_DRIVER_PIN_INA2, HIGH);
        digitalWrite(MOTOR_DRIVER_PIN_INB2, LOW);
        status_motor2_ina = true;
        status_motor2_inb = false;
    }

    pwmPin2.write(speed / 255.0);
}

// Set speed for motor 1 and 2
void motor_driver_setSpeeds(int m1Speed, int m2Speed)
{
    motor_driver_setM1Speed(m1Speed);
    motor_driver_setM2Speed(m2Speed);
}

// Brake motor 1, brake is a number between 0 and 255
void motor_driver_setM1Brake(int brake)
{
    // normalize brake
    if (brake < 0)
    {
        brake = -brake;
    }

    if (brake > 255) // Max brake
    {
        brake = 255;
    }

    status_motor1_pwm = brake;

    digitalWrite(MOTOR_DRIVER_PIN_INA1, LOW);
    digitalWrite(MOTOR_DRIVER_PIN_INB1, LOW);
    status_motor2_ina = false;
    status_motor2_inb = false;

    pwmPin1.write(brake / 255.0);
}

// Brake motor 2, brake is a number between 0 and 255
void motor_driver_setM2Brake(int brake)
{
    // normalize brake
    if (brake < 0)
    {
        brake = -brake;
    }

    if (brake > 255) // Max brake
    {
        brake = 255;
    }

    status_motor2_pwm = brake;

    digitalWrite(MOTOR_DRIVER_PIN_INA2, LOW);
    digitalWrite(MOTOR_DRIVER_PIN_INB2, LOW);
    status_motor2_ina = false;
    status_motor2_inb = false;

    pwmPin2.write(brake / 255.0);
}

// Brake motor 1 and 2, brake is a number between 0 and 255
void motor_driver_setBrakes(int m1Brake, int m2Brake)
{
    motor_driver_setM1Brake(m1Brake);
    motor_driver_setM2Brake(m2Brake);
}

// Return motor 1 current value in milliamps.
// (3300 mV / 1024 ct ) / ( 144 mV/A) = 22.38 mA / ct
float motor_driver_getM1CurrentMilliamps(void)
{
    return analogRead(MOTOR_DRIVER_PIN_CS1) * 22.38;
}

// Return motor 2 current value in milliamps.
// (3300 mV / 1024 ct ) / ( 144 mV/A) = 22.38 mA / ct
float motor_driver_getM2CurrentMilliamps(void)
{
    return analogRead(MOTOR_DRIVER_PIN_CS2) * 22.38;
}

// Return error status for motor 1
unsigned char motor_driver_getM1Fault(void)
{
    return !digitalRead(MOTOR_DRIVER_PIN_EN1DIAG1);
}

// Return error status for motor 2
unsigned char motor_driver_getM2Fault(void)
{
    return !digitalRead(MOTOR_DRIVER_PIN_EN2DIAG2);
}
