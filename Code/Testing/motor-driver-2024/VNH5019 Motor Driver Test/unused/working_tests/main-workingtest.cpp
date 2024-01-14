#include "mbed.h"

#define PWM_PIN               9
#define PWM_FREQUENCY         19000

mbed::PwmOut pwmPin( digitalPinToPinName( PWM_PIN ) );

void setup()
{
  pwmPin.period( 1.0 / PWM_FREQUENCY );
  pwmPin.write( 0.5 );
  
//   Serial.begin( 9600 );
//   while ( !Serial );
//   Serial.println( "mbed PWM pin example" );
}


void loop()
{
  static uint32_t dutyCycle = 50;
  
  dutyCycle = ( dutyCycle + 1 ) % 100;  
  pwmPin.write( dutyCycle / 100.0 );
  
  delay( 10 );
}