#include <Arduino.h>
#include <mbed.h>

#include "DualVNH5019MotorShield-nano33ble.h"

////////////////////////////////////////////////////
// Library default pins
// Arduino Pin	  VNH5019 Driver Pin	Basic Function

// Outputs to motor board

// Digital 2   	M1INA	    Motor 1 direction input A
// Digital 4    M1INB	    Motor 1 direction input B
// Digital 7   	M2INA	    Motor 2 direction input A
// Digital 8    M2INB	    Motor 2 direction input B
// Digital 9    M1PWM	    Motor 1 speed input
// Digital 10   M2PWM	    Motor 2 speed input

// Inputs to arduino

// Digital 6   	M1EN/DIAG	Motor 1 enable input/fault output
// Digital 12  	M2EN/DIAG	Motor 2 enable input/fault output
// Analog 0  	  M1CS	    Motor 1 current sense output
// Analog 1  	  M2CS	    Motor 2 current sense output
///////////////////////////////////////////////////

#define MOTOR_DRIVER_PIN_INA1 2
#define MOTOR_DRIVER_PIN_INB1 4
#define MOTOR_DRIVER_PIN_PWM1 9
#define MOTOR_DRIVER_PIN_EN1DIAG1 6
#define MOTOR_DRIVER_PIN_CS1 A0

#define MOTOR_DRIVER_PIN_INA2 7
#define MOTOR_DRIVER_PIN_INB2 8
#define MOTOR_DRIVER_PIN_PWM2 10
#define MOTOR_DRIVER_PIN_EN2DIAG2 12
#define MOTOR_DRIVER_PIN_CS2 A1

DualVNH5019MotorShield motor_driver(MOTOR_DRIVER_PIN_INA1,
                                    MOTOR_DRIVER_PIN_INB1,
                                    MOTOR_DRIVER_PIN_PWM1,
                                    MOTOR_DRIVER_PIN_EN1DIAG1,
                                    MOTOR_DRIVER_PIN_CS1,
                                    MOTOR_DRIVER_PIN_INA2,
                                    MOTOR_DRIVER_PIN_INB2,
                                    MOTOR_DRIVER_PIN_PWM2,
                                    MOTOR_DRIVER_PIN_EN2DIAG2,
                                    MOTOR_DRIVER_PIN_CS2);

void stopIfFault()
{
  // if (md.getM1Fault())
  // {
  //   digitalWrite(LED_BUILTIN, HIGH);
  //   Serial.println("M1 fault");
  //   while (1)
  //     ;
  // }
  // if (md.getM2Fault())
  // {
  //   digitalWrite(LED_BUILTIN, HIGH);
  //   Serial.println("M2 fault");
  //   while (1)
  //     ;
  // }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
  Serial.println("Dual VNH5019 Motor Shield");
  motor_driver.init();
}

void loop()
{
  Serial.println("M1 0 to 1000");
  for (int i = 0; i <= 1000; i++)
  {
    //motor_driver.setM1Speed(i);
    motor_driver.pwmPin1.write(i/1000.0);
    stopIfFault();
    delay(2);
  }

  Serial.println("M1 1000 to -1000");
  for (int i = 1000; i >= -1000; i--)
  {
    motor_driver.setM1Speed(i);
    stopIfFault();
    delay(2);
  }

  Serial.println("M1 -1000 to 0");
  for (int i = -1000; i <= 0; i++)
  {
    motor_driver.setM1Speed(i);
    stopIfFault();
    delay(2);
  }

  // for (int i = 0; i <= 400; i++)
  // {
  //   md.setM2Speed(i);
  //   stopIfFault();
  //   if (i%200 == 100)
  //   {
  //     Serial.print("M2 current: ");
  //     Serial.println(md.getM2CurrentMilliamps());
  //   }
  //   delay(2);
  // }

  // for (int i = 400; i >= -400; i--)
  // {
  //   md.setM2Speed(i);
  //   stopIfFault();
  //   if (i%200 == 100)
  //   {
  //     Serial.print("M2 current: ");
  //     Serial.println(md.getM2CurrentMilliamps());
  //   }
  //   delay(2);
  // }

  // for (int i = -400; i <= 0; i++)
  // {
  //   md.setM2Speed(i);
  //   stopIfFault();
  //   if (i%200 == 100)
  //   {
  //     Serial.print("M2 current: ");
  //     Serial.println(md.getM2CurrentMilliamps());
  //   }
  //   delay(2);
  // }
}
