#include <SigFox.h>
#include <ArduinoLowPower.h>
#include "global.h"
#include "gps.h"

Point point;
SigfoxMessage msg;

void setup() {

  gpsSetup();

  gpsDeactivateStandardNMEAMessages();

  if (!SigFox.begin()) {
    debugPrint(F("Shield error or not present!"));
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
  } while (areCoordinatesStillNotFetched(point));

  msg.latitude = point.ggaLatitude.toFloat();
  msg.nsIndicator = point.northSouthIndicator;
  msg.longitude = point.ggaLongitude.toFloat();
  msg.ewIndicator = point.eastWestIndicator;

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
    debugPrint(F("\nMessage Sent !!!\n"));
  } else {
    debugPrint(F("\nError sending message\n"));
  }
  SigFox.end();
  
  debugPrint(F("Sleeping\n"));

  // Wait for 10 minutes until next message
  delay(10 * 60 * 1000);
}

void reboot() {
  NVIC_SystemReset();
  while (1) ;
}

