#include <SigFox.h>
#include <ArduinoLowPower.h>
#include "global.h"
#include "gps.h"

Point point;

void setup() {

  gpsSetup();

  gpsDeactivateStandardNMEAMessages();

  if (!SigFox.begin()) {
    Serial.println("Shield error or not present!");
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
  } while (areCoordinatesStillNotFetched(point));

  SigfoxMessage msg;
// msg.latitude = turn point.ggaLatitude (String) into float and set here
  
/*
   // Start Sigfox module
   SigFox.begin();
   delay(100);
  
   SigFox.beginPacket();
   SigFox.write((uint8_t*)&msg,sizeof(msg));
   SigFox.endPacket(false);
   SigFox.end();
   int result = SigFox.endPacket();
  
  // Check result
  if (result == 0) {
    Serial.println("Message Sent !!!");
  } else {
    Serial.println("Error sending message: " + result);
  }
*/

  // Clears all pending interrupts
  SigFox.status();
  delay(1);

  // Send the module to the deepest sleep
  SigFox.end();

  SERIAL_DEBUG_PRINT(F("\nSleeping\n"));

  // Wait for 15 inutes until next message
  delay(15 * 60 * 1000);
}

void reboot() {
        NVIC_SystemReset();
        while (1) ;
}

