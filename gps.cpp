#include <Arduino.h>
#include "gps.h"

#define DATA_TIMEOUT 120000L
const String PUBX00 = F("PUBX,00,");

#ifdef GPS_DEBUG
#define GPS_DEBUG_PRINT(X) Serial.print(X)
#else
#define GPS_DEBUG_PRINT(X) { // nothing to do }
#endif

void gpsSetup() {
  if (GPS_DEBUG) {
    Serial.begin(9600);
    delay(500);

    while (!Serial) {};
  }
  
  Serial1.begin(9600);
  delay(500);

  while (!Serial1) {};
}

void gpsDeactivateStandardNMEAMessages() {
  delay(2000);
  Serial1.println(F("$PUBX,40,RMC,0,0,0,0*47")); // RMC OFF
  delay(100);
  Serial1.println(F("$PUBX,40,VTG,0,0,0,0*5E")); // VTG OFF
  delay(100);
  Serial1.println(F("$PUBX,40,GGA,0,0,0,0*5A")); // CGA OFF
  delay(100);
  Serial1.println(F("$PUBX,40,GSA,0,0,0,0*4E")); // GSA OFF
  delay(100);
  Serial1.println(F("$PUBX,40,GSV,0,0,0,0*59")); // GSV OFF
  delay(100);
  Serial1.println(F("$PUBX,40,GLL,0,0,0,0*5C")); // GLL OFF
  delay(1000);
}

bool hasNotExpiredYet(unsigned long lastTime) {
  return (millis() - lastTime) < DATA_TIMEOUT;
}

bool waitUntilSerialDataIsAvailable(unsigned long lastTime) {
  while (!Serial1.available() && hasNotExpiredYet(lastTime));
}

Point gpsGetPoint() {  
  delay(2000);
  Serial1.println(F("$PUBX,00*33")); // Request GPS position
  delay(100);

  bool found = false;
  String receivedData = String(PUBX00);
  int i = 0;
  int numCommas = 0;
  unsigned long lastTime = millis();
  while (!found && hasNotExpiredYet(lastTime)) {
        
    // barrier to assure serial data available
    waitUntilSerialDataIsAvailable(lastTime);

    char c = Serial1.read();
    GPS_DEBUG_PRINT(c);

    if (i < PUBX00.length()) {
      // Expected response still not found

      if (c == PUBX00.charAt(i)) {
        i++;
      } else {
        i = 0;
      }
    } else {
      // Expected response was found, now collecting data
      GPS_DEBUG_PRINT(F("Phrase found"));

      if (c == ',') {
        numCommas++;
      } else if (c == '*') {
        
      } else {
        
      }

      receivedData += (char) c;
    }
  }
  
  Point p;
  p.ggaLatitude = String("hello world");
  return p;
}


