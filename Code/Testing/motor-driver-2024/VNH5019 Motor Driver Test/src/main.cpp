#include <Arduino.h>

#include "DualVNH5019MotorShield.h"


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





DualVNH5019MotorShield md;

// void setup() {
//   // initialize digital pin LED_BUILTIN as an output.
//   pinMode(LED_BUILTIN, OUTPUT);
// }

// // the loop function runs over and over again forever
// void loop() {
//   digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
//   delay(100);                      // wait for a second
//   digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
//   delay(100);                      // wait for a second
// }


// void loop()
// {
//   Serial.println("a");
//   delay(500);
// }

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
  md.init();
}

void loop()
{
  Serial.println("M1 0 to 400");
  for (int i = 0; i <= 400; i++)
  {
    md.setM1Speed(i);
    stopIfFault();
    if (i % 200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
    }
    delay(2);
  }

  Serial.println("M1 400 to -400");
  for (int i = 400; i >= -400; i--)
  {
    md.setM1Speed(i);
    stopIfFault();
    if (i % 200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
    }
    delay(2);
  }

  Serial.println("M1 -400 to 0");
  for (int i = -400; i <= 0; i++)
  {
    md.setM1Speed(i);
    stopIfFault();
    if (i % 200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
    }
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
