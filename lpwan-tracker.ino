#include <SigFox.h>
#include <ArduinoLowPower.h>
#include "global.h"
#include "gps.h"

Point point;
SigfoxMessage msg;
uint8_t buffer[100];

void setup() {

  gpsSetup();

  gpsDeactivateStandardNMEAMessages();

  if (!SigFox.begin()) {
    Serial.println(F("Shield error or not present!"));
    reboot();
  }
  
  if (DEBUG) {
    // Activate debug mode
    SigFox.debug();
  }

  // Send the module to the deepest sleep
  SigFox.end();
}

void loop() {

  do {
    gpsFillPoint(point);
    SERIAL_DEBUG_PRINT(F("\nLatitude: "));
    SERIAL_DEBUG_PRINT(point.ggaLatitude);
    SERIAL_DEBUG_PRINT(F("\nN/S: "));
    SERIAL_DEBUG_PRINT(point.northSouthIndicator);
    SERIAL_DEBUG_PRINT(F("\n"));
  } while (areCoordinatesStillNotFetched(point));

  msg.latitude = point.ggaLatitude.toFloat();
  msg.nsIndicator = point.northSouthIndicator;
  msg.longitude = point.ggaLongitude.toFloat();
  msg.ewIndicator = point.eastWestIndicator;

  Serial.println(msg.latitude, 10);
  Serial.println(msg.longitude, 10);
  Serial.println(msg.nsIndicator);
  Serial.println(msg.ewIndicator);

  // Start Sigfox module
  SigFox.begin();
  delay(100);
  SigFox.status();
  delay(1);  
  SigFox.beginPacket();
  SigFox.write((uint8_t*)&msg,sizeof(msg));
  int result = SigFox.endPacket();
  
  // Check result
  if (result == 0) {
    SERIAL_DEBUG_PRINT(F("\nMessage Sent !!!\n"));
  } else {
    SERIAL_DEBUG_PRINT("\nError sending message\n");
  }
  SigFox.end();
  
/*
  buffer = (uint8_t*)&msg;
  3637.0329589844
  430.2767333984
  N
  W
*/

  SERIAL_DEBUG_PRINT(F("\nSleeping\n"));

  // Wait for 5 minutes until next message
  delay(5 * 60 * 1000);
}

void reboot() {
  NVIC_SystemReset();
  while (1) ;
}

