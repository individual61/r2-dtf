#include <parameters.h>
#include <function_declarations_and_globals.h>

mbed::PwmOut pwmPin1(digitalPinToPinName(MOTOR_DRIVER_PIN_PWM1)); // Initialize pwmPin1 with PWM1 pin.
mbed::PwmOut pwmPin2(digitalPinToPinName(MOTOR_DRIVER_PIN_PWM2)); // Initialize pwmPin1 with PWM1 pin.

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
  Serial.println("Dual VNH5019 Motor Shield");

motor_driver_init();

 //   pwmPin1.period( 1.0 / PWM_FREQUENCY );
 // pwmPin1.write( 0.5 );


  //pwmPin1.write(500 / 1000.0);
}

void loop()
{
  Serial.println("M1 0 to 1000");
  for (int i = 0; i <= 1000; i++)
  {
    motor_driver_setM1Speed(i);
    motor_driver_setM2Speed(i);
    delay(2);
  }
}
