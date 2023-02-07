// This uses timer1 on ATtiny85 to control up to 5 servos.
// Sawchuk.

#define USE_LCD
#ifdef USE_LCD
#include <TinyWireM.h>                  // I2C Master lib for ATTinys which use USI
#include <LiquidCrystal_I2C_Tiny.h>
LiquidCrystal_I2C lcd(0x3E,16,2);  // set the LCD address to 0x3E for a 16 chars and 2 line display
// The LCD uses pins 0 and 2 with TinyWireM
#endif

#define SERVO1_PIN 1 // for ATtiny85 test
#define SERVO2_PIN 4 // for ATtiny85 test

#include "ServoATtiny85.h"
Servo servo1, servo2;

void setup() {
#ifdef USE_LCD
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("setup");
  delay(1000);
#endif
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
#ifdef USE_LCD
  lcd.print(" Ready");
#endif
//  delay(1000);
}

void loop() {
  int angle = map(analogRead(A3),0,1023,0,180);
#ifdef USE_LCD
  lcd.clear();
  lcd.print(angle);
  lcd.setCursor(6,0);
  lcd.print(180-angle);
#endif
  
  servo1.write(angle);
  servo2.write(180-angle);

//#ifdef USE_LCD
//  lcd.setCursor(0,1);
//  lcd.print(servo1.readMicroseconds());
//  lcd.setCursor(6,1);
//  lcd.print(servo2.readMicroseconds());
//#endif
  
  delay(100);
}

