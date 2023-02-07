/*    _                           _      ____ _        _ 
 *   | |    __ _ _   _ _ __   ___| |__  / ___| |_ _ __| |
 *   | |   / _` | | | | '_ \ / __| '_ \| |   | __| '__| |
 *   | |__| (_| | |_| | | | | (__| | | | |___| |_| |  | |
 *   |_____\__,_|\__,_|_| |_|\___|_| |_|\____|\__|_|  |_|
 *   Version 0.12                                        
 */

#include "ServoATtiny85.h"
#include "EEPROM.h"

/* The PWM output pin on the MCU which is attached to the PWM input of the ESC */
int throttlePin = 3;
/**/

Servo throttle;

void setup() {
  Serial.begin(9600);
  throttle.attach(throttlePin);
}

void loop() {
  char *configData[6];
  char *configArray = NULL;
  byte index = 0;

  configArray = strtok(readCharFromEEPROM(0), ",");
  while (configArray != NULL)
  {
    configData[index] = configArray;
    index++;
    configArray = strtok(NULL, ",");
  }
  
  /* How much power to use for launching the car, as high as traction permits. 1-100% */
  int launchPower = atoi(configData[0]);
  /* How many miliseconds we launch before increasing power, as short as traction permits. 1ms is equal to 0.001 seconds. */
  int launchTime = atoi(configData[1]);
  /* Maximum throttle position from 1 to 100% */
  int maxPower = atoi(configData[2]);

  throttle.write(resolvePosition(launchPower));
  delay(launchTime);
  throttle.write(resolvePosition(maxPower));
  delay(2000);

  if (Serial.available() > 0) {
    String newConfig = Serial.read();
    writeStringToEEPROM(0, newConfig);
    Serial.print(newConfig);
  }
  delay(30000);
}

int resolvePosition(int throttlePercent) {
  int throttlePosition = (throttlePercent / 100) * 180;
  return throttlePosition;
}

void writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}

char* readCharFromEEPROM(int addrOffset)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  return data;
}
