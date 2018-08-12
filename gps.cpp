#include <Arduino.h>
#include "global.h"
#include "gps.h"

#define DATA_TIMEOUT 120000L

const String PUBX00 = F("PUBX,00,");

void gpsSetup() {
  if (DEBUG) {
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

void resetPoint(Point & point) {
  point.ggaLatitude = String("");
  point.northSouthIndicator = '\0';
  point.ggaLongitude = String("");
  point.eastWestIndicator = '\0';
  point.accuracy = String("");
  point.altitude = String("");
}

void gpsFillPoint(Point & point) {
  delay(2000);
  Serial1.println(F("$PUBX,00*33")); // Request GPS position
  delay(100);

  resetPoint(point);

  bool found = false;
  int i = 0;
  int numCommas = 0;
  unsigned long lastTime = millis();
  while (!found && hasNotExpiredYet(lastTime)) {
        
    // barrier to assure serial data available
    waitUntilSerialDataIsAvailable(lastTime);

    char c = Serial1.read();
    SERIAL_DEBUG_PRINT(c);

    if (i < PUBX00.length()) {
      // Expected response still not found

      if (c == PUBX00.charAt(i)) {
        i++;
      } else {
        i = 0;
      }
    } else {
      // Expected response was found, now collecting data
      if (c == ',') {
        numCommas++;
      } else if (c == '*') {
        found = true;
      } else {
        if (numCommas == 1) {
          // Store latitude
          point.ggaLatitude += (char) c;
        } else if (numCommas == 2) {
          point.northSouthIndicator = (char) c;
        } else if (numCommas == 3) {
          point.ggaLongitude += (char) c;
        } else if (numCommas == 4) {
          point.eastWestIndicator = (char) c;
        } else if (numCommas == 5) {
          point.altitude += (char) c;
        } else if (numCommas == 7) {
          point.accuracy += (char) c;
        }
      }
    }
  }
}

boolean areCoordinatesStillNotFetched(Point point) {
  return point.ggaLatitude.length() == 0 || point.ggaLatitude == F("0000.00000");
}


