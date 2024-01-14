#include "mbed.h"
#include <Arduino.h>

#define PWM_PIN_1 10
#define PWM_PIN_2 9
#define PWM_FREQUENCY 19000

mbed::PwmOut pwmPin1(digitalPinToPinName(PWM_PIN_1));
mbed::PwmOut pwmPin2(digitalPinToPinName(PWM_PIN_2));

void setup()
{
    pwmPin1.period(1.0 / PWM_FREQUENCY);
    pwmPin1.write(1.0);

    pwmPin2.period(1.0 / PWM_FREQUENCY);
    pwmPin2.write(0.0);

    Serial.begin(115200);
    while (!Serial)
        ;
    Serial.println("mbed PWM pin example");
}

static uint32_t dutyCycle1 = 100;
static uint32_t dutyCycle2 = 100;

void loop()
{

    for (int i = 0; i <= 1000; i++)
    {
        // motor_driver.setM1Speed(i);
        Serial.println(i / 1000.0);
        pwmPin1.write(i / 1000.0);
        delay(2);
    }

    // dutyCycle1 = (dutyCycle1 + 1) % 100;
    // pwmPin1.write(dutyCycle1 / 100.0);

    // dutyCycle2 = (dutyCycle2 - 1) % 100;
    // pwmPin2.write(dutyCycle2 / 100.0);

    delay(10);
}
