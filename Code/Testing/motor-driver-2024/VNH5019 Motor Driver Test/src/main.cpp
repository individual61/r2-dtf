#include <parameters.h>
#include <function_declarations_and_globals.h>

mbed::PwmOut pwmPin1(digitalPinToPinName(MOTOR_DRIVER_PIN_PWM1)); // Initialize pwmPin1 with PWM1 pin.
mbed::PwmOut pwmPin2(digitalPinToPinName(MOTOR_DRIVER_PIN_PWM2)); // Initialize pwmPin1 with PWM1 pin.

char serialInputchar;

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

//unsigned long time_start;
//unsigned long time_now;
//time_now = millis()


void loop()
{

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

    delay(500);
    digitalWrite(22, HIGH);
    digitalWrite(23, HIGH);
    digitalWrite(24, HIGH);
  }
  // Serial.println("M1 0 to 1000");
  // for (int i = 0; i <= 1000; i++)
  // {
  //   motor_driver_setM1Speed(i);
  //   motor_driver_setM2Speed(i);
  //   delay(2);
  // }
}
